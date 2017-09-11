////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Util/TimeUtil.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"
#include "ServerEntity/ServerEntityManager.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServiceEntity/Game/GameClusterServiceEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"


#include "Table/TableSystem.h"

#include "ServerEntity/SvrEntityServerEntity.h"
#include "ServerEntity/GenericServerEntity.h"

#include "Protocol/Policy/EntityServerNetPolicy.h"

#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"


#include "Net/NetServerPeerTCP.h"
#include "Net/NetSvrDef.h"
#include "Net/NetServerUDP.h"
#include "GameInstance/GameEntityManager.h"


#include "GameSvrConst.h"
#include "GameServerClass.h"
#include "Transaction/GameServerTrans.h"
#include "GameInstance/GameClusterServiceEntity.h"

#include "DB/GameConspiracyDB.h"
#include "DB/GameTransactionDB.h"
#include "DB/AccountDB.h"
#include "DB/RankingDB.h"
#include "DB/LoginSessionDB.h"






namespace SF {
namespace GameServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameServer::GameServer()
		: BrServer(NetClass::Game)
		, m_pNetPublic(nullptr)
		, m_pGameClusterCfg(nullptr)
		, m_TableVersion(0)
		, m_PresetGameConfigID(1)
		, m_PresetGameConfig(nullptr)
	{
	}


	GameServer::~GameServer()
	{
	}

	

	Svr::ServerEntity* GameServer::CreateLoopbackEntity()
	{
		return new(GetHeap()) Svr::GameServerEntity;
	}
	
	// Update game config
	Result GameServer::UpdateGameConfig(uint configPresetID)
	{
		Result hr = ResultCode::SUCCESS;

		GameConfigType *pGameConfig = nullptr;
		svrChk( ::conspiracy::GameConfigTbl::FindItem(configPresetID, pGameConfig ) );

		// set value only if it succeeded
		m_TableVersion = GameTable::GetTableVersion();
		m_PresetGameConfigID = configPresetID;
		m_PresetGameConfig = pGameConfig;

	Proc_End:

		return hr;
	}


	// Apply configuration
	Result GameServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

		m_pGameClusterCfg = GetMyConfig()->pGameCluster;
		svrChkPtr(m_pGameClusterCfg);

		svrChk(Svr::BrServer::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result GameServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk(GameTable::InitializeTable(Service::ServerConfig) );

		svrChk( UpdateGameConfig(m_PresetGameConfigID) );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result GameServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	Result GameServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::GameClusterServiceEntity *pGameService = nullptr;
		uint componentID = 0;
		SockFamily privateNetSockFamily;


		Engine::GetInstance()->AddComponent<SF::GameServer::GameEntityManager>(GetMyConfig()->EntityControlCount);

		svrChk(Svr::BrServer::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);


		// Register entity servers
		// All server should use same sock family(IPV4 or IPV6)
		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;

		// Register entity servers
		// All server should use same sock family(IPV4 or IPV6)
		for (auto& itServer : Service::ServerConfig->GetServers())
		{
			if (!itServer->Name.StartWith("BREntityServer"))
				continue;

			Svr::EntityServerEntity *pEntity = nullptr;
			NetAddress netAddress(privateNetSockFamily, itServer->PrivateNet.IP, itServer->PrivateNet.Port);

			svrChk(GetComponentCarrier().GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<Svr::EntityServerEntity>(itServer->UID, NetClass::Entity, netAddress, pEntity));
		}

		{
			// Register game conspiracy cluster as a slave
			auto pMySvr = (const ServerConfig::GameServer*)GetMyConfig();
			svrMem(pGameService = new(GetHeap()) GameClusterServiceEntity(GetGameClusterInfo()->GameClusterID, &pMySvr->PublicNet, ClusterMembership::Slave));
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, pGameService));
			svrChk(GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pGameService));
			GetComponentCarrier().AddComponent(pGameService);
		}


		// Account DB
		svrChk(AddDBCluster<DB::AccountDB>(Service::ServerConfig->FindDBCluster("AccountDB")));

		svrChk(AddDBCluster<DB::LoginSessionDB>(Service::ServerConfig->FindDBCluster("LoginSessionDB")));

		// Game DB initialize
		svrChkPtr(GetGameClusterInfo());
		svrChk(AddDBCluster<DB::GameConspiracyDB>(GetGameClusterInfo()->FindDBCluster("GameDB")));

		svrChk(AddDBCluster<DB::GameTransactionDB>(GetGameClusterInfo()->FindDBCluster("GameTransactionLogDB")));

		// Ranking DB 
		svrChk(AddDBCluster<DB::RankingDB>(Service::ServerConfig->FindDBCluster("RankingDB")) );

		// push Startup transaction
		{
			TransactionPtr pProcess;
			svrMem( pProcess = new(GetHeap()) GameServerStartProcess(GetHeap()) );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(ThisThread::GetThreadID(),pProcess) );
		}


		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		// Queue items
		componentID = Svr::ServerComponentID_MatchingQueueWatcherService_4x1;
		for (ClusterID matchingQueueClusterID = ClusterID::MatchingQueue_Game_4x1; matchingQueueClusterID <= ClusterID::MatchingQueue_Max; matchingQueueClusterID++, componentID++)
		{
			//Svr::MatchingQueueServiceEntity *pQueueEntity = nullptr;
			Svr::MatchingQueueWatcherServiceEntity *pQueueWatcherEntity = nullptr;

			// Local watchers
			svrMem( pQueueWatcherEntity = new(GetHeap()) Svr::MatchingQueueWatcherServiceEntity(matchingQueueClusterID, componentID) );
			svrChk( Service::EntityManager->AddEntity( EntityFaculty::Service, pQueueWatcherEntity ) );
			svrChk(GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pQueueWatcherEntity ) );
			svrChk(GetComponentCarrier().AddComponent(pQueueWatcherEntity) );
		}


		// Adding matching entities
		componentID = Svr::ServerComponentID_MatchingWatcherService_4;
		for( ClusterID matchingQueueClusterID = ClusterID::Matching_Game_4; matchingQueueClusterID <= ClusterID::Matching_Game_8; matchingQueueClusterID++, componentID++ )
		{
			//Svr::MatchingServiceEntity *pMatcherEntity = nullptr;
			Svr::MatchingWatcherServiceEntity *pWatcherEntity = nullptr;

			// Local watchers
			svrMem( pWatcherEntity = new(GetHeap()) Svr::MatchingWatcherServiceEntity(matchingQueueClusterID, componentID) );
			svrChk(Service::EntityManager->AddEntity( EntityFaculty::Service, pWatcherEntity ) );
			svrChk(GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pWatcherEntity ) );
			svrChk(GetComponentCarrier().AddComponent(pWatcherEntity) );
		}



		{
			Svr::GameInstanceManagerWatcherServiceEntity *pGameInstanceManagerWatcher = nullptr;
			// Local slave entity manager service
			svrMem( pGameInstanceManagerWatcher = new(GetHeap()) Svr::GameInstanceManagerWatcherServiceEntity(GetGameClusterInfo()->GameClusterID) );
			svrChk( Service::EntityManager->AddEntity( EntityFaculty::Service, pGameInstanceManagerWatcher ) );
			svrChk( GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pGameInstanceManagerWatcher ) );
			GetComponentCarrier().AddComponent(pGameInstanceManagerWatcher);
		}

		{
			Svr::GamePartyManagerWatcherServiceEntity *pGamePartyManagerWatcher = nullptr;
			// Local slave entity manager service
			svrMem( pGamePartyManagerWatcher = new(GetHeap()) Svr::GamePartyManagerWatcherServiceEntity() );
			svrChk( Service::EntityManager->AddEntity( EntityFaculty::Service, pGamePartyManagerWatcher ) );
			svrChk( GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pGamePartyManagerWatcher ) );
			GetComponentCarrier().AddComponent(pGamePartyManagerWatcher);
		}



	Proc_End:

		return hr;
	}


	// Close Private Network
	Result GameServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseNetPrivate();

		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}


	// Initialize private Network
	Result GameServer::InitializeNetPublic()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( CloseNetPublic() );

		svrChkPtr(GetGameConfig());

		svrMem(m_pNetPublic = new(GetHeap()) Net::ServerMUDP(GetMyConfig()->UID, GetNetClass()));
		m_pNetPublic->RegisterToEngineObjectManager();
		svrChk( m_pNetPublic->HostOpen( GetNetClass(), GetGameConfig()->PublicNet.ListenIP, GetGameConfig()->PublicNet.Port ) );

		//// Game server only accept public connection with valid peerID(AuthTicket)
		//m_pNetPublic->SetUseAddressMap(false);

		//m_PublicNetAddressIPv4 = NetAddress(SockFamily::IPV4, GetGameConfig()->NetPublic->IPV4.c_str(), GetGameConfig()->NetPublic->Port);
		//svrChk(Net::CheckLocalAddress(SockFamily::IPV4, m_PublicNetAddressIPv4));

	Proc_End:

		return hr;
	}


	// Close Public Network
	Result GameServer::CloseNetPublic()
	{
		Result hr = ResultCode::SUCCESS;

		if( m_pNetPublic == nullptr )
			return ResultCode::SUCCESS;

		svrChk( m_pNetPublic->HostClose() );

		m_pNetPublic = nullptr;

	Proc_End:

		return hr;
	}

	// Run the task
	Result GameServer::TickUpdate(TimerAction *pAction)
	{
		if (m_TableVersion != GameTable::GetTableVersion())
		{
			UpdateGameConfig(m_PresetGameConfigID);
		}

		return Svr::BrServer::TickUpdate(pAction);
	}


	// create remote entity by class
	Result GameServer::CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
		case NetClass::Entity:
			pServerEntity = new(GetHeap()) Svr::EntityServerEntity();
			break;
		default:
			pServerEntity = new(GetHeap()) Svr::GenericServerEntity();
			break;
		};

		if( pServerEntity == nullptr )
			return ResultCode::OUT_OF_MEMORY;

		return ResultCode::SUCCESS;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//



	


}; // namespace GameServer
}; // namespace SF







