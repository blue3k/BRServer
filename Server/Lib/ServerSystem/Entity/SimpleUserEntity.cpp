////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Util/Utility.h"
#include "SvrConst.h"
#include "Entity/SimpleUserEntity.h"
#include "Transaction/Transaction.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Task/ServerTaskEvent.h"
#include "Net/ConnectionUDP.h"


namespace SF {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//


	SimpleUserEntity::SimpleUserEntity()
		:SimpleEntity(Const::ENTITY_SIMPLE_TRANS_QUEUE, Const::ENTITY_SIMPLE_TRANSRES_QUEUE),
		m_pConnection(nullptr)
	{
		SetTickInterval(DurationMS(Svr::Const::SIMPLEUSER_TICKTASK_INTERVAL));
	}

	SimpleUserEntity::~SimpleUserEntity()
	{
	}


	// set connection
	Result SimpleUserEntity::SetConnection(SharedPointerT<Net::Connection>&& pConn)
	{
		MutexScopeLock localLock(m_ConnectionLock);

		m_pConnection = std::forward<SharedPointerT<Net::Connection>>(pConn);
		if (m_pConnection != nullptr)
		{
			m_pConnection->SetEventHandler(this);

			// purge received guaranted messages
			OnRecvMessage((Net::Connection*)m_pConnection, MessageDataPtr());

			// This connection will be updated with User entity
			m_pConnection->SetTickFlags(0);
			//m_pConnection->GetNet()->TakeOverConnection((Net::Connection*)m_pConnection);
		}

		return ResultCode::SUCCESS;
	}

	// Release connection if has
	void SimpleUserEntity::ReleaseConnection()
	{
		MutexScopeLock localLock(m_ConnectionLock);

		if( m_pConnection == nullptr )
			return;

		m_pConnection->SetEventHandler(nullptr);
		m_pConnection->DisconnectNRelease();
		m_pConnection = SharedPointerT<Net::Connection>();
	}

	void SimpleUserEntity::GetConnectionShared(SharedPointerT<Net::Connection>& outConn)
	{
		MutexScopeLock localLock(m_ConnectionLock);

		outConn = m_pConnection;
	}

	// Initialize entity to proceed new connection
	Result SimpleUserEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		SetAccountID( 0 );
		SetAuthTicket( 0 );

		svrChk(SimpleEntity::InitializeEntity( newEntityID ) );

	Proc_End:

		return hr;
	}



	// Close entity and clear transaction
	Result SimpleUserEntity::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		SetAccountID( 0 );
		SetAuthTicket( 0 );

		if (m_pConnection != nullptr)
			m_pConnection->SetEventHandler(nullptr);

		svrChk(SimpleEntity::TerminateEntity() );

	Proc_End:

		return hr;
	}


	// Process Connection event
	Result SimpleUserEntity::ProcessConnectionEvent( const Net::ConnectionEvent& conEvent )
	{
		Result hr = ResultCode::SUCCESS;

		switch( conEvent.EventType )
		{
		case Net::ConnectionEvent::EVT_CONNECTION_RESULT:
			break;
		case Net::ConnectionEvent::EVT_DISCONNECTED:
			break;
		case Net::ConnectionEvent::EVT_STATE_CHANGE:
			break;
		default:
			break;
		};

	//Proc_End:

		return hr;
	}


	// Process Message and release message after all processed
	Result SimpleUserEntity::ProcessMessageData(MessageDataPtr &pIMsg)
	{
		Result hr = ResultCode::SUCCESS;
		EntityID entityID; // entity ID to route
		Message::MessageHeader *pMsgHdr = nullptr;
		TransactionPtr pNewTrans;
		ThreadID currentThreadID = ThisThread::GetThreadID();

		svrChkPtr(pIMsg);
		pMsgHdr = pIMsg->GetMessageHeader();

		switch (pMsgHdr->msgID.IDs.Type)
		{
		case Message::MSGTYPE_RESULT:
			svrChk(ProcessMessageResult(pIMsg));
			goto Proc_End;

			break;
		case Message::MSGTYPE_COMMAND:
		case Message::MSGTYPE_EVENT:
		{
			//Assert(m_pHandlerTable);
			if (!(GetMessageHandlerTable()->HandleMessage<TransactionPtr&>(*GetConnection(), pIMsg, pNewTrans)))
			{
				svrTrace(Error, "Failed to handle remote message Entity:{0}:{1}, MsgID:{2}", typeid(*this).name(), GetEntityID(), pMsgHdr->msgID);
				svrErr(ResultCode::SVR_NOTEXPECTED_MESSAGE);
			}
			break;
		}
		default:
			svrTrace(Error, "Not Processed Remote message Entity:{0}:{1}, MsgID:{2}", typeid(*this).name(), GetEntityID(), pMsgHdr->msgID);
			svrErr(ResultCode::SVR_NOTEXPECTED_MESSAGE);
			break;
		};


		if (pNewTrans != nullptr)
		{
			if (pNewTrans->GetOwnerEntity() == nullptr)
			{
				svrChk(pNewTrans->InitializeTransaction(this));
			}

			if (pNewTrans->IsDirectProcess())
			{
				Result hrRes = pNewTrans->StartTransaction();
				if (!pNewTrans->IsClosed())
				{
					pNewTrans->CloseTransaction(hrRes);
				}
			}
			else
			{
				Assert(pNewTrans->GetTransID().GetEntityID() == GetEntityID());
				svrChk(PendingTransaction(currentThreadID, pNewTrans));
			}
		}

	Proc_End:

		if (pNewTrans != nullptr && !pNewTrans->IsClosed())
		{
			pNewTrans->CloseTransaction(hr);
		}


		ReleaseTransaction(pNewTrans);

		return ResultCode::SUCCESS;

	}



	Result SimpleUserEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		MessageDataPtr pIMsg;
		Net::ConnectionEvent conEvent;


		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		// Update connection
		// We need to make a copy here while m_pConnection can be changed on another thread
		SharedPointerT<Net::Connection> pConn;
		GetConnectionShared(pConn);

		if (pConn != nullptr)
		{
			if (pConn->GetRunningThreadID() == ThreadID())
				pConn->SetRunningThreadID(ThisThread::GetThreadID());

			if(pConn->GetRunningThreadID() == ThisThread::GetThreadID())
				pConn->UpdateNetCtrl();
		}

		// We need to check m_pConnection again because the connection can be release during update
		if (pConn != nullptr)
		{
			// Process Connection event
			auto loopCount = pConn->GetConnectionEventCount();
			for (decltype(loopCount) iProc = 0; iProc < loopCount; iProc++)
			{
				if (!(pConn->DequeueConnectionEvent(conEvent)))
					break;

				ProcessConnectionEvent( conEvent );
			}


			if (pConn->GetConnectionState() != Net::ConnectionState::DISCONNECTED)
			{
				// Process message
				loopCount = pConn->GetRecvMessageCount();
				for( decltype(loopCount) iProc = 0; iProc < loopCount; iProc++ )
				{
					if (!(pConn->GetRecvMessage(pIMsg)))
						break;

					ProcessMessageData(pIMsg );
				}
			}
		}

		svrChk(SimpleEntity::TickUpdate(pAction) );


	Proc_End:


		pIMsg = nullptr;

		return hr;
	}


	// Set Account ID, this will update Account ID table
	Result SimpleUserEntity::SetAccountID( AccountID accID )
	{
		m_AccountID = accID;
		return ResultCode::SUCCESS;
	}


	//// Called when this entity have a routed message
	//Result SimpleUserEntity::OnRoutedMessage(MessageDataPtr &pMsg)
	//{
	//	// TODO: Call process message directly when it runs on the same thread
	//	Result hr = GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(this, WeakPointerT<Net::Connection>(), pMsg));
	//	if ((hr))
	//		pMsg = nullptr;

	//	return hr;
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Overriding IConnectionEventHandler
	void SimpleUserEntity::OnConnectionEvent(Net::Connection* pConn, const Net::ConnectionEvent& evt)
	{
		Assert(pConn != nullptr);
		Assert(evt.EventType != Net::ConnectionEvent::EventTypes::EVT_NONE);
		// this one is free to call on other thread
		ProcessConnectionEvent(evt);
	}

	Result SimpleUserEntity::OnRecvMessage(Net::Connection* pConn, MessageDataPtr& pMsg)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(this, WeakPointerT<Net::Connection>(pConn), pMsg));
	}

	Result SimpleUserEntity::OnNetSyncMessage(Net::Connection* pConn)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(ServerTaskEvent::EventTypes::PACKET_MESSAGE_SYNC_EVENT, this, WeakPointerT<Net::Connection>(pConn)));
	}

	Result SimpleUserEntity::OnNetSendReadyMessage(Net::Connection* pConn)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(ServerTaskEvent::EventTypes::PACKET_MESSAGE_SEND_EVENT, this, WeakPointerT<Net::Connection>(pConn)));
	}


	Result SimpleUserEntity::OnEventTask(const ServerTaskEvent& eventTask)
	{
		Result hr = ResultCode::SUCCESS;

		TransactionPtr pCurTran;
		MessageDataPtr pMsg;
		Net::ConnectionUDPBase* pConn = nullptr;
		auto pMyConn = GetConnection();

		if (pMyConn != nullptr)
		{
			// Event task stuff will be called from entity 
			// This should be the running thread from now on
			//if (pMyConn->GetRunningThreadID() != ThisThread::GetThreadID())
			//	pMyConn->SetRunningThreadID(ThisThread::GetThreadID());
			// - Yes, but it could be still managed by connection manager. let's see what happens
			if (pMyConn->GetRunningThreadID() == ThreadID())
				pMyConn->SetRunningThreadID(ThisThread::GetThreadID());
			else
			{
				if (pMyConn->GetRunningThreadID() != ThisThread::GetThreadID())
				{
					//	return ResultCode::SUCCESS_FALSE; // this event should be rescheduled
					switch (eventTask.EventType)
					{
					case ServerTaskEvent::EventTypes::PACKET_MESSAGE_EVENT:
						AssertRel(eventTask.EventData.MessageEvent.pMessage == nullptr);
						// fallthru
					case ServerTaskEvent::EventTypes::CONNECTION_EVENT:
					case ServerTaskEvent::EventTypes::PACKET_MESSAGE_SYNC_EVENT:
					case ServerTaskEvent::EventTypes::PACKET_MESSAGE_SEND_EVENT:
					case ServerTaskEvent::EventTypes::TRANSRESULT_EVENT:
						return ResultCode::SUCCESS_FALSE;
					default:
						return ResultCode::UNEXPECTED;
					}
				}
			}
		}

		switch (eventTask.EventType)
		{
		case ServerTaskEvent::EventTypes::CONNECTION_EVENT:
			ProcessConnectionEvent(*eventTask.EventData.pConnectionEvent);
			break;
		case ServerTaskEvent::EventTypes::PACKET_MESSAGE_EVENT:
			pMsg = std::forward<MessageDataPtr>(const_cast<ServerTaskEvent&>(eventTask).EventData.MessageEvent.pMessage);
			if (pMsg != nullptr)
			{
				ProcessMessageData(pMsg);
			}
			else
			{
				pConn = dynamic_cast<Net::ConnectionUDPBase*>(*GetConnection());
				if (pConn != nullptr)
					pConn->ProcGuarrentedMessageWindow([&](MessageDataPtr& pMsg){ ProcessMessageData(pMsg); });
			}
			break;
		case ServerTaskEvent::EventTypes::PACKET_MESSAGE_SYNC_EVENT:
			if (pMyConn != nullptr) pMyConn->UpdateSendQueue();
			break;
		case ServerTaskEvent::EventTypes::PACKET_MESSAGE_SEND_EVENT:
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
					svrTrace(Warning, "Transaction result for TID:{0} is failed to route.", eventTask.EventData.pTransResultEvent->GetTransID());
					auto pRes = const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent);
					IHeap::Delete(pRes);
					//svrErr(ResultCode::FAIL);
				}
			}
			else
			{
				svrTrace(SVR_TRANSACTION, "Failed to process transaction result. null Transaction result.");
			}
			break;
		default:
			return ResultCode::UNEXPECTED;
		}

	//Proc_End:

		return hr;
	}


}; // namespace Svr
}; // namespace SF



