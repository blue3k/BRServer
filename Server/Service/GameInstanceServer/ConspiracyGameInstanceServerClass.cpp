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
#include "GameInstance/GameInstanceManager.h"


#include "Table/TableSystem.h"

#include "ServerEntity/EntityServerEntity.h"
#include "ServerEntity/GameServerEntity.h"
#include "ServerEntity/ConspiracyGameInstanceServerEntity.h"


#include "Protocol/Policy/EntityServerIPolicy.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "Net/NetServer.h"
#include "ServerSystem/EntityManager.h"
#include "GameInstance/GameInstanceEntity.h"


#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"
//#include "PlayerInfoManager.h"
#include "Transaction/GameInstanceServerTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"


#define PATH_DATABASE L"../../Data"



namespace BR {
namespace ConspiracyGameInstanceServer {

	
	//// Get Entity Server
	//EntityServerEntity* GetEntityServer()
	//{
	//	EntityServerEntity *pEntityServer = nullptr;
	//	Svr::ServerEntity *pServerEntity = nullptr;
	//	Svr::ServerEntityManager *pRemoteMgr = Svr::GetServerComponent<Svr::ServerEntityManager>();

	//	if( pRemoteMgr 
	//		&& SUCCEEDED(pRemoteMgr->GetServerEntity( BR::NetClass::Entity, pServerEntity ) ) )
	//	{
	//		pEntityServer = dynamic_cast<EntityServerEntity*>(pServerEntity);
	//	}

	//	return pEntityServer;
	//}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameInstanceServer::GameInstanceServer()
		:BrServer(BR::NetClass::Game)
		,m_pGameClusterCfg(nullptr)
	{
	}


	GameInstanceServer::~GameInstanceServer()
	{
	}


	Svr::ServerEntity* GameInstanceServer::CreateLoopbackEntity()
	{
		return new ConspiracyGameInstanceServerEntity;
	}


	// Apply configuration
	HRESULT GameInstanceServer::ApplyConfiguration()
	{
		HRESULT hr = S_OK;

		//const Svr::Config::GameCluster* pMyGame = nullptr;
		const Svr::Config::GenericServer* pMySvr = nullptr;
		svrChkPtr(Svr::Config::GetConfig().GameCluster);
		m_pGameClusterCfg = Svr::Config::GetConfig().GameCluster;

		std::for_each( Svr::Config::GetConfig().GameCluster->GameInstancess.begin(), Svr::Config::GetConfig().GameCluster->GameInstancess.end(), 
			[&]( const Svr::Config::GenericServer* pServer )
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
	HRESULT GameInstanceServer::InitializeServerResource()
	{
		HRESULT hr = S_OK;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk(GameTable::InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	HRESULT GameInstanceServer::CloseServerResource()
	{
		HRESULT hr = S_OK;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	HRESULT GameInstanceServer::InitializeNetPrivate()
	{
		HRESULT hr = S_OK;
		SockFamily privateNetSockFamily;

		svrChk(Svr::BrServer::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);


		// Register entity servers
		// All server should use same sock family(IPV4 or IPV6)
		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;
		for( auto itEntity = Svr::Config::GetConfig().EntityServers.begin(); itEntity != Svr::Config::GetConfig().EntityServers.end(); ++itEntity )
		{
			EntityServerEntity *pEntity = nullptr;
			auto pEntityCfg = *itEntity;

			NetAddress netAddress(privateNetSockFamily, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port);

			svrChk(GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<EntityServerEntity>(pEntityCfg->UID, NetClass::Entity, netAddress, pEntity));
		}



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		{
		GameInstanceManagerServiceEntity *pGameInstanceManager = nullptr;
		svrMem( pGameInstanceManager = new GameInstanceManagerServiceEntity(ClusterID::GameInstanceManager, ClusterMembership::Slave) );
		svrChk( GetMyServer()->GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pGameInstanceManager ) );
		svrChk( GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pGameInstanceManager ) );
		svrChk( AddComponent(pGameInstanceManager) );
		}


		// push Startup transaction
		{
			Svr::Transaction * pProcess = nullptr;
			svrMem( pProcess = new GameInstanceServerStartProcess );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk(PendingTransaction(ThisThread::GetThreadID(), pProcess));
		}



	Proc_End:

		return hr;
	}


	// Close Private Network
	HRESULT GameInstanceServer::CloseNetPrivate()
	{
		HRESULT hr = S_OK;

		hr = Svr::BrServer::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}



	// create remote entity by class
	HRESULT GameInstanceServer::CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
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




}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







