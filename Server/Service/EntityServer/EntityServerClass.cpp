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
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "ServerConfig/SFServerConfig.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
//#include "ServiceEntity/EntityManagerServiceEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "Util/TimeUtil.h"

#include "EntitySvrConst.h"
#include "EntityServerClass.h"
#include "Transaction/EntityServerTrans.h"

#include "ServerEntity/GenericServerEntity.h"
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
		: BrServer(NetClass::Entity)
	{
	}


	EntityServer::~EntityServer()
	{
	}
	
	
	Svr::ServerEntity* EntityServer::CreateLoopbackEntity()
	{
		return new(GetMemoryManager()) Svr::EntityServerEntity;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Public Network event handling
	//

	// Apply configuration
	Result EntityServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

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

		Engine::GetInstance()->AddComponent<Svr::EntityManager>(GetMyConfig()->EntityControlCount);

		svrChk(Svr::BrServer::InitializeNetPrivate() );

	
		// Register entity servers
		// All server should use same sock family(IPV4 or IPV6)
		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;

		for( auto& itServer : Service::ServerConfig->GetServers() )
		{
			if (!itServer->Name.StartWith("BREntityServer"))
				continue;

			EntityServerEntity *pEntity = nullptr;
			if(itServer->Name != Util::GetServiceName() ) // 
			{
				NetAddress netAddress(privateNetSockFamily, itServer->PrivateNet.IP, itServer->PrivateNet.Port);

				svrChk( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<EntityServerEntity>(itServer->UID, NetClass::Entity, netAddress, pEntity ) );
			}
		}


		// We are going to enable this when the entity servers are connected
		GetNetPrivate()->SetIsEnableAccept(false);



		// Add my entity manager entity
		svrChk(Svr::GetServerComponent<Svr::ServerEntityManager>()->UpdateEntityManagerServerEntity( GetLoopbackServerEntity() ) );


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
			TransactionPtr pProcess;
			svrMem( pProcess = new(GetHeap()) EntityServerStartProcess(GetHeap()) );
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
	Result EntityServer::CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
		//case NetClass::Game:
		//	pServerEntity = new(GetMemoryManager()) GameServerEntity();
		//	break;
		//case NetClass::Login:
		//	pServerEntity = new(GetMemoryManager()) LoginServerEntity();
		//	break;
		case NetClass::Entity:
			pServerEntity = new(GetMemoryManager()) EntityServerEntity();
			break;
		default:
			pServerEntity = new(GetMemoryManager()) Svr::GenericServerEntity();
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







