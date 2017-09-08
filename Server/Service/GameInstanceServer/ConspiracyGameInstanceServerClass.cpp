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
#include "ServerEntity/GenericServerEntity.h"
#include "GameInstance/GameInstanceManager.h"


#include "Table/TableSystem.h"

#include "ServerEntity/SvrEntityServerEntity.h"


#include "Protocol/Policy/EntityServerNetPolicy.h"

#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Net/NetSvrDef.h"
#include "Entity/EntityManager.h"
#include "GameInstance/GameInstanceEntity.h"


#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"

#include "Transaction/GameInstanceServerTrans.h"
#include "GameInstance/GameEntityManager.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"


#define PATH_DATABASE L"../../Data"



namespace SF {
namespace ConspiracyGameInstanceServer {

	

	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameInstanceServer::GameInstanceServer()
		:BrServer(NetClass::Game)
		,m_pGameClusterCfg(nullptr)
	{
	}


	GameInstanceServer::~GameInstanceServer()
	{
	}


	Svr::EntityManager* GameInstanceServer::CreateEntityManager()
	{
		return new(GetMemoryManager()) GameEntityManager;
	}

	Svr::ServerEntity* GameInstanceServer::CreateLoopbackEntity()
	{
		return new(GetMemoryManager()) Svr::GenericServerEntity;
	}


	// Apply configuration
	Result GameInstanceServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr(GetMyConfig());

		m_pGameClusterCfg = GetMyConfig()->pGameCluster;
		svrChkPtr( m_pGameClusterCfg );

		svrChk(Svr::BrServer::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result GameInstanceServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk(GameTable::InitializeTable(Service::ServerConfig) );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result GameInstanceServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	Result GameInstanceServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;
		SockFamily privateNetSockFamily;

		svrChk(Svr::BrServer::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);


		// Register entity servers
		// All server should use same sock family(IPV4 or IPV6)
		privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;
		for( auto itEntity = Service::ServerConfig->EntityServers.begin(); itEntity != Service::ServerConfig->EntityServers.end(); ++itEntity )
		{
			Svr::EntityServerEntity *pEntity = nullptr;
			auto pEntityCfg = *itEntity;

			NetAddress netAddress(privateNetSockFamily, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port);

			svrChk(GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<Svr::EntityServerEntity>(pEntityCfg->UID, NetClass::Entity, netAddress, pEntity));
		}



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		{
		GameInstanceManagerServiceEntity *pGameInstanceManager = nullptr;
		svrMem( pGameInstanceManager = new(GetMemoryManager()) GameInstanceManagerServiceEntity(ClusterID::GameInstanceManager, ClusterMembership::Slave) );
		svrChk( Svr::GetServerComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pGameInstanceManager ) );
		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pGameInstanceManager ) );
		svrChk( AddComponent(pGameInstanceManager) );
		}


		// push Startup transaction
		{
			Svr::Transaction * pProcess = nullptr;
			svrMem( pProcess = new(GetMemoryManager()) GameInstanceServerStartProcess );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk(PendingTransaction(ThisThread::GetThreadID(), pProcess));
		}



	Proc_End:

		return hr;
	}


	// Close Private Network
	Result GameInstanceServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}



	// create remote entity by class
	Result GameInstanceServer::CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
		case NetClass::Entity:
			pServerEntity = new(GetMemoryManager()) Svr::EntityServerEntity();
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
	//	virtual Entity interface
	//




}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







