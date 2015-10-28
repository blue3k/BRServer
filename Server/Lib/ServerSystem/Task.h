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
#include "ServerSystem/TimeScheduler.h"

namespace BR {
namespace Svr
{
	class TaskManager;
	class TaskWorker;
	struct EventTask;
	class TimerAction;
	class TickTaskTimerAction;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask class -  interface for task operation
	//

	class TickTask : public SharedObject
	{
	private:

		// Work load for
		SysUInt		m_TaskLoad;

		// TickTask Group index
		SysUInt		m_TaskGroupIdx;

		// TickTask manager pointer
		TaskManager	*m_pTaskManager;
		TaskWorker	*m_pTaskWorker;

		// TickTask UID
		UINT32		m_TaskID;

		// task retry count
		UINT		m_RetryCount;

		// Task tick interval(maximum)
		DurationMS		m_TickInterval;

		SharedPointerT<TickTaskTimerAction>			m_TimerAction;



	public:
		// Constructor
		TickTask();
		virtual ~TickTask();

		// Get TickTask ID
		inline UINT32 GetTaskID() const;
		void SetTaskID(UINT32 uiTaskID);

		// Get TickTask group
		inline SysUInt GetTaskGroupID() const;
		inline void SetTaskGroupID( SysUInt uiTaskGroupIdx );

		// Get TickTask group
		TaskManager* GetTaskManager();
		void SetTaskManager( TaskManager *pManager );

		TaskWorker* GetTaskWorker();

		TickTaskTimerAction* GetTimerAction()					{ return (TickTaskTimerAction*)m_TimerAction; }

		// Get task Load
		inline SysUInt GetTaskLoad() const;

		DurationMS GetTickInterval() const;
		inline void SetTickInterval(DurationMS tickInterval)							{ m_TickInterval = tickInterval; }

		TimeStampMS GetScheduledTickTime() const;
		void SetNextScheduledTickTime(TimeStampMS tickNext);


		// retry count
		inline UINT GetRetryCount() const										{ return m_RetryCount;  }
		inline void ResetRetryCount()											{ m_RetryCount = 0; }
		inline void IncRetryCount()												{ m_RetryCount++; }

		// Get task Load
		inline void SetTaskLoad( SysUInt workLoad );

		// Run the task
		// return S_FALSE means it doesn't need to rescheduled
		virtual HRESULT TickUpdate(TimerAction *pAction = nullptr) = 0;

		virtual void OnAddedToTaskManager(TaskWorker *pWorker);
		virtual void OnRemovedFromTaskManager(TaskWorker *pWorker);

		virtual HRESULT OnEventTask(const EventTask& eventTask) { Assert(false);  return E_NOTIMPL; }
	};


	////////////////////////////////////////////////////////////////////////
	//
	//  TickTaskTimerAction
	//

	class TickTaskTimerAction : public TimerAction
	{
	private:

		TickTask* m_pTickTask;

	public:
		TickTaskTimerAction(TickTask* pTickTask);
		~TickTaskTimerAction();

		void DisposeWeakPointer();

		virtual bool UpdateTick() override;

		virtual const char* GetDebugString() override;
	};


#include "Task.inl"


	extern template class SharedPointerT<TickTask>;

}; // namespace Svr
}; // namespace BR

