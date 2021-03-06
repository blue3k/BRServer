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
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"

#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"

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
#include "ServerSystem/EntityManager.h"


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
		, m_GameID(GameID::Game)
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
		HRESULT hr = S_SYSTEM_OK;

		const Svr::Config::ModuleServer* pMySvr = nullptr;

		std::for_each( Svr::Config::GetConfig().ModuleServers.begin(), Svr::Config::GetConfig().ModuleServers.end(), 
			[&]( const Svr::Config::ModuleServer* pServer )
		{
			if( pServer->Name == Util::GetServiceNameA() )
			{
				pMySvr = pServer;
			}
		});

		svrChkPtr( pMySvr );

		m_GameID = pMySvr->GetGameID();

		SetMyConfig( pMySvr );

		svrChk(Svr::BrServer::ApplyConfiguration() );


	Proc_End:

		return hr;
	}


	// Initialize server resource
	HRESULT SharedModuleServer::InitializeServerResource()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(Svr::BrServer::InitializeServerResource() );

		//svrChk( conspiracy::InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	HRESULT SharedModuleServer::CloseServerResource()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	HRESULT SharedModuleServer::InitializeNetPrivate()
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::Config::ModuleServer *pServerConfig = nullptr;
		SockFamily privateNetSockFamily;

		svrChk(Svr::BrServer::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);

		if (Svr::Config::GetConfig().EntityServers.size() > 0)
		{
			// Register entity servers
			// All server should use same sock family(IPV4 or IPV6)
			privateNetSockFamily = GetMyServer()->GetNetPrivate()->GetLocalAddress().SocketFamily;
			for (auto itEntity = Svr::Config::GetConfig().EntityServers.begin(); itEntity != Svr::Config::GetConfig().EntityServers.end(); ++itEntity)
			{
				Svr::EntityServerEntity *pEntity = nullptr;
				auto pEntityCfg = *itEntity;

				NetAddress netAddress(privateNetSockFamily, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port);

				svrChk(GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<Svr::EntityServerEntity>(pEntityCfg->UID, NetClass::Entity, netAddress, pEntity));
			}
		}
		else
		{
			// No entity server is assigned.
			// Add local entity manager
		}


		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		pServerConfig = (Svr::Config::ModuleServer*)( GetMyConfig() );

		std::for_each( pServerConfig->Modules.begin(), pServerConfig->Modules.end(), [&]( Svr::Config::ModuleBase* pModule )
		{
			RegisterClusteredService(pModule);
		});

		// push Startup transaction
		{
			Svr::Transaction * pProcess = nullptr;
			svrMem( pProcess = new SharedModuleServerStartProcess );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(ThisThread::GetThreadID(), pProcess) );
		}


	Proc_End:

		return hr;
	}


	// Close Private Network
	HRESULT SharedModuleServer::CloseNetPrivate()
	{
		HRESULT hr = S_SYSTEM_OK;

		hr = Svr::BrServer::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}



	// create remote entity by class
	HRESULT SharedModuleServer::CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity )
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
			return E_SYSTEM_OUTOFMEMORY;

		return S_SYSTEM_OK;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//


}; // namespace SharedModuleServer
}; // namespace BR







