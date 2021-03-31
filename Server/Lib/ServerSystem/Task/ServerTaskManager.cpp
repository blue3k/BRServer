////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Server TickTask manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "ServerLog/SvrLog.h"
#include "SvrTrace.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Multithread/SFThread.h"
#include "Util/SFTimeUtil.h"
#include "Task/ServerTaskManager.h"
#include "Task/ServerTaskEvent.h"


namespace SF {



	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask execution Group class -  interface for task operation
	//


	// Constructor/Destructor
	TaskWorker::TaskWorker( TickTaskManager* pTaskManager, int iQueuePageSize )
		: m_pTaskManager(pTaskManager)
		, m_GroupWorkLoad(0)
		, m_GroupWorkLoadDiff(0)
		, m_GroupID(0)
		, m_ulLoopInterval(3)
		, m_EventTask(GetSystemHeap())
		, m_TaskFunctions(GetSystemHeap())
		, m_PendingAddTask(GetSystemHeap(), iQueuePageSize)
		, m_PendingRemoveTask(GetSystemHeap(), iQueuePageSize)
		, m_TaskList(GetSystemHeap())
		, m_TimeScheduler(GetSystemHeap())
	{
	}


	TaskWorker::~TaskWorker()
	{
		m_PendingAddTask.ClearQueue();
		m_PendingRemoveTask.ClearQueue();

		m_TaskList.ClearMap();
	}

	// Add event task
	Result TaskWorker::AddEventTask(ServerTaskEvent&& pEvtTask)
	{
		if (pEvtTask.EventType == ServerTaskEvent::EventTypes::NONE)
			return ResultCode::INVALID_ARG;

		return m_EventTask.Enqueue(Forward<ServerTaskEvent>(pEvtTask));
	}

	Result TaskWorker::AddTaskFunction(std::function<void()>&& pTaskFunction)
	{
		return m_TaskFunctions.Enqueue(Forward<std::function<void()>>(pTaskFunction));
	}

	// Add task
	Result TaskWorker::PendingAddTask( TickTask* pTask )
	{
		pTask->SetTaskGroupID( GetGroupID() );
		pTask->SetTaskManager(m_pTaskManager);
		m_GroupWorkLoadDiff += pTask->GetTaskLoad();

		return m_PendingAddTask.Enqueue( pTask );
	}


	// Remove task
	Result TaskWorker::PendingRemoveTask( TickTask* pTask )
	{
		Result hr = m_PendingRemoveTask.Enqueue( pTask );

		if( (hr) )
			m_GroupWorkLoadDiff -= pTask->GetTaskLoad();

		return hr;
	}

	Result TaskWorker::UpdateAddTickTaskQueue()
	{
		auto loopCount = m_PendingAddTask.GetEnqueCount();
		for (; loopCount > 0; loopCount--)
		{
			SharedPointerT<TickTask> pTask;
			if (m_PendingAddTask.Dequeue(pTask) == Result(ResultCode::SUCCESS))
			{
				Assert(pTask->GetTaskID() != 0);
				if (pTask->GetTaskID() == 0)
				{
					svrTrace(Error, "Failed to insert a tick task:{0}", pTask->GetTaskID());
				}

				SharedPointerT<TickTask> pFound;
				if ((m_TaskList.Find(pTask->GetTaskID(), pFound)))
				{
					if (pTask->GetRetryCount())
					{
						pTask->IncRetryCount();
						m_PendingAddTask.Enqueue(pTask);
						continue;
					}

					// maximum retry is exceeded, just add
					svrTrace(Warning, "Failed to insert a unique task:{0}, task already exists, douplicated task will be exists", pTask->GetTaskID());
				}

				pTask->ResetRetryCount();
				m_TaskList.Insert(pTask->GetTaskID(), pTask);

				pTask->OnAddedToTaskManager(this);
				pTask->TickUpdate();
				// If it need to be scheduled
				if (pTask->GetTimerAction() && pTask->GetTickInterval() != DurationMS(0))
				{
					GetTimeScheduler().AddTimerAction(GetThreadID(), pTask->GetTimerAction());
				}
				else
				{
					Assert(pTask->GetTickInterval() == DurationMS(0));
				}

			}
		}

		return ResultCode::SUCCESS;
	}

	Result TaskWorker::UpdateRemoveTickTaskQueue()
	{
		StaticArray<SharedPointerT<TickTask>, 256> removedObjects(GetSystemHeap());
		auto loopCount = m_PendingRemoveTask.GetEnqueCount();
		for (; loopCount > 0; loopCount--)
		{
			SharedPointerT<TickTask> pTask;
			if (m_PendingRemoveTask.Dequeue(pTask) == Result(ResultCode::SUCCESS))
			{
				SharedPointerT<TickTask> pFound;
				if ((m_TaskList.Find(pTask->GetTaskID(), pFound)))
				{
					if (!(m_TaskList.Remove(pTask->GetTaskID(), pFound)))
					{
						svrTrace(Error, "Failed to remove a task:{0}", pTask->GetTaskID());
					}
					else if (pTask != pFound)
					{
						// insert it back
						m_TaskList.Insert(pFound->GetTaskID(), pFound);

						if (pTask->GetRetryCount() < MAX_TASK_RETRY)
						{
							pTask->IncRetryCount();

							// pending again
							m_PendingRemoveTask.Enqueue(pTask);
							svrTrace(Debug1, "Remove is failed, retry task:{0}", pTask->GetTaskID());
							continue;
						}
						else
						{
							svrTrace(Error, "Remove is failed, delete the task:{0}", pTask->GetTaskID());
							// throw it away, delete can cause crash
							continue;
						}
					}

					m_TimeScheduler.RemoveTimerAction(ThisThread::GetThreadID(), pTask->GetTimerAction());
					pTask->OnRemovedFromTaskManager(this);
					pTask->SetTaskGroupID(0);
					removedObjects.push_back(std::forward<SharedPointerT<TickTask>>(pTask));
				}
				else
				{
					if (pTask->GetRetryCount() < MAX_TASK_RETRY)
					{
						pTask->IncRetryCount();
						m_PendingRemoveTask.Dequeue(pTask);
						svrTrace(Debug1, "Remove is failed, retry task:{0}", pTask->GetTaskID());
						continue;
					}
					else
					{
						svrTrace(Error, "Try to remove not-existing task:{0}, taskgroup:{1}, from taskgroup:{2}", pTask->GetTaskID(), pTask->GetTaskGroupID(), GetGroupID());
						// throw it away, delete can cause crash
					}
				}
			}
		}

		// Commit changes before actually release object pointers, so that the references of deleted objects are cleared
		m_TimeScheduler.CommitChanges(ThisThread::GetThreadID());
		removedObjects.Clear();

		return ResultCode::SUCCESS;
	}

	Result TaskWorker::UpdateTickTask()
	{
		// Update tasks and workload
		SysUInt WorkLoad = 0;
		m_TaskList.ForeachOrder(0, INT_MAX, [&WorkLoad](uint64_t taskID, const SharedPointerT<TickTask>& pTask)-> bool
		{
			pTask->TickUpdate();
			WorkLoad += pTask->GetTaskLoad();
			return true;
		});

		m_GroupWorkLoad = WorkLoad;
		m_GroupWorkLoadDiff = 0;

		return ResultCode::SUCCESS;
	}

	Result TaskWorker::UpdateEventTask()
	{
		auto threadID = GetThreadID();
		auto loopCount = m_EventTask.GetEnqueCount();
		for (; loopCount > 0; loopCount--)
		{
			ServerTaskEvent pEvtTask;
			if (!(m_EventTask.Dequeue(pEvtTask)))
				break;

			SharedPointerT<TickTask> tickTask;
			tickTask = pEvtTask.TaskPtr.AsSharedPtr();
			if (tickTask == nullptr)
				continue;

			switch (pEvtTask.EventType)
			{
			case ServerTaskEvent::EventTypes::POKE_TICK_EVENT:
				tickTask->TickUpdate();
				continue;
			default:
				break;
			};

			if (!tickTask->OnEventTask(pEvtTask))
			{
				svrTrace(Error, "ServerTaskEvent is failed, Evt:{0}", (uint)pEvtTask.EventType.load(std::memory_order_relaxed))
			}

			m_TimeScheduler.Reschedul(threadID, tickTask->GetTimerAction());
		}

		return ResultCode::SUCCESS;
	}

	Result TaskWorker::UpdateTaskFunction()
	{
		auto loopCount = m_TaskFunctions.GetEnqueCount();
		for (; loopCount > 0; loopCount--)
		{
			std::function<void()> pTaskFunction;
			if (!(m_TaskFunctions.Dequeue(pTaskFunction)))
				break;

			pTaskFunction();
		}

		return ResultCode::SUCCESS;
	}

	Result TaskWorker::UpdateTimer()
	{
		m_TimeScheduler.UpdateTick(std::this_thread::get_id());
		return ResultCode::SUCCESS;
	}

	// run function
	void TaskWorker::Run()
	{
		m_TimeScheduler.UpdateWorkingThreadID(std::this_thread::get_id());

		while( 1 )
		{
			DurationMS loopInterval = UpdateInterval( GetBaseLoopInterval() );
			
			if (CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				break;
			}

			UpdateRemoveTickTaskQueue();

			UpdateAddTickTaskQueue();

			UpdateEventTask();

			UpdateTimer();

		} // while
	}


	//////////////////////////////////////////////////////////
	// Single task worker

	SingleTaskWorker::SingleTaskWorker(TickTask* pTask)
		: m_pTask(pTask)
	{
	}

	bool SingleTaskWorker::Tick()
	{
		if (m_pTask)
			m_pTask->TickUpdate();

		return true;
	}



	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask Manager base class -  interface for task manager
	//



	SyncCounter	TickTaskManager::stm_GroupIDGen;


	// Constructor/Destructor
	TickTaskManager::TickTaskManager()
		: m_TaskGroups(GetSystemHeap())
		, m_SingleTaskWorkers(GetSystemHeap())
	{
	}

	TickTaskManager::~TickTaskManager()
	{
		TerminateManager();
	}

	// Get/Set Working group count
	Result TickTaskManager::SetWorkGroupCount( size_t WorkGroupCount )
	{
		Result hr = ResultCode::SUCCESS;

		// decrease case not allowed
		if( m_TaskGroups.size() >= WorkGroupCount )
			return ResultCode::INVALID_ARG;

		for (size_t iGroup = m_TaskGroups.size(); iGroup < WorkGroupCount; iGroup++)
		{
			TaskWorker *pGroup = nullptr;

			svrCheckMem( pGroup = new(GetSystemHeap()) TaskWorker(this) );

			pGroup->SetGroupID(iGroup + 1);
			m_TaskGroups.push_back(pGroup);
		}

		return hr;
	}


	// Initialize TickTaskManager
	Result TickTaskManager::InitializeManager( uint uiNumGroup )
	{
		SetWorkGroupCount( uiNumGroup );

		for (TaskWorker* pTaskWorker : m_TaskGroups)
		{
			pTaskWorker->Start();
		}

		return ResultCode::SUCCESS;
	}

	// Terminate TickTaskManager
	Result TickTaskManager::TerminateManager()
	{
		for (auto itThread : m_TaskGroups)
		{
			if (itThread->GetThreadID() != ThreadID())
				itThread->Stop(true);

			IHeap::Delete(itThread);
		}
		m_TaskGroups.Clear();


		for (auto itThread : m_SingleTaskWorkers)
		{
			if (itThread->GetThreadID() != ThreadID())
				itThread->Stop(true);

			IHeap::Delete(itThread);
		}
		m_SingleTaskWorkers.Clear();


		return ResultCode::SUCCESS;
	}


	// Add event task
	Result TickTaskManager::AddEventTask(SysUInt groupID, ServerTaskEvent&& pEvtTask)
	{
		Result hr = ResultCode::SUCCESS;

		if (groupID <= 0 || groupID > m_TaskGroups.size())
		{
			return ResultCode::SVR_INVALID_TASK_GROUPID;
		}

		svrCheck(m_TaskGroups[groupID - 1]->AddEventTask(std::forward<ServerTaskEvent>(pEvtTask)));

		return hr;
	}

	Result TickTaskManager::RunOnTaskThread(SysUInt groupID, std::function<void()>&& pTaskFunction)
	{
		Result hr = ResultCode::SUCCESS;

		if (groupID <= 0 || groupID > m_TaskGroups.size())
		{
			return ResultCode::SVR_INVALID_TASK_GROUPID;
		}

		svrCheck(m_TaskGroups[groupID - 1]->AddTaskFunction(std::forward<std::function<void()>>(pTaskFunction)));

		return hr;
	}

	// Add TickTask
	Result TickTaskManager::AddTickTask(TickTask* pTask)
	{
		Result hr = ResultCode::SUCCESS;

		if (pTask->UseDesignatedThread())
		{
			auto newThread = new(GetSystemHeap()) SingleTaskWorker(pTask);
			svrCheckPtr(newThread);
			newThread->Start();
			m_SingleTaskWorkers.push_back(newThread);
		}
		else
		{
			SysUInt BestGroupLoad = (SysUInt)-1;
			TaskWorker* pBestGroup = nullptr;

			// search best match
			for (TaskWorker* pTaskWorker : m_TaskGroups)
			{
				if (pTaskWorker == nullptr)
					continue;

				SysUInt GroupLoad = pTaskWorker->GetGroupWorkLoad();
				if (GroupLoad < BestGroupLoad)
				{
					BestGroupLoad = GroupLoad;
					pBestGroup = pTaskWorker;
				}
			}


			if (pBestGroup == nullptr && m_TaskGroups.size())
			{
				pBestGroup = m_TaskGroups[0];
			}

			svrCheckPtr(pBestGroup);

			svrCheck(pBestGroup->PendingAddTask(pTask));
		}

		return hr;
	}

	// Remove TickTask
	Result TickTaskManager::RemoveTickTask( TickTask *pTask )
	{
		Result hr = ResultCode::SUCCESS;
		SysUInt groupID;

		svrCheckPtr(pTask);

		groupID = pTask->GetTaskGroupID();
		if (groupID <= 0 || groupID > m_TaskGroups.size())
			return ResultCode::UNEXPECTED;

		svrCheck(m_TaskGroups[groupID - 1]->PendingRemoveTask(pTask));

		return hr;
	}

} // namespace SF

