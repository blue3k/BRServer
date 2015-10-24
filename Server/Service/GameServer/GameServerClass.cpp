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
#include "Common/TimeUtil.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/LoginClusterServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"

#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"


#include "Table/TableSystem.h"

#include "ServerEntity/EntityServerEntity.h"
#include "ServerEntity/GameServerEntity.h"
#include "ServerEntity/LoginServerEntity.h"

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
		:BrServer(BR::NetClass::Game)
		,m_pNetPublic(nullptr)
		,m_pGameClusterCfg(nullptr)
		,m_PresetGameConfigID(1)
		,m_PresetGameConfig(nullptr)
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
		return new GameServerEntity;
	}
	
	// Update game config
	HRESULT GameServer::UpdateGameConfig(UINT configPresetID)
	{
		HRESULT hr = S_OK;

		GameConfigType *pGameConfig = nullptr;
		svrChk( ::conspiracy::GameConfigTbl::FindItem(configPresetID, pGameConfig ) );

		// set value only if it succeeded
		m_PresetGameConfigID = configPresetID;
		m_PresetGameConfig = pGameConfig;

	Proc_End:

		return hr;
	}


	// Apply configuration
	HRESULT GameServer::ApplyConfiguration()
	{
		HRESULT hr = S_OK;

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

		svrChk(__super::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	HRESULT GameServer::InitializeServerResource()
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeServerResource() );

		svrChk(GameTable::InitializeTable() );

		svrChk( UpdateGameConfig(m_PresetGameConfigID) );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	HRESULT GameServer::CloseServerResource()
	{
		HRESULT hr = S_OK;

		svrChk( __super::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	HRESULT GameServer::InitializeNetPrivate()
	{
		HRESULT hr = S_OK;
		Svr::LoginClusterServiceEntity *pLoginService = nullptr;
		Svr::GameClusterServiceEntity *pGameService = nullptr;
		UINT componentID = 0;

		svrChk( __super::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);


		// Register entity servers
		for( auto itEntity = Svr::Config::GetConfig().EntityServers.begin(); itEntity != Svr::Config::GetConfig().EntityServers.end(); ++itEntity )
		{
			EntityServerEntity *pEntity = nullptr;
			auto pEntityCfg = *itEntity;

			svrChk(GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<EntityServerEntity>(pEntityCfg->UID, NetClass::Entity, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port, pEntity));
		}

		// Register login cluster
		svrMem( pLoginService = new Svr::LoginClusterServiceEntity(ClusterMembership::StatusWatcher) );
		svrChk( GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pLoginService ) );
		svrChk( GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pLoginService ) );
		AddComponent(pLoginService);


		// Register game conspiracy cluster as a slave
		svrMem( pGameService = new Svr::GameClusterServiceEntity(GameID::Conspiracy, ClusterMembership::Slave) );
		svrChk( GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pGameService ) );
		svrChk( GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pGameService ) );
		AddComponent(pGameService);


		// Account DB
		svrChk(InitializeDBCluster<DB::AccountDB>(Svr::Config::GetConfig().AccountDB));

		svrChk(InitializeDBCluster<DB::LoginSessionDB>(Svr::Config::GetConfig().LoginSessionDB));

		// Game DB initialize
		svrChkPtr(Svr::Config::GetConfig().GameCluster);
		svrChk(InitializeDBCluster<DB::GameConspiracyDB>(Svr::Config::GetConfig().GameCluster->GameDB));

		svrChk(InitializeDBCluster<DB::GameTransactionDB>(Svr::Config::GetConfig().GameCluster->GameTransactionLogDB));

		// Ranking DB 
		svrChk( InitializeDBCluster<DB::RankingDB>(Svr::Config::GetConfig().RankingDB) );

		// push Startup transaction
		{
			Svr::Transaction * pProcess = nullptr;
			svrMem( pProcess = new GameServerStartProcess );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(GetCurrentThreadId(),pProcess) );
		}



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		// Queue items
		componentID = Svr::ServerComponentID_MatchingQueueWatcherService_4x1;
		for (ClusterID matchingQueueClusterID = ClusterID::MatchingQueue_Game_4x1; matchingQueueClusterID <= ClusterID::MatchingQueue_Max; matchingQueueClusterID++, componentID++)
		{
			Svr::MatchingQueueServiceEntity *pQueueEntity = nullptr;
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
			Svr::MatchingServiceEntity *pMatcherEntity = nullptr;
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
		svrMem( pGameInstanceManagerWatcher = new Svr::GameInstanceManagerWatcherServiceEntity(GameID::Conspiracy) );
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
	HRESULT GameServer::CloseNetPrivate()
	{
		HRESULT hr = S_OK;

		hr = __super::CloseNetPrivate();

		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}


	// Initialize private Network
	HRESULT GameServer::InitializeNetPublic()
	{
		HRESULT hr = S_OK;

		svrChk( CloseNetPublic() );

		svrChkPtr( GetGameConfig() );

		svrMem(m_pNetPublic = new BR::Net::ServerMUDP(GetMyConfig()->UID, GetNetClass()));

		svrChk( m_pNetPublic->HostOpen( GetNetClass(), GetGameConfig()->NetPublic->IP.c_str(), GetGameConfig()->NetPublic->Port ) );

		// Game server only accept public connection with valid peerID(AuthTicket)
		m_pNetPublic->GetConnectionManager().SetUseAddressMap(false);

		m_PublicNetAddressIPv4 = m_pNetPublic->GetLocalAddress();
		Net::GetLocalAddressIPv4(m_PublicNetAddressIPv4);

	Proc_End:

		return hr;
	}


	// Close Public Network
	HRESULT GameServer::CloseNetPublic()
	{
		HRESULT hr = S_OK;

		if( m_pNetPublic == nullptr )
			return S_OK;

		svrChk( m_pNetPublic->CloseAllConnection() );

		svrChk( m_pNetPublic->HostClose() );

		delete m_pNetPublic;

	Proc_End:

		return hr;
	}


	// create remote entity by class
	HRESULT GameServer::CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
		case BR::NetClass::Login:
			pServerEntity = new LoginServerEntity();
			break;
		case BR::NetClass::Game:
			pServerEntity = new GameServerEntity();
			break;
		case BR::NetClass::Entity:
			pServerEntity = new EntityServerEntity();
			break;
		default:
			break;
		};

		if( pServerEntity == nullptr )
			return E_OUTOFMEMORY;

		return S_OK;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//



	


}; // namespace GameServer
}; // namespace BR







