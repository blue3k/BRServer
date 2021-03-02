////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Game service entity implementation
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
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "ServiceEntity/Game/PlayerDirectoryManager.h"
#include "ServiceEntity/Game/GameServiceTrans.h"
#include "Entity/EntityManager.h"
#include "Server/BrServer.h"
#include "DB/LoginSessionDB.h"





namespace SF {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameServiceEntity::GameServiceEntity(GameID gameID, const ServerConfig::NetPublic *publicNetSocket, const EndpointAddress& endpoint)
		: super(gameID, ClusterID::Game, endpoint)
		, m_PublicNetConfig(publicNetSocket)
		, m_NewConnectionQueue(GetHeap())
	{
		AssertRel(publicNetSocket != nullptr);
	}

	GameServiceEntity::~GameServiceEntity()
	{
	}

	Result GameServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		ServerServiceInformation* pServiceInfo = nullptr;
		auto pServerInst = Svr::BrServer::GetInstance();

		svrCheckPtr(pServerInst);
		svrCheckPtr(m_PublicNetConfig);

		GetCustomAttributes().SetValue("PublicIPV4", m_PublicNetConfig->IPV4);
		GetCustomAttributes().SetValue("PublicIPV6", m_PublicNetConfig->IPV6);
		GetCustomAttributes().SetValue("PublicPort", m_PublicNetConfig->Port);

		svrCheck(super::InitializeEntity(newEntityID) );


		auto serverID = Service::ServerConfig->UID;
		if (m_PublicNetConfig->Protocol == "TCP")
		{
			svrCheckMem(m_pNetPublic = NewObject<Net::ServerTCP>(GetHeap(), serverID, NetClass::Game));
		}
		else if (m_PublicNetConfig->Protocol == "UDP")
		{
			svrCheckMem(m_pNetPublic = NewObject<Net::ServerUDP>(GetHeap(), serverID, NetClass::Game));
		}
		else // (m_PublicNetConfig->Protocol == "MRUDP")
		{
			svrCheckMem(m_pNetPublic = NewObject<Net::ServerMUDP>(GetHeap(), serverID, NetClass::Game));
		}

		m_pNetPublic->SetNewConnectionhandler([this](SharedPointerT<Net::Connection> conn)
		{
			SharedPointerAtomicT<Net::Connection> pConTem;
			pConTem = std::forward<SharedPointerT<Net::Connection>>(conn);
			m_NewConnectionQueue.Enqueue(pConTem);
		});

		svrCheck(m_pNetPublic->HostOpen(NetClass::Game, m_PublicNetConfig->ListenIP, m_PublicNetConfig->Port));

		svrCheck(pServerInst->AddDBCluster<DB::LoginSessionDB>(Service::ServerConfig->FindDBCluster("LoginSessionDB")));

		svrCheck(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::GameInstanceManager));
		svrCheck(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::GameInstance));

		// Register tables
		svrCheck(Service::DataTableManager->LoadDataTable("ZoneTable", "ZoneId", "gtbl_ZoneTable"));

		// FIXME: matching queue
		//svrCheck(Service::ServiceDirectory->WatchForService(GetGameID(), ClusterID::MatchingQueue_Game_4x1, ClusterID::MatchingQueue_Game_4x1W));
		//svrCheck(Service::ServiceDirectory->WatchForService(GetGameID(), ClusterID::MatchingQueue_Game_8x1, ClusterID::MatchingQueue_Game_8x1W));

		m_pNetPublic->SetIsEnableAccept(true);

		return hr;
	}

	// clear transaction
	Result GameServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrCheck(super::ClearEntity() );

		if (m_pNetPublic != nullptr)
		{
			svrCheck(m_pNetPublic->HostClose());
			m_pNetPublic = nullptr;
		}

		return hr;
	}

	Result GameServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrCheck(super::TickUpdate(pAction) );

		// check below only if we are working
		if (GetEntityState() != EntityState::WORKING)
			return hr;

		if( Svr::BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			return hr;

		svrCheck(ProcessNewConnection());

		return hr;
	}




	// Process network event
	Result GameServiceEntity::ProcessNewConnection()
	{
		Result hr = ResultCode::SUCCESS;
		Entity* pEntity = nullptr;
		UniquePtr<Svr::GamePlayerEntity> pGamePlayerEntity;
		SharedPointerT<Net::Connection> pConn;

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
				m_NewConnectionQueue.Enqueue(std::forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic));
				break;
			case Net::ConnectionState::CONNECTED:
				break;
			default:
				assert(connectionState == Net::ConnectionState::DISCONNECTED); // I want to see when this happens
				pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);
				Service::ConnectionManager->RemoveConnection(pConn);
				pConn->DisconnectNRelease("Disconnected before handed over to GameService");
				pConn = nullptr;
				break;
			}

			if (pConnAtomic == nullptr)
				continue;

			pConn = Forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic);

			svrCheckPtr(pConn);

			svrCheck(Service::EntityManager->CreateEntity(GetClusterID(), EntityFaculty::User, pEntity));
			pGamePlayerEntity.reset(dynamic_cast<Svr::GamePlayerEntity*>(pEntity));
			svrCheckPtr(pGamePlayerEntity);

			svrCheck(Service::EntityManager->AddEntity(EntityFaculty::User, pGamePlayerEntity.get()));

			// Assign connection after AddEntity, because InitializeEntity will clean up connection
			if (!pGamePlayerEntity->SetConnection(Forward<Net::ConnectionPtr>(pConn)))
			{
				// NOTE: We need to mark to close this
				pGamePlayerEntity->ClearEntity();
			}

			pGamePlayerEntity.release();
		}


		return ResultCode::SUCCESS;
	}



} // namespace SF {

