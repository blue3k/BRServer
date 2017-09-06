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
#include "ServerSystem/SvrConfig.h"
#include "ServerEntity/ServerEntityManager.h"
//#include "ServerSystem/ServiceEntity/EntityManagerServiceEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"

#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/GamePartyManagerServiceEntity.h"

#include "Table/TableSystem.h"

#include "ServerEntity/EntityServerEntity.h"
#include "ServerEntity/GenericServerEntity.h"


#include "Protocol/Policy/EntityServerNetPolicy.h"

#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Policy/GameMasterServerNetPolicy.h"

#include "Net/NetServerPeer.h"
#include "Net/NetSvrDef.h"
#include "Net/NetServerUDP.h"
#include "Entity/EntityManager.h"


#include "SharedModuleSvrConst.h"
#include "SharedModuleServerClass.h"

#include "Transaction/SharedModuleServerTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"


#define PATH_DATABASE L"../../Data"



namespace SF {
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
		return new(GetMemoryManager()) Svr::ServerEntity;
	}


	// Apply configuration
	Result SharedModuleServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

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
	Result SharedModuleServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		//svrChk( conspiracy::InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result SharedModuleServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

		svrChk(GameTable::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	Result SharedModuleServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;
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
			svrMem( pProcess = new(GetMemoryManager()) SharedModuleServerStartProcess );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk( PendingTransaction(ThisThread::GetThreadID(), pProcess) );
		}


	Proc_End:

		return hr;
	}


	// Close Private Network
	Result SharedModuleServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}



	// create remote entity by class
	Result SharedModuleServer::CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity )
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


}; // namespace SharedModuleServer
}; // namespace SF







