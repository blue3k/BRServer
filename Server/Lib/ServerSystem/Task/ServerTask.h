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
#include "Task/TimerScheduler.h"

namespace SF {


	class TickTaskManager;
	class TaskWorker;
	struct ServerTaskEvent;
	class TimerAction;
	class TickTaskTimerAction;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask class -  interface for task operation
	//

	class TickTask : public SharedObject
	{
	public:

		const int DEFAULT_TICKTASK_INTERVAL = 1000;

	private:

		MemoryManager m_MemoryManager;

		// Work load for
		SysUInt		m_TaskLoad;

		// TickTask Group index
		SysUInt		m_TaskGroupIdx;

		// TickTask manager pointer
		TickTaskManager	*m_pTaskManager;
		TaskWorker	*m_pTaskWorker;

		// TickTask UID
		uint32_t		m_TaskID;

		// task retry count
		uint		m_RetryCount;

		// Task tick interval(maximum)
		DurationMS		m_TickInterval;

		SharedPointerT<TickTaskTimerAction>			m_TimerAction;



	public:
		// Constructor
		TickTask();
		virtual ~TickTask();

		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }
		IHeap& GetHeap() { return m_MemoryManager; }

		// Get TickTask ID
		inline uint32_t GetTaskID() const;
		void SetTaskID(uint32_t uiTaskID);

		// Get TickTask group
		inline SysUInt GetTaskGroupID() const;
		inline void SetTaskGroupID( SysUInt uiTaskGroupIdx );

		// Get TickTask group
		TickTaskManager* GetTaskManager();
		void SetTaskManager( TickTaskManager *pManager );

		TaskWorker* GetTaskWorker();

		TickTaskTimerAction* GetTimerAction()					{ return *m_TimerAction; }

		// Get task Load
		inline SysUInt GetTaskLoad() const;

		DurationMS GetTickInterval() const;
		inline void SetTickInterval(DurationMS tickInterval)							{ m_TickInterval = tickInterval; }

		TimeStampMS GetScheduledTickTime() const;
		void SetNextScheduledTickTime(TimeStampMS tickNext);


		// retry count
		inline uint GetRetryCount() const										{ return m_RetryCount;  }
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

		virtual bool OnTimerTick() override;

		virtual const char* GetDebugString() override;
	};


#include "ServerTask.inl"



	extern template class SharedPointerT<TickTask>;
	template<> inline SharedPointerT<TickTask> DefaultValue<SharedPointerT<TickTask>>() { return SharedPointerT<TickTask>(); }


}; // namespace SF


