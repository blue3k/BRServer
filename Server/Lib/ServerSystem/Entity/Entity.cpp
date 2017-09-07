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
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeSystem.h"
#include "SvrConst.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "SvrTrace.h"
#include "Task/ServerTaskEvent.h"
#include "Entity/EntityTimerActions.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityTable.h"
#include "Net/Message.h"

#include "Protocol/Policy/ServerNetPolicy.h"



namespace SF {

	template class SharedPointerT<Svr::Entity>;
	template class WeakPointerT<Svr::Entity>;

	namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//

	Entity::Entity( uint uiTransQueueSize, uint TransResQueueSize )
		: TickTask()
		, m_State(EntityState::FREE)
		, m_EntityUID(0)
		, m_ulCreateTime(TimeStampMS::min())
		, m_lTransIdx(0)
		, m_transactionQueue(GetMemoryManager(), uiTransQueueSize)
		, m_HandlerTable(GetMemoryManager())
	{

	}

	Entity::~Entity()
	{
		_ClearEntity();
		//Util::SafeDelete(m_pHandlerTable);
	}

	TimeStampMS Entity::SetBestScehdulingTime(Transaction* pTrans)
	{
		auto nextTick = Util::Time.GetTimeMs() + GetTickInterval();
		if (pTrans)
		{
			nextTick = Util::TimeMinNonZero(nextTick, pTrans->GetTimerExpireTime());
			nextTick = Util::TimeMinNonZero(nextTick, pTrans->GetHeartBitTimeout());
		}

		Assert(nextTick != TimeStampMS::min() && nextTick != TimeStampMS::max());

		nextTick = Util::TimeMinNonZero(nextTick, GetScheduledTickTime());

		Assert(nextTick != TimeStampMS::min() && nextTick != TimeStampMS::max());
		SetNextScheduledTickTime(nextTick);

		return nextTick;
	}

	void Entity::ReleaseTransaction(TransactionPtr& pTrans)
	{
		pTrans = nullptr;
	}

	// Initialize entity to proceed new connection
	Result Entity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() != EntityState::FREE )
		{
			Assert( 0 );
			svrErr( ResultCode::UNEXPECTED );
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

	Result Entity::_ClearEntity()
	{
		// just drop transaction with freed entity
		if (m_transactionQueue.GetEnqueCount() > 0)
		{
			if (m_transactionQueue.GetEnqueCount() > 1)
				svrTrace(SVR_TRANSACTION, "Trans Not closed Ent:{0}, {1} Trans remain", typeid(*this).name(), m_transactionQueue.GetEnqueCount());

			while (m_transactionQueue.GetEnqueCount() > 0)
			{
				TransactionPtr pCurTran;
				if ((m_transactionQueue.Dequeue(pCurTran)))
				{
					pCurTran->CloseTransaction(ResultCode::UNEXPECTED);
					ReleaseTransaction(pCurTran);
				}
			}
		}
		m_transactionQueue.ClearQueue();


		if (BrServer::GetInstance())
		{
			SharedPointerT<Entity> erased;
			GetEntityTable().Erase(GetEntityID(), erased);
		}

		return ResultCode::SUCCESS;
	}

	Result Entity::ClearEntity()
	{
		return _ClearEntity();
	}
	

	// Close entity and clear transaction
	Result Entity::TerminateEntity()
	{
		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		ClearEntity();

		SetEntityState( EntityState::FREE );

		SetEntityUID(0);

		return ResultCode::SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Message handling
	//

	// register message handlers
	Result Entity::RegisterMessageHandlers()
	{
		// nothing for default
		return ResultCode::SUCCESS;
	}

	// Process Message and release message after all processed
	Result Entity::ProcessMessage(ServerEntity* pServerEntity, Net::Connection *pCon, MessageDataPtr &pMsg)
	{
		Result hr = ResultCode::SUCCESS, hrRes;
		EntityID entityID; // entity ID to route
		Message::MessageHeader *pMsgHdr = nullptr;
		TransactionPtr pNewTrans;
		RouteContext routeContext;
		SharedPointerT<Entity> pEntity;

		svrChkPtr(pMsg);
		pMsgHdr = pMsg->GetMessageHeader();

		// First try to route message
		routeContext = *(RouteContext*)pMsg->GetMessageData();
		assert(routeContext.GetTo().UID == 0 || routeContext.GetTo() == GetEntityUID());

		switch (pMsgHdr->msgID.IDs.Type)
		{
		case Message::MSGTYPE_RESULT:
			svrChk(ProcessMessageResult(pMsg));
			goto Proc_End;
			break;
		case Message::MSGTYPE_COMMAND:
		case Message::MSGTYPE_EVENT:
		{
			Assert(GetMessageHandlerTable());
			if (!(GetMessageHandlerTable()->HandleMessage<TransactionPtr&>(pCon, pMsg, pNewTrans)))
			{
				// If it couldn't find a handler in server entity handlers, looking for it in server loopback entity
				MessageHandlerType handler;
				assert(false);// this shouldn't be happened now. We route all message to destination entity
				hr = GetLoopbackServerEntity()->GetMessageHandlerTable()->GetHandler(pMsg->GetMessageHeader()->msgID, handler);
				if (!(hr))
				{
					assert(false);
					svrTrace(Error, "No message handler {0}:{1}, MsgID:{2}", typeid(*this).name(), GetEntityUID(), pMsgHdr->msgID);
					svrErr(ResultCode::SVR_NO_MESSAGE_HANDLER);
				}

				svrChk(handler(pCon, pMsg, pNewTrans));
			}
			break;
		}
		default:
			svrTrace(Error, "Not Processed Remote message Entity:{0}:{1}, MsgID:{2}", typeid(*this).name(), GetEntityUID(), pMsgHdr->msgID);
			svrErr(ResultCode::SVR_NOTEXPECTED_MESSAGE);
			break;
		};


		if (pNewTrans != nullptr)
		{
			pNewTrans->SetServerEntity(pServerEntity);

			if (pNewTrans->GetOwnerEntity() == nullptr)
			{
				hr = pNewTrans->InitializeTransaction(this);
				if (!(hr)) goto Proc_End;
			}

			if (pNewTrans->IsDirectProcess())
			{
				assert(false); // disable direct process for now
				// This need to be run on correct worker thread
				hrRes = pNewTrans->StartTransaction();
				if (!pNewTrans->IsClosed())
				{
					pNewTrans->CloseTransaction(hrRes);
				}
			}
			else
			{
				assert(this == pNewTrans->GetOwnerEntity());
				if (this == pNewTrans->GetOwnerEntity())
				{
					auto threadID = ThisThread::GetThreadID();
					hrRes = PendingTransaction(threadID, pNewTrans);
					if (!hrRes)
					{
						if (GetEntityState() == EntityState::WORKING) // This is normal silence the error log
						{
							svrErr(hrRes);
						}
						else
						{
							pNewTrans->SetClosed();
							goto Proc_End;
						}
					}
				}

				if (pNewTrans != nullptr && BrServer::GetInstance())
				{
					assert(false); // This shouldn't be happend now
					svrChk(GetEntityTable().RouteTransaction(pNewTrans->GetTransID().GetEntityID(), pNewTrans));
				}
			}
		}

	Proc_End:

		if (pNewTrans != nullptr)
		{
			if (!(hr))
			{
				svrTrace(Error, "Transaction initialization is failed {0} Entity:{1}, MsgID:{2}", typeid(*this).name(), GetEntityUID(), pMsgHdr->msgID);
				if (pMsgHdr->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
				{
					Policy::NetSvrPolicyServer(pCon).GenericFailureRes(pNewTrans->GetMessageRouteContext().GetSwaped(), pNewTrans->GetParentTransID(), hr);
				}
			}

			if (!pNewTrans->IsClosed() && pNewTrans->GetOwnerEntity() != nullptr)
			{
				svrTrace(Error, "Transaction isn't closed Transaction:{0}, MsgID:{1}", typeid(*pNewTrans).name(), pMsgHdr->msgID);
				pNewTrans->CloseTransaction(hr);
			}

			ReleaseTransaction(pNewTrans);
		}


		return ResultCode::SUCCESS;
	}

	Result Entity::ProcessMessageResult(MessageDataPtr &pMsg )
	{
		Result hr = ResultCode::SUCCESS;
		MessageResult *pMsgRes = nullptr;
		TransactionResult *pTransRes = nullptr;
		auto pMySvr = BrServer::GetInstance();
		TransactionPtr pTransaction;

		svrChkPtr(pMySvr);
		svrMem( pMsgRes = new(GetMemoryManager()) MessageResult );
		svrChk( pMsgRes->SetMessage( pMsg ) );
		pMsg = nullptr;

		pTransRes = pMsgRes;

		if (pTransRes->GetTransID().GetEntityID() != GetEntityID())
		{
			svrTrace(Warning, "Invalid message result routing Entity:{0}, msgID:{1} to Entity:{2}", GetEntityID(), pMsgRes->GetMsgID(), pTransRes->GetTransID().GetEntityID());
			goto Proc_End;
		}
		//assert(pTransRes->GetTransID().GetEntityID() == GetEntityID());

		if (GetTaskWorker()->GetThreadID() == ThisThread::GetThreadID())
		{
			if (!(FindActiveTransaction(pTransRes->GetTransID(), pTransaction)))
			{
				svrTrace(SVR_TRANSACTION, "Transaction result for TID:{0} is failed to route. msgid:{1}", pTransRes->GetTransID(), pMsgRes->GetMsgID());
				goto Proc_End;// svrErr(ResultCode::FAIL);
			}
			svrChk(ProcessTransactionResult(pTransaction, pTransRes));
		}
		else
		{
			svrChk(PendingTransactionResult(pTransRes));
		}

	Proc_End:


		IHeap::Delete(pTransRes);

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
	Result Entity::PendingTransaction(ThreadID thisThreadID, TransactionPtr &pTrans)
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::FAIL;

		if( pTrans == nullptr )
			return ResultCode::INVALID_POINTER;

		pTrans->SetOwnerEntity( this );
		pTrans->SetTransID( TransactionID( (uint32_t)GetEntityID(), (uint)GenTransIndex() ) ); 

		svrChk( m_transactionQueue.Enqueue(pTrans) );

		pTrans = nullptr;

		{
			// poke tick
			auto pTimerAction = GetTimerAction();
			auto pWorker = GetTaskWorker();
			if (pWorker != nullptr
				&& pTimerAction != nullptr
				&& pWorker->GetThreadID() == thisThreadID) // Only if both are on the same worker thread
			{
				if (pTimerAction->GetScheduledTime() == TimeStampMS::max() || Util::TimeSince(pTimerAction->GetScheduledTime()) <= DurationMS(0)) // next time this entity will tick
				{
					SetNextScheduledTickTime(Util::Time.GetTimeMs());
					if (pTimerAction->GetScheduledTime() != TimeStampMS::max()) // Don't push when it isn't shceduled yet
						pWorker->GetTimeScheduler().Reschedul(thisThreadID, pTimerAction);
				}
			}
			else
			{
				// We can't reschedule here, just poke it
				// And a error can be happened during initialization, they will be rescheduled
				// TODO: find better way
				GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(this));
			}
		}

	Proc_End:

		ReleaseTransaction(pTrans);

		return hr;
	}

	Result Entity::ProcessTransaction(TransactionPtr &pTrans)
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr(pTrans);

		switch (pTrans->GetState())
		{
		case Transaction::STATE_WAITSTART:
			if (pTrans->IsPrintTrace())
			{
				svrTrace(SVR_DBGTRANS, "Trans Start TID:{0}:{1}, Entity:{2}", pTrans->GetTransID(), typeid(*pTrans).name(), GetEntityUID());
			}
			if (!(pTrans->StartTransaction()))// make transaction start
			{
				if (!pTrans->IsClosed())
					pTrans->CloseTransaction(ResultCode::UNEXPECTED);
			}
			else if (pTrans->GetState() == Transaction::STATE_WAITSTART)
			{
				Assert(0);
				pTrans->CloseTransaction(ResultCode::SVR_INVALID_TRANSITION);
			}
			break;
		case Transaction::STATE_STARTED:
			if (pTrans->CheckTimer())
			{
				TransactionResult *pTranRes = nullptr;
				svrMem(pTranRes = new(GetMemoryManager()) TimerResult);
				pTrans->UpdateHeartBitTime();
				pTrans->RecordTransactionHistory(pTranRes);
				pTrans->ProcessTransaction(pTranRes);
				IHeap::Delete(pTranRes);
			}
			else if (!(pTrans->CheckHeartBitTimeout()))// Transaction time out
			{
				if (pTrans->IsPrintTrace())
				{
					svrTrace(SVR_TRANSACTION, "Trans Timeout TID:{0}:{1}, Entity:{2}",
						pTrans->GetTransID(),
						typeid(*pTrans).name(),
						GetEntityUID());
				}
				if (!pTrans->IsClosed())
					pTrans->CloseTransaction(ResultCode::SVR_TIMEOUT);
			}
			break;
		default:
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
					svrTrace(SVR_DBGTRANS, "Trans closed TID:{0}:{1}, Entity:{2}",
						pTrans->GetTransID(), typeid(*pTrans).name(),
						GetEntityUID());
				}
				pTrans->FlushTransaction();
				ReleaseTransaction(pTrans);

				pTrans = nullptr;
			}
		}

		SetBestScehdulingTime(*pTrans);

		return hr;
	}

	// Pending transaction result
	Result Entity::PendingTransactionResult( TransactionResult* &pTransRes )
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::FAIL;

		svrChkPtr( pTransRes );

		svrChk(GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(this, pTransRes)))
		
	Proc_End:

		IHeap::Delete(pTransRes);
		pTransRes = nullptr;

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
				m_TimerAction = new(GetMemoryManager()) TickTaskTimerAction(this);

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
}; // namespace SF



