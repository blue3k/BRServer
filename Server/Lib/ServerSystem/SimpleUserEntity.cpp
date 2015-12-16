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
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/Utility.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/SimpleUserEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/EventTask.h"
#include "Net/ConnectionUDP.h"


namespace BR {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//


	SimpleUserEntity::SimpleUserEntity()
		:SimpleEntity(Const::Default::ENTITY_SIMPLE_TRANS_QUEUE, Const::Default::ENTITY_SIMPLE_TRANSRES_QUEUE),
		m_pConnection(nullptr)
	{
		SetTickInterval(DurationMS(Svr::Const::SIMPLEUSER_TICKTASK_INTERVAL));
	}

	SimpleUserEntity::~SimpleUserEntity()
	{
	}


	// set connection
	HRESULT SimpleUserEntity::SetConnection(SharedPointerT<Net::Connection>&& pConn)
	{
		MutexScopeLock localLock(m_ConnectionLock);

		m_pConnection = std::forward<SharedPointerT<Net::Connection>>(pConn);
		if (m_pConnection != nullptr)
		{
			m_pConnection->SetConnectionEventHandler(this);

			// purge received guaranted messages
			OnRecvMessage((Net::Connection*)m_pConnection, nullptr);

			m_pConnection->GetNet()->TakeOverConnection((Net::Connection*)m_pConnection);
		}

		return S_OK;
	}

	// Release connection if has
	void SimpleUserEntity::ReleaseConnection()
	{
		MutexScopeLock localLock(m_ConnectionLock);

		if( m_pConnection == nullptr )
			return;

		m_pConnection->SetConnectionEventHandler(nullptr);
		m_pConnection->GetNet()->ReleaseConnection((Net::Connection*)m_pConnection);
		m_pConnection = SharedPointerT<Net::Connection>();
	}

	void SimpleUserEntity::GetConnectionShared(SharedPointerT<Net::Connection>& outConn)
	{
		MutexScopeLock localLock(m_ConnectionLock);

		outConn = m_pConnection;
	}

	// Initialize entity to proceed new connection
	HRESULT SimpleUserEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		SetAccountID( 0 );
		SetAuthTicket( 0 );

		svrChk(SimpleEntity::InitializeEntity( newEntityID ) );

	Proc_End:

		return hr;
	}



	// Close entity and clear transaction
	HRESULT SimpleUserEntity::TerminateEntity()
	{
		HRESULT hr = S_OK;

		if( GetEntityState() == EntityState::FREE )
			return S_OK;

		SetAccountID( 0 );
		SetAuthTicket( 0 );

		if (m_pConnection != nullptr)
			m_pConnection->SetConnectionEventHandler(nullptr);

		svrChk(SimpleEntity::TerminateEntity() );

	Proc_End:

		return hr;
	}


	// Process Connection event
	HRESULT SimpleUserEntity::ProcessConnectionEvent( const Net::IConnection::Event& conEvent )
	{
		HRESULT hr = S_OK;

		switch( conEvent.EventType )
		{
		case Net::IConnection::Event::EVT_CONNECTION_RESULT:
			break;
		case Net::IConnection::Event::EVT_DISCONNECTED:
			break;
		case Net::IConnection::Event::EVT_STATE_CHANGE:
			break;
		default:
			break;
		};

	//Proc_End:

		return hr;
	}


	// Process Message and release message after all processed
	HRESULT SimpleUserEntity::ProcessMessage(Message::MessageData* &pIMsg)
	{
		HRESULT hr = S_OK;
		EntityID entityID; // entity ID to route
		Message::MessageHeader *pMsgHdr = nullptr;
		Svr::Transaction *pNewTrans = nullptr;
		ThreadID currentThreadID = GetTaskWorker() != nullptr ? GetTaskWorker()->GetThreadID() : ThisThread::GetThreadID();

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
			Assert(m_pHandlerTable);
			if (FAILED(m_pHandlerTable->HandleMessage<Svr::Transaction*&>(GetConnection(), pIMsg, pNewTrans)))
			{
				svrTrace(Trace::TRC_ERROR, "Failed to handle remote message Entity:%0%:%1%, MsgID:%2%", typeid(*this).name(), GetEntityID(), pMsgHdr->msgID);
				svrErr(E_SVR_NOTEXPECTED_MESSAGE);
			}
			break;
		}
		default:
			svrTrace(Trace::TRC_ERROR, "Not Processed Remote message Entity:%0%:%1%, MsgID:%2%", typeid(*this).name(), GetEntityID(), pMsgHdr->msgID);
			svrErr(E_SVR_NOTEXPECTED_MESSAGE);
			break;
		};


		if (pNewTrans)
		{
			if (pNewTrans->GetOwnerEntity() == nullptr)
			{
				svrChk(pNewTrans->InitializeTransaction(this));
			}

			if (pNewTrans->IsDirectProcess())
			{
				HRESULT hrRes = pNewTrans->StartTransaction();
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

		if (pNewTrans && !pNewTrans->IsClosed())
		{
			pNewTrans->CloseTransaction(hr);
		}


		ReleaseTransaction(pNewTrans);
		Util::SafeRelease(pIMsg);

		return S_OK;

	}



	HRESULT SimpleUserEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
		Message::MessageData *pIMsg = nullptr;
		Net::IConnection::Event conEvent;


		if( GetEntityState() == EntityState::FREE )
			return S_OK;

		// Update connection
		// We need to make a copy here while m_pConnection can be changed on another thread
		SharedPointerT<Net::Connection> pConn;
		GetConnectionShared(pConn);

		if (pConn != nullptr)
			pConn->UpdateNetCtrl();

		// We need to check m_pConnection again because the connection can be release during update
		if (pConn != nullptr)
		{
			// Process Connection event
			auto loopCount = pConn->GetConnectionEventCount();
			for (decltype(loopCount) iProc = 0; iProc < loopCount; iProc++)
			{
				if (FAILED(pConn->DequeueConnectionEvent(conEvent)))
					break;

				ProcessConnectionEvent( conEvent );
			}


			if (pConn->GetConnectionState() != Net::IConnection::STATE_DISCONNECTED)
			{
				// Process message
				loopCount = pConn->GetRecvMessageCount();
				for( decltype(loopCount) iProc = 0; iProc < loopCount; iProc++ )
				{
					if (FAILED(pConn->GetRecvMessage(pIMsg)))
						break;

					ProcessMessage( pIMsg );

					Util::SafeRelease( pIMsg );
				}
			}
		}

		svrChk(SimpleEntity::TickUpdate(pAction) );


	Proc_End:

		if( pIMsg )
			Util::SafeDelete( pIMsg );


		return hr;
	}


	// Set Account ID, this will update Account ID table
	HRESULT SimpleUserEntity::SetAccountID( AccountID accID )
	{
		m_AccountID = accID;
		return S_OK;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Overriding IConnectionEventHandler
	void SimpleUserEntity::OnConnectionEvent(Net::IConnection* pConn, const Net::IConnection::Event& evt)
	{
		Assert(pConn != nullptr);
		Assert(evt.EventType != Net::IConnection::Event::EventTypes::EVT_NONE);
		// this one is free to call on other thread
		ProcessConnectionEvent(evt);
	}

	HRESULT SimpleUserEntity::OnRecvMessage(Net::IConnection* pConn, Message::MessageData* pMsg)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(this, WeakPointerT<Net::IConnection>(pConn), pMsg));
	}

	HRESULT SimpleUserEntity::OnNetSyncMessage(Net::IConnection* pConn)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(EventTask::EventTypes::PACKET_MESSAGE_SYNC_EVENT, this, WeakPointerT<Net::IConnection>(pConn)));
	}

	HRESULT SimpleUserEntity::OnNetSendReadyMessage(Net::IConnection* pConn)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(EventTask::EventTypes::PACKET_MESSAGE_SEND_EVENT, this, WeakPointerT<Net::IConnection>(pConn)));
	}


	HRESULT SimpleUserEntity::OnEventTask(const Svr::EventTask& eventTask)
	{
		HRESULT hr = S_OK;

		Transaction *pCurTran = nullptr;
		Message::MessageData* pMsg = nullptr;
		Net::ConnectionUDPBase* pConn = nullptr;
		auto pMyConn = GetConnection();

		switch (eventTask.EventType)
		{
		case Svr::EventTask::EventTypes::CONNECTION_EVENT:
			ProcessConnectionEvent(*eventTask.EventData.pConnectionEvent);
			break;
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_EVENT:
			pMsg = eventTask.EventData.MessageEvent.pMessage;
			pConn = BR_DYNAMIC_CAST(Net::ConnectionUDPBase*,GetConnection());
			if (pMsg != nullptr)
			{
				ProcessMessage(pMsg);
				Util::SafeRelease(pMsg);
			}
			else
			{
				if (pConn != nullptr)
					pConn->ProcGuarrentedMessageWindow([&](Message::MessageData* pMsg){ ProcessMessage(pMsg); });
			}
			break;
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_SYNC_EVENT:
			if (pMyConn != nullptr) pMyConn->UpdateSendQueue();
			break;
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_SEND_EVENT:
			if (pMyConn != nullptr) pMyConn->UpdateSendBufferQueue();
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
					svrTrace(Trace::TRC_WARN, "Transaction result for TID:%0% is failed to route.", eventTask.EventData.pTransResultEvent->GetTransID());
					auto pRes = const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent);
					Util::SafeRelease(pRes);
					//svrErr(E_FAIL);
				}
			}
			else
			{
				svrTrace(Svr::TRC_TRANSACTION, "Faild to process transaction result. null Transaction result.");
			}
			break;
		default:
			return E_UNEXPECTED;
		}

	//Proc_End:

		return hr;
	}


}; // namespace Svr
}; // namespace BR



