////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Game instance service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "MemoryManager/SFMemory.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceTrans.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "Component/SFLibraryComponentAdapter.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Service/ServerService.h"
#include "Util/SFRandom.h"
#include "Net/SFNetServerTCP.h"
#include "Net/SFNetServerUDP.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "PerformanceCounter/PerformanceCounterClient.h"
#include "Net/SFConnectionAction.h"


namespace SF {
	namespace Svr {

		static DurationMS GameInstanceManagerServiceEntity_NotJoinedConnectionTimeout(60 * 1000);


		//////////////////////////////////////////////////////////////////////////
		//
		//	Timeout connection
		//


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity informations
		//

		GameInstanceManagerServiceEntity::GameInstanceManagerServiceEntity(GameID gameID, ServerConfig::ServerModuleGameInstanceManager* config, ClusterID clusterID, const EndpointAddress& endpoint)
			: super(gameID, clusterID, endpoint)
			, m_NetPublicConfig(GetHeap())
			, m_NewConnectionQueue(GetHeap())
			, m_NumberOfInstance("NumberOfGameInstances")
			, m_GameInstances(GetHeap())
		{
			m_NetPublicConfig = config->PublicNet;

			RegisterMessageHandler<GameInstanceManagerTransCreateGameInstance>();
		}

		GameInstanceManagerServiceEntity::~GameInstanceManagerServiceEntity()
		{
		}

		Result GameInstanceManagerServiceEntity::InitializeEntity(EntityID newEntityID)
		{
			ScopeContext hr;
			EntityUID entityUID;
			PerformanceCounterInstance* pInstance = nullptr;

			svrCheck(super::InitializeEntity(newEntityID));

			svrCheck(Service::DataTableManager->LoadDataTable("ZoneTable", "ZoneId", "gtbl_ZoneTable"));

			pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
			if (pInstance != nullptr)
			{
				pInstance->AddCounter(&m_NumberOfInstance);
			}

			auto serverID = Service::ServerConfig->UID;
			if (m_NetPublicConfig.Protocol == "TCP")
			{
				svrCheckMem(m_pNetPublic = NewObject<Net::ServerTCP>(GetHeap(), serverID, NetClass::GameInstance));
			}
			else if (m_NetPublicConfig.Protocol == "UDP")
			{
				svrCheckMem(m_pNetPublic = NewObject<Net::ServerUDP>(GetHeap(), serverID, NetClass::GameInstance));
			}
			else // (m_NetPublicConfig.Protocol == "MRUDP")
			{
				svrCheckMem(m_pNetPublic = NewObject<Net::ServerMUDP>(GetHeap(), serverID, NetClass::GameInstance));
			}


			m_pNetPublic->SetNewConnectionhandler([this](SharedPointerT<Net::Connection>& conn)
				{
					SharedPointerAtomicT<Net::Connection> pConTem;
					pConTem = std::forward<SharedPointerT<Net::Connection>>(conn);

					//pConTem->AddStateAction(Net::ConnectionState::CONNECTED, &GameInstanceManagerServiceEntity_JoinTimeoutAction);

					pConTem->AddMessageDelegateUnique(uintptr_t(this), 
						Message::PlayInstance::JoinGameInstanceCmd::MID.GetMsgID(),
						[this, pCon = pConTem.get()](Net::Connection*, SharedPointerT<Message::MessageData>& pMsgData)
						{
							
							return true;
						});
					m_NewConnectionQueue.Enqueue(pConTem);
				});

			svrCheck(m_pNetPublic->HostOpen(NetClass::Game, m_NetPublicConfig.ListenIP, m_NetPublicConfig.Port));

			m_pNetPublic->SetIsEnableAccept(true);


			auto pEngine = Engine::GetInstance();
			if (pEngine)
				pEngine->AddComponent(new(GetEngineHeap()) LibraryComponentAdapter<GameInstanceManagerServiceEntity>(GetEngineHeap(), this));

			return hr;
		}

		Result GameInstanceManagerServiceEntity::OnNewInstance(GameInstanceEntity* pGameInstance)
		{
			ScopeContext hr;

			svrCheckPtr(pGameInstance);

			++m_NumberOfInstance;
			//m_LocalWorkload.fetch_add(1, std::memory_order_relaxed);

			{
				SF::MutexScopeLock scopeLock(m_GameInstanceListLock);
				svrCheck(m_GameInstances.Insert(pGameInstance->GetEntityUID(), pGameInstance));
			}

			return hr;
		}

		// Called when a game instance is deleted
		Result GameInstanceManagerServiceEntity::FreeGameInstance(GameInsUID gameUID)
		{
			GameInstanceEntity* pGameInstance = nullptr;
			ScopeContext hr;

			{
				SF::MutexScopeLock scopeLock(m_GameInstanceListLock);
				svrCheck(m_GameInstances.Remove(gameUID, pGameInstance));
			}

			--m_NumberOfInstance;

			return hr;
		}

		// Initialize server component
		Result GameInstanceManagerServiceEntity::InitializeComponent()
		{

			return ResultCode::SUCCESS;
		}

		// Terminate server component
		void GameInstanceManagerServiceEntity::DeinitializeComponent()
		{
			m_pNetPublic->HostClose();
			m_pNetPublic.reset();

		}

		// Process network event
		Result GameInstanceManagerServiceEntity::ProcessNewConnection()
		{
			Result hr = ResultCode::SUCCESS;
			Entity* pEntity = nullptr;
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
					
				default:
					assert(connectionState == Net::ConnectionState::DISCONNECTED); // I want to see when this happens
					pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);
					Service::ConnectionManager->RemoveConnection(pConn);
					pConn->DisconnectNRelease("Disconnected before handed over to GameService");
					pConn = nullptr;
					pConnAtomic = nullptr;
					break;
				}
			}


			return ResultCode::SUCCESS;
		}


	} // namespace Svr {
} // namespace SF {
