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
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "Util/SFRandom.h"
#include "DB/DBClusterManager.h"
#include "Net/NetSvrDef.h"
#include "Net/NetServerPeerTCP.h"
#include "ServerEntity/ServerEntity.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "Entity/EntityTable.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerEntity/SvrLoopbackConnection.h"
#include "SvrConst.h"
#include "ServerEntity/SvrEntityServerEntity.h"
#include "Server/BrServerUtil.h"
#include "PerformanceCounter/PerformanceCounterClient.h"
#include "Transaction/Transaction.h"
#include "ServerConfig/SFServerConfig.h"
#include "DB/Factory.h"
#include "Table/TableSystem.h"
#include "Service/ServerService.h"

#include "Transaction/ExternalTransactionManager.h"
#include "Service/ServerService.h"


namespace SF{
namespace Svr{



	BrServer *BrServer::stm_pServerInstance = NULL;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Server definition
	//
	BrServer::BrServer( NetClass netClass )
		: m_Components(GetMemoryManager())
		, m_State(ServerState::STOPED)
		, m_uiUID(0)
		, m_NetClass(netClass )
		, m_pMyConfig(nullptr)
		, m_ServerUpUTCTIme(TimeStampSec::min())
		, m_NumberServicesToWait(0)
		, m_DBManagers(GetMemoryManager())
		//, m_bIsStartProcessDone(false)
		, m_bIsNetPublicEnabled(false)
		, m_bIsKillSignaled(false)
		, m_bStartTransaction(false)
		, m_NewConnectionQueue(GetMemoryManager())
	{
		SetInstance( this );

		// main server class has private thread for task
		SetTickInterval(DurationMS(0));


		// TODO: to component
		DB::Factory::InitializeDBFactory();
	}


	BrServer::~BrServer()
	{
		m_pLoopbackServerEntity = nullptr;

		DB::Factory::TerminateDBFactory();
	}


	// Set server instance
	void BrServer::SetInstance( BrServer *pServerInstance )
	{
		Assert( BrServer::stm_pServerInstance == NULL );
		BrServer::stm_pServerInstance = pServerInstance;
	}

	
	// Process Private network event
	Result BrServer::ProcessNewConnection()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		SharedPointerT<Net::Connection> pConn;

		auto numQueued = m_NewConnectionQueue.GetEnqueCount();
		for (uint iQueue = 0; iQueue < numQueued; iQueue++)
		{
			SharedPointerAtomicT<Net::Connection> pConnAtomic;

			if (!m_NewConnectionQueue.Dequeue(pConnAtomic))
				break;

			switch (pConnAtomic->GetConnectionState())
			{
			case Net::ConnectionState::CONNECTING:
				m_NewConnectionQueue.Enqueue(std::forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic));
				break;
			case Net::ConnectionState::CONNECTED:
				break;
			default:
				assert(false); // I want to see when this happenes
				pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);
				pConn->Dispose();
				Service::ConnectionManager->RemoveConnection(pConn);
				pConn = nullptr;
				break;
			}

			if (pConnAtomic == nullptr)
				continue;

			pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);
			auto& remoteInfo = pConn->GetRemoteInfo();
			if ((GetServerComponent<ServerEntityManager>()->AddOrGetServerEntity((ServerID)remoteInfo.PeerID, remoteInfo.PeerClass, pServerEntity)))
			{
				auto localConn = pServerEntity->GetLocalConnection();
				auto oldConn = pServerEntity->GetRemoteConnection();
				if (localConn == nullptr || localConn != (Net::Connection*)pConn)
				{
					//Assert(pConn->GetRefCount() == 0);
					if (oldConn != nullptr)
						oldConn->Disconnect("ProcessNewConnection, Disconnect old connection");
					svrChk(pServerEntity->SetRemoteConnection((Net::Connection*)pConn));
				}
				pConn = nullptr;
				pServerEntity = nullptr;
				break;
			}
			else
			{
				// We need to handle this again
				pConnAtomic = std::forward<SharedPointerT<Net::Connection>>(pConn);
				m_NewConnectionQueue.Enqueue(std::forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic));
			}
		}


Proc_End:


		Util::SafeDelete( pServerEntity );

		return hr;
	}



	// Process Public network event
	Result BrServer::ProcessPublicNetworkEvent()
	{
		Result hr = ResultCode::SUCCESS;

	//Proc_End:

		return hr;
	}

	Result BrServer::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;

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


	// Apply configuration
	Result BrServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

		SetServerUID( GetMyConfig()->UID );

	//Proc_End:

		return hr;
	}
	
	Result BrServer::InitializeMonitoring()
	{
		Result hr = ResultCode::SUCCESS;
		NetAddress svrAddress;
		auto monitoringConfig = Service::ServerConfig->FindGenericServer("MonitoringServer");
		// Skip if not specified
		if (monitoringConfig == nullptr)
			return hr;

		svrChk(Net::SetNetAddress(svrAddress, monitoringConfig->PrivateNet.IP, monitoringConfig->PrivateNet.Port));

		Assert(GetServerUID() != 0);
		svrChk(PerformanceCounterClient::Initialize(GetServerUID(), svrAddress));

	Proc_End:

		return hr;
	}
	
	// Initialize server basic entities
	Result BrServer::InitializeEntities()
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntity *pLoopbackEntity = nullptr;
		ServerEntity *pEntity = nullptr;
		EntityManager *pEntityManager = nullptr;
		Svr::LoopbackConnection *pConn = nullptr;

		svrMem( pEntityManager = CreateEntityManager() );
		svrChk( pEntityManager->InitializeManager( Const::ENTITY_GAMEMANAGER_THREAD ) );
		svrChk( m_Components.AddComponent(pEntityManager) );

		svrChk(m_Components.AddComponent<ServerEntityManager>() );


		// Initialize remote entity manager
		svrChk( GetServerComponent<ServerEntityManager>()->InitializeManager( GetMyConfig()->EntityControlCount ) );


		// Add loop back entity
		svrMem( pLoopbackEntity = CreateLoopbackEntity() );
		pLoopbackEntity->SetEntityUID( EntityUID(GetServerUID(),EntityID(EntityFaculty::Server,0) ) );
		pLoopbackEntity->SetServerID(GetServerUID());
		pLoopbackEntity->SetServerUpTime( GetServerUpTime() );
		svrMem( pConn = new(GetMemoryManager()) Svr::LoopbackConnection( GetNetClass(), pLoopbackEntity) );
		pLoopbackEntity->SetLocalConnection((Net::Connection*)pConn);
		pConn = nullptr;

		pEntity = pLoopbackEntity;
		svrChk(m_Components.GetComponent<Svr::ServerEntityManager>()->AddOrGetServerEntity(GetServerUID(), GetNetClass(), pEntity));
		pEntity = nullptr;

		SetLoopbackServerEntity(pLoopbackEntity);
		pLoopbackEntity = nullptr;


		if( GetNetClass() == NetClass::Entity )
		{
			svrChk(m_Components.AddComponent<ClusterManagerServiceEntity>(ClusterMembership::Slave) );
		}
		else
		{
			svrChk(m_Components.AddComponent<ClusterManagerServiceEntity>(ClusterMembership::StatusWatcher) );
		}

		svrChk(m_Components.GetComponent<EntityManager>()->AddEntity(
			EntityID( EntityFaculty::Service,(uint)ClusterID::ClusterManager ), 
			m_Components.GetComponent<ClusterManagerServiceEntity>() ) );

	Proc_End:

		//Util::SafeRelease( pConn );
		Util::SafeDelete( pLoopbackEntity );

		return hr;
	}

	Result BrServer::InitializeComponents()
	{
		Result hr = ResultCode::SUCCESS;
		auto myConfig = GetMyConfig();
		auto myGameConfig = dynamic_cast<const ServerConfig::GameServer*>(myConfig);
		svrChkPtr(myConfig);

		if (myGameConfig != nullptr)
		{
			for (auto& componentConfig : myGameConfig->Components)
			{
				char strRelativePath[1024];
				auto google = dynamic_cast<ServerConfig::ServerComponentGoogle*> (componentConfig);
				if (google != nullptr)
				{
					StrUtil::Format(strRelativePath, "../../Config/{0}", google->P12KeyFile);
					svrChk(m_Components.AddComponent<ExternalTransactionManager>());
					svrChk(m_Components.GetComponent<ExternalTransactionManager>()->InitializeManagerGoogle(
						strRelativePath,
						google->Account,
						google->AuthScopes));
				}

				auto ios = dynamic_cast<ServerConfig::ServerComponentIOS*> (componentConfig);
				if (ios != nullptr)
				{
					svrChk(m_Components.AddComponent<ExternalTransactionManager>());
					svrChk(m_Components.GetComponent<ExternalTransactionManager>()->InitializeManagerIOS(ios->URL));
				}
			}

		}

		svrChk(m_Components.InitializeComponents());


	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result BrServer::InitializeServerResource()
	{
		return ResultCode::SUCCESS;
	}

	// Close server and release resource
	Result BrServer::CloseServerResource()
	{
		m_Components.TerminateComponents();

		return ResultCode::SUCCESS;
	}


	bool BrServer::OnStart()
	{
		Result hr = ResultCode::SUCCESS;

		SetServerState( ServerState::STARTING );

		m_ServerUpUTCTIme = Util::Time.GetTimeUTCSec();

		svrTrace( Info, "Starting Server" );

		svrTrace( Info, "Apply configuration" );
		// Apply configuration
		hr = ApplyConfiguration();
		if( !(hr) )
		{
			svrTrace( Error, "Failed Apply configuration, hr={0:X8}", hr );
			svrErr( hr );
		}

		hr = InitializeMonitoring();
		if (!(hr))
		{
			svrTrace(Error, "Failed Apply configuration, hr={0:X8}", hr);
			svrErr(hr);
		}

		// Initialize server resource
		svrTrace( Info, "Initialize server resource" );
		hr = InitializeServerResource();
		if( !(hr) )
		{
			svrTrace( Error, "Failed Initialize resource, hr={0:X8}", hr );
			svrErr( hr );
		}

		// Initialize Network
		svrTrace( Info, "Initialize Private network" );
		hr = InitializeNetPrivate();
		if( !(hr) )
		{
			svrTrace( Error, "Failed Initialize Private Network, hr={0:X8}", hr );
			svrErr( hr );
		}

		svrChk( InitializeComponents() );

		svrChk(InitializeNetPublic());

	Proc_End:


		if( !( hr ) )
		{
			SetServerState( ServerState::STOPED );
			svrTrace( Info, "Start failed hr:{0:X8}", hr );
		}
		else
		{
			svrTrace( Info, "Start process done" );
		}

		return (hr);
	}


	void BrServer::Run()
	{
		//Result hr = ResultCode::SUCCESS;
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
		Result hr = ResultCode::SUCCESS;

		SetServerState( ServerState::STOPING );

		hr = CloseNetPrivate();
		if( !(hr) )
		{
			svrTrace( Error, "Failed Close Private Network, hr={0:X8}", hr );
		}

		hr = CloseServerResource();
		if( !(hr) )
		{
			svrTrace( Error, "Failed Close Private Network, hr={0:X8}", hr );
		}

		// clear components
		m_Components.ClearComponents();

		PerformanceCounterClient::Terminate();


		SetServerState( ServerState::STOPED );

		svrTrace( Info, "Server closed hr:{0:X8}", hr );

		return true;
	}




	// Initialize private Network
	Result BrServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;


		svrChk( CloseNetPrivate() );

		if( GetMyConfig() == nullptr )
		{
			svrTrace( Error, "No configuration is specified for this server {0}", typeid(*this).name() );
			svrErr( ResultCode::UNEXPECTED );
		}

		// Create private network and open it
		svrMem( m_pNetPrivate = new(GetMemoryManager()) Net::ServerPeerTCP(GetMyConfig()->UID, GetNetClass()) );

		m_pNetPrivate->SetNewConnectionhandler([this](SharedPointerT<Net::Connection>& newConn)
		{
			SharedPointerAtomicT<Net::Connection> pConnAtomic;
			pConnAtomic = std::forward<SharedPointerT<Net::Connection>>(newConn);
			m_NewConnectionQueue.Enqueue(std::forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic));
		});

		svrChk( m_pNetPrivate->HostOpen( GetNetClass(), GetMyConfig()->PrivateNet.IP, GetMyConfig()->PrivateNet.Port ) );

		svrTrace( Info, "Initialize basic entities" );
		hr = InitializeEntities();
		if( !(hr) )
		{
			svrTrace( Error, "Failed Initialize basic entities, hr={0:X8}", hr );
			svrErr( hr );
		}


	Proc_End:

		if( !(hr) )
		{
			CloseNetPublic();
			CloseNetPrivate();
		}

		return hr;
	}

	// Close Private Network
	Result BrServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;


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
		if( m_pNetPrivate != nullptr )
		{
			hr = m_pNetPrivate->HostClose();
			m_pNetPrivate = nullptr;
		}

	//Proc_End:

		return hr;
	}

	EntityManager* BrServer::CreateEntityManager()
	{
		return new(GetMemoryManager()) EntityManager;
	}

	ServerEntity* BrServer::CreateLoopbackEntity()
	{
		return new(GetMemoryManager()) ServerEntity( Const::SERVER_TRANS_QUEUE, Const::SERVER_TRANSRES_QUEUE );
	}

	// Initialize private Network
	Result BrServer::InitializeNetPublic()
	{
		return ResultCode::SUCCESS;
	}

	// Close Public Network
	Result BrServer::CloseNetPublic()
	{
		return ResultCode::SUCCESS;
	}


	Result BrServer::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		

		// Process private network event
		if( !(ProcessNewConnection()) )
		{
			svrTrace( SVR_DBGFAIL, "ProcessNewConnection : {0:X8}", hr );
		}


		if( !(ProcessPublicNetworkEvent()) )
		{
			svrTrace( SVR_DBGFAIL, "ProcessPublicNetworkEvent : {0:X8}", hr );
		}

		for (auto itMgr : m_DBManagers)
		{
			itMgr->Update();
		}

		GameTable::Update();

		MasterEntity::TickUpdate(pAction);


	//Proc_End:

		return hr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual interface definition
	//

	// Get Service name
	Result BrServer::StartServer()
	{
		if( GetServerState() == ServerState::STOPED )
		{
			SetServerState( ServerState::STARTING );
			Start();
		}

		return ResultCode::SUCCESS;
	}

	// Get Service name
	Result BrServer::StopServer()
	{
		if( GetServerState() != ServerState::STOPED )
		{
			Stop( true );
		}

		return ResultCode::SUCCESS;
	}





}; //namespace SF{
}; //namespace Svr{















