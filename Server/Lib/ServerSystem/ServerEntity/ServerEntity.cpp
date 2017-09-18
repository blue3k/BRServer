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
#include "Net/NetServerPeerTCP.h"
#include "SvrConst.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Task/ServerTaskEvent.h"
#include "Entity/EntityTable.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityManager.h"

#include "ServerEntity/SvrEntityServerEntity.h"
#include "Protocol/Policy/ServerNetPolicy.h"

#include "Service/EngineServices.h"


namespace SF {
namespace Svr {

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//


	ServerEntity::ServerEntity( uint uiTransQueueSize, uint TransResQueueSize )
		: MasterEntity( uiTransQueueSize, TransResQueueSize)
		, m_RecvMessageQueue(GetMemoryManager())
		, m_ServerID(0)
		, m_bIsInitialConnect(true)
		, m_ReceivedServerStatus(false)
		, m_ServerUpTime(TimeStampSec::min())
		, m_LocalConnectionRetryWait(Svr::Const::REMOTE_CONNECTION_RETRY)
	{
	}

	ServerEntity::~ServerEntity()
	{
	}

	// set connection
	Result ServerEntity::SetConnection(SharedPointerAtomicT<Net::Connection> &destConn, Net::Connection * pConn)
	{
		Result hr = ResultCode::SUCCESS;
		MutexScopeLock localLock(m_ConnectionLock);

		Assert(destConn == nullptr || destConn == pConn);

		destConn = SharedPointerT<Net::Connection>(pConn);
		svrChkPtr(pConn);

		pConn->SetEventHandler(this);

		// Server instance uses serverid as peerID
		if (m_ServerID == 0 && pConn->GetRemoteInfo().PeerID != 0)
			m_ServerID = (ServerID)pConn->GetRemoteInfo().PeerID;

		// This connection will be updated with server entity
		pConn->SetTickFlags(0);
		pConn->SetRunningThreadID(ThreadID());

	Proc_End:

		return hr;
	}

	Result ServerEntity::SetRemoteConnection( Net::Connection *pConn )
	{
		MutexScopeLock localLock(m_ConnectionLock);

		auto pCurConn = (Net::Connection*)m_pConnRemote;
		if (pCurConn != nullptr)
		{
			pCurConn->SetEventHandler(nullptr);
			pCurConn->DisconnectNRelease("ServerEntity Replacing remote connection");

			auto localCon = m_pConnLocal;
			Assert(localCon == nullptr || localCon != pCurConn);
			Assert(pConn == nullptr || localCon != pConn);

			m_pConnRemote = SharedPointerT<Net::Connection>();


			if (pConn == nullptr)
				return ResultCode::SUCCESS;
		}
		return SetConnection(m_pConnRemote, pConn);
	}

	Result ServerEntity::SetLocalConnection(Net::Connection *pConn)
	{
		m_LocalConnectionRetryTime = TimeStampMS::min();
		//Assert(m_pConnRemotePrev.load(std::memory_order_relaxed) != pConn);
		Assert(m_pConnRemote != pConn);
		return SetConnection(m_pConnLocal, pConn);
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
		if (localCon != nullptr)
		{
			localCon->SetEventHandler(nullptr);
			localCon->DisconnectNRelease("Terminate Server entity");
		}
		m_pConnLocal = nullptr;

		auto remoteCon = m_pConnRemote;
		if (remoteCon != nullptr)
		{
			remoteCon->SetEventHandler(nullptr);
			remoteCon->DisconnectNRelease("Terminate Server entity");
		}
		m_pConnRemote = nullptr;

		svrChk(MasterEntity::TerminateEntity() );

	Proc_End:

		return hr;
	}

	//// Called when this entity have a routed message
	//Result ServerEntity::OnRoutedMessage(MessageDataPtr &pMsg)
	//{
	//	// TODO: Call process message directly when it runs on the same thread
	//	Result hr = GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(this, WeakPointerT<Net::Connection>(), pMsg));
	//	if ((hr))
	//		pMsg = nullptr;

	//	return hr;
	//}

	// Process Message and release message after all processed
	Result ServerEntity::ProcessMessage(ServerEntity* pServerEntity, Net::Connection *pCon, MessageDataPtr &pMsg )
	{
		RouteContext routeContext;
		TransactionID transID;
		SharedPointerT<Entity> pEntity;

		if(pServerEntity == nullptr)
			pServerEntity = this;

		// First try to route message
		pMsg->GetRouteInfo(routeContext, transID);
		if (routeContext.GetTo() != GetEntityUID() && (Service::EntityTable->find(routeContext.GetTo(), pEntity)))
		{
			return pEntity->ProcessMessage(pServerEntity, pCon, pMsg);
		}

		if (routeContext.GetTo().UID != 0)
		{
			if (pMsg->GetMessageHeader()->msgID.IDs.Type == Message::MSGTYPE_COMMAND)
				Policy::NetSvrPolicyServer(SharedPointerT<Net::Connection>(pCon)).GenericFailureRes(routeContext.GetSwaped(), transID, ResultCode::SVR_INVALID_ENTITYUID);

			pMsg = nullptr;
			return ResultCode::SUCCESS_FALSE;
		}

		return super::ProcessMessage(pServerEntity, pCon, pMsg);
	}


	// Process Connection event
	Result ServerEntity::ProcessConnectionEvent( const Net::ConnectionEvent& conEvent )
	{
		Result hr = ResultCode::SUCCESS;

		switch( conEvent.EventType )
		{
		case Net::ConnectionEvent::EVT_CONNECTION_RESULT:
			if( conEvent.hr )
			{
				auto myConfig = BrServer::GetInstance()->GetMyConfig();
				auto netPrivate = Svr::BrServer::GetInstance()->GetNetPrivate();

				svrChkPtr(myConfig);


				svrTrace( SVR_DBGSVR, "Sending Server Connected to Entity Server from:{0}", myConfig->Name );

				// This is a replication of a remote server. ServerID in EntityUID will have remote server id then local serverID
				svrChk(Policy::NetPolicyServer(GetConnection()).ServerConnectedC2SEvt(RouteContext(Svr::BrServer::GetInstance()->GetServerUID(), 0),
					BrServer::GetInstance()->GetServerUpTime().time_since_epoch().count(),
					netPrivate->GetLocalAddress()));
			}

			if( conEvent.hr )
				m_bIsInitialConnect = false;
			break;
		case Net::ConnectionEvent::EVT_DISCONNECTED:
			m_ReceivedServerStatus = false;
			break;
		case Net::ConnectionEvent::EVT_STATE_CHANGE:
			break;
		default:
			break;
		};

	Proc_End:

		return ResultCode::SUCCESS;
	}

	Result ServerEntity::UpdateConnection(Net::Connection* pConn)
	{
		Result hr = ResultCode::SUCCESS;
		MessageDataPtr pMsg;
		Net::ConnectionEvent conEvent;

		if (pConn == nullptr)
			return hr;

		pConn->SetRunningThreadID(ThisThread::GetThreadID());

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

		if (pConn->GetConnectionState() != Net::ConnectionState::DISCONNECTED)
		{
			// Process message
			loopCount = pConn->GetRecvMessageCount();
			for (decltype(loopCount) iProc = 0; iProc < loopCount; iProc++)
			{
				if (!(pConn->GetRecvMessage(pMsg)))
					break;

				ProcessMessage(this, pConn, pMsg);
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

		return hr;
	}

	Result ServerEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		Net::Connection* pConn = nullptr;
		Net::Connection* pConnRemote = nullptr;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		svrChk(MasterEntity::TickUpdate(pAction) );

		pConn = (Net::Connection*)(Net::Connection*)m_pConnLocal;
		if (pConn != nullptr)
		{
			if (pConn->GetConnectionState() == Net::ConnectionState::DISCONNECTED)
			{
				if (m_LocalConnectionRetryTime != TimeStampMS::min())
				{
					auto& pIOCallback = pConn->GetNetIOHandler();
					if (pIOCallback.GetIsIORegistered())
					{
						pConn->CloseConnection("ServerEntity connection clean up");
						pConn->CloseSocket();
					}
					else if (Util::TimeSince(m_LocalConnectionRetryTime) > m_LocalConnectionRetryWait)
					{
						m_LocalConnectionRetryWait = Util::TimeMinNonZero(m_LocalConnectionRetryWait + DurationMS(Svr::Const::REMOTE_CONNECTION_RETRY), DurationMS(Svr::Const::REMOTE_CONNECTION_RETRY_MAX));

						m_LocalConnectionRetryTime = Util::Time.GetTimeMs();
						auto remoteInfo = pConn->GetRemoteInfo();
						auto localInfo = pConn->GetLocalInfo();
						if (GetPrivateNetAddress().Port != 0)
						{
							remoteInfo.PeerAddress = GetPrivateNetAddress();
							svrChk(pConn->Connect(localInfo, remoteInfo));
						}
						else
						{
							svrChk(pConn->Connect(localInfo, remoteInfo));
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
				if(pConn->GetConnectionState() == Net::ConnectionState::CONNECTED)
					m_LocalConnectionRetryWait = DurationMS(Svr::Const::REMOTE_CONNECTION_RETRY);

				m_LocalConnectionRetryTime = TimeStampMS::min();

				svrChk(UpdateConnection(pConn));
			}
		}

		pConnRemote = (Net::Connection*)m_pConnRemote;

		Assert(pConn == nullptr || pConn != pConnRemote);

		svrChk(UpdateConnection(pConnRemote));

	Proc_End:


		return hr;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////
	// Overriding IConnectionEventHandler
	void ServerEntity::OnConnectionEvent(Net::Connection* pConn, const Net::ConnectionEvent& evt)
	{
		Assert(pConn != nullptr);
		Assert(evt.EventType != Net::ConnectionEvent::EventTypes::EVT_NONE);
		// this one is free to call on other thread
		ProcessConnectionEvent(evt);
	}

	Result ServerEntity::OnRecvMessage(Net::Connection* pConn, MessageDataPtr& pMsg)
	{
		if (GetTaskManager() == nullptr)
			return ResultCode::INVALID_STATE;

		return GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(this, WeakPointerT<Net::Connection>(pConn), pMsg));
	}

	Result ServerEntity::OnNetSyncMessage(Net::Connection* pConn)
	{
		if (GetTaskManager() == nullptr)
			return ResultCode::INVALID_STATE;

		return GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(ServerTaskEvent::EventTypes::PACKET_MESSAGE_SYNC_EVENT, this, WeakPointerT<Net::Connection>(pConn)));
	}

	Result ServerEntity::OnNetSendReadyMessage(Net::Connection* pConn)
	{
		if (GetTaskManager() == nullptr)
			return ResultCode::INVALID_STATE;

		return GetTaskManager()->AddEventTask(GetTaskGroupID(), ServerTaskEvent(ServerTaskEvent::EventTypes::PACKET_MESSAGE_SEND_EVENT, this, WeakPointerT<Net::Connection>(pConn)));
	}

	Result ServerEntity::OnEventTask(ServerTaskEvent& eventTask)
	{
		TransactionPtr pCurTran;
		MessageDataPtr pMsg;
		SharedPointerT<Net::Connection> pMyConn;

		switch (eventTask.EventType)
		{
		case ServerTaskEvent::EventTypes::CONNECTION_EVENT:
			ProcessConnectionEvent(*eventTask.EventData.pConnectionEvent);
			break;
		case ServerTaskEvent::EventTypes::PACKET_MESSAGE_EVENT:
			pMsg = eventTask.EventData.MessageEvent.pMessage;
			if (pMsg != nullptr)
			{
				ProcessMessage(this, *GetConnection(), pMsg);
			}
			else
			{
				svrTrace(Error, "null message pointer in event taqsk");
			}
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
					auto pNonConst = const_cast<TransactionResult*>(eventTask.EventData.pTransResultEvent);
					IHeap::Delete(pNonConst);
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

		return ResultCode::SUCCESS;
	}



}; // namespace Svr
}; // namespace SF



