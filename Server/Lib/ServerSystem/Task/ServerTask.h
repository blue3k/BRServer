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
#include "Object/SharedObject.h"
#include "Object/SharedPointer.h"
#include "Task/TimeScheduler.h"

namespace BR {


	class TaskManager;
	class TaskWorker;
	struct ServerTaskEvent;
	class TimerAction;
	class TickTaskTimerAction;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask class -  interface for task operation
	//

	class TickTask : public SF::SharedObject
	{
	public:

		const int DEFAULT_TICKTASK_INTERVAL = 1000;

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
		// return ResultCode::SUCCESS_FALSE means it doesn't need to rescheduled
		virtual Result TickUpdate(TimerAction *pAction = nullptr) = 0;

		virtual void OnAddedToTaskManager(TaskWorker *pWorker);
		virtual void OnRemovedFromTaskManager(TaskWorker *pWorker);

		virtual Result OnEventTask(const ServerTaskEvent& eventTask) { Assert(false);  return ResultCode::NOT_IMPLEMENTED; }
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


#include "ServerTask.inl"



	extern template class SharedPointerT<TickTask>;


}; // namespace BR

namespace SF
{
	template<> inline SharedPointerT<BR::TickTask> DefaultValue<SharedPointerT<BR::TickTask>>() { return SharedPointerT<BR::TickTask>(); }
}


