////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server interface implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "Common/StrUtil.h"
#include "Common/MemLog.h"
#include "Common/TimeUtil.h"
#include "Common/BrRandom.h"
#include "Common/BrXML.h"
#include "DB/QueryManager.h"
#include "Net/NetServer.h"
#include "Net/NetServerPeerTCP.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/SvrLoopbackConnection.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/ServerEntity/EntityServerEntity.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrConfig.h"
#include "DB/Factory.h"

#include "ServerSystem/ExternalTransactionManager.h"


namespace BR{
namespace Svr{



	BrServer *BrServer::stm_pServerInstance = NULL;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Server definition
	//
	BrServer::BrServer( NetClass netClass )
		: m_State(ServerState::STOPED)
		, m_uiUID(0)
		, m_pNetPrivate(nullptr)
		, m_NetClass(netClass )
		, m_pMyConfig(nullptr)
		, m_ServerUpUTCTIme(TimeStampSec::min())
		, m_pLoopbackServerEntity(nullptr)
		, m_NumberServicesToWait(0)
		, m_bIsStartProcessDone(false)
		, m_bIsNetPublicEnabled(false)
		, m_bIsKillSignaled(false)
		, m_bStartTransaction(false)
	{
		SetInstance( this );

		xmlInitParser();

		// main server class has private thread for task
		SetTickInterval(DurationMS(0));

		// initialize exception Handler
		Trace::InitExceptionHandler();

		DB::Factory::InitializeDBFactory();

		LIBXML_TEST_VERSION
	}


	BrServer::~BrServer()
	{
		//CloseNetPublic();
		//CloseNetPrivate();

		Util::SafeDelete( m_pLoopbackServerEntity );

		/*
		* Cleanup function for the XML library.
		*/
		xmlCleanupParser();
		/*
		* this is to debug memory for regression tests
		*/
		xmlMemoryDump();

		DB::Factory::TerminateDBFactory();
	}


	// Set server instance
	void BrServer::SetInstance( BrServer *pServerInstance )
	{
		Assert( BrServer::stm_pServerInstance == NULL );
		BrServer::stm_pServerInstance = pServerInstance;
	}

	
	// Process Private network event
	HRESULT BrServer::ProcessPrivateNetworkEvent()
	{
		HRESULT hr = S_OK;
		Net::INet::Event curEvent;
		Svr::ServerEntity *pServerEntity = nullptr;
		Net::Connection *pConn = nullptr;

		if( m_pNetPrivate == nullptr )
			return S_OK;


		while( SUCCEEDED(m_pNetPrivate->DequeueNetEvent( curEvent )) )
		{
			pServerEntity = nullptr;
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

				for (int iTry = 0; iTry < 4; iTry++)
				{
					std::atomic_thread_fence(std::memory_order_acquire);

					if (SUCCEEDED(GetServerComponent<ServerEntityManager>()->AddOrGetServerEntity((ServerID)pConn->GetConnectionInfo().RemoteID, pConn->GetConnectionInfo().RemoteClass, 
						pServerEntity)))
					{
						auto localConn = pServerEntity->GetLocalConnection();
						auto oldConn = pServerEntity->GetRemoteConnection();
						if (localConn == nullptr || localConn != pConn)
						{
							//Assert(pConn->GetRefCount() == 0);
							if (oldConn != nullptr)
								oldConn->Disconnect("ProcessPrivateNetworkEvent, Disconnect old connection");
							svrChk(pServerEntity->SetRemoteConnection(pConn));
						}
						pConn = nullptr;
						pServerEntity = nullptr;
						break;
					}

					ThisThread::SleepFor(DurationMS(0));
				}
				Assert(pConn == nullptr);

				break;
			case Net::INet::Event::EVT_CONNECTION_DISCONNECTED:
				break;
			default:
				break;
			};
		}


Proc_End:

		if( pConn && m_pNetPrivate )
			m_pNetPrivate->GetConnectionManager().PendingReleaseConnection(pConn);

		Util::SafeDelete( pServerEntity );

		return hr;
	}



	// Process Public network event
	HRESULT BrServer::ProcessPublicNetworkEvent()
	{
		HRESULT hr = S_OK;

	//Proc_End:

		return hr;
	}

	HRESULT BrServer::TerminateEntity()
	{
		HRESULT hr = S_OK;

		if (GetThreadID() != ThisThread::GetThreadID())
			Stop(true);

		svrChk(MasterEntity::TerminateEntity());

	Proc_End:

		return hr;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Thread interface
	//

	// initialize memory pool
	HRESULT BrServer::InitializeMemoryPool()
	{
		HRESULT hr = S_OK;

	//Proc_End:

		return hr;
	}

	// Terminate memory pool
	HRESULT BrServer::TerminateMemoryPool()
	{
		HRESULT hr = S_OK;

	//Proc_End:

		return hr;
	}

	// Apply configuration
	HRESULT BrServer::ApplyConfiguration()
	{
		HRESULT hr = S_OK;

		SetServerUID( GetMyConfig()->UID );

	//Proc_End:

		return hr;
	}
	
	HRESULT BrServer::InitializeMonitoring()
	{
		HRESULT hr = S_OK;
		Svr::Config::NetSocket *netInfo = nullptr;
		NetAddress svrAddress;

		svrChkPtr(Svr::Config::GetConfig().MonitoringServer);
		netInfo = Svr::Config::GetConfig().MonitoringServer->NetPrivate;
		svrChkPtr(netInfo);

		svrChk(Net::SetNetAddress(svrAddress, netInfo->IP.c_str(), netInfo->Port));

		Assert(GetServerUID() != 0);
		svrChk(PerformanceCounterClient::Initialize(GetServerUID(), svrAddress));

	Proc_End:

		return hr;
	}
	
	// Initialize server basic entities
	HRESULT BrServer::InitializeEntities()
	{
		HRESULT hr = S_OK;
		ServerEntity *pLoopbackEntity = nullptr;
		ServerEntity *pEntity = nullptr;
		EntityManager *pEntityManager = nullptr;
		Svr::LoopbackConnection *pConn = nullptr;

		svrMem( pEntityManager = CreateEntityManager() );
		svrChk( pEntityManager->InitializeManager( Const::ENTITY_GAMEMANAGER_THREAD ) );
		svrChk( AddComponent(pEntityManager) );

		svrChk( AddComponent<ServerEntityManager>() );


		// Initialzie remote entity manager
		svrChk( GetServerComponent<ServerEntityManager>()->InitializeManager( GetMyConfig()->EntityControlCount ) );


		// Add loopback entity
		svrMem( pLoopbackEntity = CreateLoopbackEntity() );
		pLoopbackEntity->SetEntityUID( EntityUID(GetServerUID(),EntityID(EntityFaculty::Server,0) ) );
		pLoopbackEntity->SetServerID(GetServerUID());
		pLoopbackEntity->SetServerUpTime( GetServerUpTime() );
		svrMem( pConn = new Svr::LoopbackConnection( GetNetClass(), pLoopbackEntity) );
		pLoopbackEntity->SetLocalConnection((Net::Connection*)pConn);
		pConn = nullptr;

		pEntity = pLoopbackEntity;
		svrChk(GetComponent<Svr::ServerEntityManager>()->AddOrGetServerEntity(GetServerUID(), GetNetClass(), pEntity));
		pEntity = nullptr;

		SetLoopbackServerEntity(pLoopbackEntity);
		pLoopbackEntity = nullptr;


		if( GetNetClass() == NetClass::Entity )
		{
			svrChk( AddComponent<ClusterManagerServiceEntity>(ClusterMembership::Slave) );
		}
		else
		{
			svrChk( AddComponent<ClusterManagerServiceEntity>(ClusterMembership::StatusWatcher) );
		}

		svrChk( GetComponent<EntityManager>()->AddEntity( 
			EntityID( EntityFaculty::Service,(UINT)ClusterID::ClusterManager ), 
			GetComponent<ClusterManagerServiceEntity>() ) );

	Proc_End:

		//Util::SafeRelease( pConn );
		Util::SafeDelete( pLoopbackEntity );

		return hr;
	}

	HRESULT BrServer::InitializeComponents()
	{
		HRESULT hr = S_OK;
		auto myConfig = GetMyConfig();

		svrChkPtr(myConfig);

		for (auto componentConfig : myConfig->ServerComponents)
		{
			char strRelativePath[1024];
			auto google = dynamic_cast<Config::ServerComponentGoogle* > (componentConfig);
			if (google != nullptr)
			{
				StrUtil::Format(strRelativePath, "../../Config/{0}", google->P12KeyFile.c_str());
				svrChk(AddComponent<ExternalTransactionManager>());
				svrChk(GetComponent<ExternalTransactionManager>()->InitializeManagerGoogle(
					strRelativePath,
					google->Account.c_str(), 
					google->AuthScopes.c_str()));
			}

			auto ios = dynamic_cast<Config::ServerComponentIOS* > (componentConfig);
			if (ios != nullptr)
			{
				svrChk(AddComponent<ExternalTransactionManager>());
				svrChk(GetComponent<ExternalTransactionManager>()->InitializeManagerIOS(ios->URL.c_str()));
			}
		}

		svrChk(ServerComponentCarrier::InitializeComponents());


	Proc_End:

		return hr;
	}


	// Initialize server resource
	HRESULT BrServer::InitializeServerResource()
	{
		srand( (UINT)Util::Time.GetRawTimeMs().time_since_epoch().count() );
		Util::Random.Srand( nullptr );
		return S_OK;
	}

	// Close server and release resource
	HRESULT BrServer::CloseServerResource()
	{
		TerminateComponents();

		return S_OK;
	}


	bool BrServer::OnStart()
	{
		HRESULT hr = S_OK;

		SetServerState( ServerState::STARTING );

		m_ServerUpUTCTIme = Util::Time.GetTimeUTCSec();

		svrTrace( Trace::TRC_TRACE, "Starting Server" );

		svrTrace( Trace::TRC_TRACE, "Initialize Memory" );
		// Initialize Memory Pool
		hr = InitializeMemoryPool();
		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed to Initialize Memory Pool, hr={0:X8}", hr );
			svrErr( hr );
		}

		svrTrace( Trace::TRC_TRACE, "Apply configuration" );
		// Apply configuration
		hr = ApplyConfiguration();
		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed Apply configuration, hr={0:X8}", hr );
			svrErr( hr );
		}

		hr = InitializeMonitoring();
		if (FAILED(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed Apply configuration, hr={0:X8}", hr);
			svrErr(hr);
		}

		// Initialize server resource
		svrTrace( Trace::TRC_TRACE, "Initialize server resource" );
		hr = InitializeServerResource();
		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed Initialize resource, hr={0:X8}", hr );
			svrErr( hr );
		}

		// Initialize Network
		svrTrace( Trace::TRC_TRACE, "Initialize Private network" );
		hr = InitializeNetPrivate();
		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed Initialize Private Network, hr={0:X8}", hr );
			svrErr( hr );
		}

		svrChk( InitializeComponents() );

		InitializeMemLogger( MemLog::Logging::LOG_ALL, 0 );

		svrChk(InitializeNetPublic());

	Proc_End:


		if( FAILED( hr ) )
		{
			SetServerState( ServerState::STOPED );
			svrTrace( Trace::TRC_TRACE, "Start failed hr:{0:X8}", hr );
		}
		else
		{
			svrTrace( Trace::TRC_TRACE, "Start process done" );
		}

		return SUCCEEDED(hr);
	}


	void BrServer::Run()
	{
		//HRESULT hr = S_OK;
		const DurationMS lMinCheckTime = DurationMS(10); // 10ms

		m_bIsKillSignaled = false;
		m_bStartTransaction = false;

		if (!OnStart()) return;


		// We need to set this value manually
		//SetServerState( ServerState::RUNNING );

		UpdateWorkingThreadID(GetThreadID());

		// Main loop
		while (1)
		{
			auto loopInterval = UpdateInterval( lMinCheckTime );
			if( !m_bIsKillSignaled )
			{
				if (CheckKillEvent(loopInterval))
				{
					m_bIsKillSignaled = true;

					SetServerState( ServerState::STOPING );

					EnableNetPublic( false );
				}
			}
			else // Waiting all connection closed and user entity close completed
			{
				ThisThread::SleepFor(loopInterval);

				// just end for now
				break;
			}


			// Do entity update
			TickUpdate();

		}


	//Proc_End:

		// Server stoping
		SetServerState( ServerState::STOPING );

		OnEnd();

	}


	bool BrServer::OnEnd()
	{
		HRESULT hr = S_OK;

		SetServerState( ServerState::STOPING );

		hr = CloseNetPrivate();
		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed Close Private Network, hr={0:X8}", hr );
		}

		hr = CloseServerResource();
		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed Close Private Network, hr={0:X8}", hr );
		}

		GetEntityTable().Clear();

		// clear components
		ClearComponents();

		PerformanceCounterClient::Terminate();

		hr = TerminateMemoryPool();
		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed to terminate memory pool, hr={0:X8}", hr );
		}


		if( GetMemLogger() )
		{
			GetMemLogger()->Terminate();
		}

		SetServerState( ServerState::STOPED );

		svrTrace( Trace::TRC_TRACE, "Server closed hr:{0:X8}", hr );

		return true;
	}




	// Initialize private Network
	HRESULT BrServer::InitializeNetPrivate()
	{
		HRESULT hr = S_OK;


		svrChk( CloseNetPrivate() );

		if( GetMyConfig() == nullptr )
		{
			svrTrace( Trace::TRC_ERROR, "No configuration is specified for this server {0}", typeid(*this).name() );
			svrErr( E_UNEXPECTED );
		}


		// Create private network and open it
		svrMem( m_pNetPrivate = new Net::ServerPeerTCP(GetMyConfig()->UID, GetNetClass()) );
		svrChkPtr(GetMyConfig()->NetPrivate);
		svrChk( m_pNetPrivate->HostOpen( GetNetClass(), GetMyConfig()->NetPrivate->IP.c_str(), GetMyConfig()->NetPrivate->Port ) );

		svrTrace( Trace::TRC_TRACE, "Initialize basic entities" );
		hr = InitializeEntities();
		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed Initialize basic entities, hr={0:X8}", hr );
			svrErr( hr );
		}


	Proc_End:

		if( FAILED(hr) )
		{
			CloseNetPublic();
			CloseNetPrivate();
		}

		return hr;
	}

	// Close Private Network
	HRESULT BrServer::CloseNetPrivate()
	{
		HRESULT hr = S_OK;


		// Terminate remote entity manager
		if( Svr::GetServerComponent<Svr::ServerEntityManager>() )
		{
			Svr::GetServerComponent<Svr::ServerEntityManager>()->TerminateManager();
		}

		if (Svr::GetServerComponent<Svr::EntityManager>())
		{
			Svr::GetServerComponent<Svr::EntityManager>()->TerminateManager();
		}

		// close private net
		if( m_pNetPrivate )
		{
			hr = m_pNetPrivate->HostClose();
			m_pNetPrivate->Release();
			m_pNetPrivate = nullptr;
		}

	//Proc_End:

		return hr;
	}

	EntityManager* BrServer::CreateEntityManager()
	{
		return new EntityManager;
	}

	ServerEntity* BrServer::CreateLoopbackEntity()
	{
		return new ServerEntity( Const::SERVER_TRANS_QUEUE, Const::SERVER_TRANSRES_QUEUE );
	}

	// Initialize private Network
	HRESULT BrServer::InitializeNetPublic()
	{
		return S_OK;
	}

	// Close Public Network
	HRESULT BrServer::CloseNetPublic()
	{
		return S_OK;
	}


	HRESULT BrServer::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
		

		// Process private network event
		if( FAILED(ProcessPrivateNetworkEvent()) )
		{
			svrTrace( Svr::TRC_DBGFAIL, "ProcessPrivateNetworkEvent : {0:X8}", hr );
		}


		if( FAILED(ProcessPublicNetworkEvent()) )
		{
			svrTrace( Svr::TRC_DBGFAIL, "ProcessPublicNetworkEvent : {0:X8}", hr );
		}

		for (auto itMgr : m_DBManagers)
		{
			itMgr->Update();
		}


		MasterEntity::TickUpdate(pAction);


	//Proc_End:

		return hr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual interface definition
	//

	// Get Service name
	HRESULT BrServer::StartServer()
	{
		if( GetServerState() == ServerState::STOPED )
		{
			SetServerState( ServerState::STARTING );
			Start();
		}

		return S_OK;
	}

	// Get Service name
	HRESULT BrServer::StopServer()
	{
		if( GetServerState() != ServerState::STOPED )
		{
			Stop( true );
		}

		return S_OK;
	}


}; //namespace BR{
}; //namespace Svr{















