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
//#include "ServerSystem/ServiceEntity/EntityManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/LoginClusterServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"

#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"



#include "Table/TableSystem.h"

#include "ServerEntity/EntityServerEntity.h"
#include "ServerEntity/GameServerEntity.h"


#include "Protocol/Policy/EntityServerIPolicy.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "Net/NetServerPeer.h"
#include "Net/NetServer.h"
#include "Net/NetServerUDP.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ExternalTask.h"

#include "SharedModuleSvrConst.h"
#include "SharedModuleServerClass.h"

#include "Transaction/SharedModuleServerTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"


#define PATH_DATABASE L"../../Data"



namespace BR {
namespace SharedModuleServer {

	


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	SharedModuleServer::SharedModuleServer()
		:BrServer(BR::NetClass::Game)
	{
	}


	SharedModuleServer::~SharedModuleServer()
	{
	}


	Svr::ServerEntity* SharedModuleServer::CreateLoopbackEntity()
	{
		return new Svr::ServerEntity;
	}


	// Apply configuration
	HRESULT SharedModuleServer::ApplyConfiguration()
	{
		HRESULT hr = S_OK;

		const Svr::Config::SharedModuleServer* pMySvr = nullptr;

		std::for_each( Svr::Config::GetConfig().SharedModuleServers.begin(), Svr::Config::GetConfig().SharedModuleServers.end(), 
			[&]( const Svr::Config::SharedModuleServer* pServer )
		{
			if( pServer->Name == Util::GetServiceNameA() )
			{
				pMySvr = pServer;
			}
		});

		svrChkPtr( pMySvr );

		SetMyConfig( pMySvr );

		svrChk(__super::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	HRESULT SharedModuleServer::InitializeServerResource()
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeServerResource() );

		//svrChk( conspiracy::InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	HRESULT SharedModuleServer::CloseServerResource()
	{
		HRESULT hr = S_OK;

		svrChk( __super::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk( conspiracy::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	HRESULT SharedModuleServer::InitializeNetPrivate()
	{
		HRESULT hr = S_OK;
		Svr::LoginClusterServiceEntity *pLoginService = nullptr;
		Svr::GameClusterServiceEntity *pGameService = nullptr;
		UINT componentID = 0;
		Svr::Config::SharedModuleServer *pServerConfig = nullptr;

		svrChk( __super::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);

		// Register entity servers
		for( auto itEntity = Svr::Config::GetConfig().EntityServers.begin(); itEntity != Svr::Config::GetConfig().EntityServers.end(); ++itEntity )
		{
			EntityServerEntity *pEntity = nullptr;
			auto pEntityCfg = *itEntity;

			svrChk(GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<EntityServerEntity>(pEntityCfg->UID, NetClass::Entity, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port, pEntity));
		}



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		pServerConfig = (Svr::Config::SharedModuleServer*)( GetMyConfig() );

		std::for_each( pServerConfig->Modules.begin(), pServerConfig->Modules.end(), [&]( Svr::Config::Module* pModule )
		{
			RegisterClusteredService(pModule->ModuleID, pModule->Parameters.c_str());
		});


		// push Startup transaction
		{
			Svr::Transaction * pProcess = nullptr;
			svrMem( pProcess = new SharedModuleServerStartProcess );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(GetCurrentThreadId(), pProcess) );
		}



	Proc_End:

		return hr;
	}


	// Close Private Network
	HRESULT SharedModuleServer::CloseNetPrivate()
	{
		HRESULT hr = S_OK;

		hr = __super::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}



	// create remote entity by class
	HRESULT SharedModuleServer::CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity )
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
			pServerEntity = new Svr::ServerEntity;
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


}; // namespace SharedModuleServer
}; // namespace BR







