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

#include "Server/BrServer.h"
#include "Entity/EntityManager.h"

#include "Variable/SFVariableTable.h"
#include "Object/SFLibraryComponentAdapter.h"

#include "Server/BrServer.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"
#include "Protocol/ServerService/GameInstanceManagerService.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "Service/ServerService.h"


namespace SF {
	namespace Svr {

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity server class
		//

		GameInstanceEntity::GameInstanceEntity()
			: super()
			, m_GamePlayerByUID(GetHeap())
			, m_PendingReleasePlayer(GetHeap())
			, m_ComponentManger(GetHeap())
		{
			SetTickInterval(Const::GAMEINSTANCE_TICK_TIME);
			SetEmptyInstanceKillTimeOut(Const::GAMEINSTANCE_EMPTYINSTANCE_KILL_TIMEOUT);
		}

		GameInstanceEntity::~GameInstanceEntity()
		{
		}

		// Initialize entity to proceed new connection
		Result GameInstanceEntity::InitializeEntity(EntityID newEntityID)
		{
			ScopeContext hr;

			svrCheck(super::InitializeEntity(newEntityID));

			// TODO: add components
			svrCheck(m_ComponentManger.InitializeComponents());

			m_AcceptJoin = true;

			VariableTable attributes;
			attributes.SetValue("MaxPlayer", m_MaxPlayer);
			attributes.SetValue("ZoneTableID", m_ZoneTableID);
			attributes.SetValue("InstanceType", m_InstanceType);

			svrCheck(Service::ServiceDirectory->RegisterLocalService(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, GetEntityUID(), {}, attributes));

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

		// Run entity
		Result GameInstanceEntity::TickUpdate(TimerAction* pAction)
		{
			ScopeContext hr;
			auto CurTime = Util::Time.GetTimeMs();
			StaticArray<PlayerID, 64> LeaverList(GetHeap());
			GameInstancePlayer* pGamePlayer = nullptr;
			PlayerID pltID;
			int playerCount = 0;

			hr = super::TickUpdate(pAction);
			if (!hr || hr == ResultCode::SUCCESS_FALSE)
				return hr;

			// Update Players
			m_GamePlayerByUID.ForeachOrder(0, m_MaxPlayer, 
				[&CurTime, &playerCount](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
				{
					if (pPlayer->GetRemoteEndpoint() != nullptr)
						playerCount++;

					pPlayer->UpdateGamePlayer(CurTime);
					return true;
				});

			UpdateGameStatus(CurTime);

			// Leave player
			while ((m_PendingReleasePlayer.Dequeue(pltID)))
			{
				if ((m_GamePlayerByUID.Find(pltID, pGamePlayer)))
				{
					playerCount--;
					LeavePlayer(pGamePlayer);

					if (playerCount <= 0)
					{
						CloseGameInstance();
					}

				}
			}

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

			// Call check timer to update
			m_TimeToKill.CheckTimer();

			return hr;
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

			Service::ServiceDirectory->RemoveLocalService(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, GetEntityUID());

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
		Result GameInstanceEntity::InitializeGameEntity(const VariableTable& attributes)
		{
			ScopeContext hr;
			GameInstancePlayer* pPlayer = nullptr;

			m_MaxPlayer = attributes.GetValue<uint>("MaxPlayer"_crc);
			m_InstanceType = attributes.GetValue<StringCrc32>("Type"_crc);
			m_ZoneTableID = attributes.GetValue<uint32_t>("ZoneTableID"_crc);

			m_TotalJoinedPlayer = 0;

			// set kill timer
			if (m_InstanceType != "Static")
			{
				Assert(m_EmptyInstanceKillTimeOut.count() != 0);
				SetGameKillTimer(m_EmptyInstanceKillTimeOut);
				m_TimeToKill.SetTimerFunc([&]() { OnGameKillTimer(); });
			}

			return hr;
		}




		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Game Player
		//


		Result GameInstanceEntity::CreatePlayerInstance(const PlayerInformation& playerInfo, SFUniquePtr<Svr::GameInstancePlayer>& pPlayer)
		{
			pPlayer.reset(new(GetHeap()) GameInstancePlayer(this, playerInfo));

			return pPlayer != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
		}

		// Register new player to join
		Result GameInstanceEntity::AddPlayerToJoin(SFUniquePtr<Svr::GameInstancePlayer>& pPlayer)
		{
			ScopeContext hr;
			GameInstancePlayer* pFound = nullptr;

			svrCheckPtr(pPlayer);
			if (m_GamePlayerByUID.Find(pPlayer->GetPlayerID(), pFound))
			{
				svrError(ResultCode::GAME_ALREADY_IN_GAME);
			}

			m_TotalJoinedPlayer++;

			svrCheck(m_GamePlayerByUID.Insert(pPlayer->GetPlayerID(), pPlayer.get()));

			// clear game killing timer
			m_TimeToKill.ClearTimer();

			pPlayer.release();

			return hr;
		}


		// Player leave
		Result GameInstanceEntity::LeavePlayer(GameInstancePlayer*& pPlayer)
		{
			ScopeContext hr;

			pPlayer->SetRemoteEndpoint(nullptr, 0);

			// We will leave him as an inactive player so the clean-up and any notify aren't needed

			svrTrace(SVR_INFO, "LeavePlayer, remain:{0}", m_GamePlayerByUID.size());

			if (m_GamePlayerByUID.size() == 0) // if no player remain
			{
				SetGameKillTimer(m_EmptyInstanceKillTimeOut);
			}


			return hr;
		}

		Result GameInstanceEntity::LeavePlayer(PlayerID pltID)
		{
			ScopeContext hr;

			svrCheck(m_PendingReleasePlayer.Enqueue(pltID));

			return hr;
		}

		// Leave all player
		Result GameInstanceEntity::LeaveAllPlayerForGameDelete()
		{
			m_GamePlayerByUID.ForeachOrder(0, m_MaxPlayer, [&](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
				{
					if (pPlayer->GetPlayerEntityUID().UID != 0)
						NetSvrPolicyGameInstance(pPlayer->GetRemoteEndpoint()).PlayerKickedS2CEvt(RouteContext(GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayer->GetPlayerID());

					LeavePlayer(pPlayer);

					Util::SafeDelete(pPlayer);

					return true;
				});

			m_GamePlayerByUID.ClearMap();

			return ResultCode::SUCCESS;
		}

		// Find Player id
		Result GameInstanceEntity::FindPlayer(PlayerID pltID, GameInstancePlayer*& pGamePlayer)
		{
			ScopeContext hr;

			if (!(m_GamePlayerByUID.Find(pltID, pGamePlayer)))
			{
				return ResultCode::SVR_PLAYER_NOT_FOUND;
			}

			return hr;
		}


		// Called when a player get out of game
		Result GameInstanceEntity::OnPlayerGetOutOfGame(GameInstancePlayer* pPlayer)
		{
			ScopeContext hr;

			return hr;
		}

	} // Svr
} // SF

