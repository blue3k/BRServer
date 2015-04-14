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
#include "ServerSystem/TimeSchedulerAction.h"



namespace BR {
namespace Svr {



	// Shared object base
	class TimeScheduler
	{
	public:

	private:
		// Minimum timer tick time when the action doesn't assigned a valid tick time
		ULONG m_FailSafeTimerTickInterval;

		// Assert on invalid tick time
		bool m_AssertOnInvalidTickTime;

		// timer map
		DualSortedMap<UINT64, SharedPointerT<TimerAction>> m_TimerMap;
		//BR::CriticalSection m_WriteLock;
		ThreadID m_WorkingThreadID;

		std::atomic<int> m_IsWriteLocked;

		std::function<bool(const UINT64&, const SharedPointerT<TimerAction>&)> m_TimerTickActionUpdate;

		ULONG m_GetNextTickResult;
		std::function<bool(const UINT64&, const SharedPointerT<TimerAction>&)> m_TimerTickActionGetNextTick;

	public:

		TimeScheduler();

		void SetFailSafeTimerTickInterval(ULONG timerTick)				{ m_FailSafeTimerTickInterval = timerTick; }
		void SetAssertOnInvalidTickTime(bool assertOn)					{ m_AssertOnInvalidTickTime = assertOn; }

		void UpdateWorkingThreadID(ThreadID threadID);

		HRESULT AddTimerAction(ThreadID threadID, TimerAction* pAction);
		HRESULT RemoveTimerAction(ThreadID threadID, TimerAction* pAction);
		HRESULT CommitChanges(ThreadID threadID);

		HRESULT Reschedul(ThreadID threadID, TimerAction* pAction);

		ULONG GetNextTimeTick();

		CounterType GetScheduledItemCount()								{ return m_TimerMap.GetItemCount(); }
		CounterType GetChangedItemCount()								{ return m_TimerMap.GetWriteItemCount(); }

		void ValidateTimerKeys();

		void UpdateTick(ThreadID threadID);
	};

}; // namespace Svr
}; // namespace BR

