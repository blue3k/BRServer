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
#include "Util/TimeUtil.h"

#include "EntitySvrConst.h"
#include "EntityServerClass.h"
#include "Transaction/EntityServerTrans.h"

#include "ServerSystem/ServerEntity/GenericServerEntity.h"
#include "ServerEntity/EntityServerEntity.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Transaction/EntityTransactionServer.h"
#include "Transaction/EntityTransactionUser.h"


#define PATH_DATABASE "../../../Data"


namespace SF {
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
	Result EntityServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;
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
	Result EntityServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0)) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result EntityServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	Result EntityServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ClusterManagerServiceEntity *pClusterManager = nullptr;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;
		SockFamily privateNetSockFamily;


		svrChk(Svr::BrServer::InitializeNetPrivate() );

	
		// Register entity servers
		// All server should use same sock family(IPV4 or IPV6)
		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;
		for( auto itEntity = Svr::Config::GetConfig().EntityServers.begin(); itEntity != Svr::Config::GetConfig().EntityServers.end(); ++itEntity )
		{
			EntityServerEntity *pEntity = nullptr;
			auto pEntityCfg = *itEntity;
			if( pEntityCfg->Name != Util::GetServiceNameA() ) // 
			{
				NetAddress netAddress(privateNetSockFamily, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port);

				svrChk( GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<EntityServerEntity>( pEntityCfg->UID, NetClass::Entity, netAddress, pEntity ) );
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
			if( !(pClusterManager->GetClusterServiceEntity( clusterID, pServiceEntity )) )
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
	Result EntityServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseNetPrivate();


		return hr;
	}





	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//




	// create remote entity by class
	Result EntityServer::CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
		//case BR::NetClass::Game:
		//	pServerEntity = new GameServerEntity();
		//	break;
		//case BR::NetClass::Login:
		//	pServerEntity = new LoginServerEntity();
		//	break;
		case BR::NetClass::Entity:
			pServerEntity = new EntityServerEntity();
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
	//	Private Network event handling
	//



	//////////////////////////////////////////////////////////////////////////
	//
	//	Private connection handler
	//




}; // namespace EntityServer
}; // namespace SF







