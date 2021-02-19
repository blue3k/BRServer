////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Server interface implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Util/SFStrUtil.h"
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
#include "ServerEntity/GenericServerEntity.h"
#include "SvrConst.h"
#include "Server/BrServerUtil.h"
#include "PerformanceCounter/PerformanceCounterClient.h"
#include "Transaction/Transaction.h"
#include "ServerConfig/SFServerConfig.h"
#include "DB/Factory.h"
#include "Service/ServerService.h"
#include "Multithread/SFThread.h"

#include "Transaction/ExternalTransactionManager.h"
#include "Service/ServerService.h"




#include "ServiceEntity/Game/PlayerDirectoryManager.h"
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
//#include "ServiceEntity/MonitoringServiceEntity.h"
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
		BrServer::BrServer()
			: m_Components(GetHeap())
			, m_ServerUpUTCTIme(UTCTimeStampSec::min())
			, m_NumberServicesToWait(0)
			, m_DBManagers(GetHeap())
			, m_ModuleFactories(GetHeap())
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

		Result BrServer::TerminateEntity()
		{
			return super::TerminateEntity();
		}


		void BrServer::AddModuleFactory(StringCrc32 NameCrc, ModuleFactoryFuncType&& Constructor)
		{
			m_ModuleFactories.insert(NameCrc, Constructor);
		}

		Result BrServer::InitializeModuleFactory()
		{
			//AddModuleFactory("ModMonitoring"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
			//	{
			//		return Service::EntityManager->AddEntity(EntityFaculty::Service, new(ThisServer->GetHeap()) Svr::MonitoringServiceEntity);
			//	});

			AddModuleFactory("ModLogin"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pLogin = (ServerConfig::ServerModulePublicService*)config;
					if (!ThisServer->AddServiceEntity<LoginServiceEntity>(&pLogin->PublicNet, config->Endpoint))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModGame"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pGame = (ServerConfig::ServerModulePublicService*)config;
					if (!ThisServer->AddServiceEntity<GameServiceEntity>(gameID, &pGame->PublicNet, config->Endpoint))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModGameInstanceManager"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pGame = (ServerConfig::ServerModuleGameInstanceManager*)config;
					if (!ThisServer->AddServiceEntity<Svr::GameInstanceManagerServiceEntity>(gameID, pGame, ClusterID::GameInstanceManager))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModRanking"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					if (!ThisServer->AddServiceEntity<Svr::RankingServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::Ranking))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModChatting"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					if (!ThisServer->AddServiceEntity<Svr::ChatChannelManagerServiceEntity>(Service::ServerConfig->GameClusterID, config->Endpoint))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModGamePartyManager"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					if (!ThisServer->AddServiceEntity<GamePartyManagerServiceEntity>(Service::ServerConfig->GameClusterID))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatching_Game_4"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pMatching = (ServerConfig::ServerModuleMatching_4*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::Matching_Game_4, pMatching->UseBot))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatching_Game_8"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pMatching = (ServerConfig::ServerModuleMatching_8*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::Matching_Game_8, pMatching->UseBot))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModPurchaseValidateGoogle"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
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

			AddModuleFactory("ModPurchaseValidateIOS"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleIOSPurchaseValidate*)config;
					ThisServer->GetComponentCarrier().AddComponentWithAdapter<Svr::ExternalTransactionManager>();
					return ThisServer->GetComponentCarrier().GetComponent<Svr::ExternalTransactionManager>()->InitializeManagerIOS(pModule->URL, pModule->AltURL);
				});

			AddModuleFactory("ModRelay"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					if (!ThisServer->AddServiceEntity<Svr::RelayServiceEntity>(Service::ServerConfig->GameClusterID, pModule->PublicNet, pModule->MaximumRelayInstances))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x1"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x1, ClusterMembership::Slave))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x2"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x2, ClusterMembership::Slave))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x3"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					if (!ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x3, ClusterMembership::Slave))
						return ResultCode::OUT_OF_MEMORY;
					return ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x4"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x4, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x5"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x5, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x6"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x6, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x7"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x7, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x1S"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x1S, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_8x1W"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_8x1W, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_4x1"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_4x1, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_4x1S"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_4x1S, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_4x1W"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_4x1W, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_4x2"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_4x2, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			AddModuleFactory("ModMatchingQueue_Game_4x3"_crc, [](BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config)
				{
					auto pModule = (ServerConfig::ServerModuleRelayService*)config;
					return ThisServer->AddServiceEntity<Svr::MatchingQueueServiceEntity>(Service::ServerConfig->GameClusterID, ClusterID::MatchingQueue_Game_4x3, ClusterMembership::Slave) == nullptr ? ResultCode::OUT_OF_MEMORY : ResultCode::SUCCESS;
				});

			return ResultCode::SUCCESS;
		}

		Result BrServer::CreateServerInstanceZK(const char* nodeName)
		{
			char nodePath[512];
			char basePath[512];
			NetAddress myIPV4Address;

			auto pZKSession = Service::ZKSession->GetZookeeperSession();
			if (pZKSession == nullptr || !pZKSession->IsConnected())
				return ResultCode::NOT_INITIALIZED;

			if (!pZKSession->Exists(Service::ServerConfig->DataCenter.Path))
				pZKSession->ACreate(Service::ServerConfig->DataCenter.Path, Json::Value(Json::objectValue), nullptr, 0);

			StrUtil::Format(basePath, "{0}/{1}", Service::ServerConfig->DataCenter.Path, Const::ZK_SERVER_INSTANCE_NODE);
			if (!pZKSession->Exists(basePath))
				pZKSession->ACreate(basePath, Json::Value(Json::objectValue), nullptr, 0);

			StrUtil::Format(nodePath, "{0}/{1}", basePath, nodeName);
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
			return ResultCode::SUCCESS;
		}


		Result BrServer::InitializeMonitoring()
		{
			Result hr = ResultCode::SUCCESS;

			// TODO: update to graphite
			//NetAddress svrAddress;
			//auto monitoringConfig = Service::ServerConfig->MonitoringServer;
			//// Skip if not specified
			//if (monitoringConfig == nullptr)
			//	return hr;

			//svrChk(Net::SetNetAddress(svrAddress, monitoringConfig->PrivateNet.IP, monitoringConfig->PrivateNet.Port));

			//Assert(GetServerUID() != 0);
			//svrChk(PerformanceCounterClient::Initialize(GetServerUID(), svrAddress));

		//Proc_End:

			return hr;
		}

		// Initialize server basic entities
		Result BrServer::InitializeEntities()
		{
			Result hr = ResultCode::SUCCESS;
			//UniquePtr<ServerEntity> pEntity;

			// Add loop back entity
			//pEntity.reset(CreateLoopbackEntity());
			//svrCheckMem(pEntity.get());
			//svrCheck(Service::ServerEntityManager->AddOrGetServerEntity(GetServerUID(), pEntity.get()));

			//SetLoopbackServerEntity(pEntity.release());

			return hr;
		}

		Result BrServer::InitializeComponents()
		{
			Result hr = ResultCode::SUCCESS;

			for (auto& itModule : Service::ServerConfig->Modules)
			{
				svrCheck(RegisterModule(itModule));
			}

			svrCheck(m_Components.InitializeComponents());

			return hr;
		}


		// Initialize server resource
		Result BrServer::InitializeServerResource()
		{
			Result hr;

			svrCheck(InitializeEntity(EntityID(EntityFaculty::Server, 0)));

			return hr;
		}

		// Close server and release resource
		Result BrServer::CloseServerResource()
		{
			Result hr;

			svrCheck(TerminateEntity());

			return hr;
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
			ScopeContext hr([this](Result result)
				{
					if (result)
						return;

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
			svrTrace(Info, "Initialize entities");
			hr = InitializeEntities();
			if (!hr)
			{
				svrTrace(Error, "Failed Initialize basic entities, hr={0:X8}", hr);
			}

			svrTrace(Error, "initializing Components");
			svrCheck(InitializeComponents());

			svrTrace(Error, "initializing ZK");
			svrCheck(CreateServerInstanceZK(Util::GetServiceName()));

			svrCheck(InitializingServices());

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

			hr = CloseEntities();
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





		// Close Private Network
		Result BrServer::CloseEntities()
		{
			Result hr = ResultCode::SUCCESS;

			// Terminate remote entity manager
			Service::EntityManager->Clear();

			return hr;
		}

		// create remote entity by class
		Result BrServer::CreateServerEntity(Svr::ServerEntity*& pServerEntity)
		{
			pServerEntity = new(GetHeap()) GenericServerEntity();

			if (pServerEntity == nullptr)
				return ResultCode::OUT_OF_MEMORY;

			return ResultCode::SUCCESS;
		}

		Result BrServer::InitializingServices()
		{
			Result hr;

			for (auto& itService : GetLocalServiceEntities())
			{
				if (!itService->GetInitialized())
					itService->StartInitialization();
			}

			return hr;
		}

		ServerEntity* BrServer::CreateLoopbackEntity()
		{
			ServerEntity* pLoopbackEntity = nullptr;
			Svr::LoopbackConnection* pConn = nullptr;
			ScopeContext hr([this, &pLoopbackEntity](Result hr)->ServerEntity*
				{
					return pLoopbackEntity;
				});

			pLoopbackEntity = new(GetHeap()) ServerEntity(Const::SERVER_TRANS_QUEUE, Const::SERVER_TRANSRES_QUEUE);
			pLoopbackEntity->SetEntityUID(EntityUID(GetServerUID(), EntityID(EntityFaculty::Server, 0)));
			pLoopbackEntity->SetServerID(GetServerUID());
			pLoopbackEntity->SetServerUpTime(GetServerUpTime());

			//svrCheckPtr(GetNetPrivate());
			//pLoopbackEntity->SetPrivateNetAddress(GetNetPrivate()->GetLocalAddress());

			//svrCheckMem(pConn = new(GetHeap()) Svr::LoopbackConnection(GetNetClass(), pLoopbackEntity));
			//pLoopbackEntity->SetLocalConnection((Net::Connection*)pConn);
			pConn = nullptr;

			return pLoopbackEntity;
		}

		Result BrServer::EmplaceModuleFactory(StringCrc32 moduleName, ModuleFactoryFuncType constructor)
		{
			return m_ModuleFactories.emplace(moduleName, constructor);
		}


		Result BrServer::TickUpdate(TimerAction* pAction)
		{
			Result hr = ResultCode::SUCCESS;

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

		Result BrServer::AddServiceEntity(ServiceEntity* pServiceEntity)
		{
			Result hr;

			if (!Service::EntityManager->AddEntity(EntityFaculty::Service, pServiceEntity))
			{
				return ResultCode::UNEXPECTED;
			}

			m_LocalServiceEntities.push_back(pServiceEntity);

			// It is done in ServiceEntity
			//Service::ServiceDirectory->RegisterLocalService(pServiceEntity);

			return hr;
		}

		// Create clustered service
		Result BrServer::RegisterModule(ServerConfig::ServerModule* module)
		{
			StringCrc32 moduleName = (const char*)module->ModuleName;

			auto itFound = m_ModuleFactories.find(moduleName);
			if (itFound != m_ModuleFactories.end())
			{
				return (*itFound)(this, Service::ServerConfig->GameClusterID, module);
			}
			else
			{
				return ResultCode::INVALID_ARG;
			}
		}

	} //namespace Svr{
} //namespace SF{















