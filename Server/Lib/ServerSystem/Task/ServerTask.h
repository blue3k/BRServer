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
#include "MemoryManager/SFMemoryPool.h"
#include "Object/SFSharedObject.h"
#include "Object/SFSharedPointer.h"
#include "Task/SFTimerScheduler.h"

namespace SF {


	class TickTaskManager;
	class TaskWorker;
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

		Heap m_Heap;

		// Work load for
		uint32_t		m_TaskLoad = 1;

		// TickTask Group index
		uint32_t		m_TaskGroupIdx = 0;

		// TickTask manager pointer
		TickTaskManager* m_pTaskManager{};
		TaskWorker* m_pTaskWorker{};

		// TickTask UID
		uint32_t		m_TaskID = 0;

		// task retry count
		uint		m_RetryCount = 0;

		// Task tick interval(maximum)
		DurationMS		m_TickInterval = DurationMS(DEFAULT_TICKTASK_INTERVAL);

		SharedPointerT<TickTaskTimerAction>			m_TimerAction;

		bool m_UseDesignatedThread =  false;

	public:
		// Constructor
		TickTask();
		TickTask(IHeap& heap);
		virtual ~TickTask();

		IHeap& GetHeap() { return *m_Heap.get(); }

		// Get TickTask ID
		SF_FORCEINLINE uint32_t GetTaskID() const { return m_TaskID; }
		void SetTaskID(uint32_t uiTaskID);

		// Get TickTask group
		SF_FORCEINLINE uint32_t GetTaskGroupID() const { return m_TaskGroupIdx; }
		SF_FORCEINLINE void SetTaskGroupID(uint32_t uiTaskGroupIdx) { m_TaskGroupIdx = uiTaskGroupIdx; }

		bool UseDesignatedThread() const { return m_UseDesignatedThread; }
		void SetUseDesignatedThread(bool bValue) { m_UseDesignatedThread = bValue; }

		// Get TickTask group
		SF_FORCEINLINE TickTaskManager* GetTaskManager() { return m_pTaskManager; }
		SF_FORCEINLINE void SetTaskManager(TickTaskManager* pManager)
		{
			Assert(m_pTaskManager == nullptr);
			m_pTaskManager = pManager;
		}

		TaskWorker* GetTaskWorker() { return m_pTaskWorker; }

		TickTaskTimerAction* GetTimerAction() { return *m_TimerAction; }

		// Get task Load
		SF_FORCEINLINE uint32_t GetTaskLoad() const { return m_TaskLoad; }

		DurationMS GetTickInterval() const;
		inline void SetTickInterval(DurationMS tickInterval) { m_TickInterval = tickInterval; }

		TimeStampMS GetScheduledTickTime() const;
		void SetNextScheduledTickTime(TimeStampMS tickNext);

		// Kick the task so that it updated asap
		void KickTickUpdate();

		// retry count
		SF_FORCEINLINE uint GetRetryCount() const	{ return m_RetryCount;  }
		SF_FORCEINLINE void ResetRetryCount()		{ m_RetryCount = 0; }
		SF_FORCEINLINE void IncRetryCount()			{ m_RetryCount++; }

		// Get task Load
		SF_FORCEINLINE void SetTaskLoad(uint32_t workLoad) { m_TaskLoad = workLoad; }

		// Run the task
		// return ResultCode::SUCCESS_FALSE means it doesn't need to rescheduled
		virtual Result TickUpdate(TimerAction *pAction = nullptr) = 0;

		virtual void OnAddedToTaskManager(TaskWorker *pWorker);
		virtual void OnRemovedFromTaskManager(TaskWorker *pWorker);

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

	extern template class SharedPointerT<TickTask>;

} // namespace SF



