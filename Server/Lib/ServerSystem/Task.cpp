////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "ServerSystem/Task.h"
#include "ServerSystem/TaskManager.h"
#include "ServerSystem/TimeScheduler.h"
#include "Common/TimeUtil.h"
#include "ServerSystem/SvrConst.h"


namespace BR {
namespace Svr {
	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask class -  interface for task operation
	//

	template class SharedPointerT<TickTask>;

	// Constructor
	TickTask::TickTask()
		: m_pTaskManager(nullptr)
		, m_TaskLoad(1)
		, m_TaskGroupIdx(0)
		, m_RetryCount(0)
		, m_TickInterval(Svr::Const::DEFAULT_TICKTASK_INTERVAL) // default task tick interval is 1sec
		, m_TimerAction(nullptr)
		, m_pTaskWorker(nullptr)
		, m_TaskID(0)
	{
		m_TimerAction = SharedPointerT<TickTaskTimerAction>(new TickTaskTimerAction(this));
	}

	TickTask::~TickTask()
	{
		if (m_TimerAction!=nullptr)
			m_TimerAction->DisposeWeakPointer();
		//Util::SafeDelete(m_TimerAction);
	}


	void TickTask::SetTaskID(UINT32 uiTaskID)
	{
		Assert(uiTaskID != 0);
		m_TaskID = uiTaskID;
		m_TimerAction->TimeData.ObjectID = m_TaskID;
	}


	DurationMS TickTask::GetTickInterval() const
	{
		return m_TickInterval;
	}

	TimeStampMS TickTask::GetScheduledTickTime() const
	{
		return m_TimerAction->GetScheduledTime();
	}

	void TickTask::SetNextScheduledTickTime(TimeStampMS tickNext)
	{
		auto currentSchedule = m_TimerAction->GetScheduledTime();
		auto newTime = Util::TimeMin(currentSchedule, tickNext);
		m_TimerAction->SetNextTickTime(newTime);
	}


	void TickTask::OnAddedToTaskManager(TaskWorker *pWorker)
	{
		Assert(pWorker);
		if (pWorker == nullptr) return;

		Assert(GetTaskID() != 0);

		m_pTaskWorker = pWorker;

		m_TimerAction->TimeData.ObjectID = GetTaskID();

		if (GetTickInterval() > DurationMS(0))
		{
			m_TimerAction->SetNextTickTime(Util::Time.GetTimeMs() + GetTickInterval());
		}
	}

	void TickTask::OnRemovedFromTaskManager(TaskWorker *pWorker)
	{
		m_pTaskWorker = nullptr;

		if (m_TimerAction != nullptr)
		{
			m_TimerAction->DisposeWeakPointer();
		}
	}



	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Default timer action
	//

	TickTaskTimerAction::TickTaskTimerAction(TickTask* pTickTask)
		: m_pTickTask(pTickTask)
	{
	}

	TickTaskTimerAction::~TickTaskTimerAction()
	{
		Assert(m_pTickTask == nullptr);
	}

	void TickTaskTimerAction::DisposeWeakPointer()
	{
		m_pTickTask = nullptr;
	}

	bool TickTaskTimerAction::UpdateTick()
	{
		TickTask* pTickTask = m_pTickTask;

		if (pTickTask == nullptr)
			return false;

		auto nextDiff = (LONG)(TimeData.NextTickTime - Util::Time.GetTimeMs()).count();
		if (nextDiff <= 0)
		{
			// Do default rescheduling
			auto nextTick = Util::Time.GetTimeMs() + pTickTask->GetTickInterval();
			SetNextTickTime(nextTick);
		}

		if (pTickTask->TickUpdate(this) == S_FALSE)
			return false;

		return true;
	}

	const char* TickTaskTimerAction::GetDebugString()
	{
		TickTask* pTickTask = m_pTickTask;
		if (pTickTask != nullptr) return typeid(*pTickTask).name();

		return "TickTaskTimerAction";
	}


}; // namespace Svr
}; // namespace BR

