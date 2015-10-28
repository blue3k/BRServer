////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/MemoryPool.h"
#include "Common/SharedPointer.h"
#include "Common/DualSortedMap.h"
#include "Common/TimeUtil.h"
#include "Common/SystemSynchronization.h"



namespace BR {
namespace Svr {


	class TimerAction : public SharedObject
	{
	public:

#pragma pack(push, 4)
		union TimeKey {
			struct {
				UINT32 ObjectID;
				TimeStampMS NextTickTime;
			};

			UINT64 TimerKey;

			TimeKey() {}
		};
#pragma pack(pop)

	private:

		TimeKey m_InQueueKey;

		friend class TimeScheduler;

	protected:

		const TimeKey& GetInQueueKey() { return m_InQueueKey; }

	public:
		TimeKey TimeData;

		TimerAction();
		virtual ~TimerAction();

		bool IsScheduled()									{ return m_InQueueKey.NextTickTime != TimeStampMS::max(); }
		void ClearTime()									{ TimeData.NextTickTime = TimeStampMS::max(); }

		TimeStampMS GetNexTickTime()						{ return TimeData.NextTickTime; }
		void SetNextTickTime(TimeStampMS nextTickTime);

		TimeStampMS GetScheduledTime()						{ Assert(m_InQueueKey.NextTickTime != TimeStampMS::min());  return m_InQueueKey.NextTickTime; }

		virtual bool UpdateTick() { return false; }

		virtual const char* GetDebugString() = 0;
	};

	extern template class SharedPointerT < TimerAction > ;

}; // namespace Svr
}; // namespace BR

