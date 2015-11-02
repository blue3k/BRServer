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


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Net/NetServerPeer.h"
#include "Net/NetServerPeerTCP.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/EventTask.h"

#include "ServerSystem/ServerEntity/EntityServerEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "Protocol/Policy/ServerIPolicy.h"


namespace BR {
namespace Svr {

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//


	ServerEntity::ServerEntity( UINT uiTransQueueSize, UINT TransResQueueSize )
		: MasterEntity( uiTransQueueSize, TransResQueueSize)
		, m_pConnRemote(nullptr)
		, m_pConnLocal(nullptr)
		, m_bIsInitialConnect(true)
		//, m_pConnRemotePrev(nullptr),
		, m_ServerID(0)
		, m_ServerUpTime(TimeStampSec::min())
		, m_LocalConnectionRetryWait(Svr::Const::REMOTE_CONNECTION_RETRY)
	{
	}

	ServerEntity::~ServerEntity()
	{
	}

	// set connection
	HRESULT ServerEntity::SetConnection(SharedPointerT<Net::IConnection> &destConn, BR::Net::IConnection * pConn)
	{
		HRESULT hr = S_OK;
		MutexScopeLock localLock(m_ConnectionLock);

		Assert(destConn == nullptr || destConn == pConn);

		destConn = SharedPointerT<Net::IConnection>(pConn);
		svrChkPtr(pConn);

		pConn->SetConnectionEventHandler(this);

		svrChk(pConn->CreatePolicy(POLICY_SERVER));
		svrChk(pConn->CreatePolicy(POLICY_SVR_SERVER));

		if (m_ServerID == 0 && pConn->GetConnectionInfo().RemoteID != 0)
			m_ServerID = (ServerID)pConn->GetConnectionInfo().RemoteID;

	Proc_End:

		return hr;
	}

	HRESULT ServerEntity::SetRemoteConnection( BR::Net::IConnection *pConn )
	{
		MutexScopeLock localLock(m_ConnectionLock);

		auto pCurConn = (Net::IConnection*)m_pConnRemote;
		if (pCurConn != nullptr)
		{
			pCurConn->SetConnectionEventHandler(nullptr);
			pCurConn->GetNet()->ReleaseConnection(pCurConn);

			auto localCon = m_pConnLocal;
			Assert(localCon == nullptr || localCon != pCurConn);
			Assert(pConn == nullptr || localCon != pConn);

			m_pConnRemote = SharedPointerT<Net::Connection>();


			if (pConn == nullptr)
				return S_OK;
		}
		return SetConnection(m_pConnRemote, pConn);
	}

	HRESULT ServerEntity::SetLocalConnection(BR::Net::IConnection *pConn)
	{
		m_LocalConnectionRetryTime = TimeStampMS::min();
		//Assert(m_pConnRemotePrev.load(std::memory_order_relaxed) != pConn);
		Assert(m_pConnRemote != pConn);
		return SetConnection(m_pConnLocal, pConn);
	}

	void ServerEntity::GetConnectionShared(SharedPointerT<Net::Connection>& outConn)
	{
		MutexScopeLock localLock(m_ConnectionLock);

		auto pConn = GetConnection();
		outConn = pConn != nullptr ? (Net::Connection*)pConn : SharedPointerT<Net::Connection>();
	}

	// Initialize entity to proceed new connection
	HRESULT ServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeEntity( newEntityID ) );

		m_RecvMessageQueue.ClearQueue();

		// Initial connect
		m_bIsInitialConnect = true;
		m_LocalConnectionRetryTime = TimeStampMS::min();

	Proc_End:

		return hr;
	}



	// Close entity and clear transaction
	HRESULT ServerEntity::TerminateEntity()
	{
		HRESULT hr = S_OK;

		if( GetEntityState() == EntityState::FREE )
			return S_OK;

		auto localCon = m_pConnLocal;
		if (localCon != nullptr) localCon->SetConnectionEventHandler(nullptr);

		auto remoteCon = m_pConnRemote;
		if (remoteCon != nullptr) remoteCon->SetConnectionEventHandler(nullptr);

		svrChk( __super::TerminateEntity() );

	Proc_End:

		return hr;
	}

	// Process Message and release message after all processed
	HRESULT ServerEntity::ProcessMessage( Net::IConnection *pCon, Message::MessageData* &pMsg )
	{
		HRESULT hr = S_OK;
		EntityID entityID; // entity ID to route
		Message::MessageHeader *pMsgHdr = nullptr;
		Svr::Transaction *pNewTrans = nullptr;

		svrChkPtr( pMsg );
		pMsgHdr = pMsg->GetMessageHeader();

		switch( pMsgHdr->msgID.IDs.Type )
		{
		case Message::MSGTYPE_RESULT:
			svrChk( ProcessMessageResult( pMsg ) );
			goto Proc_End;

			break;
		case Message::MSGTYPE_COMMAND:
		case Message::MSGTYPE_EVENT:
		{
			Assert(GetMessageHandlerTable());
			if(FAILED( GetMessageHandlerTable()->HandleMessage<Svr::Transaction*&>( pCon, pMsg, pNewTrans ) ) )
			{
				// If it couldn't find a handler in server entity handlers, looking for it in server loopback entity
				MessageHandlerType handler;

				hr = GetLoopbackServerEntity()->GetMessageHandlerTable()->GetHandler(pMsg->GetMessageHeader()->msgID,handler);
				if(FAILED(hr))
				{
					svrTrace( Trace::TRC_ERROR, "No message handler %0%:%1%, MsgID:%2%", typeid(*this).name(), GetEntityUID(), pMsgHdr->msgID );
					svrErr(E_SVR_NO_MESSAGE_HANDLER);
				}

				svrChk( handler( pCon, pMsg, pNewTrans ) );
			}
			break;
		}
		default:
			svrTrace( Trace::TRC_ERROR, "Not Processed Remote message Entity:%0%:%1%, MsgID:%2%", typeid(*this).name(), GetEntityUID(), pMsgHdr->msgID );
			svrErr( E_SVR_NOTEXPECTED_MESSAGE );
			break;
		};


		if( pNewTrans )
		{
			if( pNewTrans->GetOwnerEntity() == nullptr )
			{
				hr = pNewTrans->InitializeTransaction(this);
				if (FAILED(hr)) goto Proc_End;
			}

			if( pNewTrans->IsDirectProcess() )
			{
				HRESULT hrRes = pNewTrans->StartTransaction();
				if( !pNewTrans->IsClosed() )
				{
					pNewTrans->CloseTransaction(hrRes);
				}
			}
			else
			{
				if (this == pNewTrans->GetOwnerEntity())
				{
					auto threadID = GetTaskWorker() ? GetTaskWorker()->GetThreadID() : ThisThread::GetThreadID();
					PendingTransaction(threadID, pNewTrans);
				}

				if (pNewTrans != nullptr && BrServer::GetInstance())
				{
					svrChk(BrServer::GetInstance()->GetEntityTable().RouteTransaction(pNewTrans->GetTransID().EntityID, pNewTrans));
				}
			}
		}

	Proc_End:

		if (pNewTrans != nullptr)
		{
			if (FAILED(hr))
			{
				svrTrace(Trace::TRC_ERROR, "Transaction initialization is failed %0% Entity:%1%, MsgID:%2%", typeid(*this).name(), GetEntityUID(), pMsgHdr->msgID);
				if (pMsgHdr->msgID.IDs.Type == BR::Message::MSGTYPE_COMMAND)
				{
					pCon->GetPolicy<Policy::ISvrPolicyServer>()->GenericFailureRes(pNewTrans->GetParentTransID(), hr, pNewTrans->GetMessageRouteContext().GetSwaped());
				}
			}

			if (!pNewTrans->IsClosed() && pNewTrans->GetOwnerEntity() != nullptr)
			{
				//Assert(false);
				svrTrace(Trace::TRC_ERROR, "Transaction isn't closed Transaction:%0%, MsgID:%1%", typeid(*pNewTrans).name(), pMsgHdr->msgID);
				pNewTrans->CloseTransaction(hr);
			}

			ReleaseTransaction(pNewTrans);
		}

		Util::SafeRelease( pMsg );

		return S_OK;
	}


	// Process Connection event
	HRESULT ServerEntity::ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent )
	{
		HRESULT hr = S_OK;

		switch( conEvent.EventType )
		{
		case BR::Net::IConnection::Event::EVT_CONNECTION_RESULT:
			if( SUCCEEDED(conEvent.Value.hr) )
			{
				NetAddress publicAddr;
				auto myConfig = BrServer::GetInstance()->GetMyConfig();
				auto netPrivate = Svr::BrServer::GetInstance()->GetNetPrivate();
				const Svr::Config::PublicServer *pGeneric = nullptr;

				svrChkPtr( BrServer::GetInstance() );
				svrChkPtr( BrServer::GetInstance()->GetMyConfig() );
				pGeneric = dynamic_cast<const Svr::Config::PublicServer*>(myConfig);
				if( pGeneric )
				{
					svrChkPtr(pGeneric->NetPublic);
					svrChk( StrUtil::StringCpy( publicAddr.strAddr, pGeneric->NetPublic->IP.c_str() ) );
					publicAddr.usPort = pGeneric->NetPublic->Port;
				}
				else
				{
					memset( &publicAddr, 0, sizeof(NetAddress) );
				}

				svrTrace( Svr::TRC_DBGSVR, "Sending Server Connected to Entity Server from:%0%", BrServer::GetInstance()->GetMyConfig()->Name.c_str() );

				Policy::IPolicyServer *pPolicy = GetConnection()->GetPolicy<Policy::IPolicyServer>();
				svrChkPtr(pPolicy);
				const ServerServiceInformation* pServerServiceInfo = Svr::BrServer::GetInstance()->GetComponent<ClusterManagerServiceEntity>()->GetMyServiceInfo();
				ServiceInformation serviceInformation( pServerServiceInfo->GetEntityUID(), 
					pServerServiceInfo->GetClusterMembership(), 
					pServerServiceInfo->GetServiceStatus(), 
					Svr::BrServer::GetInstance()->GetNetPrivate()->GetNetClass(),
					Svr::BrServer::GetInstance()->GetNetPrivate()->GetLocalAddress(), 
					Svr::BrServer::GetInstance()->GetServerUpTime(), 
					pServerServiceInfo->GetWorkload() );

				// This is a replication of a remote server. ServerID in EntityUID wil have remote server id then local serverID
				svrChk(pPolicy->ServerConnectedC2SEvt(RouteContext(Svr::BrServer::GetInstance()->GetServerUID(), 0),
					serviceInformation, 
					BrServer::GetInstance()->GetServerUpTime().time_since_epoch().count(),
					publicAddr, netPrivate->GetLocalAddress()));
			}

			if( SUCCEEDED(conEvent.Value.hr) )
				m_bIsInitialConnect = false;
			break;
		case BR::Net::IConnection::Event::EVT_DISCONNECTED:
			m_ReceivedServerStatus = false;
			break;
		case BR::Net::IConnection::Event::EVT_STATE_CHANGE:

			break;
		};

	Proc_End:

		return S_OK;
	}

	HRESULT ServerEntity::UpdateConnection(BR::Net::IConnection* pConn)
	{
		HRESULT hr = S_OK;
		Message::MessageData *pMsg = nullptr;
		BR::Net::IConnection::Event conEvent;

		if (pConn == nullptr)
			return hr;

		// update connection
		pConn->UpdateNetCtrl();

		// Process Connection event
		auto loopCount = pConn->GetConnectionEventCount();
		for (int iProc = 0; iProc < loopCount; iProc++)
		{
			if (FAILED(pConn->DequeueConnectionEvent(conEvent)))
				break;

			ProcessConnectionEvent(conEvent);
		}

		if (pConn->GetConnectionState() != BR::Net::IConnection::STATE_DISCONNECTED)
		{
			// Process message
			loopCount = pConn->GetRecvMessageCount();
			for (int iProc = 0; iProc < loopCount; iProc++)
			{
				if (FAILED(pConn->GetRecvMessage(pMsg)))
					break;

				ProcessMessage(pConn, pMsg);

				Util::SafeRelease(pMsg);
			}
		}
		else
		{
			// Clear server up time if the connection is closed
			if (GetServerUpTime() != TimeStampSec::min())
			{
				SetServerUpTime(TimeStampSec::min());
			}
		}

	Proc_End:

		if (pMsg)
			Util::SafeDelete(pMsg);

		return hr;
	}

	HRESULT ServerEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;

		if( GetEntityState() == EntityState::FREE )
			return S_OK;

		svrChk( __super::TickUpdate(pAction) );

		auto pConn = (Net::IConnection*)m_pConnLocal;
		if (pConn != nullptr)
		{
			if (pConn->GetConnectionState() == Net::IConnection::STATE_DISCONNECTED)
			{
				if (m_LocalConnectionRetryTime != TimeStampMS::min())
				{
					if ((Util::Time.GetTimeMs() - m_LocalConnectionRetryTime) > m_LocalConnectionRetryWait)
					{
						m_LocalConnectionRetryWait = Util::TimeMinNonZero(m_LocalConnectionRetryWait + DurationMS(Svr::Const::REMOTE_CONNECTION_RETRY), DurationMS(Svr::Const::REMOTE_CONNECTION_RETRY_MAX));

						m_LocalConnectionRetryTime = Util::Time.GetTimeMs();
						auto connectionInfo = pConn->GetConnectionInfo();
						if (GetPrivateNetAddress().usPort != 0)
						{
							svrChk(pConn->GetNet()->Connect(pConn, (UINT)connectionInfo.RemoteID, connectionInfo.RemoteClass, GetPrivateNetAddress().strAddr, GetPrivateNetAddress().usPort));
						}
						else
						{
							svrChk(pConn->GetNet()->Connect(pConn, (UINT)connectionInfo.RemoteID, connectionInfo.RemoteClass, connectionInfo.Remote.strAddr, connectionInfo.Remote.usPort));
						}
					}
				}
				else
				{
					m_LocalConnectionRetryTime = Util::Time.GetTimeMs();
				}
			}
			else
			{
				if(pConn->GetConnectionState() == Net::IConnection::STATE_CONNECTED)
					m_LocalConnectionRetryWait = DurationMS(Svr::Const::REMOTE_CONNECTION_RETRY);

				m_LocalConnectionRetryTime = TimeStampMS::min();

				svrChk(UpdateConnection(pConn));
			}
		}

		auto pConnRemote = (Net::IConnection*)m_pConnRemote;
		//auto prevConn = m_pConnRemotePrev.load(std::memory_order_relaxed);
		Assert(pConn == nullptr || pConn != pConnRemote);
		//Assert(prevConn == nullptr || prevConn != pConnRemote);
		//if (prevConn != nullptr && pConnRemote != prevConn)
		//{
		//	prevConn->CloseConnection();
		//	Assert(prevConn->GetRefCount() == 1);
		//	prevConn->GetNet()->ReleaseConnection(prevConn);
		//	m_pConnRemotePrev.store(nullptr, std::memory_order_relaxed);
		//}
		svrChk(UpdateConnection(pConnRemote));

	Proc_End:


		return hr;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Overriding IConnectionEventHandler
	void ServerEntity::OnConnectionEvent(Net::IConnection* pConn, const Net::IConnection::Event& evt)
	{
		Assert(pConn != nullptr);
		Assert(evt.EventType != Net::IConnection::Event::EventTypes::EVT_NONE);
		// this one is free to call on other thread
		ProcessConnectionEvent(evt);
	}

	HRESULT ServerEntity::OnRecvMessage(Net::IConnection* pConn, Message::MessageData* pMsg)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(this, WeakPointerT<Net::IConnection>(pConn), pMsg));
	}

	HRESULT ServerEntity::OnNetSyncMessage(Net::IConnection* pConn, Net::NetCtrlIDs netCtrlID)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(this, WeakPointerT<Net::IConnection>(pConn)));
	}

	HRESULT ServerEntity::OnEventTask(const Svr::EventTask& eventTask)
	{
		Transaction *pCurTran = nullptr;
		Message::MessageData* pMsg = nullptr;

		switch (eventTask.EventType)
		{
		case Svr::EventTask::EventTypes::CONNECTION_EVENT:
			ProcessConnectionEvent(*eventTask.EventData.pConnectionEvent);
			break;
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_EVENT:
			pMsg = eventTask.EventData.MessageEvent.pMessage;
			if (pMsg != nullptr)
			{
				ProcessMessage(GetConnection(), pMsg);
				Util::SafeRelease(pMsg);
			}
			else
			{
				svrTrace(Trace::TRC_ERROR, "null message pointer in event taqsk");
			}
			break;
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_SEND_EVENT:
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
					Util::SafeRelease(const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent));
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

		return S_OK;
	}



}; // namespace Svr
}; // namespace BR



