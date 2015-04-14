////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Local UID Generator
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/GlobalUIDGenerator.h"
#include "Common/TimeUtil.h"




namespace BR {

	GlobalUIDGenerator::GlobalUIDGenerator()
		:m_ServerID(0)
	{
		//AssertRel( serverID < 256 );
		m_time = Util::Time.GetTimeMs();
	}

	// Generate New ID
	GlobalUID GlobalUIDGenerator::NewUID() const
	{
		GlobalUID uid;

		uid.ServerID = m_ServerID;
		// we are going to use only 32 least significant bit, This should be enough while less than 16m IDs are generated for a sec.
		uid.ID = (UINT32)m_CounterForID.fetch_add(1,std::memory_order_relaxed);
#ifdef DEBUG
		if( (uid.ID & 0xF) == 0 ) // for test generate new time every 16 ids
#else
		if( (uid.ID & 0xFFFFFF) == 0 )
#endif
		{
			UINT32 newTime = Util::Time.GetTimeMs();
			while(newTime == m_time) // we have to stall untile we have on the same sec
			{
				// Maximum GUID for this sec, sleep a little before retry
				defTrace( Trace::TRC_TRACE, "GlobalUIDGenerator: Maximum GUID for this sec, sleep a little before retry" );
				// Do some other thread's job
				Sleep(0);
				newTime = Util::Time.GetTimeMs();
			}

			m_time = newTime;
		}

		uid.Time = m_time;

		defTrace(Trace::TRC_TRACE, "GlobalUIDGenerator: New GUID %0%", (float)uid.UID);

		static GlobalUID uidPrev(0);
		Assert(uidPrev != uid);
		uidPrev = uid;

		return uid;
	}

}; // namespace BR



