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
#include "Net/SFMessage.h"
#include "Service/ServerService.h"
#include "Protocol/Policy/ServerNetPolicy.h"



namespace SF
{

	//template class SharedPointerT<Svr::Entity>;
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
		, m_transactionQueue(GetHeap(), uiTransQueueSize)
		, m_HandlerTable(GetHeap())
	{
	}

	Entity::~Entity()
	{
		_ClearEntity();
	}

	void Entity::Dispose()
	{
		TickTask::Dispose();
		ClearEntity();
	}

	void Entity::SetEntityUID(EntityUID entityUID, bool silenceAssert)
	{
		//assert(silenceAssert || entityUID.GetEntityID() > 0);
		assert(silenceAssert || entityUID.GetServerID() > 0);
		m_EntityUID = entityUID;
	}


	TimeStampMS Entity::SetBestScehdulingTime(Transaction* pTrans)
	{
		auto nextTick = Util::Time.GetTimeMs() + GetTickInterval();
		if (pTrans)
		{
			nextTick = Util::TimeMinNonZero(nextTick, pTrans->GetTimerExpireTime());
			nextTick = Util::TimeMinNonZero(nextTick, pTrans->GetHeartbeatTimeout());
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
			svrCheck( ResultCode::UNEXPECTED );
		}

		// register message handlers
		RegisterMessageHandlers();


		svrCheck( ClearEntity() );

		auto serverId = GetMyServerID();
		SetEntityUID( EntityUID(serverId, newEntityID) );
		SetTaskID( newEntityID );

		SetEntityState( EntityState::WORKING );

		SetEntityCreateTime( Util::Time.GetTimeMs() );

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
			Service::EntityTable->erase(GetEntityID());
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
	Result Entity::ProcessMessage(const SharedPointerT<MessageEndpoint>& remoteEndpoint, MessageDataPtr &pMsg)
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
			if (!(GetMessageHandlerTable().HandleMessage<TransactionPtr&>(pMsg, pNewTrans)))
			{
				// If it couldn't find a handler in server entity handlers, looking for it in server loopback entity
				MessageHandlerType handler;
				assert(false);// this shouldn't be happened now. We route all message to destination entity
				hr = GetLoopbackServerEntity()->GetMessageHandlerTable().GetHandler(pMsg->GetMessageHeader()->msgID, handler);
				if (!(hr))
				{
					assert(false);
					svrTrace(Error, "No message handler {0}:{1}, MsgID:{2}", typeid(*this).name(), GetEntityUID(), pMsgHdr->msgID);
					svrErr(ResultCode::SVR_NO_MESSAGE_HANDLER);
				}

				svrChk(handler(pMsg, pNewTrans));
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
			pNewTrans->SetRemoteEndpoint(remoteEndpoint);

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
					svrChk(Service::EntityTable->RouteTransaction(pNewTrans->GetTransID().GetEntityID(), pNewTrans));
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
					auto senderEndpoint = pNewTrans->GetRemoteEndpoint();
					if (senderEndpoint != nullptr)
						NetSvrPolicyServer(senderEndpoint).GenericFailureRes(pNewTrans->GetMessageRouteContext().GetSwaped(), pNewTrans->GetParentTransID(), hr);
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
		UniquePtr<TransactionResult> pTransRes;
		auto pMySvr = BrServer::GetInstance();
		TransactionPtr pTransaction;

		svrChkPtr(pMySvr);
		svrMem( pMsgRes = new(GetHeap()) MessageResult );
		svrChk( pMsgRes->SetMessage( pMsg ) );
		pMsg = nullptr;

		pTransRes.reset(pMsgRes);
		pMsgRes = nullptr;

		if (pTransRes->GetTransID().GetEntityID() != GetEntityID())
		{
			svrTrace(Warning, "Invalid message result routing Entity:{0}, msgID:{1} to Entity:{2}", GetEntityID(), pTransRes->GetMsgID(), pTransRes->GetTransID().GetEntityID());
			goto Proc_End;
		}
		//assert(pTransRes->GetTransID().GetEntityID() == GetEntityID());

		if (GetTaskWorker()->GetThreadID() == ThisThread::GetThreadID())
		{
			if (!(FindActiveTransaction(pTransRes->GetTransID(), pTransaction)))
			{
				svrTrace(SVR_TRANSACTION, "Transaction result for TID:{0} is failed to route. msgid:{1}", pTransRes->GetTransID(), pTransRes->GetMsgID());
				goto Proc_End;// svrErr(ResultCode::FAIL);
			}
			svrChk(ProcessTransactionResult(pTransaction, pTransRes));
		}
		else
		{
			svrChk(PendingTransactionResult(pTransRes));
		}

	Proc_End:

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
				UniquePtr<TransactionResult> pTranRes(new(GetHeap()) TimerResult);
				svrMem(pTranRes);
				pTrans->UpdateHeartbeatTime();
				pTrans->RecordTransactionHistory(pTranRes);
				pTrans->ProcessTransaction(pTranRes);
			}
			else if (!(pTrans->CheckHeartbeatTimeout()))// Transaction time out
			{
				if (pTrans->IsPrintTrace())
				{
					svrTrace(SVR_TRANSACTION, "Trans Timeout TID:{0}:{1}, EntityID:{2}",
						pTrans->GetTransID(),
						pTrans->GetOwnerUID(),
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
	Result Entity::PendingTransactionResult(UniquePtr<TransactionResult>& pTransRes)
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::FAIL;

		svrChkPtr( pTransRes );

		// TODO: Use unique ptr in queue
		svrChk(GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(this, pTransRes.get())));
		pTransRes.release();
		
	Proc_End:

		return hr;
	}


} // namespace Svr
} // namespace SF



