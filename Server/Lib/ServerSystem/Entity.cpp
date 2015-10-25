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


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EventTask.h"
#include "ServerSystem/EntityTimerActions.h"
#include "ServerSystem/BRServer.h"
#include "Common/Message.h"




namespace BR {
namespace Svr
{
	template class SharedPointerT<Entity>;
	template class WeakPointerT<Entity>;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//

	Entity::Entity( UINT uiTransQueueSize, UINT TransResQueueSize )
		: m_State(EntityState::FREE)
		, m_ulCreateTime(0)
		, m_lTransIdx(0)
		, m_transactionQueue(uiTransQueueSize)
		, m_EntityUID(0)
		, m_pHandlerTable(nullptr)
	{
	}

	Entity::~Entity()
	{
		_ClearEntity();
		Util::SafeDelete(m_pHandlerTable);
	}

	ULONG Entity::SetBestScehdulingTime(Transaction* pTrans)
	{
		auto nextTick = Util::Time.GetTimeMs() + GetTickInterval();
		if (pTrans)
		{
			nextTick = Util::TimeMin(nextTick, pTrans->GetTimerExpireTime());
			nextTick = Util::TimeMin(nextTick, pTrans->GetHeartBitTimeout());
		}

		Assert(nextTick != 0 && nextTick != -1);

		nextTick = Util::TimeMin(nextTick, GetScheduledTickTime());

		Assert(nextTick != 0 && nextTick != -1);
		SetNextScheduledTickTime(nextTick);

		return nextTick;
	}

	void Entity::ReleaseTransaction(Transaction* pTrans)
	{
		SharedPointerT<Transaction> trans(pTrans);
	}

	// Initialize entity to proceed new connection
	HRESULT Entity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		if( GetEntityState() != EntityState::FREE )
		{
			Assert( 0 );
			svrErr( E_ABORT );
		}

		// create message handler table
		if( m_pHandlerTable == nullptr )
		{
			svrMem( m_pHandlerTable = new MessageHandlerTable<MessageHandlerType>(GetAllocator()) );
		}

		// register message handlers
		RegisterMessageHandlers();


		svrChk( ClearEntity() );

		SetEntityUID( EntityUID(GetMyServerID(), newEntityID) );
		SetTaskID( newEntityID );

		SetEntityState( EntityState::WORKING );

		SetEntityCreateTime( Util::Time.GetTimeMs() );

	Proc_End:

		return hr;
	}

	HRESULT Entity::_ClearEntity()
	{
		// just drop transaction with freed entity
		if (m_transactionQueue.GetEnqueCount() > 0)
		{
			if (m_transactionQueue.GetEnqueCount() > 1)
				svrTrace(Svr::TRC_TRANSACTION, "Trans Not closed Ent:%0%, %1% Trans remain", typeid(*this).name(), m_transactionQueue.GetEnqueCount());

			while (m_transactionQueue.GetEnqueCount() > 0)
			{
				Transaction *pCurTran = nullptr;
				if (SUCCEEDED(m_transactionQueue.Dequeue(pCurTran)))
				{
					pCurTran->CloseTransaction(E_UNEXPECTED);
					ReleaseTransaction(pCurTran);
				}
			}
		}
		m_transactionQueue.ClearQueue();


		if (BrServer::GetInstance())
		{
			SharedPointerT<Entity> erased;
			BrServer::GetInstance()->GetEntityTable().Erase(GetEntityID(), erased);
		}

		return S_OK;
	}

	HRESULT Entity::ClearEntity()
	{
		return _ClearEntity();
	}
	

	// Close entity and clear transaction
	HRESULT Entity::TerminateEntity()
	{
		if( GetEntityState() == EntityState::FREE )
			return S_OK;

		ClearEntity();

		SetEntityState( EntityState::FREE );

		SetEntityUID(0);

		return S_OK;
	}
	

	//////////////////////////////////////////////////////////////////////////
	//
	//	Message handling
	//

	// register message handlers
	HRESULT Entity::RegisterMessageHandlers()
	{
		// nothing for default
		return S_OK;
	}

	HRESULT Entity::ProcessMessageResult( Message::MessageData* &pMsg )
	{
		HRESULT hr = S_OK;
		MessageResult *pMsgRes = nullptr;
		TransactionResult *pTransRes = nullptr;
		auto pMySvr = BrServer::GetInstance();
		SharedPointerT<Entity> pEntity;
		Transaction *pTransaction = nullptr;

		svrChkPtr(pMySvr);
		svrMem( pMsgRes = new MessageResult );
		svrChk( pMsgRes->SetMessage( pMsg ) );
		pMsg = nullptr;

		pTransRes = pMsgRes;

		//svrChk( BrServer::GetInstance()->GetEntityTable().RouteTransactionResult( pTransRes ) );
		svrChk(pMySvr->GetEntityTable().Find(pTransRes->GetTransID().EntityID, pEntity));

		if (pEntity->GetTaskGroupID() == GetTaskGroupID() && pEntity->GetTaskManager() == GetTaskManager()) // If assigned on the same thread
		{
			if (FAILED(pEntity->FindActiveTransaction(pTransRes->GetTransID(), pTransaction)))
			{
				svrTrace(Svr::TRC_TRANSACTION, "Transaction result for TID:%0% is failed to route. msgid:%1%", pTransRes->GetTransID(), pMsgRes->GetMsgID());
				goto Proc_End;// svrErr(E_FAIL);
			}
			svrChk(pEntity->ProcessTransactionResult(pTransaction, pTransRes));
			pTransRes = nullptr;
		}
		else
		{
			svrChk(pEntity->PendingTransactionResult(pTransRes));
		}

	Proc_End:

		Util::SafeRelease(pMsg);
		Util::SafeRelease(pTransRes);

		return hr;
	}

	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Transaction handling
	//

	// Get Entity transaction queue count
	CounterType Entity::GetPendingTransactionCount() const
	{
		return m_transactionQueue.GetEnqueCount();
	}
	
	// Pending new transaction job
	HRESULT Entity::PendingTransaction(ThreadID thisThreadID, Transaction* &pTrans)
	{
		HRESULT hr = S_OK;

		if( GetEntityState() == EntityState::FREE )
			return E_FAIL;

		if( pTrans == nullptr )
			return E_POINTER;

		pTrans->SetOwnerEntity( this );
		pTrans->SetTransID( TransactionID( (UINT32)GetEntityID(), GenTransIndex() ) ); 

		svrChk( m_transactionQueue.Enqueue(pTrans) );

		pTrans = nullptr;

		// poke tick
		auto pTimerAction = GetTimerAction();
		auto pWorker = GetTaskWorker();
		if (pWorker != nullptr 
			&& pTimerAction != nullptr
			&& pWorker->GetThreadID() == thisThreadID) // Only if both are on the same worker thread
		{
			if (pTimerAction->GetScheduledTime() == -1 || Util::TimeSince(pTimerAction->GetScheduledTime()) < 0) // next time this entity will tick
			{
				SetNextScheduledTickTime(Util::Time.GetTimeMs());
				if (pTimerAction->GetScheduledTime() != -1) // Don't push when it isn't shceduled yet
					pWorker->GetTimeScheduler().Reschedul(thisThreadID, pTimerAction);
			}
		}
		else
		{
			// We can't reschedule here, just poke it
			GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(this));
		}

	Proc_End:

		ReleaseTransaction(pTrans);

		return hr;
	}

	HRESULT Entity::ProcessTransaction(Transaction* &pTrans)
	{
		HRESULT hr = S_OK;

		svrChkPtr(pTrans);

		switch (pTrans->GetState())
		{
		case Transaction::STATE_WAITSTART:
			if (pTrans->IsPrintTrace())
			{
				svrTrace(Svr::TRC_TRANSACTION, "Trans Start TID:%0%:%1%, Entity:%2%", pTrans->GetTransID(), typeid(*pTrans).name(), GetEntityUID());
			}
			if (FAILED(pTrans->StartTransaction()))// make transaction start
			{
				if (!pTrans->IsClosed())
					pTrans->CloseTransaction(E_UNEXPECTED);
			}
			else if (pTrans->GetState() == Transaction::STATE_WAITSTART)
			{
				Assert(0);
				pTrans->CloseTransaction(E_SVR_INVALID_TRANSITION);
			}
			break;
		case Transaction::STATE_STARTED:
			if (pTrans->CheckTimer())
			{
				TransactionResult *pTranRes = nullptr;
				svrMem(pTranRes = new TimerResult);
				pTrans->UpdateHeartBitTime();
				pTrans->RecordTransactionHistory(pTranRes);
				pTrans->ProcessTransaction(pTranRes);
				Util::SafeRelease(pTranRes);
			}
			else if (FAILED(pTrans->CheckHeartBitTimeout()))// Transaction time out
			{
				if (pTrans->IsPrintTrace())
				{
					svrTrace(Svr::TRC_TRANSACTION, "Trans Timeout TID:%0%:%1%, Entity:%2%",
						pTrans->GetTransID(),
						typeid(*pTrans).name(),
						GetEntityUID());
				}
				if (!pTrans->IsClosed())
					pTrans->CloseTransaction(E_SVR_TIMEOUT);
			}
			break;
		};

	Proc_End:

		// Clear closed transaction
		if (pTrans != nullptr)
		{
			if (pTrans->GetState() == Transaction::STATE_CLOSED)
			{
				if (pTrans->IsPrintTrace())
				{
					svrTrace(Svr::TRC_TRANSACTION, "Trans closed TID:%0%:%1%, Entity:%2%",
						pTrans->GetTransID(), typeid(*pTrans).name(),
						GetEntityUID());
				}
				pTrans->FlushTransaction();
				ReleaseTransaction(pTrans);

				pTrans = nullptr;
			}
		}

		SetBestScehdulingTime(pTrans);

		return hr;
	}

	// Pending transaction result
	HRESULT Entity::PendingTransactionResult( TransactionResult* &pTransRes )
	{
		HRESULT hr = S_OK;

		if( GetEntityState() == EntityState::FREE )
			return E_FAIL;

		svrChkPtr( pTransRes );

		svrChk(GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(this, pTransRes)))
		pTransRes = nullptr;
		
	Proc_End:

		Util::SafeRelease( pTransRes );

		return hr;
	}
/*
	void Entity::OnAddedToTaskManager(TaskWorker *pWorker)
	{
		Assert(pWorker);
		if (pWorker == nullptr) return;

		if (GetTickInterval() > 0)
		{
			if (m_TimerAction == nullptr)
				m_TimerAction = new TickTaskTimerAction(this);

			m_TimerAction->SetNextTickTime(Util::Time.GetTimeMs() + GetTickInterval());
			pWorker->GetTimeScheduler().AddTimerAction(m_TimerAction);
		}
		else
		{

		}
	}

	void Entity::OnRemovedFromTaskManager(TaskWorker *pWorker)
	{
		if (m_TimerAction != nullptr)
		{
			m_TimerAction->DisposeWeakPointer();
			m_TimerAction->SetNextTickTime(0);
		}
	}
*/


}; // namespace Svr
}; // namespace BR


