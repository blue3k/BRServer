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
#include "Common/HRESSvrSys.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
//#include "Table/TableSystem.h"

#include "ServerSystem/ServerEntity/EntityServerEntity.h"
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

#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/LoginClusterServiceEntity.h"

#include "LoginSvrConst.h"
#include "LoginServerClass.h"
#include "Entity/LoginPlayerEntity.h"
#include "Transaction/LoginServerTrans.h"

#include "DB/AccountDB.h"
#include "DB/LoginSessionDB.h"


#define PATH_DATABASE L"../../Data"



namespace BR {
namespace LoginServer {

	
	//// Get Entity Server
	//Svr::EntityServerEntity* GetEntityServer()
	//{
	//	Svr::EntityServerEntity *pEntityServer = nullptr;
	//	Svr::ServerEntity *pServerEntity = nullptr;
	//	Svr::ServerEntityManager *pRemoteMgr = Svr::GetServerComponent<Svr::ServerEntityManager>();

	//	if( pRemoteMgr == nullptr ) return nullptr;

	//	if( SUCCEEDED(pRemoteMgr->GetServerEntity( NetClass::Entity, pServerEntity ) ) )
	//	{
	//		pEntityServer = BR_DYNAMIC_CAST(Svr::EntityServerEntity*, pServerEntity);
	//	}

	//	return pEntityServer;
	//}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	LoginServer::LoginServer()
		:BrServer(NetClass::Login)
		,m_pNetPublic(nullptr)
	{
	}


	LoginServer::~LoginServer()
	{
	}

	
	Svr::EntityManager* LoginServer::CreateEntityManager()
	{
		return new Svr::EntityManager;
	}
	

	// Apply configuration
	HRESULT LoginServer::ApplyConfiguration()
	{
		HRESULT hr = S_OK;
		const Svr::Config::PublicServer* pMySvr = nullptr;

		std::for_each( Svr::Config::GetConfig().LoginServers.begin(), Svr::Config::GetConfig().LoginServers.end(), 
			[&]( const Svr::Config::PublicServer* pServer )
		{
			if( pServer->Name == Util::GetServiceNameA() )
			{
				pMySvr = pServer;
			}
		});

		svrChkPtr( pMySvr );
		SetMyConfig( pMySvr );

		svrChk(__super::ApplyConfiguration() );


		m_AuthTicketGenerator.SetServerID(pMySvr->UID);

	Proc_End:

		return hr;
	}


	// Initialize server resource
	HRESULT LoginServer::InitializeServerResource()
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeServerResource() );

		//svrChk( conspiracy::InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	HRESULT LoginServer::CloseServerResource()
	{
		HRESULT hr = S_OK;

		svrChk( __super::CloseServerResource() );

		svrChk( TerminateEntity() );

//		svrChk( conspiracy::TerminateTable() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	HRESULT LoginServer::InitializeNetPrivate()
	{
		HRESULT hr = S_OK;
		Svr::LoginClusterServiceEntity *pLoginService = nullptr;
		Svr::ClusteredServiceEntity *pClusteredEntity = nullptr;

		svrChk( __super::InitializeNetPrivate() );

		GetMyServer()->GetNetPrivate()->SetIsEnableAccept(true);


		// Register entity servers
		for( auto itEntity = Svr::Config::GetConfig().EntityServers.begin(); itEntity != Svr::Config::GetConfig().EntityServers.end(); ++itEntity )
		{
			Svr::EntityServerEntity *pEntity = nullptr;
			auto pEntityCfg = *itEntity;
			if( pEntityCfg->Name != Util::GetServiceNameA() )
			{
				svrChk(GetComponent<Svr::ServerEntityManager>()->GetOrRegisterServer<Svr::EntityServerEntity>(pEntityCfg->UID, NetClass::Entity, pEntityCfg->NetPrivate->IP.c_str(), pEntityCfg->NetPrivate->Port, pEntity));
			}
		}

		// Account DB
		svrChk( InitializeDBCluster<DB::AccountDB>(Svr::Config::GetConfig().AccountDB) );

		// Session DB initialize
		svrChk( InitializeDBCluster<DB::LoginSessionDB>(Svr::Config::GetConfig().LoginSessionDB) );

		// Register game clusters, so that login server can monitor game servers status
		for( ClusterID gameCluster = (ClusterID)((UINT)ClusterID::Game + 1); gameCluster < ClusterID::Game_Max; gameCluster++ )
		{
			svrChk( GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->CreateWatcherForCluster( gameCluster, ClusterType::Shard, pClusteredEntity ) );
		}

		// Local slave entity manager service
		svrMem( pLoginService = new Svr::LoginClusterServiceEntity(ClusterMembership::Slave) );
		svrChk( GetMyServer()->GetComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::Service, pLoginService ) );
		svrChk( GetMyServer()->GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity( pLoginService ) );
		GetMyServer()->AddComponent(pLoginService);

		// push Startup transaction
		{
			Svr::Transaction * pProcess = nullptr;
			svrMem( pProcess = new LoginServerStartProcess );
			svrChk( pProcess->InitializeTransaction(this) );
			svrChk(PendingTransaction(GetCurrentThreadId(), pProcess));
		}

	Proc_End:

		return hr;
	}


	// Close Private Network
	HRESULT LoginServer::CloseNetPrivate()
	{
		HRESULT hr = S_OK;

		hr = __super::CloseNetPrivate();


		// Server Entity Manager will clear this
		SetLoopbackServerEntity( nullptr );

		return hr;
	}


	// Initialize private Network
	HRESULT LoginServer::InitializeNetPublic()
	{
		HRESULT hr = S_OK;
		Svr::Config::PublicServer* pMyConfog = nullptr;

		svrChk( CloseNetPublic() );

		pMyConfog = BR_DYNAMIC_CAST(Svr::Config::PublicServer*,const_cast<Svr::Config::GenericServer*>(GetMyConfig()));
		svrChkPtr( pMyConfog );

		svrMem(m_pNetPublic = new BR::Net::ServerMUDP(GetMyConfig()->UID, GetNetClass()));

		svrChk( m_pNetPublic->HostOpen( GetNetClass(), pMyConfog->NetPublic->IP.c_str(), pMyConfog->NetPublic->Port ) );


	Proc_End:

		return hr;
	}


	// Close Public Network
	HRESULT LoginServer::CloseNetPublic()
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
	HRESULT LoginServer::CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity )
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
			pServerEntity = new Svr::EntityServerEntity();
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




	// Process Private network event
	HRESULT LoginServer::ProcessPublicNetworkEvent()
	{
		HRESULT hr = S_OK;
		Net::INet::Event curEvent;
		LoginPlayerEntity *pLoginPlayerEntity = nullptr;
		Net::Connection *pConn = nullptr;

		if( m_pNetPublic == nullptr )
			return S_OK;

		while( SUCCEEDED(m_pNetPublic->DequeueNetEvent( curEvent )) )
		{
			pConn = nullptr;

			switch( curEvent.EventType )
			{
			case Net::INet::Event::EVT_NET_INITIALIZED:
				break;
			case Net::INet::Event::EVT_NET_CLOSED:
				break;
			case Net::INet::Event::EVT_NEW_CONNECTION:
				if( curEvent.EventConnection == nullptr )
					break;

				pConn = dynamic_cast<Net::Connection*>(curEvent.EventConnection);

				svrChkPtr( pConn );
				svrChkPtr( Svr::GetServerComponent<Svr::EntityManager>() );

				svrMem( pLoginPlayerEntity = new LoginPlayerEntity );

				svrChk( Svr::GetServerComponent<Svr::EntityManager>()->AddEntity( EntityFaculty::User, pLoginPlayerEntity) );

				if( FAILED(pLoginPlayerEntity->SetConnection( pConn )) )
				{
					// NOTE: We need to mark to close this
					pLoginPlayerEntity->ClearEntity();
				}

				pLoginPlayerEntity = nullptr;

				break;
			case Net::INet::Event::EVT_CONNECTION_DISCONNECTED:
				break;
			};
		}


Proc_End:

		if( pConn && m_pNetPublic )
			m_pNetPublic->GetConnectionManager().PendingReleaseConnection(pConn);

		Util::SafeDelete( pLoginPlayerEntity );

		return hr;
	}

	

}; // namespace LoginServer
}; // namespace BR



