////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Net/SFMessage.h"
#include "Util/SFRandom.h"
#include "Math/SF3DMathSerialization.h"

#include "Server/BrServer.h"
#include "Entity/EntityManager.h"

#include "Variable/SFVariableTable.h"
#include "Component/SFLibraryComponentAdapter.h"

#include "Server/BrServer.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"
#include "ServiceEntity/Game/GameInstanceMapObject.h"
#include "Protocol/GameInstanceManagerService.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "ServiceEntity/Game/Transaction/GameInstanceTrans.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "Service/ServerService.h"
#include "Actor/Movement/SFActorMovement.h"
#include "ServiceEntity/Game/GameInstancePlayerComponent.h"


namespace SF {
	namespace Svr {


		class PingObjectDirectoryComponent : public Component
		{
		public:
			static constexpr StringCrc32 ComponentID = "PingObjectDirectoryComponent"_crc;

			using super = Component;


			PingObjectDirectoryComponent(GameInstanceEntity* owner)
				: Component(ComponentID)
				, m_Owner(owner)
			{
			}

			virtual Result InitializeComponent() override
			{
				Result hr;

				m_TimeToPing.SetTimer(Const::GAMEINSTANCE_PING_STATUS);

				m_ObjectAttributes.Clear();
				m_ObjectAttributes.SetValue("MaxPlayer", m_Owner->GetMaxPlayer());
				m_ObjectAttributes.SetValue("ZoneTableID", m_Owner->GetZoneTableID());
				m_ObjectAttributes.SetValue("Type", m_Owner->GetInstanceType());
				m_ObjectAttributes.SetValue("NumPlayers", int32_t(m_Owner->GetNumPlayer()));

				svrCheck(Service::ServiceDirectory->PingObjectDirectory(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, m_Owner->GetEntityUID(), m_ObjectAttributes));

				svrCheck(super::InitializeComponent());

				return hr;
			}

			virtual void TickUpdate() override
			{
				// Update registry
				if (m_TimeToPing.CheckTimer())
				{
					m_ObjectAttributes.SetValue("NumPlayers", int32_t(m_Owner->GetNumPlayer()));
					Service::ServiceDirectory->PingObjectDirectory(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, m_Owner->GetEntityUID(), m_ObjectAttributes);
					m_TimeToPing.SetTimer(Const::GAMEINSTANCE_PING_STATUS);
				}

			}

		private:

			GameInstanceEntity* m_Owner{};
			Util::TimeStampTimer m_TimeToPing;
			VariableTable m_ObjectAttributes;

		};

		constexpr StringCrc32 PingObjectDirectoryComponent::ComponentID;



		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity server class
		//

		GameInstanceEntity::GameInstanceEntity()
			: super()
			, m_GamePlayerByPlayerID(GetHeap())
			, m_PendingReleasePlayer(GetHeap())
			, m_PendingJoinedPlayer(GetHeap())
			, m_MapObjects(GetHeap())
			, m_ComponentManger(GetHeap())
		{
			SetTickInterval(Const::GAMEINSTANCE_TICK_TIME);
			SetEmptyInstanceKillTimeOut(Const::GAMEINSTANCE_EMPTYINSTANCE_KILL_TIMEOUT);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			// game instance
			RegisterMessageHandler<GameEntityTransJoinGameInstance>();
			RegisterMessageHandler<GameEntityTransLeaveGameInstance>();

			RegisterMessageHandler<GameInstanceTransPlayerMovement>();

			RegisterMessageHandler<GameInstanceTransOccupyMapObject>();
			RegisterMessageHandler<GameInstanceTransUnoccupyMapObject>();
			RegisterMessageHandler<GameInstanceTransUseMapObject>();
		}

		GameInstanceEntity::~GameInstanceEntity()
		{
		}

		GameInstancePlayer* GameInstanceEntity::CreatePlayer(EntityUID playerEntityUID, const PlayerInformation& player)
		{
			return new(GetHeap()) GameInstancePlayer(this, playerEntityUID, player);
		}

		Svr::GameInstanceMapObject* GameInstanceEntity::CreateMapObject(StringCrc32 mapObjectId)
		{
			return new(GetHeap()) Svr::GameInstanceMapObject(this, mapObjectId);
		}

		Result GameInstanceEntity::GetMapObject(StringCrc32 mapObjectId, GameInstanceMapObject*& pMapObject)
		{
			if (!m_MapObjects.Find(mapObjectId, pMapObject))
				return ResultCode::MAPOBJECT_NOT_FOUND;

			return ResultCode::SUCCESS;
		}

		// Initialize entity to proceed new connection
		Result GameInstanceEntity::InitializeEntity(EntityID newEntityID)
		{
			ScopeContext hr;

			svrCheck(super::InitializeEntity(newEntityID));

			// add components
			m_ComponentManger.AddComponent<PingObjectDirectoryComponent>(this);

			svrCheck(m_ComponentManger.InitializeComponents());

			m_AcceptJoin = true;

			m_LatestTickTime = Util::Time.GetRawTimeMs();
			m_MovementFrame = 0;

			return hr;
		}

		// Close entity and clear transaction
		Result GameInstanceEntity::TerminateEntity()
		{
			ScopeContext hr;

			if (GetEntityState() == EntityState::FREE)
				return ResultCode::SUCCESS;

			m_ComponentManger.TerminateComponents();

			CloseGameInstance();

			super::TerminateEntity();

			m_AcceptJoin = false;

			Service::EntityManager->RemoveEntity(this);

			return hr;
		}

		uint32_t GameInstanceEntity::UpdateMovementTick(TimeStampMS newUpdateTimeStamp)
		{
			auto deltaTime = newUpdateTimeStamp - m_LatestTickTime;
			auto deltaFrames = deltaTime.count() / ActorMovement::DeltaMSPerFrame;

			m_MovementFrame += deltaFrames;
			m_LatestTickTime += DurationMS(deltaFrames * ActorMovement::DeltaMSPerFrame);

			return deltaFrames;
		}

		// Run entity
		Result GameInstanceEntity::TickUpdate(TimerAction* pAction)
		{
			ScopeContext hr;
			auto CurTime = Util::Time.GetRawTimeMs();
			StaticArray<PlayerID, 64> LeaverList(GetHeap());
			GameInstancePlayer* pGamePlayer = nullptr;
			int playerCount = 0;

			{
				MutexScopeLock lock(m_UpdateLock); // Message and Transaction handling should be exclusive access

				hr = super::TickUpdate(pAction);
				if (!hr || hr == ResultCode::SUCCESS_FALSE)
					return hr;

				UpdateJoinedPlayers();
				UpdateReleasedPlayers();
			}

			auto deltaFrames = UpdateMovementTick(CurTime);

			// Update Players
			m_GamePlayerByPlayerID.ForeachOrder(0, m_MaxPlayer, 
				[this, &CurTime, &playerCount](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
				{
					if (pPlayer->GetRemoteEndpoint() != nullptr)
						playerCount++;

					pPlayer->UpdateGamePlayer(CurTime, m_MovementFrame);

					if (pPlayer->GetRemoveTimer().CheckTimer())
					{
						PendingLeavePlayer(pPlayer->GetPlayerID());
					}
					else
					{
						if ((pPlayer->GetBroadCastedMovementFrame() - pPlayer->GetLatestMovement().MoveFrame) > ActorMovement::MoveFrameTimeout)
						{
							pPlayer->SetBroadCastedMovementFrame(pPlayer->GetLatestMovement().MoveFrame);

							// Broad cast movement
							// TODO: Need to add spatial management
							m_GamePlayerByPlayerID.ForeachOrder(0, m_MaxPlayer, 
								[this, pMyPlayer = pPlayer](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
								{
									//if (pMyPlayer->GetPlayerID() == playerID)
									//	return true;

									if (pPlayer->GetRemoteEndpoint() == nullptr)
										return true;

									NetSvrPolicyPlayInstance policy(pPlayer->GetRemoteEndpoint());
									policy.PlayerMovementS2CEvt(GetEntityUID(), pMyPlayer->GetPlayerID(), pMyPlayer->GetLatestMovement());

									return true;
								});
						}
					}

					return true;
				});

			UpdateGameStatus(CurTime);


			if (GetEntityState() == EntityState::FREE)
				return ResultCode::SUCCESS_FALSE;

			m_ComponentManger.TickUpdate();

			return hr;
		}



		////////////////////////////////////////////////////////////
		//
		//	Update processes
		//


		// Update Game status
		Result GameInstanceEntity::UpdateGameStatus(TimeStampMS ulCurTime)
		{
			ScopeContext hr;

			// Call check timer to update and timer function is called
			m_TimeToKill.CheckTimer();

			return hr;
		}

		void GameInstanceEntity::UpdateReleasedPlayers()
		{
			PlayerID playerId{};
			GameInstancePlayer* pGamePlayer = nullptr;

			// Leave player
			while (m_PendingReleasePlayer.Dequeue(playerId))
			{
				if (m_GamePlayerByPlayerID.Find(playerId, pGamePlayer))
				{
					LeavePlayer(pGamePlayer);

					if (m_GamePlayerByPlayerID.size() <= 0 && GetInstanceType() != "Static"_crc)
					{
						CloseGameInstance();
					}

				}
			}
		}

		void GameInstanceEntity::UpdateJoinedPlayers()
		{
			PlayerID playerId{};
			GameInstancePlayer* pJoinedPlayer = nullptr;

			// Leave player
			while (m_PendingJoinedPlayer.Dequeue(playerId))
			{
				if (!m_GamePlayerByPlayerID.Find(playerId, pJoinedPlayer))
					continue;

				m_GamePlayerByPlayerID.ForeachOrder(0, m_MaxPlayer, 
					[this, pJoinedPlayer](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
					{
						if (pJoinedPlayer->GetPlayerID() == playerID)
							return true;

						if (pPlayer->GetRemoteEndpoint() == nullptr)
							return true;

						NetSvrPolicyPlayInstance policy(pPlayer->GetRemoteEndpoint());
						policy.NewPlayerInViewS2CEvt(GetEntityUID(), pJoinedPlayer->GetPlayerID(), pJoinedPlayer->GetCharacterVisual());

						NetSvrPolicyPlayInstance policyJoined(pJoinedPlayer->GetRemoteEndpoint());
						policyJoined.NewPlayerInViewS2CEvt(GetEntityUID(), pPlayer->GetPlayerID(), pPlayer->GetCharacterVisual());

						return true;
					});
			}
		}

		////////////////////////////////////////////////////////////
		//
		//	Game timers
		//


		// set game instance kill timer
		Result GameInstanceEntity::SetGameKillTimer(DurationMS ulWaitTime)
		{
			// Server Notice ?
			if (m_InstanceType != "Static")
				return m_TimeToKill.SetTimer(ulWaitTime);
			return ResultCode::SUCCESS;
		}


		// On Game Kill timer
		void GameInstanceEntity::OnGameKillTimer()
		{
			// TODO: I should  push close entity task
			CloseGameInstance();
		}

		// Close Game Instance
		void GameInstanceEntity::CloseGameInstance()
		{
			if (GetEntityState() == EntityState::WORKING)
				SetEntityState(EntityState::CLOSING);

			m_AcceptJoin = false;


			if (m_InstanceClosed)
				return;

			m_InstanceClosed = true;

			// TODO: delete game instance?
			svrTrace(SVR_INFO, "CloseGameInstance:{0}", GetEntityUID());

			LeaveAllPlayerForGameDelete();

			//Service::ServiceDirectory->RemoveLocalService(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, GetEntityUID());

			// Remove from local service provider
			auto pGameInstanceManagerAdapter = Engine::GetEngineComponent<LibraryComponentAdapter<GameInstanceManagerServiceEntity>>();
			if (pGameInstanceManagerAdapter)
			{
				(*pGameInstanceManagerAdapter)->FreeGameInstance(GetEntityUID());
			}
		}



		////////////////////////////////////////////////////////////
		//
		//	Game entity
		//

		// Initialize entity to proceed new connection
		Result GameInstanceEntity::InitializeGameEntity(const ServerConfig::NetPublic& netPublic, const VariableTable& attributes)
		{
			ScopeContext hr;
			GameInstancePlayer* pPlayer = nullptr;

			m_MaxPlayer = attributes.GetValue<uint>("MaxPlayer"_crc);
			m_InstanceType = attributes.GetValue<StringCrc32>("Type"_crc);
			m_ZoneTableID = attributes.GetValue<uint32_t>("ZoneTableID"_crc);

			m_AddressIPV4.FromString(netPublic.IPV4, netPublic.Port);
			m_AddressIPV6.FromString(netPublic.IPV6, netPublic.Port);

			m_TotalJoinedPlayer = 0;

			// set kill timer
			if (m_InstanceType != "Static")
			{
				Assert(m_EmptyInstanceKillTimeOut.count() != 0);
				SetGameKillTimer(m_EmptyInstanceKillTimeOut);
				m_TimeToKill.SetTimerFunc([&]() { OnGameKillTimer(); });
			}


			auto zoneTableRow = Service::DataTableManager->FindRow("ZoneTable", GetZoneTableID());
			if (zoneTableRow)
			{
				m_StartMove.Position = Vector4(
					zoneTableRow->GetValue<float>("StartPosX"), 
					zoneTableRow->GetValue<float>("StartPosY"), 
					zoneTableRow->GetValue<float>("StartPosZ"));

				svrTrace(Error, "zone start pos for {0}, {1}", GetZoneTableID(), m_StartMove.Position);
			}
			else
			{
				svrTrace(Error, "No zone table entry for {0}", GetZoneTableID());
			}


			return hr;
		}




		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Player
		//


		Result GameInstanceEntity::PlayerConnected(PlayerID playerId, const SharedPointerT<Net::Connection>& connection, ActorMovement& outMovement)
		{
			Result hr;

			{
				GameInstancePlayer* pPlayer{};
				MutexScopeLock lock(m_UpdateLock);

				svrCheck(m_GamePlayerByPlayerID.Find(playerId, pPlayer));

				pPlayer->SetRemoteConnection(connection);
				pPlayer->SetJoined(true);

				if (pPlayer->GetMovementManager())
					outMovement = pPlayer->GetMovementManager()->GetLatestSimulatedMovement();
				outMovement.MoveFrame = GetMovementFrame();
			}

			connection->GetRecvMessageDelegates().AddDelegateUnique(uintptr_t(this),
				[this](Net::Connection* pCon, const MessageDataPtr& pMsg)
				{
					MutexScopeLock lock(m_UpdateLock);
					ProcessMessage(pCon->GetMessageEndpoint(), pMsg);
				});


			m_PendingJoinedPlayer.Enqueue(playerId);

			connection->SetEventFireMode(Net::Connection::EventFireMode::Immediate);

			return hr;
		}

		// Register new player to join
		Result GameInstanceEntity::AddPlayerToJoin(EntityUID playerEntityUID, const PlayerInformation& playerInfo, const VariableTable& characterVisual, const VariableTable& characterAttribute)
		{
			ScopeContext hr;
			GameInstancePlayer* pFound = nullptr;
			SFUniquePtr<Svr::GameInstancePlayer> pPlayer;

			if (m_GamePlayerByPlayerID.Find(playerInfo.PlayerID, pFound))
			{
				svrError(ResultCode::GAME_ALREADY_IN_GAME);
			}

			pPlayer.reset(CreatePlayer(playerEntityUID, playerInfo));
			svrCheckPtr(pPlayer);

			svrCheck(pPlayer->InitializePlayer(this));

			pPlayer->SetCharacterData(characterVisual, characterAttribute);

			m_TotalJoinedPlayer++;

			svrCheck(m_GamePlayerByPlayerID.Insert(pPlayer->GetPlayerID(), pPlayer.get()));

			if (pPlayer->GetMovementManager())
			{
				m_StartMove.MoveFrame = GetMovementFrame() - 1;
				svrTrace(Error, "Player start pos for zone:{0}, player:{1}, pos:{2}", GetZoneTableID(), pPlayer->GetPlayerID(), m_StartMove.Position);
				pPlayer->GetMovementManager()->ResetMovement(m_StartMove);
			}
			pPlayer->GetRemoveTimer().SetTimer(Const::GAMEINSTANCE_PLAYER_REMOVE);

			// clear game killing timer
			m_TimeToKill.ClearTimer();

			pPlayer.release();

			return hr;
		}


		// Player leave
		Result GameInstanceEntity::LeavePlayer(GameInstancePlayer* pPlayer)
		{
			ScopeContext hr;

			pPlayer->ReleaseConnection("Player left");

			GameInstancePlayer* pRemoved{};
			m_GamePlayerByPlayerID.Remove(pPlayer->GetPlayerID(), pRemoved);
			assert(pRemoved == pPlayer);

			// We will leave him as an inactive player so the clean-up and any notify aren't needed

			m_GamePlayerByPlayerID.ForeachOrder(0, m_MaxPlayer,
				[this, pRemoved](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
				{
					if (pRemoved->GetPlayerID() == playerID)
						return true;

					if (pPlayer->GetRemoteEndpoint() == nullptr)
						return true;

					NetSvrPolicyPlayInstance policy(pPlayer->GetRemoteEndpoint());
					policy.RemovePlayerFromViewS2CEvt(GetEntityUID(), pRemoved->GetPlayerID());

					return true;
				});

			svrTrace(SVR_INFO, "LeavePlayer, remain:{0}", m_GamePlayerByPlayerID.size());


			if (m_GamePlayerByPlayerID.size() == 0) // if no player remain
			{
				SetGameKillTimer(m_EmptyInstanceKillTimeOut);
			}

			IHeap::Delete(pPlayer);

			return hr;
		}

		Result GameInstanceEntity::PendingLeavePlayer(PlayerID pltID)
		{
			ScopeContext hr;

			svrCheck(m_PendingReleasePlayer.Enqueue(pltID));

			return hr;
		}

		// Leave all player
		Result GameInstanceEntity::LeaveAllPlayerForGameDelete()
		{
			m_GamePlayerByPlayerID.ForeachOrder(0, m_MaxPlayer, [&](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
				{
					// TODO:
					//if (pPlayer->GetPlayerEntityUID().UID != 0)
					//	NetSvrPolicyGameInstance(pPlayer->GetRemoteEndpoint()).PlayerKickedS2CEvt(RouteContext(GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayer->GetPlayerID());

					LeavePlayer(pPlayer);

					return true;
				});

			m_GamePlayerByPlayerID.ClearMap();

			return ResultCode::SUCCESS;
		}

		// Find Player id
		Result GameInstanceEntity::FindPlayer(PlayerID pltID, GameInstancePlayer*& pGamePlayer)
		{
			ScopeContext hr;

			if (!(m_GamePlayerByPlayerID.Find(pltID, pGamePlayer)))
			{
				return ResultCode::SVR_PLAYER_NOT_FOUND;
			}

			return hr;
		}


		// Called when a player get out of game
		Result GameInstanceEntity::OnPlayerGetOutOfGame(GameInstancePlayer* pPlayer)
		{
			Result hr;

			return hr;
		}

		Result GameInstanceEntity::NewPlayerMovement(PlayerID playerId, const ActorMovement& newMovement)
		{
			Result hr;
			GameInstancePlayer* pPlayer{};

			if (!m_GamePlayerByPlayerID.Find(playerId, pPlayer))
			{
				return ResultCode::SVR_PLAYER_NOT_FOUND;
			}

			svrCheckPtr(pPlayer->GetMovementManager());

			svrCheck(pPlayer->GetMovementManager()->GetMovementManager().EnqueueMovement(newMovement));

			return hr;
		}

	} // Svr
} // SF

