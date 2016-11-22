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
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/EntityManager.h"

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
		, m_ServerID(0)
		, m_bIsInitialConnect(true)
		, m_ReceivedServerStatus(false)
		, m_ServerUpTime(TimeStampSec::min())
		, m_pConnRemote(nullptr)
		, m_pConnLocal(nullptr)
		, m_LocalConnectionRetryWait(Svr::Const::REMOTE_CONNECTION_RETRY)
	{
	}

	ServerEntity::~ServerEntity()
	{
	}

	// set connection
	Result ServerEntity::SetConnection(SharedPointerT<Net::IConnection> &destConn, Net::IConnection * pConn)
	{
		Result hr = ResultCode::SUCCESS;
		MutexScopeLock localLock(m_ConnectionLock);

		Assert(destConn == nullptr || destConn == pConn);

		destConn = SharedPointerT<Net::IConnection>(pConn);
		svrChkPtr(pConn);

		pConn->SetConnectionEventHandler(this);

		svrChk(pConn->CreatePolicy(POLICY_SERVER));
		svrChk(pConn->CreatePolicy(POLICY_SVR_SERVER));

		if (m_ServerID == 0 && pConn->GetConnectionInfo().RemoteID != 0)
			m_ServerID = (ServerID)pConn->GetConnectionInfo().RemoteID;

		if(pConn->GetNet() != nullptr)
			pConn->GetNet()->TakeOverConnection((Net::Connection*)pConn);


	Proc_End:

		return hr;
	}

	Result ServerEntity::SetRemoteConnection( Net::IConnection *pConn )
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
				return ResultCode::SUCCESS;
		}
		return SetConnection(m_pConnRemote, pConn);
	}

	Result ServerEntity::SetLocalConnection(Net::IConnection *pConn)
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
	Result ServerEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(MasterEntity::InitializeEntity( newEntityID ) );

		m_RecvMessageQueue.ClearQueue();

		// Initial connect
		m_bIsInitialConnect = true;
		m_LocalConnectionRetryTime = TimeStampMS::min();

	Proc_End:

		return hr;
	}



	// Close entity and clear transaction
	Result ServerEntity::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		auto localCon = m_pConnLocal;
		if (localCon != nullptr) localCon->SetConnectionEventHandler(nullptr);

		auto remoteCon = m_pConnRemote;
		if (remoteCon != nullptr) remoteCon->SetConnectionEventHandler(nullptr);

		svrChk(MasterEntity::TerminateEntity() );

	Proc_End:

		return hr;
	}

	//// Called when this entity have a routed message
	//Result ServerEntity::OnRoutedMessage(Message::MessageData* &pMsg)
	//{
	//	// TODO: Call process message directly when it runs on the same thread
	//	Result hr = GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(this, WeakPointerT<Net::IConnection>(), pMsg));
	//	if ((hr))
	//		pMsg = nullptr;

	//	return hr;
	//}

	// Process Message and release message after all processed
	Result ServerEntity::ProcessMessage(ServerEntity* pServerEntity, Net::IConnection *pCon, Message::MessageData* &pMsg )
	{
		RouteContext routeContext;
		TransactionID transID;
		SharedPointerT<Entity> pEntity;

		if(pServerEntity == nullptr)
			pServerEntity = this;

		// First try to route message
		pMsg->GetRouteInfo(routeContext, transID);
		if (routeContext.GetTo() != GetEntityUID() && (GetServerComponent<EntityManager>()->FindEntity(routeContext.GetTo(), pEntity)))
		{
			return pEntity->ProcessMessage(pServerEntity, pCon, pMsg);
		}

		if (routeContext.GetTo() != 0)
		{
			if(pMsg->GetMessageHeader()->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
				pCon->GetPolicy<Policy::ISvrPolicyServer>()->GenericFailureRes(routeContext.GetSwaped(), transID, ResultCode::E_SVR_INVALID_ENTITYUID);

			Util::SafeRelease(pMsg);
			return ResultCode::SUCCESS_FALSE;
		}

		return super::ProcessMessage(pServerEntity, pCon, pMsg);
	}


	// Process Connection event
	Result ServerEntity::ProcessConnectionEvent( const Net::IConnection::Event& conEvent )
	{
		Result hr = ResultCode::SUCCESS;

		switch( conEvent.EventType )
		{
		case Net::IConnection::Event::EVT_CONNECTION_RESULT:
			if( conEvent.hr )
			{
				auto myConfig = BrServer::GetInstance()->GetMyConfig();
				auto netPrivate = Svr::BrServer::GetInstance()->GetNetPrivate();

				svrChkPtr(myConfig);


				svrTrace( Svr::TRC_DBGSVR, "Sending Server Connected to Entity Server from:{0}", myConfig->Name.c_str() );

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
					netPrivate->GetLocalAddress()));
			}

			if( conEvent.hr )
				m_bIsInitialConnect = false;
			break;
		case Net::IConnection::Event::EVT_DISCONNECTED:
			m_ReceivedServerStatus = false;
			break;
		case Net::IConnection::Event::EVT_STATE_CHANGE:
			break;
		default:
			break;
		};

	Proc_End:

		return ResultCode::SUCCESS;
	}

	Result ServerEntity::UpdateConnection(Net::IConnection* pConn)
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageData *pMsg = nullptr;
		Net::IConnection::Event conEvent;

		if (pConn == nullptr)
			return hr;

		// update connection
		pConn->UpdateNetCtrl();

		// Process Connection event
		auto loopCount = pConn->GetConnectionEventCount();
		for (decltype(loopCount) iProc = 0; iProc < loopCount; iProc++)
		{
			if (!(pConn->DequeueConnectionEvent(conEvent)))
				break;

			ProcessConnectionEvent(conEvent);
		}

		if (pConn->GetConnectionState() != Net::IConnection::STATE_DISCONNECTED)
		{
			// Process message
			loopCount = pConn->GetRecvMessageCount();
			for (decltype(loopCount) iProc = 0; iProc < loopCount; iProc++)
			{
				if (!(pConn->GetRecvMessage(pMsg)))
					break;

				ProcessMessage(this, pConn, pMsg);

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

	//Proc_End:

		if (pMsg)
			Util::SafeDelete(pMsg);

		return hr;
	}

	Result ServerEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		Net::Connection* pConn = nullptr;
		Net::IConnection* pConnRemote = nullptr;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		svrChk(MasterEntity::TickUpdate(pAction) );

		pConn = (Net::Connection*)(Net::IConnection*)m_pConnLocal;
		if (pConn != nullptr)
		{
			if (pConn->GetConnectionState() == Net::IConnection::STATE_DISCONNECTED)
			{
				if (m_LocalConnectionRetryTime != TimeStampMS::min())
				{
					auto pIOCallback = pConn->GetIOCallback();
					if (pIOCallback != nullptr && pIOCallback->GetIsIORegistered())
					{
						if(pConn->GetSocket() != INVALID_SOCKET)
							pConn->CloseSocket();
						else if(Util::TimeSince(m_LocalConnectionRetryTime) > DurationMS(5*60*1000))
						{
							// waited too much, force clear
							pIOCallback->OnIOUnregistered();
						}
					}
					else if (Util::TimeSince(m_LocalConnectionRetryTime) > m_LocalConnectionRetryWait)
					{
						m_LocalConnectionRetryWait = Util::TimeMinNonZero(m_LocalConnectionRetryWait + DurationMS(Svr::Const::REMOTE_CONNECTION_RETRY), DurationMS(Svr::Const::REMOTE_CONNECTION_RETRY_MAX));

						m_LocalConnectionRetryTime = Util::Time.GetTimeMs();
						auto connectionInfo = pConn->GetConnectionInfo();
						if (GetPrivateNetAddress().usPort != 0)
						{
							svrChk(pConn->GetNet()->Connect(pConn, (UINT)connectionInfo.RemoteID, connectionInfo.RemoteClass, GetPrivateNetAddress()));
						}
						else
						{
							svrChk(pConn->GetNet()->Connect(pConn, (UINT)connectionInfo.RemoteID, connectionInfo.RemoteClass, connectionInfo.Remote));
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

		pConnRemote = (Net::IConnection*)m_pConnRemote;
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

	Result ServerEntity::OnRecvMessage(Net::IConnection* pConn, Message::MessageData* pMsg)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(this, WeakPointerT<Net::IConnection>(pConn), pMsg));
	}

	Result ServerEntity::OnNetSyncMessage(Net::IConnection* pConn)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(EventTask::EventTypes::PACKET_MESSAGE_SYNC_EVENT, this, WeakPointerT<Net::IConnection>(pConn)));
	}

	Result ServerEntity::OnNetSendReadyMessage(Net::IConnection* pConn)
	{
		return GetTaskManager()->AddEventTask(GetTaskGroupID(), EventTask(EventTask::EventTypes::PACKET_MESSAGE_SEND_EVENT, this, WeakPointerT<Net::IConnection>(pConn)));
	}

	Result ServerEntity::OnEventTask(const Svr::EventTask& eventTask)
	{
		Transaction *pCurTran = nullptr;
		Message::MessageData* pMsg = nullptr;
		SharedPointerT<Net::IConnection> pMyConn;

		switch (eventTask.EventType)
		{
		case Svr::EventTask::EventTypes::CONNECTION_EVENT:
			ProcessConnectionEvent(*eventTask.EventData.pConnectionEvent);
			break;
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_EVENT:
			pMsg = eventTask.EventData.MessageEvent.pMessage;
			if (pMsg != nullptr)
			{
				ProcessMessage(this, GetConnection(), pMsg);
				Util::SafeRelease(pMsg);
			}
			else
			{
				svrTrace(Trace::TRC_ERROR, "null message pointer in event taqsk");
			}
			break;
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_SYNC_EVENT:
			eventTask.EventData.MessageEvent.pConn.GetSharedPointer(pMyConn);
			if (pMyConn != nullptr) pMyConn->UpdateSendQueue();
			break;
		case Svr::EventTask::EventTypes::PACKET_MESSAGE_SEND_EVENT:
			eventTask.EventData.MessageEvent.pConn.GetSharedPointer(pMyConn);
			if (pMyConn != nullptr) pMyConn->UpdateSendBufferQueue();
			break;
		case Svr::EventTask::EventTypes::TRANSRESULT_EVENT:
			if (eventTask.EventData.pTransResultEvent != nullptr)
			{
				if ((FindActiveTransaction(eventTask.EventData.pTransResultEvent->GetTransID(), pCurTran)))
				{
					ProcessTransactionResult(pCurTran, eventTask.EventData.pTransResultEvent);
				}
				else
				{
					auto pNonConst = const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent);
					Util::SafeRelease(pNonConst);
				}
			}
			else
			{
				svrTrace(Svr::TRC_TRANSACTION, "Faild to process transaction result. null Transaction result.");
			}
			break;
		default:
			return ResultCode::UNEXPECTED;
		}

		return ResultCode::SUCCESS;
	}



}; // namespace Svr
}; // namespace BR



