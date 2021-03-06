////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Local UID Generator
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "Util/GlobalUIDGenerator.h"
#include "Util/SFTimeUtil.h"




namespace SF {

	constexpr CounterType GlobalUIDGenerator::MAX_INDEX_DIFF;

	GlobalUIDGenerator::GlobalUIDGenerator()
		:m_ServerID(0)
	{
		// Server ID should be smaller than 255
		Assert(m_ServerID <= 255)

		m_time = Util::Time.GetRawUTCSec();
	}

	void GlobalUIDGenerator::SetServerID(uint serverID)
	{
		m_ServerID = serverID;
		AssertRel(serverID < 256); // we have assumption
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
				if (indexDiff == MAX_INDEX_DIFF)
				{
					// update index sync
					UTCTimeStampSec newTime = Util::Time.GetRawUTCSec();
					while (newTime == m_time) // we have to stall until we have new time stamp
					{
						// Maximum GUID for this sec, sleep a little before retry
						//defTrace(Debug1, "GlobalUIDGenerator: Maximum GUID for this sec, sleep a little before retry");
						// Do some other thread's job
						ThisThread::SleepFor(DurationMS(0));
						newTime = Util::Time.GetRawUTCSec();
					}

					// update time and sync index
					m_time = newTime;
					m_GeneratorSync.store(myIndex, std::memory_order_release);
				}
				else if (indexDiff > MAX_INDEX_DIFF)
				{
					// too far away
					// we need to check difference again
					ThisThread::SleepFor(DurationMS(4));
				}
			} while (indexDiff > MAX_INDEX_DIFF);

			uid.ID = ((uint32_t)myIndex) & 0xFFFFFF;

			// we don't accept zero as ID value, we need another index
		} while (uid.ID == 0);

		uid.SvrID = m_ServerID & 0XFF;
		uid.Time = m_time.time_since_epoch().count();

		defTrace(Debug1, "GlobalUIDGenerator: New GUID {0}", uid.UID);

		static GlobalUID uidPrev(0);
		Assert(uidPrev != uid);
		uidPrev = uid;

		return uid;
	}

}; // namespace SF



