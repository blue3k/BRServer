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



#include "ServerSystemPCH.h"
#include "Multithread/SFThread.h"
#include "Util/SFStrUtil.h"
#include "Task/ServerTask.h"
#include "Task/ServerTaskManager.h"
#include "Task/SFTimerScheduler.h"
#include "Util/SFTimeUtil.h"



namespace SF {

	template class SharedPointerT<TickTask>;

	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask class -  interface for task operation
	//


	// Constructor
	TickTask::TickTask()
		: m_Heap("TickTask", GetSystemHeap())
	{
		m_TimerAction = new(GetHeap()) TickTaskTimerAction(this);
	}

	TickTask::TickTask(IHeap& heap)
		: m_Heap("TickTask", heap)
	{
		m_TimerAction = new(GetHeap()) TickTaskTimerAction(this);
	}

	TickTask::~TickTask()
	{
		if (m_TimerAction!=nullptr)
			m_TimerAction->DisposeWeakPointer();
		m_TimerAction = nullptr;
		//Util::SafeDelete(m_TimerAction);
	}


	void TickTask::SetTaskID(uint32_t uiTaskID)
	{
		Assert(uiTaskID != 0);
		m_TaskID = uiTaskID;
		m_TimerAction->TimeData.Components.ObjectId = m_TaskID;
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
		auto newTime = Util::TimeMinNonZero(currentSchedule, tickNext);
		m_TimerAction->SetNextTickTime(newTime);
	}

	void TickTask::KickTickUpdate()
	{
		SetNextScheduledTickTime(Util::Time.GetRawTimeMs());
		if (GetTaskWorker())
			GetTaskWorker()->GetTimeScheduler().KickTickUpdate(m_TimerAction.get());
	}

	void TickTask::OnAddedToTaskManager(TaskWorker *pWorker)
	{
		Assert(pWorker);
		if (pWorker == nullptr) return;

		Assert(GetTaskID() != 0);

		m_pTaskWorker = pWorker;

		m_TimerAction->TimeData.Components.ObjectId = GetTaskID();

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

	bool TickTaskTimerAction::OnTimerTick()
	{
		TickTask* pTickTask = m_pTickTask;

		if (pTickTask == nullptr)
			return false;

		auto nextDiff = (INT)(TimeData.Components.NextTickTime - Util::Time.GetTimeMs()).count();
		if (nextDiff <= 0)
		{
			// Do default rescheduling
			auto nextTick = Util::Time.GetTimeMs() + pTickTask->GetTickInterval();
			SetNextTickTime(nextTick);
		}

		if (pTickTask->TickUpdate(this) == Result(ResultCode::SUCCESS_FALSE))
			return false;

		return true;
	}

	const char* TickTaskTimerAction::GetDebugString()
	{
		TickTask* pTickTask = m_pTickTask;
		if (pTickTask != nullptr) return typeid(*pTickTask).name();

		return "TickTaskTimerAction";
	}



}; // namespace SF

