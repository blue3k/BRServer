////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server interface implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "String/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "Util/SFRandom.h"
#include "DB/DBClusterManager.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetServerPeerTCP.h"
#include "ServerEntity/ServerEntity.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "Entity/EntityTable.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerEntity/SvrLoopbackConnection.h"
#include "SvrConst.h"
#include "Server/BrServerUtil.h"
#include "PerformanceCounter/PerformanceCounterClient.h"
#include "Transaction/Transaction.h"
#include "ServerConfig/SFServerConfig.h"
#include "DB/Factory.h"
#include "Service/ServerService.h"
#include "Thread/SFThread.h"

#include "Transaction/ExternalTransactionManager.h"
#include "Service/ServerService.h"




#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Login/LoginServiceEntity.h"
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceServiceEntity.h"
#include "ServiceEntity/Chat/ChatChannelManagerServiceEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"
//#include "ServiceEntity/Character/CharacterDataManagerServiceEntity.h"
#include "ServiceEntity/RankingServiceEntity.h"
#include "ServiceEntity/MonitoringServiceEntity.h"
#include "ServiceEntity/Relay/RelayServiceEntity.h"


#include "Transaction/ExternalTransactionManager.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"


#include "Net/SFNetServerPeerTCP.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetServerUDP.h"
#include "Entity/EntityManager.h"


#include "DB/AccountDB.h"



namespace SF {
	namespace Svr {



		BrServer* BrServer::stm_pServerInstance = nullptr;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Server definition
		//
		BrServer::BrServer(NetClass netClass)
			: m_Components(GetHeap())
			, m_NetClass(netClass)
			, m_ServerUpUTCTIme(UTCTimeStampSec::min())
			, m_NumberServicesToWait(0)
			, m_DBManagers(GetHeap())
			, m_NewConnectionQueue(GetHeap())
			, m_ModuleConstructors(GetHeap())
		{
			SetInstance(this);

			// TODO: Fix server memory leak on shutdown
			GetHeap().SetIgnoreMemmoryLeak(true);

			// main server class has private thread for task
			SetTickInterval(DurationMS(0));

			// TODO: to component
			DB::Factory::InitializeDBFactory();

			InitializeModuleFactory();
		}


		BrServer::~BrServer()
		{
			m_pLoopbackServerEntity = nullptr;

			DB::Factory::TerminateDBFactory();
		}


		// Set server instance
		void BrServer::SetInstance(BrServer* pServerInstance)
		{
			Assert(BrServer::stm_pServerInstance == NULL);
			BrServer::stm_pServerInstance = pServerInstance;
		}


		// Process Private network event
		Result BrServer::ProcessNewConnection()
		{
			Result hr = ResultCode::SUCCESS;
			Svr::ServerEntity* pServerEntity = nullptr;
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
				if ((Service::ServerEntityManager->AddOrGetServerEntity((ServerID)remoteInfo.PeerID, remoteInfo.PeerClass, pServerEntity)))
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


			Util::SafeDelete(pServerEntity);

			return hr;
		}



		// Process Public network event
		Result BrServer::ProcessPublicNetworkEvent()
		{
			Result hr = ResultCode::SUCCESS;

			return hr;
		}

		Result BrServer::TerminateEntity()
		{
			return MasterEntity::TerminateEntity();
		}

		Result BrServer::InitializeModuleFactory()
		{
			m_ModuleConstructors.insert("ModMonitoring"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					return Service::EntityManager->AddEntity(EntityFaculty::Service, new(ThisServer->GetHeap()) Svr::MonitoringServiceEntity);
				});

			m_ModuleConstructors.insert("ModLogin"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pLogin = (ServerConfig::ServerModulePublicService*)config;
					if (!ThisServer->AddServiceEntity<Svr::LoginServiceEntity>(&pLogin->PublicNet))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModGame"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pGame = (ServerConfig::ServerModulePublicService*)config;
					if (!ThisServer->AddServiceEntity<Svr::GameServiceEntity>(gameID, &pGame->PublicNet))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModGameInstanceManager"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pGame = (ServerConfig::ServerModuleGameInstanceManager*)config;
					if (!ThisServer->AddServiceEntity<Svr::GameInstanceManagerServiceEntity>(gameID, pGame, ClusterID::GameInstanceManager))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
					//svrChk(GetComponentCarrier().AddComponentWithAdapter(pGameInstanceManager)); // ?
				});

			m_ModuleConstructors.insert("ModRanking"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					if (!ThisServer->AddServiceEntity<Svr::RankingServiceEntity>(GetServerGameID(), ClusterID::Ranking))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModChatting"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					if (!ThisServer->AddServiceEntity<Svr::ChatChannelManagerServiceEntity>(GetServerGameID()))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModGamePartyManager"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					if (!ThisServer->AddServiceEntity<Svr::GamePartyManagerServiceEntity>(GetServerGameID()))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatching_Game_4"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pMatching = (ServerConfig::ServerModuleMatching_4*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingServiceEntity>(GetServerGameID(), ClusterID::Matching_Game_4, pMatching->UseBot))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatching_Game_8"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pMatching = (ServerConfig::ServerModuleMatching_8*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingServiceEntity>(GetServerGameID(), ClusterID::Matching_Game_8, pMatching->UseBot))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModPurchaseValidateGoogle"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleGooglePurchaseValidate*)config;
					char strRelativePath[1024];
					StrUtil::Format(strRelativePath, "../../Config/{0}", pModule->P12KeyFile);
					ThisServer->GetComponentCarrier().AddComponentWithAdapter<Svr::ExternalTransactionManager>();
					ThisServer->GetComponentCarrier().GetComponent<Svr::ExternalTransactionManager>()->InitializeManagerGoogle(
						strRelativePath,
						pModule->Account,
						pModule->AuthScopes);

					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModPurchaseValidateIOS"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleIOSPurchaseValidate*)config;
					ThisServer->GetComponentCarrier().AddComponentWithAdapter<Svr::ExternalTransactionManager>();
					return ThisServer->GetComponentCarrier().GetComponent<Svr::ExternalTransactionManager>()->InitializeManagerIOS(pModule->URL, pModule->AltURL);
				});

			m_ModuleConstructors.insert("ModRelay"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					if (!ThisServer->AddServiceEntity<Svr::RelayServiceEntity>(GetServerGameID(), pModule->PublicNet, pModule->MaximumRelayInstances))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x1"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x1, ClusterMembership::Slave))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x2"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x2, ClusterMembership::Slave))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x3"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x3, ClusterMembership::Slave))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x4"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x4, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x5"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x5, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x6"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x6, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x7"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x7, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x1S"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x1S, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_8x1W"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_8x1W, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_4x1"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_4x1, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_4x1S"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_4x1S, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_4x1W"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_4x1W, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_4x2"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_4x2, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			m_ModuleConstructors.insert("ModMatchingQueue_Game_4x3"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(GetServerGameID(), ClusterID::MatchingQueue_Game_4x3, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			return ResultCode::SUCCESS;
		}

		Result BrServer::CreateServerInstanceZK(const char* nodeName)
		{
			char nodePath[512];
			NetAddress myIPV4Address;

			auto pZKSession = Service::ZKSession->GetZookeeperSession();
			if (pZKSession == nullptr || !pZKSession->IsConnected())
				return ResultCode::NOT_INITIALIZED;

			StrUtil::Format(nodePath, "{0}/{1}", Const::ZK_SERVER_INSTANCE_NODE, nodeName);

			if (!pZKSession->Exists(Const::ZK_SERVER_SERVICE_BASE))
				pZKSession->ACreate(Const::ZK_SERVER_SERVICE_BASE, Json::Value(Json::objectValue), nullptr, 0);

			if (!pZKSession->Exists(Const::ZK_SERVER_INSTANCE_NODE))
				pZKSession->ACreate(Const::ZK_SERVER_INSTANCE_NODE, Json::Value(Json::objectValue), nullptr, 0);

			if (pZKSession->Exists(nodePath))
				pZKSession->Delete(nodePath);

			Net::GetLocalAddress(SockFamily::IPV4, myIPV4Address);

			auto nodeValue = Json::Value(Json::objectValue);
			nodeValue["ServiceName"] = Util::GetServiceName();
			nodeValue["IPAddress"] = myIPV4Address.Address;

#if SF_PLATFORM == SF_PLATFORM_WINDOWS
			nodeValue["ProcessID"] = (uint)GetCurrentProcessId();
#else
			nodeValue["ProcessID"] = (uint)getpid();
#endif

			pZKSession->ACreate(nodePath, nodeValue, nullptr, Zookeeper::NODE_FLAG_EPHEMERAL);

			return ResultCode::SUCCESS;
		}


		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual Thread interface
		//

		Result BrServer::SetupConfiguration()
		{
			SetMyConfig(Service::ServerConfig->FindGenericServer(Util::GetServiceName()));
			return GetMyConfig() != nullptr ? ResultCode::SUCCESS : ResultCode::UNEXPECTED;
		}

		Result BrServer::CreateEntityManager()
		{
			return Service::EntityManager->AddEntity(
				EntityID(EntityFaculty::Service, (uint)ClusterID::ClusterManager),
				dynamic_cast<Svr::ClusterManagerServiceEntity*>(*Service::ClusterManager));
		}

		// Apply configuration
		Result BrServer::ApplyConfiguration()
		{
			Result hr = ResultCode::SUCCESS;

			SetServerUID(GetMyConfig()->UID);

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
			ServerEntity* pEntity = nullptr;

			if (GetNetPrivate() == nullptr)
			{
				// If net private hasn't initialized. we don't need entity system.
				return ResultCode::SUCCESS;
			}

			// Add loop back entity
			svrMem(pEntity = CreateLoopbackEntity());
			svrChk(Service::ServerEntityManager->AddOrGetServerEntity(GetServerUID(), GetNetClass(), pEntity));

			SetLoopbackServerEntity(pEntity);
			pEntity = nullptr;



		Proc_End:

			Util::SafeDelete(pEntity);

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


		Result BrServer::OnStart()
		{
			FunctionContext hr([this](Result result)
				{
					if (result)
						return;

					CloseNetPublic();
					CloseNetPrivate();

					Service::EntityManager->Clear();

					m_Components.TerminateComponents();
					m_Components.ClearComponents();

					Service::EntityManager->FlushDeletedEntity();

					SetServerState(ServerState::STOPED);
					svrTrace(Info, "Start failed hr:{0:X8}", result);
				});

			SetServerState(ServerState::STARTING);

			m_ServerUpUTCTIme = Util::Time.GetTimeUTCSec();

			svrTrace(Info, "Starting Server");

			svrTrace(Info, "Setup configuration");
			svrCheck(SetupConfiguration());

			svrCheckPtr(GetMyConfig());

			// Apply configuration
			svrTrace(Info, "Applying Configuration");
			hr = ApplyConfiguration();
			if (!(hr))
			{
				svrTrace(Error, "Failed Apply configuration, hr={0:X8}", hr);
				return hr;
			}

			// Register cluster manager as entity
			svrTrace(Info, "Initialize Cluster manager");
			svrCheck(CreateEntityManager());


			svrTrace(Info, "initializing Monitoring");
			hr = InitializeMonitoring();
			if (!(hr))
			{
				svrTrace(Error, "Failed Apply configuration, hr={0:X8}", hr);
				return hr;
			}

			// Initialize server resource
			svrTrace(Info, "Initialize server resource");
			hr = InitializeServerResource();
			if (!(hr))
			{
				svrTrace(Error, "Failed Initialize resource, hr={0:X8}", hr);
				return hr;
			}

			// Initialize Network
			svrTrace(Info, "Initialize Private network");
			hr = InitializeNetPrivate();
			if (!(hr))
			{
				svrTrace(Error, "Failed Initialize Private Network, hr={0:X8}", hr);
				return hr;
			}

			svrTrace(Error, "initializing Components");
			svrCheck(InitializeComponents());

			svrTrace(Error, "initializing NetPublic");
			svrCheck(InitializeNetPublic());

			svrTrace(Error, "initializing ZK");
			svrCheck(CreateServerInstanceZK(Util::GetServiceName()));

			svrTrace(Info, "Start process done");

			return hr;
		}

		void BrServer::Run(Thread* pThread)
		{
			//Result hr = ResultCode::SUCCESS;
			const DurationMS lMinCheckTime = DurationMS(10); // 10ms

			m_bIsKillSignaled = false;
			m_bStartTransaction = false;

			if (!OnStart())
				return;

			// We need to set this value manually
			//SetServerState( ServerState::RUNNING );

			UpdateWorkingThreadID(ThisThread::GetThreadID());

			// Main loop
			while (1)
			{
				auto loopInterval = pThread->UpdateInterval(lMinCheckTime);
				if (!m_bIsKillSignaled)
				{
					if (pThread->CheckKillEvent(loopInterval))
					{
						m_bIsKillSignaled = true;

						SetServerState(ServerState::STOPING);

						EnableNetPublic(false);
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
			SetServerState(ServerState::STOPING);

			OnEnd();

		}


		bool BrServer::OnEnd()
		{
			Result hr = ResultCode::SUCCESS;

			SetServerState(ServerState::STOPING);

			hr = CloseNetPrivate();
			if (!(hr))
			{
				svrTrace(Error, "Failed Close Private Network, hr={0:X8}", hr);
			}

			hr = CloseServerResource();
			if (!(hr))
			{
				svrTrace(Error, "Failed Close Private Network, hr={0:X8}", hr);
			}

			// clear components
			m_Components.TerminateComponents();
			m_Components.ClearComponents();

			PerformanceCounterClient::Terminate();


			SetServerState(ServerState::STOPED);

			svrTrace(Info, "Server closed hr:{0:X8}", hr);

			return true;
		}




		// Initialize private Network
		Result BrServer::InitializeNetPrivate()
		{
			FunctionContext hr([this](Result result)
				{
					if (result)
						return;

					CloseNetPublic();
					CloseNetPrivate();
				});


			svrCheck(CloseNetPrivate());

			if (GetMyConfig() == nullptr)
			{
				svrTrace(Error, "No configuration is specified for this server {0}", typeid(*this).name());
				svrCheck(ResultCode::UNEXPECTED);
			}

			// Skip private net if it is not set
			if (GetMyConfig()->PrivateNet.IP.size() > 0)
			{
				// Create private network and open it
				svrCheckMem(m_pNetPrivate = new(GetHeap()) Net::ServerPeerTCP(GetMyConfig()->UID, GetNetClass()));
				m_pNetPrivate->RegisterToEngineObjectManager();

				m_pNetPrivate->SetNewConnectionhandler([this](SharedPointerT<Net::Connection>& newConn)
					{
						SharedPointerAtomicT<Net::Connection> pConnAtomic;
						pConnAtomic = std::forward<SharedPointerT<Net::Connection>>(newConn);
						m_NewConnectionQueue.Enqueue(std::forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic));
					});

				svrCheck(m_pNetPrivate->HostOpen(GetNetClass(), GetMyConfig()->PrivateNet.IP, GetMyConfig()->PrivateNet.Port));
			}

			svrTrace(Info, "Initialize entities");
			hr = InitializeEntities();
			if (!(hr))
			{
				svrTrace(Error, "Failed Initialize basic entities, hr={0:X8}", hr);
			}

			return hr;
		}

		// Close Private Network
		Result BrServer::CloseNetPrivate()
		{
			Result hr = ResultCode::SUCCESS;

			// Terminate remote entity manager
			Service::ServerEntityManager->Clear();
			Service::ClusterManager->Clear();
			Service::EntityManager->Clear();
			//Service::EntityTable->Clear();

			// close private net
			if (m_pNetPrivate != nullptr)
			{
				hr = m_pNetPrivate->HostClose();
				m_pNetPrivate = nullptr;
			}

			return hr;
		}

		ServerEntity* BrServer::CreateLoopbackEntity()
		{
			ServerEntity* pLoopbackEntity = nullptr;
			Svr::LoopbackConnection* pConn = nullptr;
			FunctionContext hr([this, &pLoopbackEntity](Result hr)->ServerEntity*
				{
					return pLoopbackEntity;
				});

			pLoopbackEntity = new(GetHeap()) ServerEntity(Const::SERVER_TRANS_QUEUE, Const::SERVER_TRANSRES_QUEUE);
			pLoopbackEntity->SetEntityUID(EntityUID(GetServerUID(), EntityID(EntityFaculty::Server, 0)));
			pLoopbackEntity->SetServerID(GetServerUID());
			pLoopbackEntity->SetServerUpTime(GetServerUpTime());

			svrCheckPtr(GetNetPrivate());
			pLoopbackEntity->SetPrivateNetAddress(GetNetPrivate()->GetLocalAddress());

			svrCheckMem(pConn = new(GetHeap()) Svr::LoopbackConnection(GetNetClass(), pLoopbackEntity));
			pLoopbackEntity->SetLocalConnection((Net::Connection*)pConn);
			pConn = nullptr;

			return pLoopbackEntity;
		}

		Result BrServer::EmplaceModuleConstructor(StringCrc32 moduleName, ModuleContructorType constructor)
		{
			return m_ModuleConstructors.emplace(moduleName, constructor);
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


		Result BrServer::TickUpdate(TimerAction* pAction)
		{
			Result hr = ResultCode::SUCCESS;

			// Process private network event
			if (!(ProcessNewConnection()))
			{
				svrTrace(SVR_DBGFAIL, "ProcessNewConnection : {0:X8}", hr);
			}

			if (!(ProcessPublicNetworkEvent()))
			{
				svrTrace(SVR_DBGFAIL, "ProcessPublicNetworkEvent : {0:X8}", hr);
			}

			for (auto itMgr : m_DBManagers)
			{
				itMgr->Update();
			}

			// TODO: FIXME
			//GameTable::Update();

			Service::EntityManager->Update();

			MasterEntity::TickUpdate(pAction);

			return hr;
		}

		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual interface definition
		//

		// Get Service name
		Result BrServer::StartServer()
		{
			if (GetServerState() == ServerState::STOPED)
			{
				SetServerState(ServerState::STARTING);
				StartThread();
			}

			return ResultCode::SUCCESS;
		}

		// Get Service name
		Result BrServer::StopServer()
		{
			if (GetServerState() != ServerState::STOPED)
			{
				StopThread();
			}

			return ResultCode::SUCCESS;
		}

		// Create clustered service
		Result BrServer::RegisterModule(ServerConfig::ServerModule* module)
		{
			StringCrc32 moduleName = (const char*)module->ModuleName;

			auto itFound = m_ModuleConstructors.find(moduleName);
			if (itFound != m_ModuleConstructors.end())
			{
				return (*itFound)(this, GetGameID(), module);
			}
			else
			{
				return ResultCode::INVALID_ARG;
			}
		}




	} //namespace Svr{
} //namespace SF{















