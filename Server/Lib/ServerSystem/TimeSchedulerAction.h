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
#include "Common/SystemSynchronize.h"



namespace BR {
namespace Svr {


	class TimerAction : public SharedObject
	{
	public:

#pragma pack(push, 4)
		union TimeKey {
			struct {
				UINT32 ObjectID;
				UINT32 NextTickTime;
			};

			UINT64 TimerKey;
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

		bool IsScheduled()								{ return m_InQueueKey.NextTickTime != UINT_MAX; }
		void ClearTime()								{ TimeData.NextTickTime = UINT_MAX; }

		ULONG GetNexTickTime()							{ return TimeData.NextTickTime; }
		void SetNextTickTime(UINT32 nextTickTime);

		ULONG GetScheduledTime()						{ Assert(m_InQueueKey.NextTickTime != 0);  return m_InQueueKey.NextTickTime; }

		virtual bool UpdateTick() { return false; }

		virtual const char* GetDebugString() = 0;
	};

	extern template class SharedPointerT < TimerAction > ;

}; // namespace Svr
}; // namespace BR

