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
#include "Thread/Thread.h"

#include "Transaction/ExternalTransactionManager.h"
#include "Service/ServerService.h"




#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Login/LoginServiceEntity.h"
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceServiceEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServiceEntity/RankingServiceEntity.h"
#include "ServiceEntity/MonitoringServiceEntity.h"

#include "Table/TableSystem.h"

#include "ServerEntity/SvrEntityServerEntity.h"
#include "Transaction/ExternalTransactionManager.h"


#include "Protocol/Policy/EntityServerNetPolicy.h"

#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"


#include "Net/NetServerPeerTCP.h"
#include "Net/NetSvrDef.h"
#include "Net/NetServerUDP.h"
#include "Entity/EntityManager.h"


#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"



namespace SF{
namespace Svr{



	BrServer *BrServer::stm_pServerInstance = NULL;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Server definition
	//
	BrServer::BrServer( NetClass netClass )
		: m_Components(GetMemoryManager())
		, m_NetClass(netClass )
		, m_ServerUpUTCTIme(TimeStampSec::min())
		, m_NumberServicesToWait(0)
		, m_DBManagers(GetMemoryManager())
		, m_NewConnectionQueue(GetMemoryManager())
	{
		SetInstance( this );

		// TODO: 
		GetHeap().SetIgnoreMemmoryLeak(true);

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

			auto connectionState = pConnAtomic->GetConnectionState();
			switch (connectionState)
			{
			case Net::ConnectionState::CONNECTING:
				m_NewConnectionQueue.Enqueue(std::forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic));
				break;
			case Net::ConnectionState::CONNECTED:
				break;
			default:
				assert(connectionState == Net::ConnectionState::DISCONNECTED); // I want to see when this happens
				pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);
				Service::ConnectionManager->RemoveConnection(pConn);
				pConn->DisconnectNRelease("Disconnected(?) before handed over to BRServer");
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

		StopThread();

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

		if (GetMyConfig()->pGameCluster != nullptr)
			m_GameID = GetMyConfig()->pGameCluster->GameClusterID;

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
		Svr::LoopbackConnection *pConn = nullptr;

		svrChk(m_Components.AddComponentWithAdapter<ServerEntityManager>() );


		// Initialize remote entity manager
		svrChk(m_Components.GetComponent<ServerEntityManager>()->InitializeManager( GetMyConfig()->EntityControlCount ) );


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
			svrChk(m_Components.AddComponentWithAdapter<ClusterManagerServiceEntity>(ClusterMembership::Slave) );
		}
		else
		{
			svrChk(m_Components.AddComponentWithAdapter<ClusterManagerServiceEntity>(ClusterMembership::StatusWatcher) );
		}

		svrChk(Service::EntityManager->AddEntity(
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
		svrChkPtr(GetMyConfig());

		//////////////////////////////////////////////////////////////////////////////////////////
		//
		// Register service entities
		//

		for (auto& itModule : GetMyConfig()->Modules)
		{
			RegisterModule(itModule);
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

		return ResultCode::SUCCESS;
	}


	void BrServer::StartThread()
	{
		if (m_MainServerThread != nullptr)
			return;

		m_MainServerThread = new(GetHeap()) FunctorThread([this](Thread* pThread)
		{
			Run(pThread);
		});

		m_MainServerThread->Start();
	}

	void BrServer::StopThread()
	{
		if (m_MainServerThread == nullptr)
			return;

		m_MainServerThread->Stop(true);
		IHeap::Delete(m_MainServerThread);
	}


	bool BrServer::OnStart()
	{
		Result hr = ResultCode::SUCCESS;

		SetServerState( ServerState::STARTING );

		m_ServerUpUTCTIme = Util::Time.GetTimeUTCSec();

		svrTrace( Info, "Starting Server" );

		svrTrace( Info, "Apply configuration" );

		SetMyConfig(Service::ServerConfig->FindGenericServer(Util::GetServiceName()));
		svrChkPtr(GetMyConfig());

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
			// TODO: more clean up

			CloseNetPublic();
			CloseNetPrivate();

			// TODO
			Service::EntityManager->Clear();

			m_Components.TerminateComponents();
			m_Components.ClearComponents();

			Service::EntityManager->FlushDeletedEntity();

			SetServerState( ServerState::STOPED );
			svrTrace( Info, "Start failed hr:{0:X8}", hr );
		}
		else
		{
			svrTrace( Info, "Start process done" );
		}

		return (hr);
	}


	void BrServer::Run(Thread* pThread)
	{
		//Result hr = ResultCode::SUCCESS;
		const DurationMS lMinCheckTime = DurationMS(10); // 10ms

		m_bIsKillSignaled = false;
		m_bStartTransaction = false;

		if (!OnStart()) return;


		// We need to set this value manually
		//SetServerState( ServerState::RUNNING );

		UpdateWorkingThreadID(ThisThread::GetThreadID());

		// Main loop
		while (1)
		{
			auto loopInterval = pThread->UpdateInterval( lMinCheckTime );
			if( !m_bIsKillSignaled )
			{
				if (pThread->CheckKillEvent(loopInterval))
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
		m_Components.TerminateComponents();
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
		m_pNetPrivate->RegisterToEngineObjectManager();

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

		// close private net
		if( m_pNetPrivate != nullptr )
		{
			hr = m_pNetPrivate->HostClose();
			m_pNetPrivate = nullptr;
		}

	//Proc_End:

		return hr;
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

		Service::EntityManager->Update();

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
			StartThread();
		}

		return ResultCode::SUCCESS;
	}

	// Get Service name
	Result BrServer::StopServer()
	{
		if( GetServerState() != ServerState::STOPED )
		{
			StopThread();
		}

		return ResultCode::SUCCESS;
	}

	template< class ServiceEntityType, typename... ConstructorArgs >
	ServiceEntityType* BrServer::AddServiceEntity(ConstructorArgs... constructorArgs)
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ClusteredServiceEntity *pServiceEntityTest = nullptr;
		ServiceEntityType* pServiceEntity = nullptr;

		svrMem(pServiceEntity = new(GetHeap()) ServiceEntityType(constructorArgs...));

		if ((GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity(pServiceEntity->GetClusterID(), pServiceEntityTest)))
		{
			if (pServiceEntityTest == pServiceEntity)
				goto Proc_End;

			if (pServiceEntity->GetClusterMembership() <= ClusterMembership::StatusWatcher)
			{
				svrTrace(Warning, "Duplicated entity watcher entity {0} EntityUID:{1}, while adding {2} EntityUID:{3}", typeid(*pServiceEntityTest).name(), pServiceEntityTest->GetEntityUID(), typeid(*pServiceEntity).name(), pServiceEntity->GetEntityUID());
			}
			else
			{
				svrTrace(Error, "Duplicated cluster entity {0} EntityUID:{1}, while adding {2} EntityUID:{3}", typeid(*pServiceEntityTest).name(), pServiceEntityTest->GetEntityUID(), typeid(*pServiceEntity).name(), pServiceEntity->GetEntityUID());
			}
			IHeap::Delete(pServiceEntity);
			goto Proc_End;
		}

		svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, pServiceEntity));
		svrChk(GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pServiceEntity));


	Proc_End:

		return pServiceEntity;
	}


	template< class ServiceEntityType, typename... ConstructorArgs >
	Result BrServer::AddServiceEntityComponent(ConstructorArgs... constructorArgs)
	{
		Result hr = ResultCode::SUCCESS;

		auto pServiceEntity = AddServiceEntity<ServiceEntityType>(constructorArgs...);
		if (pServiceEntity == nullptr)
			return ResultCode::FAIL;

		svrChk(GetComponentCarrier().AddComponentWithAdapter(pServiceEntity));

	Proc_End:

		return hr;
	}


	Result BrServer::RegisterClustereWatcherComponents(ClusterID clusterID, Svr::ServerComponentID componentIDStart, Svr::ServerComponentID componentIDEnd)
	{
		Result hr = ResultCode::SUCCESS;

		if (componentIDStart != 0 && componentIDEnd != 0)
		{
			for (INT componentID = componentIDStart; componentID <= componentIDEnd; componentID++, clusterID++)
			{
				if (GetComponentCarrier().GetComponent(componentID) != nullptr)
				{
					continue;
				}

				svrChkPtr(AddServiceEntity<Svr::MatchingQueueWatcherServiceEntity>(clusterID));
			}
		}

	Proc_End:

		Assert((hr));

		return hr;
	}

	// Create clustered service
	Result BrServer::RegisterModule(ServerConfig::ServerModule* module)
	{
		Result hr = ResultCode::SUCCESS;
		char strRelativePath[1024];
		FixedString32 moduleName = module->ModuleName;
		svrChkPtr(module);

		switch (moduleName)
		{
		case "ModMonitoring"_hash32:
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, new(GetHeap()) Svr::MonitoringServiceEntity));
			break;

		case "ModLogin"_hash32:
		{
			auto pLogin = (ServerConfig::ServerModulePublicService*)module;
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, new(GetHeap()) Svr::LoginServiceEntity(&pLogin->PublicNet)));
			break;
		}

		case "ModGame"_hash32:
		{
			auto pGame = (ServerConfig::ServerModulePublicService*)module;
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, new(GetHeap()) Svr::GameServiceEntity(GetGameID(), &pGame->PublicNet)));
			svrChkPtr(AddServiceEntity<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			break;
		}

		case "ModRanking"_hash32:
		{
			Svr::ClusteredServiceEntity* pServiceEntity = new(GetHeap()) Svr::RankingServiceEntity(ClusterID::Ranking, ClusterMembership::Master);
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, pServiceEntity));
			svrChk(GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pServiceEntity));
			break;
		}

		case "ModGameInstanceManager"_hash32:
		{
			Svr::ClusteredServiceEntity* pServiceEntity = new(GetHeap()) Svr::GameInstanceManagerServiceEntity(ClusterID::GameInstanceManager, ClusterMembership::Master);
			svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, pServiceEntity));
			svrChk(GetComponentCarrier().GetComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pServiceEntity));
			break;
		}

		case "ModGamePartyManager"_hash32:
			svrChk(AddServiceEntityComponent<Svr::GamePartyManagerServiceEntity>(ClusterMembership::Slave));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1W));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1W));
			break;

		case "ModMatching_Game_4"_hash32:
		{
			auto pMatching = (ServerConfig::ServerModuleMatching_4*)module;
			svrChkPtr(AddServiceEntity<Svr::MatchingServiceEntity>(ClusterID::Matching_Game_4, ClusterMembership::Slave, pMatching->UseBot));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1, Svr::ServerComponentID_MatchingQueueWatcherService_4x1W));
			break;
		}
		case "ModMatching_Game_8"_hash32:
		{
			auto pMatching = (ServerConfig::ServerModuleMatching_8*)module;
			svrChkPtr(AddServiceEntity<Svr::MatchingServiceEntity>(ClusterID::Matching_Game_8, ClusterMembership::Slave, pMatching->UseBot));
			svrChk(AddServiceEntityComponent<Svr::GameInstanceManagerWatcherServiceEntity>(GetGameID()));
			svrChk(RegisterClustereWatcherComponents(ClusterID::MatchingQueue_Game_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1, Svr::ServerComponentID_MatchingQueueWatcherService_8x1W));
			break;
		}
		case "ModPurchaseValidateGoogle"_hash32:
		{
			auto pModule = (ServerConfig::ServerModuleGooglePurchaseValidate*)module;
			StrUtil::Format(strRelativePath, "../../Config/{0}", pModule->P12KeyFile);
			svrChk(GetComponentCarrier().AddComponentWithAdapter<Svr::ExternalTransactionManager>());
			svrChk(GetComponentCarrier().GetComponent<Svr::ExternalTransactionManager>()->InitializeManagerGoogle(
				strRelativePath,
				pModule->Account,
				pModule->AuthScopes));
			break;
		}
		case "ModPurchaseValidateIOS"_hash32:
		{
			auto pModule = (ServerConfig::ServerModuleIOSPurchaseValidate*)module;
			svrChk(GetComponentCarrier().AddComponentWithAdapter<Svr::ExternalTransactionManager>());
			svrChk(GetComponentCarrier().GetComponent<Svr::ExternalTransactionManager>()->InitializeManagerIOS(pModule->URL, pModule->AltURL));
			break;
		}

		case "ModMatchingQueue_Game_8x1"_hash32:
		case "ModMatchingQueue_Game_8x2"_hash32:
		case "ModMatchingQueue_Game_8x3"_hash32:
		case "ModMatchingQueue_Game_8x4"_hash32:
		case "ModMatchingQueue_Game_8x5"_hash32:
		case "ModMatchingQueue_Game_8x6"_hash32:
		case "ModMatchingQueue_Game_8x7"_hash32:
		case "ModMatchingQueue_Game_8x1S"_hash32:
		case "ModMatchingQueue_Game_8x1W"_hash32:

		case "ModMatchingQueue_Game_4x1"_hash32:
		case "ModMatchingQueue_Game_4x2"_hash32:
		case "ModMatchingQueue_Game_4x3"_hash32:
		case "ModMatchingQueue_Game_4x1S"_hash32:
		case "ModMatchingQueue_Game_4x1W"_hash32:
		{
			ClusterID clusterID;
			switch (moduleName)
			{
			case "ModMatchingQueue_Game_8x1"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x1;	break;
			case "ModMatchingQueue_Game_8x2"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x2;	break;
			case "ModMatchingQueue_Game_8x3"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x3;	break;
			case "ModMatchingQueue_Game_8x4"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x4;	break;
			case "ModMatchingQueue_Game_8x5"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x5;	break;
			case "ModMatchingQueue_Game_8x6"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x6;	break;
			case "ModMatchingQueue_Game_8x7"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x7;	break;
			case "ModMatchingQueue_Game_8x1S"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x1S;	break;
			case "ModMatchingQueue_Game_8x1W"_hash32:		clusterID = ClusterID::MatchingQueue_Game_8x1W;	break;

			case "ModMatchingQueue_Game_4x1"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x1;	break;
			case "ModMatchingQueue_Game_4x2"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x2;	break;
			case "ModMatchingQueue_Game_4x3"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x3;	break;
			case "ModMatchingQueue_Game_4x1S"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x1S;	break;
			case "ModMatchingQueue_Game_4x1W"_hash32:		clusterID = ClusterID::MatchingQueue_Game_4x1W;	break;
			default:
				assert(false);
				svrTrace(Error, "Invalid cluster ID for module{0}", module->ModuleName);
				break;
			}

			svrChkPtr(AddServiceEntity<Svr::MatchingQueueServiceEntity>(clusterID, ClusterMembership::Slave));
			break;
		}
		default:
			assert(false);
			svrTrace(Error, "Invalid cluster ID for module: {0}", module->ModuleName);
			break;
		}


	Proc_End:

		return hr;
	}




}; //namespace SF{
}; //namespace Svr{















