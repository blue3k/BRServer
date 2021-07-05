////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "Multithread/SFThread.h"
#include "Multithread/SFSynchronization.h"
#include "Container/SFSortedMap.h"
#include "Container/SFDualSortedMap.h"
#include "Container/SFPageQueue.h"
#include "Task/ServerTask.h"
#include "ServerLog/SvrLog.h"
#include "Container/SFIndexing.h"
#include "Container/SFHashTable.h"
#include "Task/SFTimerScheduler.h"



namespace SF {


	class TickTaskManager;


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask exection Group class -  interface for task operation
	//

	class TaskWorker : public Thread
	{
	public:
		// task list type
		typedef SortedMap<uint64_t, SharedPointerT<TickTask>>	TaskList;

		const int TICK_UPDATE_TIME = 20;
		const uint MAX_TASK_RETRY = 30 * 1000 / TICK_UPDATE_TIME; // retry for 30 sec

	private:
		// TickTaskManager that include this group
		TickTaskManager*			m_pTaskManager;

		// Load for 
		SysUInt		m_GroupWorkLoad;
		SysUInt		m_GroupWorkLoadDiff;

		// Group ID
		uint32_t		m_GroupID;

		// Base loop delay
		DurationMS					m_ulLoopInterval;

		PageQueue<std::function<void()>>		m_TaskFunctions;
	
		// TickTask pending queue
		PageQueue<SharedPointerT<TickTask>>		m_PendingAddTask;

		// TickTask Remove queue
		PageQueue<SharedPointerT<TickTask>>		m_PendingRemoveTask;

		// task list for update
		TaskList				m_TaskList;

		TimerScheduler			m_TimeScheduler;

	private:

		Result UpdateAddTickTaskQueue();
		Result UpdateRemoveTickTaskQueue();
		Result UpdateTickTask();
		Result UpdateTaskFunction();
		Result UpdateTimer();

	public:

		// Constructor/Destructor
		//	iQueuePageSize : Add/Remove queue size
		TaskWorker( TickTaskManager* pTaskManager, int iQueuePageSize = -1 );
		virtual ~TaskWorker();

		// Get/Set Group Idx
		SF_FORCEINLINE uint32_t GetGroupID() { return m_GroupID; }
		SF_FORCEINLINE void SetGroupID(uint32_t ulGroupID) { m_GroupID = ulGroupID; }

		// Get Group workload
		SF_FORCEINLINE SysUInt GetGroupWorkLoad() { return m_GroupWorkLoad + m_GroupWorkLoadDiff; }

		// Set base loop delay
		inline void SetBaseLoopInterval(DurationMS dwLoopInterval) { m_ulLoopInterval = dwLoopInterval; }
		inline DurationMS GetBaseLoopInterval() { return m_ulLoopInterval; }

		TimerScheduler& GetTimeScheduler()											{ return m_TimeScheduler; }

		Result AddTaskFunction(std::function<void()>&& pTaskFunction);

		// Add task
		Result PendingAddTask( TickTask* pTask );

		// Remove task
		Result PendingRemoveTask( TickTask* pTask );

		// run function
		virtual void Run() override;
	};


	//////////////////////////////////////////////////////////
	// Single task worker
	class SingleTaskWorker : public Thread
	{
	private:

		TickTask* m_pTask{};

	public:

		SingleTaskWorker(TickTask* pTask);

		virtual bool Tick() override;
	};


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask Manager base class -  interface for task manager
	//

	class TickTaskManager : public SharedObject
	{
	public:


	private:

		// GroupID gen
		static SyncCounter	stm_GroupIDGen;

	protected:

		// TickTask groups
		DynamicArray<TaskWorker*>	m_TaskGroups;

		// thread only take care single thread
		DynamicArray<SingleTaskWorker*>	m_SingleTaskWorkers;

	public:
		// Constructor/Destructor
		TickTaskManager();
		virtual ~TickTaskManager();

		// Get/Set Working group count
		SF_FORCEINLINE Result SetWorkGroupCount( size_t WorkGroupCount );
		SF_FORCEINLINE size_t GetWorkGroupCount() { return m_TaskGroups.size(); }

		// Initialize TickTaskManager
		virtual Result InitializeManager( uint uiNumGroup = 2 );

		// Terminate TickTaskManager
		virtual Result TerminateManager();

		Result RunOnTaskThread(SysUInt groupID, std::function<void()>&& pTaskFunction);

		// Add TickTask
		virtual Result AddTickTask( TickTask* pTask );

		// Remove TickTask
		virtual Result RemoveTickTask( TickTask *pTask );
	};


} // namespace SF

