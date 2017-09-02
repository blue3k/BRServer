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
#include "ResultCode/SFResultCodeLibrary.h"
#include "Thread/Thread.h"
#include "Thread/Synchronization.h"
#include "Container/SortedMap.h"
#include "Container/DualSortedMap.h"
#include "Container/PageQueue.h"
#include "Task/ServerTask.h"
#include "ServerLog/SvrLog.h"
#include "Container/Indexing.h"
#include "Container/HashTable.h"
#include "Task/TimerScheduler.h"



namespace SF {


	class TaskManager;
	struct ServerTaskEvent;


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask exection Group class -  interface for task operation
	//

	class TaskWorker : public Thread
	{
	public:
		// task list type
		typedef SortedMap<UINT64, SharedPointerT<TickTask>>	TaskList;

		const int TICK_UPDATE_TIME = 20;
		const UINT MAX_TASK_RETRY = 30 * 1000 / TICK_UPDATE_TIME; // retry for 30 sec

	private:
		// TaskManager that include this group
		TaskManager*			m_pTaskManager;

		// Load for 
		SysUInt		m_GroupWorkLoad;
		SysUInt		m_GroupWorkLoadDiff;

		// Group ID
		SysUInt		m_GroupID;

		// Base loop delay
		DurationMS					m_ulLoopInterval;

		PageQueue<ServerTaskEvent>		m_EventTask;

		// TickTask pending queue
		PageQueue<SharedPointerT<TickTask>>		m_PendingAddTask;

		// TickTask Remove queue
		PageQueue<SharedPointerT<TickTask>>		m_PendingRemoveTask;

		// task list for update
		TaskList				m_TaskList;

		TimeScheduler			m_TimeScheduler;

	private:

		Result UpdateAddTickTaskQueue();
		Result UpdateRemoveTickTaskQueue();
		Result UpdateTickTask();
		Result UpdateEventTask();
		Result UpdateTimer();

	public:

		// Constructor/Destructor
		//	iQueuePageSize : Add/Remove queue size
		TaskWorker( TaskManager* pTaskManager, int iQueuePageSize = -1 );
		virtual ~TaskWorker();

		// Get/Set Group Idx
		inline SysUInt GetGroupID();
		inline void SetGroupID( SysUInt ulGroupID );

		// Get Group workload
		inline SysUInt GetGroupWorkLoad();

		// Set base loop delay
		inline void SetBaseLoopInterval(DurationMS dwLoopInterval );
		inline DurationMS GetBaseLoopInterval();

		TimeScheduler& GetTimeScheduler()											{ return m_TimeScheduler; }

		// Add event task
		Result AddEventTask(ServerTaskEvent&& pEvtTask);

		// Add task
		Result PendingAddTask( TickTask* pTask );

		// Remove task
		Result PendingRemoveTask( TickTask* pTask );

		// run function
		virtual void Run() override;
	};




	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask Manager base class -  interface for task manager
	//

	class TaskManager
	{
	public:

		//////////////////////////////////////////////////////////////////
		// Hash definition

		// TickTask groups
		//typedef std::unordered_map<SysUInt,TaskWorker*> TaskGroupMap;


	private:

		// GroupID gen
		static SyncCounter	stm_GroupIDGen;

	protected:

		// TickTask groups
		//TaskGroupMap	m_TaskGroups;
		DynamicArray<TaskWorker*>	m_TaskGroups;

	public:
		// Constructor/Destructor
		TaskManager();
		virtual ~TaskManager();

		// Get/Set Working group count
		Result SetWorkGroupCount( size_t WorkGroupCount );
		inline size_t GetWorkGroupCount();

		// Initialize TaskManager
		virtual Result InitializeManager( UINT uiNumGroup = 2 );

		// Terminate TaskManager
		virtual Result TerminateManager();

		// Add event task
		Result AddEventTask(SysUInt groupID, ServerTaskEvent&& pEvtTask);

		// Add TickTask
		virtual Result AddTickTask( TickTask* pTask );

		// Remove TickTask
		virtual Result RemoveTickTask( TickTask *pTask );
	};





#include "TaskManager.inl"



}; // namespace SF

