////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity class implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "SvrConst.h"
#include "Entity/MasterEntity.h"
#include "Transaction/MessageRoute.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "SvrTrace.h"
#include "Task/ServerTaskEvent.h"
#include "Entity/EntityTimerActions.h"
#include "Server/BrServer.h"
#include "Net/SFMessage.h"




namespace SF {

	template class SharedPointerT<Svr::MasterEntity>;

namespace Svr
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Master Entity class
	//		: Entity with low transaction load
	//


	MasterEntity::MasterEntity( uint uiTransQueueSize, uint TransResQueueSize )
		: Entity( uiTransQueueSize, TransResQueueSize )
		, m_uiMaxActiveTransaction(1024)
		, m_activeTransactionScheduler(GetHeap())
		, m_activeTrans(GetHeap())
	{
		SetTickInterval(DurationMS(100));
		m_activeTransactionScheduler.SetAssertOnInvalidTickTime(true);
		m_activeTransactionScheduler.SetFailSafeTimerTickInterval(DurationMS(5000));
	}

	MasterEntity::~MasterEntity()
	{
		ClearEntity();
	}

	// change maximum active transaction
	void MasterEntity::SetMaxActiveTransaction( uint uiMaxActiveTransaction )
	{
		if( uiMaxActiveTransaction < 1 ) uiMaxActiveTransaction = 1;// atleast one can processe

		m_uiMaxActiveTransaction = uiMaxActiveTransaction;
	}



	void MasterEntity::ReleaseTransaction(TransactionPtr& pTrans)
	{
		ThreadID currentThreadID = ThisThread::GetThreadID();
		SharedPointerT<Transaction> pDeleted;

		if (pTrans == nullptr)
			return;

		svrTrace(SVR_DBGTRANS, "Transaction Release TID:{0}", pTrans->GetTransID());

		if (pTrans->GetTimerAction() && pTrans->GetTimerAction()->GetScheduledTime() != TimeStampMS::max())
			m_activeTransactionScheduler.RemoveTimerAction(currentThreadID, pTrans->GetTimerAction());

		// All master entity's transaction will be managed by shared pointer
		if ((m_activeTrans.Remove(pTrans->GetTransID().GetTransactionIndex(), pDeleted)))
		{
			Assert(pDeleted == pTrans);
		}
		else if (m_pExclusiveTransaction == pTrans)
		{
			// m_pExclusiveTransaction reference must be cleared before actual transaction instance is released
			pDeleted = std::forward<SharedPointerT<Transaction>>(m_pExclusiveTransaction);
			m_pExclusiveTransaction = SharedPointerT<Transaction>();
		}
		else
		{
			pDeleted = SharedPointerT<Transaction>(pTrans);
			// New transaction can not be in activetransaction list
			//Assert(false);
		}

		pDeleted = SharedPointerT<Transaction>();

		ValidateTransactionCount();
	}

	void MasterEntity::ValidateTransactionCount()
	{
#ifdef DEBUG
		auto transactionCount = m_activeTrans.GetWriteItemCount() + (m_pExclusiveTransaction == nullptr ? 0 : 1);
		Assert(m_activeTransactionScheduler.GetChangedItemCount() == transactionCount);
#endif
	}

	// clear transaction
	Result MasterEntity::ClearEntity()
	{
		// just drop transaction with freed entity
		m_activeTrans.ForeachOrder(0, (uint)m_activeTrans.size(), [](uint key, SharedPointerT<Transaction>& value)
		{
			// Force clear the transaction
			if (value != nullptr)
				value->SetClosed();
			return true;
		});
		m_activeTrans.ClearMap();

		// Force clear the transaction
		if (m_pExclusiveTransaction != nullptr)
		{
			m_pExclusiveTransaction->SetClosed();

			m_pExclusiveTransaction = SharedPointerT<Transaction>();
		}

		m_activeTransactionScheduler.Clear();

		return Entity::ClearEntity();
	}

	Result MasterEntity::FindActiveTransaction(const TransactionID& transID, TransactionPtr &pTransaction)
	{
		if (m_pExclusiveTransaction != nullptr && m_pExclusiveTransaction->GetTransID() == transID)
		{
			pTransaction = (Transaction*)m_pExclusiveTransaction;
			return ResultCode::SUCCESS;
		}

		SharedPointerT<Transaction> activeTrans;
		if ((m_activeTrans.Find(transID.GetTransactionIndex(), activeTrans)))
		{
			pTransaction = (Transaction*)activeTrans;
			return ResultCode::SUCCESS;
		}
		return ResultCode::FAIL;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Transaction handling
	//

	// Update entity process
	//  - Make new transaction from connection queue
	//  - status update for game
	//  - Process transaction
	Result MasterEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		TransactionPtr pNewTran;
		ThreadID currentThreadID = ThisThread::GetThreadID();
		TimeStampMS nextTick = TimeStampMS::max();

		if (m_activeTransactionScheduler.GetWorkingThreadID() == ThreadID())
		{
			// Working thread hasn't assigned. assign current threadid
			UpdateWorkingThreadID(currentThreadID);
		}

		if( GetEntityState() == EntityState::FREE )
		{
			//goto Proc_End;
			return ResultCode::SUCCESS_FALSE;
		}

		if( GetEntityState() == EntityState::CLOSING )
		{
			svrTrace( SVR_TRANSACTION, "Entity Close - Entity ID : {0} , name : {1}, State : {2}", GetEntityID(), typeid(*this).name(), GetEntityState() );
			svrChk( TerminateEntity() );
			hr = ResultCode::SUCCESS_FALSE;
			goto Proc_End;
		}

		ValidateTransactionCount();
		// Flush all commit before update
		m_activeTransactionScheduler.CommitChanges(currentThreadID);
		m_activeTrans.CommitChanges();

		// process pending queue
		while (m_activeTrans.size() < m_uiMaxActiveTransaction 
			&& m_pExclusiveTransaction == nullptr // No transaction will be issued if there is a active exclusive transaction.
			&& (GetTransactionQueue().Dequeue(pNewTran))
			)
		{
			TransactionID transID( (uint32_t)GetEntityID(), 0 );

			TransactionPtr pPrevTrans;
			do {
				transID.Components.TransID = (decltype(transID.Components.TransID))GenTransIndex();
			} while ((m_activeTrans.Find(transID.GetTransactionIndex(), pPrevTrans)));


			pNewTran->SetTransID( transID );



			ProcessTransaction(pNewTran);

			if (pNewTran == nullptr)
			{
				continue;
			}

			if (pNewTran->IsExclusive())
			{
				m_pExclusiveTransaction = pNewTran;
			}
			else
			{
				svrChk(m_activeTrans.Insert(pNewTran->GetTransID().GetTransactionIndex(), pNewTran));
			}

			auto timerAction = new(GetHeap()) TimerActionTransaction(pNewTran);
			pNewTran->SetTimerAction(timerAction);
			timerAction->SetNextTickTime(pNewTran);
			svrChk(m_activeTransactionScheduler.AddTimerAction(currentThreadID, timerAction));

			pNewTran = nullptr;
		}

		ValidateTransactionCount();

		nextTick = Util::Time.GetTimeMs() + GetTickInterval();

		// process transaction status
		m_activeTransactionScheduler.UpdateTick(currentThreadID);

		ValidateTransactionCount();

		m_activeTrans.CommitChanges();

		ValidateTransactionCount();


		// Exclusive transaction will be issued after all other transaction is processed
		if (m_pExclusiveTransaction != nullptr && m_activeTrans.size() == 0)
		{
			ProcessTransaction(m_pExclusiveTransaction);

			if (m_pExclusiveTransaction != nullptr)
			{
				auto transTickTime = std::min(m_pExclusiveTransaction->GetTimerExpireTime(), m_pExclusiveTransaction->GetHeartBitTimeout());
				nextTick = std::min(transTickTime, nextTick);
			}
		}


		m_activeTransactionScheduler.CommitChanges(currentThreadID);


		ValidateTransactionCount();

		nextTick = std::min(m_activeTransactionScheduler.GetNextTimeTick(), nextTick);
		SetNextScheduledTickTime(nextTick);


		if( GetEntityState() == EntityState::CLOSING )
		{
			svrTrace( SVR_TRANSACTION, "Entity Close - Entity ID : {0} , name : {1}, State : {2}", GetEntityID(), typeid(*this).name(), GetEntityState() );
			svrChk( TerminateEntity() );
			hr = ResultCode::SUCCESS_FALSE;
			goto Proc_End;
		}


	Proc_End:

		if(pNewTran != nullptr)
			ReleaseTransaction(pNewTran);

		return hr;
	}
	
	Result MasterEntity::ProcessTransactionResult(TransactionPtr &pCurTran, TransactionResult* &pTransRes)
	{
		Result hr = ResultCode::SUCCESS;
		Result hrTem = ResultCode::SUCCESS;
		ThreadID currentThreadID = ThisThread::GetThreadID();

		svrChkPtr(pCurTran);
		svrChkPtr(pTransRes);
		if( pCurTran->IsPrintTrace() )
		{
			svrTrace( SVR_DBGTRANS, "Trans Proc TID:{0}:{1}, Entity:{2}", pCurTran->GetTransID(), typeid(*pCurTran).name(), GetEntityUID() );
		}

		pCurTran->UpdateHeartBitTime();
		if( pCurTran->IsTimerWorking() ) pCurTran->ClearTimer();
		pCurTran->RecordTransactionHistory(pTransRes);
		hrTem = pCurTran->ProcessTransaction(pTransRes);

		if( !(hrTem) )// Transaction failed
		{
			if( pCurTran->IsPrintTrace() )
			{
				svrTrace( SVR_TRANSACTION, "Trans failed hr={0:X8}, TID:{1}:{2}, Entity:{3}", 
					hrTem,
					pCurTran->GetTransID(), 
					typeid(*pCurTran).name(),
					GetEntityUID() );
			}

			if( pCurTran->GetState() != Transaction::STATE_CLOSED )
				pCurTran->CloseTransaction( hrTem );
		}

		if (pCurTran->IsClosed())
		{
			ReleaseTransaction(pCurTran);
			pCurTran = nullptr;

			svrChk(TickUpdate());
		}
		else
		{
			auto pTimerAction = pCurTran->GetTimerAction();
			pTimerAction->SetNextTickTime(pCurTran->GetNextTickTime());
			svrChk(m_activeTransactionScheduler.Reschedul(currentThreadID, pTimerAction));
		}

		SetBestScehdulingTime(*pCurTran);

	Proc_End:

		IHeap::Delete(pTransRes);
		pTransRes = nullptr;

		return hr;
	}

	void MasterEntity::UpdateWorkingThreadID(ThreadID threadID)
	{
		m_activeTransactionScheduler.UpdateWorkingThreadID(threadID);
	}

	void MasterEntity::OnAddedToTaskManager(TaskWorker *pWorker)
	{
		auto workerThreadID = pWorker->GetThreadID();
		if (workerThreadID == ThreadID())
		{
			svrTrace(Warning, "MasterEntity:{0} is added but worker has invalid thread id.");
		}
		UpdateWorkingThreadID(pWorker->GetThreadID());
		Entity::OnAddedToTaskManager(pWorker);
	}

	Result MasterEntity::OnEventTask(ServerTaskEvent& eventTask)
	{
		Result hr = ResultCode::SUCCESS;
		TransactionPtr pCurTran;
		SharedPointerT<Net::Connection> pMyConn;

		switch (eventTask.EventType)
		{
		case ServerTaskEvent::EventTypes::CONNECTION_EVENT:
		case ServerTaskEvent::EventTypes::PACKET_MESSAGE_EVENT:
			svrErr(ResultCode::NOT_IMPLEMENTED);
			break;
		case ServerTaskEvent::EventTypes::PACKET_MESSAGE_SYNC_EVENT:
			eventTask.EventData.MessageEvent.pConn.GetSharedPointer(pMyConn);
			if (pMyConn != nullptr) pMyConn->UpdateSendQueue();
			break;
		case ServerTaskEvent::EventTypes::PACKET_MESSAGE_SEND_EVENT:
			eventTask.EventData.MessageEvent.pConn.GetSharedPointer(pMyConn);
			if (pMyConn != nullptr) pMyConn->UpdateSendBufferQueue();
			break;
		case ServerTaskEvent::EventTypes::TRANSRESULT_EVENT:
			if (eventTask.EventData.pTransResultEvent != nullptr)
			{
				if ((FindActiveTransaction(eventTask.EventData.pTransResultEvent->GetTransID(), pCurTran)))
				{
					ProcessTransactionResult(pCurTran, const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent));
				}
				else
				{
					svrTrace(SVR_TRANSACTION, "Transaction result for TID:{0} is failed to route.", eventTask.EventData.pTransResultEvent->GetTransID());
					auto pNonConstTransRes = const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent);
					IHeap::Delete(pNonConstTransRes);
					svrErr(ResultCode::FAIL);
				}
			}
			else
			{
				svrTrace(SVR_TRANSACTION, "Failed to process transaction result. null Transaction result.");
			}
			break;
		default:
			svrErr(ResultCode::UNEXPECTED);
		}

	Proc_End:

		return hr;
	}

	uint MasterEntity::GetActiveTransactionCount()
	{
		return (uint)m_activeTrans.size() + (m_pExclusiveTransaction == nullptr ? 0 : 1);
	}

}; // namespace Svr
}; // namespace SF



