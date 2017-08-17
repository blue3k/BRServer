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
#include "Common/Task/TimeSchedulerAction.h"



namespace BR {



	// Shared object base
	class TimeScheduler
	{
	public:

	private:
		// Minimum timer tick time when the action doesn't assigned a valid tick time
		DurationMS m_FailSafeTimerTickInterval;

		// Assert on invalid tick time
		bool m_AssertOnInvalidTickTime;

		// timer map
		DualSortedMap<UINT64, SharedPointerT<TimerAction>> m_TimerMap;
		//BR::CriticalSection m_WriteLock;
		ThreadID m_WorkingThreadID;

		std::atomic<int> m_IsWriteLocked;

		std::function<bool(const UINT64&, const SharedPointerT<TimerAction>&)> m_TimerTickActionUpdate;
		bool TimerTickActionUpdate(const UINT64& keyVal, const SharedPointerT<TimerAction>& pAction);

		TimeStampMS m_GetNextTickResult;
		std::function<bool(const UINT64&, const SharedPointerT<TimerAction>&)> m_TimerTickActionGetNextTick;
		bool TimerTickActionGetNextTick(const UINT64& keyVal, const SharedPointerT<TimerAction>& pAction);

	public:

		TimeScheduler();

		void SetFailSafeTimerTickInterval(DurationMS timerTick)				{ m_FailSafeTimerTickInterval = timerTick; }
		void SetAssertOnInvalidTickTime(bool assertOn)						{ m_AssertOnInvalidTickTime = assertOn; }

		ThreadID GetWorkingThreadID() { return m_WorkingThreadID; }
		void UpdateWorkingThreadID(ThreadID threadID);

		Result AddTimerAction(ThreadID threadID, TimerAction* pAction);
		Result RemoveTimerAction(ThreadID threadID, TimerAction* pAction);
		Result CommitChanges(ThreadID threadID);

		Result Reschedul(ThreadID threadID, TimerAction* pAction);

		TimeStampMS GetNextTimeTick();

		CounterType GetScheduledItemCount()								{ return m_TimerMap.GetItemCount(); }
		CounterType GetChangedItemCount()								{ return m_TimerMap.GetWriteItemCount(); }

		void ValidateTimerKeys();

		void UpdateTick(ThreadID threadID);
	};


}; // namespace BR

