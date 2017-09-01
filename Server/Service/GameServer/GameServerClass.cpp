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
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"

#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"


#include "Table/TableSystem.h"

#include "ServerSystem/ServerEntity/EntityServerEntity.h"
#include "ServerSystem/ServerEntity/GenericServerEntity.h"

#include "Protocol/Policy/EntityServerIPolicy.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "Net/NetServerPeer.h"
#include "Net/NetServer.h"
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






namespace BR {
namespace GameServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameServer::GameServer()
		: BrServer(BR::NetClass::Game)
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

	
	Svr::EntityManager* GameServer::CreateEntityManager()
	{
		return new GameEntityManager;
	}
	
	Svr::ServerEntity* GameServer::CreateLoopbackEntity()
	{
		return new Svr::GameServerEntity;
	}
	
	// Update game config
	Result GameServer::UpdateGameConfig(UINT configPresetID)
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

		//const Svr::Config::GameCluster* pMyGame = nullptr;
		const Svr::Config::PublicServer* pMySvr = nullptr;
		svrChkPtr(Svr::Config::GetConfig().GameCluster);
		m_pGameClusterCfg = Svr::Config::GetConfig().GameCluster;

		std::for_each( Svr::Config::GetConfig().GameCluster->Games.begin(), Svr::Config::GetConfig().GameCluster->Games.end(), 
			[&]( const Svr::Config::PublicServer* pServer )
		{
			if( pServer->Name == Util::GetServiceNameA() )
			{
				pMySvr = pServer;
			}
		});

		svrChkPtr( pMySvr );
		svrChkPtr( m_pGameClusterCfg );

		SetMyConfig( pMySvr );

		svrChk(Svr::BrServer::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result GameServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk(GameTable::InitializeTable() );

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
		UINT componentID = 0;
		SockFamily privateNetSockFamily;

		svrChk(Svr::BrServer::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);


		// Register entity servers
		// All server should use same sock family(IPV4 or IPV6)
		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;
		for( auto itEntity = Svr::Config::GetConfig().EntityServers.begin(); itEntity != Svr::Config::GetConfig().EntityServers.end(); ++itEntity )
		{
			Svr::EntityServerEntity *pEntity = nullptr;
			auto pEntityCfg = *itEntity;

			NetAddress netAddress(privateNetSockFamily, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port);

			svrChk(GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<Svr::EntityServerEntity>(pEntityCfg->UID, NetClass::Entity, netAddress, pEntity));
		}

		{
			// Register game conspiracy cluster as a slave
			auto pMySvr = (const Svr::Config::PublicServer*)GetMyConfig();
			svrMem(pGameService = new GameClusterServiceEntity(GetGameClusterInfo()->GetGameID(), pMySvr->NetPublic, ClusterMembership::Slave));
			svrChk(GetComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::Service, pGameService));
			svrChk(GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pGameService));
			AddComponent(pGameService);
		}


		// Account DB
		svrChk(AddDBCluster<DB::AccountDB>(Svr::Config::GetConfig().AccountDB));

		svrChk(AddDBCluster<DB::LoginSessionDB>(Svr::Config::GetConfig().LoginSessionDB));

		// Game DB initialize
		svrChkPtr(Svr::Config::GetConfig().GameCluster);
		svrChk(AddDBCluster<DB::GameConspiracyDB>(Svr::Config::GetConfig().GameCluster->GameDB));

		svrChk(AddDBCluster<DB::GameTransactionDB>(Svr::Config::GetConfig().GameCluster->GameTransactionLogDB));

		// Ranking DB 
		svrChk(AddDBCluster<DB::RankingDB>(Svr::Config::GetConfig().RankingDB) );

		// push Startup transaction
		{
			Svr::Transaction * pProcess = nullptr;
			svrMem( pProcess = new GameServerStartProcess );
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
			svrMem( pQueueWatcherEntity = new Svr::MatchingQueueWatcherServiceEntity(matchingQueueClusterID, componentID) );
			svrChk( GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pQueueWatcherEntity ) );
			svrChk( GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pQueueWatcherEntity ) );
			svrChk( AddComponent(pQueueWatcherEntity) );
		}


		// Adding matching entities
		componentID = Svr::ServerComponentID_MatchingWatcherService_4;
		for( ClusterID matchingQueueClusterID = ClusterID::Matching_Game_4; matchingQueueClusterID <= ClusterID::Matching_Game_8; matchingQueueClusterID++, componentID++ )
		{
			//Svr::MatchingServiceEntity *pMatcherEntity = nullptr;
			Svr::MatchingWatcherServiceEntity *pWatcherEntity = nullptr;

			// Local watchers
			svrMem( pWatcherEntity = new Svr::MatchingWatcherServiceEntity(matchingQueueClusterID, componentID) );
			svrChk( GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pWatcherEntity ) );
			svrChk( GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pWatcherEntity ) );
			svrChk( AddComponent(pWatcherEntity) );
		}



		{
		Svr::GameInstanceManagerWatcherServiceEntity *pGameInstanceManagerWatcher = nullptr;
		// Local slave entity manager service
		svrMem( pGameInstanceManagerWatcher = new Svr::GameInstanceManagerWatcherServiceEntity(GetGameClusterInfo()->GetGameID()) );
		svrChk( GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pGameInstanceManagerWatcher ) );
		svrChk( GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pGameInstanceManagerWatcher ) );
		AddComponent(pGameInstanceManagerWatcher);
		}

		{
		Svr::GamePartyManagerWatcherServiceEntity *pGamePartyManagerWatcher = nullptr;
		// Local slave entity manager service
		svrMem( pGamePartyManagerWatcher = new Svr::GamePartyManagerWatcherServiceEntity() );
		svrChk( GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pGamePartyManagerWatcher ) );
		svrChk( GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pGamePartyManagerWatcher ) );
		AddComponent(pGamePartyManagerWatcher);
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
		svrChkPtr(GetGameConfig()->NetPublic);

		svrMem(m_pNetPublic = new BR::Net::ServerMUDP(GetMyConfig()->UID, GetNetClass()));

		svrChk( m_pNetPublic->HostOpen( GetNetClass(), GetGameConfig()->NetPublic->ListenIP.c_str(), GetGameConfig()->NetPublic->Port ) );

		// Game server only accept public connection with valid peerID(AuthTicket)
		m_pNetPublic->GetConnectionManager().SetUseAddressMap(false);

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

		svrChk( m_pNetPublic->CloseAllConnection() );

		svrChk( m_pNetPublic->HostClose() );

		delete m_pNetPublic;

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
	Result GameServer::CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
		case BR::NetClass::Entity:
			pServerEntity = new Svr::EntityServerEntity();
			break;
		default:
			pServerEntity = new Svr::GenericServerEntity();
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
}; // namespace BR







