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
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServerEntityManager.h"
//#include "ServerSystem/ServiceEntity/EntityManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "Common/TimeUtil.h"

#include "EntitySvrConst.h"
#include "EntityServerClass.h"
#include "Transaction/EntityServerTrans.h"

#include "ServerEntity/LoginServerEntity.h"
#include "ServerEntity/GameServerEntity.h"
#include "ServerEntity/EntityServerEntity.h"

#include "Protocol/Policy/GameServerIPolicy.h"
#include "Transaction/EntityTransactionServer.h"
#include "Transaction/EntityTransactionUser.h"


#define PATH_DATABASE "../../../Data"


namespace BR {
namespace EntityServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	EntityServer::EntityServer()
		:BrServer(BR::NetClass::Entity)
	{
	}


	EntityServer::~EntityServer()
	{
	}
	
	
	Svr::EntityManager* EntityServer::CreateEntityManager()
	{
		return new Svr::EntityManager;
	}

	Svr::ServerEntity* EntityServer::CreateLoopbackEntity()
	{
		return new Svr::EntityServerEntity;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Public Network event handling
	//

	// Apply configuration
	HRESULT EntityServer::ApplyConfiguration()
	{
		HRESULT hr = S_OK;
		const Svr::Config::GenericServer* pMySvr = nullptr;

		std::for_each( Svr::Config::GetConfig().EntityServers.begin(), Svr::Config::GetConfig().EntityServers.end(), 
			[&]( const Svr::Config::GenericServer* pServer )
		{
			if( pServer->Name == Util::GetServiceNameA() )
			{
				pMySvr = pServer;
			}
		});

		svrChkPtr( pMySvr );
		SetMyConfig( pMySvr );

		svrChk(Svr::BrServer::ApplyConfiguration() );

	Proc_End:

		return hr;
	}


	// Initialize server resource
	HRESULT EntityServer::InitializeServerResource()
	{
		HRESULT hr = S_OK;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0)) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	HRESULT EntityServer::CloseServerResource()
	{
		HRESULT hr = S_OK;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	HRESULT EntityServer::InitializeNetPrivate()
	{
		HRESULT hr = S_OK;
		//Svr::EntityManagerServiceEntity *pEntityManagerService = nullptr;
		Svr::ClusterManagerServiceEntity *pClusterManager = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;


		svrChk(Svr::BrServer::InitializeNetPrivate() );

	
		// Register entity servers
		for( auto itEntity = Svr::Config::GetConfig().EntityServers.begin(); itEntity != Svr::Config::GetConfig().EntityServers.end(); ++itEntity )
		{
			EntityServerEntity *pEntity = nullptr;
			auto pEntityCfg = *itEntity;
			if( pEntityCfg->Name != Util::GetServiceNameA() ) // 
			{
				svrChk( GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<EntityServerEntity>( pEntityCfg->UID, NetClass::Entity, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port, pEntity ) );
			}
		}


		// We are going to enable this when the entity servers are connected
		GetNetPrivate()->SetIsEnableAccept(false);



		// Add my entity manager entity
		svrChk( GetComponent<Svr::ServerEntityManager>()->UpdateEntityManagerServerEntity( GetLoopbackServerEntity() ) );


		// Prepare watcher clusters
		svrChkPtr( pClusterManager = Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>() );

		for( ClusterID clusterID = ClusterID::ClusterManager; clusterID < ClusterID::Max; clusterID++ )
		{
			if( FAILED(pClusterManager->GetClusterServiceEntity( clusterID, pServiceEntity )) )
			{
				// We need to add a table of cluster types
				svrChk(pClusterManager->CreateWatcherForCluster( clusterID, ClusterType::Replication, pServiceEntity ));
			}
		}


		// push Startup transaction
		{
			Svr::Transaction * pProcess = nullptr;
			svrMem( pProcess = new EntityServerStartProcess );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(ThisThread::GetThreadID(), pProcess) );
		}

	Proc_End:

		return hr;
	}

	// Close Private Network
	HRESULT EntityServer::CloseNetPrivate()
	{
		HRESULT hr = S_OK;

		hr = Svr::BrServer::CloseNetPrivate();


		return hr;
	}





	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//




	// create remote entity by class
	HRESULT EntityServer::CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
		case BR::NetClass::Game:
			pServerEntity = new GameServerEntity();
			break;
		case BR::NetClass::Login:
			pServerEntity = new LoginServerEntity();
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
	//	Private Network event handling
	//



	//////////////////////////////////////////////////////////////////////////
	//
	//	Private connection handler
	//




}; // namespace EntityServer
}; // namespace BR







