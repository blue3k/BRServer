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
		//	New connection handler
		//

		class GameInstanceManager_NewConnectionHandler : public Component
		{
		public:

			using super = Component;

			static constexpr uint32_t ComponentID = "GameInstanceManager_NewConnectionHandler"_crc;


			GameInstanceManager_NewConnectionHandler(GameInstanceManagerServiceEntity* owner, Net::Connection* pCon)
				: Component(ComponentID)
				, m_Manager(owner->AsSharedPtr<GameInstanceManagerServiceEntity>())
				, m_Connection(pCon)
			{
			}
			~GameInstanceManager_NewConnectionHandler() = default;

			virtual Result InitializeComponent() override
			{
				super::InitializeComponent();

				m_OldEventMode = m_Connection->GetEventFireMode();
				m_Connection->SetEventFireMode(Net::Connection::EventFireMode::Immediate);

				auto messageHandler = [this](Net::Connection* pConn, SharedPointerT<Message::MessageData>& pMsgData)
				{
					Message::PlayInstance::JoinGameInstanceCmd cmd(Forward<MessageDataPtr>(pMsgData));
					if (!cmd.ParseMsg())
						return;

					EntityUID instanceUID = cmd.GetPlayInstanceUID();
					PlayerID playerID = cmd.GetPlayerID();

					if (instanceUID.GetEntityID().GetFacultyID() != uint(EntityFaculty::GameInstance))
						return;

					ActorMovement movement{};
					MessageDataPtr pRes;
					SharedPointerT<Svr::Entity> pEntity;
					if (!Service::EntityTable->find(instanceUID.GetEntityID(), pEntity))
					{
						pRes = SF::Message::PlayInstance::JoinGameInstanceRes::Create(GetSystemHeap(), cmd.GetTransactionID(), ResultCode::INVALID_INSTANCEID, instanceUID, playerID, movement);
						pConn->Send(pRes);
						return;
					}

					auto pInstance = pEntity.StaticCast<GameInstanceEntity>();
					Result hr = pInstance->PlayerConnected(playerID, pConn, movement);
					if (!hr)
					{
						pRes = SF::Message::PlayInstance::JoinGameInstanceRes::Create(GetSystemHeap(), cmd.GetTransactionID(), hr, instanceUID, playerID, movement);
						pConn->Send(pRes);
						return;
					}

					// PlayerConnected will change event fire mode, use new value for recovery value on exit
					m_OldEventMode = pConn->GetEventFireMode();

					svrTrace(SVR_INFO, "JoinGameInstance:{0}, player:{1}", instanceUID, playerID);

					auto pThis = pConn->GetComponentManager().RemoveComponent<GameInstanceManager_NewConnectionHandler>();
					IHeap::Delete(pThis);

					pRes = SF::Message::PlayInstance::JoinGameInstanceRes::Create(GetSystemHeap(), cmd.GetTransactionID(), hr, instanceUID, playerID, movement);
					pConn->Send(pRes);
				};

				m_Connection->AddMessageDelegateUnique(uintptr_t(this),
					Message::PlayInstance::JoinGameInstanceCmd::MID.GetMsgID(),
					messageHandler
				);

				auto eventHandler = [this](Net::Connection* pConn, const Net::ConnectionEvent& evt)
				{
					switch (evt.Components.EventType)
					{
					case Net::ConnectionEvent::EventTypes::EVT_STATE_CHANGE:
						switch (evt.Components.State)
						{
						case Net::ConnectionState::CONNECTED:
							m_Timer.SetTimer(GameInstanceManagerServiceEntity_NotJoinedConnectionTimeout);
							break;
						default:
							break;
						}
					case Net::ConnectionEvent::EventTypes::EVT_DISCONNECTED:
					default:
						break;
					}
				};
				m_Connection->GetConnectionEventDelegates().AddDelegateUnique(uintptr_t(this), eventHandler);

				return ResultCode::SUCCESS;
			}

			virtual void TerminateComponent() override
			{
				m_Connection->RemoveMessageDelegate(uintptr_t(this), Message::PlayInstance::JoinGameInstanceCmd::MID.GetMsgID());
				m_Connection->GetConnectionEventDelegates().RemoveDelegateAll(uintptr_t(this));
				m_Connection->SetEventFireMode(m_OldEventMode);

				super::TerminateComponent();
			}

			virtual void TickUpdate() override 
			{
				super::TickUpdate();

				if (m_Timer.CheckTimer())
				{
					m_Connection->GetComponentManager().RemoveComponent<GameInstanceManager_NewConnectionHandler>();
					m_Connection->DisconnectNRelease("Joining game instance has timed out");

					IHeap::Delete(this);
				}
			}


		private:

			Net::Connection::EventFireMode m_OldEventMode = Net::Connection::EventFireMode::OnGameTick;
			WeakPointerT<GameInstanceManagerServiceEntity> m_Manager;
			Net::Connection* m_Connection{}; // this is your owner
			Util::Timer m_Timer;

		};

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


			m_pNetPublic->SetNewConnectionhandler([this](SharedPointerT<Net::Connection> conn)
				{
					SharedPointerAtomicT<Net::Connection> pConTem;
					pConTem = std::forward<SharedPointerT<Net::Connection>>(conn);

					pConTem->GetComponentManager().AddComponent<GameInstanceManager_NewConnectionHandler>(this, pConTem.get());

					m_NewConnectionQueue.Enqueue(pConTem);
				});

			svrCheck(m_pNetPublic->HostOpen(NetClass::GameInstance, m_NetPublicConfig.ListenIP, m_NetPublicConfig.Port));

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
