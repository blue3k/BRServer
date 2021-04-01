////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "MemoryManager/SFMemory.h"
#include "GameConst.h"
#include "Net/SFNetDef.h"
#include "Net/SFNetServerUDP.h"
#include "Net/SFNetServerTCP.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Login/LoginServiceEntity.h"
#include "ServiceEntity/Login/LoginPlayerEntity.h"
#include "ServiceEntity/Login/LoginPlayerEntityTrans.h"
#include "ServiceEntity/RankingServiceTrans.h"
#include "Server/BrServer.h"

#include "SvrTrace.h"
#include "SvrConst.h"
#include "ServerConfig/SFServerConfig.h"
#include "Service/ServerService.h"

#include "Protocol/LoginServerMsgClass.h"

#include "DB/AccountDB.h"
#include "DB/LoginSessionDB.h"



namespace SF {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//


	LoginServiceEntity::LoginServiceEntity(const ServerConfig::NetPublic* publicNetSocket, const EndpointAddress& endpoint)
		: super(nullptr, ClusterID::Login, endpoint)
		, m_PublicNetSocket(publicNetSocket)
		, m_pNetPublic(nullptr)
		, m_NewConnectionQueue(GetHeap())
	{
		AssertRel(publicNetSocket != nullptr);
	}

	LoginServiceEntity::~LoginServiceEntity()
	{
	}

	Result LoginServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		ScopeContext hr;
		auto pServerInst = Svr::BrServer::GetInstance();

		svrCheckPtr(pServerInst);

		svrCheck(super::InitializeEntity(newEntityID));

		LoginPlayerEntity::GetAuthTicketGenerator().SetServerID(Service::ServerConfig->UID);


		// public network
		svrCheckPtr(m_PublicNetSocket);

		auto serverID = Service::ServerConfig->UID;
		if (m_PublicNetSocket->Protocol == "TCP")
		{
			svrCheckMem(m_pNetPublic = NewObject<Net::ServerTCP>(GetHeap(), serverID, NetClass::Login));
		}
		else if (m_PublicNetSocket->Protocol == "UDP")
		{
			svrCheckMem(m_pNetPublic = NewObject<Net::ServerUDP>(GetHeap(), serverID, NetClass::Login));
		}
		else // (m_PublicNetSocket->Protocol == "MRUDP")
		{
			svrCheckMem(m_pNetPublic = NewObject<Net::ServerMUDP>(GetHeap(), serverID, NetClass::Login));
		}

		m_pNetPublic->SetNewConnectionhandler([this](SharedPointerT<Net::Connection> conn)
			{
				SharedPointerAtomicT<Net::Connection> pConTem;
				pConTem = std::forward<SharedPointerT<Net::Connection>>(conn);
				svrTrace(Debug3, "LoginServiceEntity: Received new connection");
				m_NewConnectionQueue.Enqueue(pConTem);
			});

		svrTrace(Info, "Starting Login network service");
		svrCheck(m_pNetPublic->HostOpen(NetClass::Login, m_PublicNetSocket->ListenIP, m_PublicNetSocket->Port));

		// Account DB
		svrCheck(pServerInst->AddDBCluster<DB::AccountDB>(Service::ServerConfig->FindDBCluster("AccountDB")));

		// Session DB initialize
		svrCheck(pServerInst->AddDBCluster<DB::LoginSessionDB>(Service::ServerConfig->FindDBCluster("LoginSessionDB")));

		// Service directory need to get those information for those services
		svrCheck(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::Game));
		svrCheck(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::Ranking));

		m_pNetPublic->SetIsEnableAccept(true);

		return hr;
	}

	// clear transaction
	Result LoginServiceEntity::ClearEntity()
	{
		ScopeContext hr = ResultCode::SUCCESS;

		svrCheck(super::ClearEntity());

		if (m_pNetPublic != nullptr)
			m_pNetPublic->HostClose();
		m_pNetPublic = nullptr;

		return hr;
	}

	Result LoginServiceEntity::TickUpdate(TimerAction* pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrCheck(super::TickUpdate(pAction));

		// check below only if we are working
		if (GetEntityState() != EntityState::WORKING)
			return hr;

		if (Svr::BrServer::GetInstance()->GetServerState() != ServerState::RUNNING)
			return hr;

		svrCheck(ProcessNewConnection());

		return hr;
	}



	// Process network event
	Result LoginServiceEntity::ProcessNewConnection()
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<LoginPlayerEntity> pLoginPlayerEntity;
		SharedPointerT<Net::Connection> pConn;

		if (m_pNetPublic == nullptr)
			return ResultCode::SUCCESS;


		auto numQueued = m_NewConnectionQueue.GetEnqueCount();
		for (uint iQueue = 0; iQueue < numQueued; iQueue++)
		{
			SharedPointerAtomicT<Net::Connection> pConnAtomic;

			if (!m_NewConnectionQueue.Dequeue(pConnAtomic))
				break;

			auto connectionState = pConnAtomic->GetConnectionState();
			switch (connectionState)
			{
			case Net::ConnectionState::CONNECTING:
				// We need to wait more
				m_NewConnectionQueue.Enqueue(std::forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic));
				break;
			case Net::ConnectionState::CONNECTED:
				break;
			default:
				svrTrace(Debug3, "LoginServiceEntity: Dropping disconnected connection");
				pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);
				Service::ConnectionManager->RemoveConnection(pConn);
				pConn->DisconnectNRelease("Disconnected? before handed over to LoginService");
				pConn = nullptr;
				break;
			}

			if (pConnAtomic == nullptr)
				continue;

			svrTrace(Debug3, "LoginServiceEntity: Handling connected connection");

			pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);

			svrCheckMem(pLoginPlayerEntity = new(GetHeap()) LoginPlayerEntity);

			// Assign connection before add entity. 
			// If entity tick kicked in before connection assignment, the entity considered as garbage and closed
			if (!pLoginPlayerEntity->SetConnection(std::forward<Net::ConnectionPtr>(pConn)))
			{
				// NOTE: We need to mark to close this
				pLoginPlayerEntity->ClearEntity();
			}
			else
			{
				pConn = nullptr;
			}

			svrCheck(Service::EntityManager->AddEntity(EntityFaculty::User, *pLoginPlayerEntity));

			pLoginPlayerEntity = nullptr;
		}


		return hr;
	}



} // namespace SF {



