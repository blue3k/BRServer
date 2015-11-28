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


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/MasterEntity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EventTask.h"
#include "ServerSystem/EntityTimerActions.h"
#include "ServerSystem/BRServer.h"
#include "Common/Message.h"




namespace BR {

	template class SharedPointerT<Svr::MasterEntity>;

namespace Svr
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Master Entity class
	//		: Entity with low transaction load
	//


	MasterEntity::MasterEntity( UINT uiTransQueueSize, UINT TransResQueueSize )
		: Entity( uiTransQueueSize, TransResQueueSize )
		, m_uiMaxActiveTransaction(1024)
		, m_pExclusiveTransaction(nullptr)
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
	void MasterEntity::SetMaxActiveTransaction( UINT uiMaxActiveTransaction )
	{
		if( uiMaxActiveTransaction < 1 ) uiMaxActiveTransaction = 1;// atleast one can processe

		m_uiMaxActiveTransaction = uiMaxActiveTransaction;
	}



	void MasterEntity::ReleaseTransaction(Transaction* pTrans)
	{
		ThreadID currentThreadID = GetTaskWorker() == nullptr ? ThisThread::GetThreadID() : GetTaskWorker()->GetThreadID();
		SharedPointerT<Transaction> pDeleted;

		if (pTrans == nullptr)
			return;



		if (pTrans->GetTimerAction() && pTrans->GetTimerAction()->GetScheduledTime() != TimeStampMS::max())
			m_activeTransactionScheduler.RemoveTimerAction(currentThreadID, pTrans->GetTimerAction());

		// All master entity's transaction will be managed by shared pointer
		if (SUCCEEDED(m_activeTrans.Remove(pTrans->GetTransID().GetTransactionIndex(), pDeleted)))
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
	HRESULT MasterEntity::ClearEntity()
	{
		// just drop transaction with freed entity
		m_activeTrans.ClearMap();
		m_pExclusiveTransaction = SharedPointerT<Transaction>();

		return Entity::ClearEntity();
	}

	HRESULT MasterEntity::FindActiveTransaction(const TransactionID& transID, Transaction* &pTransaction)
	{
		if (m_pExclusiveTransaction != nullptr && m_pExclusiveTransaction->GetTransID() == transID)
		{
			pTransaction = (Transaction*)m_pExclusiveTransaction;
			return S_OK;
		}

		SharedPointerT<Transaction> activeTrans;
		if (SUCCEEDED(m_activeTrans.Find(transID.GetTransactionIndex(), activeTrans)))
		{
			pTransaction = (Transaction*)activeTrans;
			return S_OK;
		}
		return E_FAIL;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Transaction handling
	//

	// Update entity process
	//  - Make new transaction from connection queue
	//  - status update for game
	//  - Process transaction
	HRESULT MasterEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
		Transaction* pNewTran = nullptr;
		ThreadID currentThreadID = GetTaskWorker() == nullptr ? ThisThread::GetThreadID() : GetTaskWorker()->GetThreadID();
		Assert(currentThreadID == ThisThread::GetThreadID());
		TimeStampMS nextTick = TimeStampMS::max();

		if( GetEntityState() == EntityState::FREE )
		{
			//goto Proc_End;
			return S_FALSE;
		}

		if( GetEntityState() == EntityState::CLOSING )
		{
			svrTrace( Svr::TRC_TRANSACTION, "Entity Close - Entity ID : %0% , name : %1%, State : %2%", GetEntityID(), typeid(*this).name(), GetEntityState() );
			svrChk( TerminateEntity() );
			hr = S_FALSE;
			goto Proc_End;
		}

		ValidateTransactionCount();
		// Flush all commit before update
		m_activeTransactionScheduler.CommitChanges(currentThreadID);
		m_activeTrans.CommitChanges();

		// process pending queue
		while (m_activeTrans.GetItemCount() < m_uiMaxActiveTransaction 
			&& m_pExclusiveTransaction == nullptr // No transaction will be issued if there is a active exclusive transaction.
			&& SUCCEEDED(GetTransactionQueue().Dequeue(pNewTran))
			)
		{
			TransactionID transID( (UINT32)GetEntityID(), 0 );

			SharedPointerT<Transaction> pPrevTrans = nullptr;
			do {
				transID.Components.TransID = GenTransIndex();
			} while (SUCCEEDED(m_activeTrans.Find(transID.GetTransactionIndex(), pPrevTrans)));


			pNewTran->SetTransID( transID );

			//if( pNewTran->IsPrintTrace() )
			//{
			//	svrTrace(Svr::TRC_TRANSACTION, "Trans NewActive TID:%0%, ParentTID:%1% %2%, Entity:%3%:%4%", 
			//		pNewTran->GetTransID(), 
			//		pNewTran->GetParentTransID(), 
			//		typeid(*(Transaction*)pNewTran).name(), GetEntityUID(), 
			//		typeid(*this).name());
			//}


			Transaction* curTrans = (Transaction*)pNewTran;
			ProcessTransaction(curTrans);

			if (curTrans == nullptr)
			{
				pNewTran = nullptr;
				continue;
			}

			SharedPointerT<Transaction> pTransPtr(pNewTran); // We need this shared pointer to generage weak pointer in TimerActionTransaction
			if (pNewTran->IsExclusive())
			{
				m_pExclusiveTransaction = pTransPtr;
			}
			else
			{
				svrChk(m_activeTrans.Insert(pTransPtr->GetTransID().GetTransactionIndex(), pTransPtr));
			}

			auto timerAction = new TimerActionTransaction(pTransPtr);
			pTransPtr->SetTimerAction(timerAction);
			timerAction->SetNextTickTime(pTransPtr);
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
		if (m_pExclusiveTransaction != nullptr && m_activeTrans.GetItemCount() == 0)
		{
			auto curTrans = (Transaction*)m_pExclusiveTransaction;
			ProcessTransaction(curTrans);

			if (curTrans != nullptr)
			{
				auto transTickTime = std::min(m_pExclusiveTransaction->GetTimerExpireTime(), m_pExclusiveTransaction->GetHeartBitTimeout());
				nextTick = std::min(transTickTime, nextTick);
			}
		}


		//m_activeTrans.CommitChanges();
		m_activeTransactionScheduler.CommitChanges(currentThreadID);


		ValidateTransactionCount();

		nextTick = std::min(m_activeTransactionScheduler.GetNextTimeTick(), nextTick);
		SetNextScheduledTickTime(nextTick);


		if( GetEntityState() == EntityState::CLOSING )
		{
			svrTrace( Svr::TRC_TRANSACTION, "Entity Close - Entity ID : %0% , name : %1%, State : %2%", GetEntityID(), typeid(*this).name(), GetEntityState() );
			svrChk( TerminateEntity() );
			hr = S_FALSE;
			goto Proc_End;
		}


	Proc_End:

		ReleaseTransaction(pNewTran);

		return hr;
	}
	
	HRESULT MasterEntity::ProcessTransactionResult(Transaction *pCurTran, TransactionResult *pTransRes)
	{
		HRESULT hr = S_OK;
		HRESULT hrTem = S_OK;
		ThreadID currentThreadID = GetTaskWorker() == nullptr ? ThisThread::GetThreadID() : GetTaskWorker()->GetThreadID();
		Assert(currentThreadID == ThisThread::GetThreadID());

		svrChkPtr(pCurTran);
		svrChkPtr(pTransRes);
		if( pCurTran->IsPrintTrace() )
		{
			svrTrace( Svr::TRC_TRANSACTION, "Trans Proc TID:%0%:%1%, Entity:%2%", pCurTran->GetTransID(), typeid(*pCurTran).name(), GetEntityUID() );
		}

		pCurTran->UpdateHeartBitTime();
		if( pCurTran->IsTimerWorking() ) pCurTran->ClearTimer();
		pCurTran->RecordTransactionHistory(pTransRes);
		hrTem = pCurTran->ProcessTransaction(pTransRes);

		if( FAILED(hrTem) )// Transaction failed
		{
			if( pCurTran->IsPrintTrace() )
			{
				svrTrace( Svr::TRC_TRANSACTION, "Trans failed hr={0:X8}, TID:%1%:%2%, Entity:%3%", 
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

		SetBestScehdulingTime(pCurTran);

	Proc_End:

		Util::SafeRelease(pTransRes);

		return hr;
	}

	void MasterEntity::UpdateWorkingThreadID(ThreadID threadID)
	{
		m_activeTransactionScheduler.UpdateWorkingThreadID(threadID);
	}

	void MasterEntity::OnAddedToTaskManager(TaskWorker *pWorker)
	{
		UpdateWorkingThreadID(pWorker->GetThreadID());
		Entity::OnAddedToTaskManager(pWorker);
	}

	HRESULT MasterEntity::OnEventTask(const Svr::EventTask& eventTask)
	{
		HRESULT hr = S_OK;
		Transaction *pCurTran = nullptr;
		Message::MessageData* pMsg = nullptr;

		switch (eventTask.EventType)
		{
		case Svr::EventTask::EventTypes::CONNECTION_EVENT:
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_EVENT:
			svrErr(E_NOTIMPL);
			break;
		case Svr::EventTask::EventTypes::TRANSRESULT_EVENT:
			if (eventTask.EventData.pTransResultEvent != nullptr)
			{
				if (SUCCEEDED(FindActiveTransaction(eventTask.EventData.pTransResultEvent->GetTransID(), pCurTran)))
				{
					ProcessTransactionResult(pCurTran, eventTask.EventData.pTransResultEvent);
				}
				else
				{
					svrTrace(Svr::TRC_TRANSACTION, "Transaction result for TID:%0% is failed to route.", eventTask.EventData.pTransResultEvent->GetTransID());
					Util::SafeRelease(const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent));
					svrErr(E_FAIL);
				}
			}
			else
			{
				svrTrace(Svr::TRC_TRANSACTION, "Faild to process transaction result. null Transaction result.");
			}
			break;
		default:
			svrErr(E_UNEXPECTED);
		}

	Proc_End:

		return hr;
	}

	UINT MasterEntity::GetActiveTransactionCount()
	{
		return (UINT)m_activeTrans.GetItemCount() + (m_pExclusiveTransaction == nullptr ? 0 : 1);
	}

}; // namespace Svr
}; // namespace BR



