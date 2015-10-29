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
#include "ServerSystem/svrTrace.h"
#include "Common/Thread.h"
#include "Common/TimeUtil.h"
#include "ServerSystem/TaskManager.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/EventTask.h"
#include "ServerSystem/EntityTimerActions.h"


namespace BR {
namespace Svr {


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TickTask exection Group class -  interface for task operation
	//


	// Constructor/Destructor
	TaskWorker::TaskWorker( TaskManager* pTaskManager, int iQueuePageSize )
		:m_pTaskManager(pTaskManager),
		m_PendingAddTask(iQueuePageSize),
		m_PendingRemoveTask(iQueuePageSize),
		m_ulLoopInterval(20)
	{
	}


	TaskWorker::~TaskWorker()
	{
		m_PendingAddTask.ClearQueue();
		//while( m_PendingAddTask.GetEnqueCount() > 0 )
		//{
		//	SharedPointerT<TickTask> pTask = nullptr;
		//	if( m_PendingAddTask.Dequeue( pTask ) == S_OK )
		//		delete pTask;
		//}

		m_PendingRemoveTask.ClearQueue();

		m_TaskList.ClearMap();
	}

	// Add event task
	HRESULT TaskWorker::AddEventTask(EventTask&& pEvtTask)
	{
		if (pEvtTask.EventType == EventTask::EventTypes::NONE)
			return E_INVALIDARG;

		return m_EventTask.Enqueue(pEvtTask);
	}

	// Add task
	HRESULT TaskWorker::PendingAddTask( TickTask* pTask )
	{
		pTask->SetTaskGroupID( GetGroupID() );
		pTask->SetTaskManager(m_pTaskManager);
		m_GroupWorkLoadDiff += pTask->GetTaskLoad();

		return m_PendingAddTask.Enqueue( pTask );
	}


	// Remove task
	HRESULT TaskWorker::PendingRemoveTask( TickTask* pTask )
	{
		HRESULT hr = m_PendingRemoveTask.Enqueue( pTask );

		if( SUCCEEDED(hr) )
			m_GroupWorkLoadDiff -= pTask->GetTaskLoad();

		return hr;
	}

	HRESULT TaskWorker::UpdateAddTickTaskQueue()
	{
		auto loopCount = m_PendingAddTask.GetEnqueCount();
		for (; loopCount > 0; loopCount--)
		{
			SharedPointerT<TickTask> pTask;
			if (m_PendingAddTask.Dequeue(pTask) == S_OK)
			{
				Assert(pTask->GetTaskID() != 0);
				if (pTask->GetTaskID() == 0)
				{
					svrTrace(Trace::TRC_ERROR, "Failed to insert a tick task:%0%", pTask->GetTaskID());
				}

				SharedPointerT<TickTask> pFound;
				if (SUCCEEDED(m_TaskList.Find(pTask->GetTaskID(), pFound)))
				{
					if (pTask->GetRetryCount())
					{
						pTask->IncRetryCount();
						m_PendingAddTask.Enqueue(pTask);
						continue;
					}

					// maximum retry is exceeded, just add
					svrTrace(Trace::TRC_WARN, "Failed to insert a unique task:%0%, task already exists, douplicated task will be exists", pTask->GetTaskID());
				}

				pTask->ResetRetryCount();
				m_TaskList.Insert(pTask->GetTaskID(), pTask);

				pTask->OnAddedToTaskManager(this);
				pTask->TickUpdate();
				if (pTask->GetTimerAction() && pTask->GetTimerAction()->GetNexTickTime() != TimeStampMS::max())
				{
					GetTimeScheduler().AddTimerAction(GetThreadID(), pTask->GetTimerAction());
				}
				else
				{
					Assert(pTask->GetTickInterval() == DurationMS(0));
				}

			}
		}

		return S_OK;
	}

	HRESULT TaskWorker::UpdateRemoveTickTaskQueue()
	{
		auto loopCount = m_PendingRemoveTask.GetEnqueCount();
		for (; loopCount > 0; loopCount--)
		{
			SharedPointerT<TickTask> pTask;
			if (m_PendingRemoveTask.Dequeue(pTask) == S_OK)
			{
				SharedPointerT<TickTask> pFound;
				if (SUCCEEDED(m_TaskList.Find(pTask->GetTaskID(), pFound)))
				{
					if (FAILED(m_TaskList.Remove(pTask->GetTaskID(), pFound)))
					{
						svrTrace(Trace::TRC_ERROR, "Failed to remove a task:%0%", pTask->GetTaskID());
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
							svrTrace(Svr::TRC_DBGSVR, "Remove is failed, retry task:%0%", pTask->GetTaskID());
							continue;
						}
						else
						{
							svrTrace(Trace::TRC_ERROR, "Remove is failed, delete the task:%0%", pTask->GetTaskID());
							// throw it away, delete can cause crash
							continue;
						}
					}

					m_TimeScheduler.RemoveTimerAction(GetThreadID(), pTask->GetTimerAction());
					pTask->OnRemovedFromTaskManager(this);
					pTask->SetTaskGroupID(0);

					//delete pTask;
				}
				else
				{
					if (pTask->GetRetryCount() < MAX_TASK_RETRY)
					{
						pTask->IncRetryCount();
						m_PendingRemoveTask.Dequeue(pTask);
						svrTrace(Svr::TRC_DBGSVR, "Remove is failed, retry task:%0%", pTask->GetTaskID());
						continue;
					}
					else
					{
						svrTrace(Trace::TRC_ERROR, "Try to remove not-existing task:%0%, taskgroup:%1%, from taskgroup:%2%", pTask->GetTaskID(), pTask->GetTaskGroupID(), GetGroupID());
						// throw it away, delete can cause crash
					}
				}
			}
		}


		return S_OK;
	}

	HRESULT TaskWorker::UpdateTickTask()
	{
		// Update tasks and workload
		SysUInt WorkLoad = 0;
		m_TaskList.ForeachOrder(0, INT_MAX, [&WorkLoad](UINT64 taskID, const SharedPointerT<TickTask>& pTask)-> bool
		{
			pTask->TickUpdate();
			WorkLoad += pTask->GetTaskLoad();
			return true;
		});

		m_GroupWorkLoad = WorkLoad;
		m_GroupWorkLoadDiff = 0;

		return S_OK;
	}

	HRESULT TaskWorker::UpdateEventTask()
	{
		auto threadID = GetThreadID();
		auto loopCount = m_EventTask.GetEnqueCount();
		for (; loopCount > 0; loopCount--)
		{
			EventTask pEvtTask;
			if (FAILED(m_EventTask.Dequeue(pEvtTask)))
				break;

			SharedPointerT<TickTask> tickTask;
			pEvtTask.TaskPtr.GetSharedPointer(tickTask);
			if (tickTask == nullptr)
				continue;

			switch (pEvtTask.EventType)
			{
			case EventTask::EventTypes::POKE_TICK_EVENT:
				tickTask->TickUpdate();
				continue;
			};

			if (FAILED(tickTask->OnEventTask(pEvtTask)))
			{
				svrTrace(Svr::TRC_ENTITY, "EventTask is failed, Evt:%0%", (UINT)pEvtTask.EventType.load(std::memory_order_relaxed))
			}

			m_TimeScheduler.Reschedul(threadID, tickTask->GetTimerAction());
		}

		return S_OK;
	}

	HRESULT TaskWorker::UpdateTimer()
	{
		m_TimeScheduler.UpdateTick(GetThreadID());
		return S_OK;
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



	BR::SyncCounter	TaskManager::stm_GroupIDGen;


	// Constructor/Destructor
	TaskManager::TaskManager()
	{
	}

	TaskManager::~TaskManager()
	{
		TerminateManager();
	}

	// Get/Set Working group count
	HRESULT TaskManager::SetWorkGroupCount( size_t WorkGroupCount )
	{
		HRESULT hr = S_OK;

		// decrease case not allowed
		if( m_TaskGroups.size() >= WorkGroupCount )
			return E_ABORT;

		for (size_t iGroup = m_TaskGroups.size(); iGroup < WorkGroupCount; iGroup++)
		{
			TaskWorker *pGroup = nullptr;

			svrMem( pGroup = new TaskWorker(this) );

			pGroup->SetGroupID(iGroup + 1);
			m_TaskGroups.push_back(pGroup);
		}

	Proc_End:

		return hr;
	}


	// Initialize TaskManager
	HRESULT TaskManager::InitializeManager( UINT uiNumGroup )
	{
		SetWorkGroupCount( uiNumGroup );

		m_TaskGroups.Foreach([](TaskWorker *pTaskWorker)
		{
			pTaskWorker->Start();
			return S_OK;
		});

		return S_OK;
	}

	// Terminate TaskManager
	HRESULT TaskManager::TerminateManager()
	{

		m_TaskGroups.Foreach([](TaskWorker *pTaskWorker)
		{
			if (pTaskWorker->GetThreadID() != ThreadID())
				pTaskWorker->Stop(true);
			delete pTaskWorker;
			return S_OK;
		});

		m_TaskGroups.Clear();

		return S_OK;
	}


	// Add event task
	HRESULT TaskManager::AddEventTask(SysUInt groupID, EventTask&& pEvtTask)
	{
		if (groupID <= 0 || groupID > m_TaskGroups.GetSize())
			return E_UNEXPECTED;

		return m_TaskGroups[groupID - 1]->AddEventTask(std::forward<EventTask>(pEvtTask));
	}

	// Add TickTask
	HRESULT TaskManager::AddTickTask( TickTask* pTask )
	{
		HRESULT hr = S_OK;
		SysUInt BestGroupLoad = (SysUInt)-1;
		TaskWorker *pBestGroup = nullptr;

		// search best match
		m_TaskGroups.Foreach([&](TaskWorker *pTaskWorker)
		{
			if (pTaskWorker == nullptr)
				return S_OK;

			SysUInt GroupLoad = pTaskWorker->GetGroupWorkLoad();
			if( GroupLoad < BestGroupLoad )
			{
				BestGroupLoad = GroupLoad;
				pBestGroup = pTaskWorker;
			}
			return S_OK;
		});


		if( pBestGroup == nullptr && m_TaskGroups.size() )
		{
			pBestGroup = m_TaskGroups[0];
		}

		svrChkPtr(pBestGroup);

		svrChk( pBestGroup->PendingAddTask( pTask ) );

	Proc_End:

		return hr;
	}

	// Remove TickTask
	HRESULT TaskManager::RemoveTickTask( TickTask *pTask )
	{
		HRESULT hr = S_OK;

		svrChkPtr(pTask);

		auto groupID = pTask->GetTaskGroupID();
		if (groupID <= 0 || groupID > m_TaskGroups.GetSize())
			return E_UNEXPECTED;

		svrChk(m_TaskGroups[groupID - 1]->PendingRemoveTask(pTask));

	Proc_End:

		return hr;
	}






}; // namespace Svr
}; // namespace Net

