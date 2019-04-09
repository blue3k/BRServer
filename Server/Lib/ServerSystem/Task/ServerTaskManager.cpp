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
#include "ServerLog/SvrLog.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Thread/SFThread.h"
#include "Util/SFTimeUtil.h"
#include "Task/ServerTaskManager.h"
//#include "Common/SvrConst.h"
#include "Task/ServerTaskEvent.h"
//#include "Task/EntityTimerActions.h"


namespace SF {



	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask exection Group class -  interface for task operation
	//


	// Constructor/Destructor
	TaskWorker::TaskWorker( TickTaskManager* pTaskManager, int iQueuePageSize )
		: m_pTaskManager(pTaskManager)
		, m_GroupWorkLoad(0)
		, m_GroupWorkLoadDiff(0)
		, m_GroupID(0)
		, m_ulLoopInterval(20)
		, m_EventTask(GetSystemHeap())
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

		return m_EventTask.Enqueue(pEvtTask);
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
					defTrace(Error, "Failed to insert a tick task:{0}", pTask->GetTaskID());
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
					defTrace(Warning, "Failed to insert a unique task:{0}, task already exists, douplicated task will be exists", pTask->GetTaskID());
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
						defTrace(Error, "Failed to remove a task:{0}", pTask->GetTaskID());
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
							defTrace(Debug1, "Remove is failed, retry task:{0}", pTask->GetTaskID());
							continue;
						}
						else
						{
							defTrace(Error, "Remove is failed, delete the task:{0}", pTask->GetTaskID());
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
						defTrace(Debug1, "Remove is failed, retry task:{0}", pTask->GetTaskID());
						continue;
					}
					else
					{
						defTrace(Error, "Try to remove not-existing task:{0}, taskgroup:{1}, from taskgroup:{2}", pTask->GetTaskID(), pTask->GetTaskGroupID(), GetGroupID());
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
			pEvtTask.TaskPtr.GetSharedPointer(tickTask);
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

			if (!(tickTask->OnEventTask(pEvtTask)))
			{
				defTrace(Error, "ServerTaskEvent is failed, Evt:{0}", (uint)pEvtTask.EventType.load(std::memory_order_relaxed))
			}

			m_TimeScheduler.Reschedul(threadID, tickTask->GetTimerAction());
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

			//GetTimeScheduler().CommitChanges();

			UpdateTimer();

			//UpdateTickTask();

		} // while
	}





	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask Manager base class -  interface for task manager
	//



	SyncCounter	TickTaskManager::stm_GroupIDGen;


	// Constructor/Destructor
	TickTaskManager::TickTaskManager()
		: m_TaskGroups(GetSystemHeap())
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

			defMem( pGroup = new(GetSystemHeap()) TaskWorker(this) );

			pGroup->SetGroupID(iGroup + 1);
			m_TaskGroups.push_back(pGroup);
		}

	Proc_End:

		return hr;
	}


	// Initialize TickTaskManager
	Result TickTaskManager::InitializeManager( uint uiNumGroup )
	{
		SetWorkGroupCount( uiNumGroup );

		m_TaskGroups.for_each([](TaskWorker *pTaskWorker)
		{
			pTaskWorker->Start();
			return ResultCode::SUCCESS;
		});

		return ResultCode::SUCCESS;
	}

	// Terminate TickTaskManager
	Result TickTaskManager::TerminateManager()
	{

		m_TaskGroups.for_each([](TaskWorker *pTaskWorker)
		{
			if (pTaskWorker->GetThreadID() != ThreadID())
				pTaskWorker->Stop(true);
			IHeap::Delete(pTaskWorker);
			return ResultCode::SUCCESS;
		});

		m_TaskGroups.Clear();

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

		defChk(m_TaskGroups[groupID - 1]->AddEventTask(std::forward<ServerTaskEvent>(pEvtTask)));

	Proc_End:

		return hr;
	}

	// Add TickTask
	Result TickTaskManager::AddTickTask( TickTask* pTask )
	{
		Result hr = ResultCode::SUCCESS;
		SysUInt BestGroupLoad = (SysUInt)-1;
		TaskWorker *pBestGroup = nullptr;

		// search best match
		m_TaskGroups.for_each([&](TaskWorker *pTaskWorker)
		{
			if (pTaskWorker == nullptr)
				return ResultCode::SUCCESS;

			SysUInt GroupLoad = pTaskWorker->GetGroupWorkLoad();
			if( GroupLoad < BestGroupLoad )
			{
				BestGroupLoad = GroupLoad;
				pBestGroup = pTaskWorker;
			}
			return ResultCode::SUCCESS;
		});


		if( pBestGroup == nullptr && m_TaskGroups.size() )
		{
			pBestGroup = m_TaskGroups[0];
		}

		defChkPtr(pBestGroup);

		defChk( pBestGroup->PendingAddTask( pTask ) );

	Proc_End:

		return hr;
	}

	// Remove TickTask
	Result TickTaskManager::RemoveTickTask( TickTask *pTask )
	{
		Result hr = ResultCode::SUCCESS;
		SysUInt groupID;

		defChkPtr(pTask);

		groupID = pTask->GetTaskGroupID();
		if (groupID <= 0 || groupID > m_TaskGroups.size())
			return ResultCode::UNEXPECTED;

		defChk(m_TaskGroups[groupID - 1]->PendingRemoveTask(pTask));

	Proc_End:

		return hr;
	}







}; // namespace SF

