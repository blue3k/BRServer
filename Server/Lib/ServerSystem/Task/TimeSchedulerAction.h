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

#include "SFTypedefs.h"
#include "Memory/MemoryPool.h"
#include "Object/SharedPointer.h"
#include "Container/DualSortedMap.h"
#include "Util/TimeUtil.h"
#include "Thread/SystemSynchronization.h"



namespace BR {



	class TimerAction : public SharedObject
	{
	public:

#pragma pack(push, 4)
		union TimeKey {
			struct {
				UINT32 ObjectID;
				TimeStampMS NextTickTime;
			} Components;

			UINT64 TimerKey;

			TimeKey() {}
		};
#pragma pack(pop)

	private:

		TimeKey m_InQueueKey;
		bool    m_Queued;
		friend class TimeScheduler;

	protected:

		const TimeKey& GetInQueueKey() { return m_InQueueKey; }

	public:
		TimeKey TimeData;

		TimerAction();
		virtual ~TimerAction();

		bool IsScheduled()									{ return m_InQueueKey.Components.NextTickTime != TimeStampMS::max(); }
		void ClearTime()									{ TimeData.Components.NextTickTime = TimeStampMS::max(); }

		TimeStampMS GetNexTickTime()						{ return TimeData.Components.NextTickTime; }
		void SetNextTickTime(TimeStampMS nextTickTime);

		TimeStampMS GetScheduledTime()						{ Assert(m_InQueueKey.Components.NextTickTime != TimeStampMS::min());  return m_InQueueKey.Components.NextTickTime; }

		virtual bool UpdateTick() { return false; }

		virtual const char* GetDebugString() = 0;
	};


	extern template class SharedPointerT < TimerAction >;

}; // namespace BR


namespace SF
{
	template<> inline SharedPointerT<BR::TimerAction> DefaultValue<SharedPointerT<BR::TimerAction>>() { return SharedPointerT<BR::TimerAction>(); }
}

