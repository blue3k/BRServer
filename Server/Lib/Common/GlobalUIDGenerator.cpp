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
		m_time = Util::Time.GetRawUTCSec();
	}

	// Generate New ID
	GlobalUID GlobalUIDGenerator::NewUID() const
	{
		GlobalUID uid;

		// generate new id until get proper value
		CounterType myIndex = 0;
		do {
			myIndex = m_CounterForID.fetch_add(1, std::memory_order_relaxed);

			// wait index diff
			CounterType indexDiff = 0;
			do {
				indexDiff = myIndex - m_GeneratorSync.load(std::memory_order_relaxed);
				if (indexDiff == UPDATE_TIME_DIFF)
				{
					// update index sync
					UINT32 newTime = Util::Time.GetRawUTCSec();
					while (newTime == m_time) // we have to stall until we are on the same time stamp
					{
						// Maximum GUID for this sec, sleep a little before retry
						defTrace(Trace::TRC_TRACE, "GlobalUIDGenerator: Maximum GUID for this sec, sleep a little before retry");
						// Do some other thread's job
						Sleep(0);
						newTime = Util::Time.GetRawUTCSec();
					}

					// update time and sync index
					m_time = newTime;
					m_GeneratorSync.store(myIndex, std::memory_order_release);
				}
				else if (indexDiff > UPDATE_TIME_DIFF)
				{
					// too far away
					// we need to check difference again
					SleepEx(4, false);
				}
			} while (indexDiff > UPDATE_TIME_DIFF);

			uid.ID = (UINT32)myIndex;

			// we don't accept zero as ID value, we need another index
		} while (uid.ID == 0);


		uid.ServerID = m_ServerID;
		uid.Time = m_time;

		defTrace(Trace::TRC_TRACE, "GlobalUIDGenerator: New GUID %0%", (float)uid.UID);

		static GlobalUID uidPrev(0);
		Assert(uidPrev != uid);
		uidPrev = uid;

		return uid;
	}

}; // namespace BR



