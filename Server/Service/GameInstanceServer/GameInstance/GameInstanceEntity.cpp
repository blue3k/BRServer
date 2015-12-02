////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "Common/TimeUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/Message.h"
#include "Common/BRRandom.h"

#include "ServerSystem/BrServer.h"
#include "ServerSystem/EntityManager.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"

#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
//#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "GameInstanceEntity.h"

#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"
#include "GameSystem/GameLogSystem.h"
#include "GameSystem/ChattingLogSystem.h"

#include "ConspiracyGameInstanceSvrConst.h"

#include "GamePlayer.h"
#include "ServerSystem/ServerService/GameInstanceManagerService.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"



BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameInstanceEntity);


namespace BR {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameInstanceEntity::GameInstanceEntity()
		: MasterEntity(64,64)
		, m_Allocator(STDAllocator::GetInstance())
		, m_PresetGameConfig(nullptr)
		, m_PresetGameConfigID(1) // 1 is default
		, m_NumBot(0)
	{
		memset(m_PlayerByIndex,0, sizeof(m_PlayerByIndex));
		SetTickInterval(DurationMS(ConspiracyGameInstanceServer::Const::GAMEINSTANCE_TICK_TIME));
	}

	GameInstanceEntity::~GameInstanceEntity()
	{
	}

	GameInstanceServer* GameInstanceEntity::GetMyOwner()
	{
		return dynamic_cast<GameInstanceServer*>(Svr::BrServer::GetInstance());
	}

	// Initialize game system
	HRESULT GameInstanceEntity::InitializeSystem()
	{
		HRESULT hr = S_OK;

		svrChk( UpdateGameConfig(m_PresetGameConfigID) );

		svrChk( AddComponent<GamePlaySystem>( this, false ) );
		svrChk( AddComponent<GameStateSystem>( this, false ) );
		svrChk( AddComponent<GameLogSystem>( this, false ) );
		svrChk( AddComponent<ChattingLogSystem>( this, false ) );

		svrChk( InitializeComponents() );

	Proc_End:

		return hr;
	}

	// Update game config
	HRESULT GameInstanceEntity::UpdateGameConfig(UINT configPresetID)
	{
		HRESULT hr = S_OK;

		conspiracy::GameConfigTbl::GameConfigItem *pGameConfig = nullptr;
		svrChk( conspiracy::GameConfigTbl::FindItem(configPresetID, pGameConfig ) );

		// set value only if it succeeded
		m_PresetGameConfigID = configPresetID;
		m_PresetGameConfig = pGameConfig;

	Proc_End:

		return hr;
	}

	// Initialize entity to proceed new connection
	HRESULT GameInstanceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk( super::InitializeEntity( newEntityID ) );

		svrChk( InitializeSystem() );

		m_AcceptJoin = true;

	Proc_End:	

		return hr;
	}

	// Close entity and clear transaction
	HRESULT GameInstanceEntity::TerminateEntity()
	{
		HRESULT hr = S_OK;

		if( GetEntityState() == EntityState::FREE )
			return S_OK;

		CloseGameInstance();

		 super::TerminateEntity();

		ClearComponents();

		m_AcceptJoin = false;

		Svr::GetServerComponent<Svr::EntityManager>()->RemoveEntity(GetMyOwner());

	Proc_End:


		return hr;
	}

	// Run entity
	HRESULT GameInstanceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
		auto CurTime = Util::Time.GetTimeMs();
		StaticArray<PlayerID,64> LeaverList;
		//GamePlayerUIDMap::iterator itPlayer;
		GamePlayer *pGamePlayer = nullptr;
		PlayerID pltID;
		INT playerCount = 0;

		hr = super::TickUpdate(pAction);
		if (FAILED(hr) || hr == S_FALSE)
			return hr;

		// Update Players
		m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&CurTime, &playerCount](const PlayerID& playerID, GamePlayer* pPlayer)-> bool
		{
			if (pPlayer->GetServerEntity() != nullptr)
				playerCount++;

			pPlayer->UpdateGamePlayer(CurTime);
			return true;
		});

		UpdateGameStatus( CurTime );

		// Leave player
		while( SUCCEEDED(m_PendingReleasePlayer.Dequeue( pltID )) )
		{
			if (SUCCEEDED(m_GamePlayerByUID.Find(pltID, pGamePlayer)))
			{
				playerCount--;
				LeavePlayer( pGamePlayer );

				if (playerCount <= 0)
				{
					CloseGameInstance();
				}

			}
		}

		if (GetEntityState() == EntityState::FREE)
			return S_FALSE;

	Proc_End:

		return hr;
	}


	////////////////////////////////////////////////////////////
	//
	//	Update processes
	//



	// Update Game status
	HRESULT GameInstanceEntity::UpdateGameStatus( TimeStampMS ulCurTime )
	{
		HRESULT hr = S_OK;

		// Call check timer to update
		m_TimeToKill.CheckTimer();

		svrChk( GetComponent<GameStateSystem>()->UpdateSystem() );

	Proc_End:

		return hr;
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	foreach implementations
	//

	// foreach game player
	HRESULT GameInstanceEntity::ForeachPlayer( std::function<HRESULT(GamePlayer* pPlayer)> func )
	{
		m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pPlayer)-> bool
		{
			HRESULT hrRes = func( pPlayer );
			if( FAILED(hrRes) )
				return false;
			return true;
		});
		return S_OK;
	}


	// foreach game player with Game policy
	HRESULT GameInstanceEntity::ForeachPlayerGameServer( std::function<HRESULT(GamePlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)> func )
	{
		m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pGamePlayer)-> bool
		{
			if( pGamePlayer == nullptr )
				return true;

			if( pGamePlayer->GetPlayerState() == PlayerState::None )
				return true;

			Policy::IPolicyGameServer *pPolicy = pGamePlayer->GetPolicy<Policy::IPolicyGameServer>();
			if( pPolicy )
			{
				HRESULT hrRes = func( pGamePlayer, pPolicy );
				if (FAILED(hrRes))
					return false;
			}
			return true;
		});
		return S_OK;
	}

	// foreach game player with Game policy
	HRESULT GameInstanceEntity::ForeachPlayerSvrGameInstance( std::function<HRESULT(GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy)> func )
	{
		m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pGamePlayer)-> bool
		{
			if( pGamePlayer == nullptr )
				return true;

			if( pGamePlayer->GetPlayerState() == PlayerState::None )
				return true;

			Policy::ISvrPolicyGameInstance *pPolicy = pGamePlayer->GetPolicy<Policy::ISvrPolicyGameInstance>();
			if( pPolicy )
			{
				HRESULT hrRes = func( pGamePlayer, pPolicy );
				if (FAILED(hrRes))
					return false;
			}

			return true;
		});

		return S_OK;
	}


	////////////////////////////////////////////////////////////
	//
	//	Game timers
	//


	// set game instance kill timer
	HRESULT GameInstanceEntity::SetGameKillTimer(DurationMS ulWaitTime )
	{
		// Server Notice ?

		return m_TimeToKill.SetTimer( ulWaitTime );
	}


	// On Game Kill timer
	void GameInstanceEntity::OnGameKillTimer()
	{
		CloseGameInstance();
	}

	// Close Game Instance
	void GameInstanceEntity::CloseGameInstance()
	{
		svrTrace(Trace::TRC_TRACE, "CloseGameInstance:%0%", GetEntityUID());

		LeaveAllPlayerForGameDelete();

		m_AcceptJoin = false;

		if (GetEntityState() == EntityState::WORKING)
			SetEntityState(EntityState::CLOSING);


		// 2. Get service entity list in the cluster
		Svr::GetServerComponent<Svr::GameInstanceManagerServiceEntity>()->FreeGameInstance(GetEntityUID());

	}



	////////////////////////////////////////////////////////////
	//
	//	Game entity
	//

	// Initialize entity to proceed new connection
	HRESULT GameInstanceEntity::InitializeGameEntity(UINT numBot, UINT maxPlayer)
	{
		HRESULT hr = S_OK;
		GamePlayer *pPlayer = nullptr;

		if (maxPlayer > GameConst::MAX_GAMEPLAYER)
		{
			svrErr(E_GAME_INAVLID_PLAYER_COUNT);
		}

		if (numBot > maxPlayer)
		{
			// Too many boot number
			svrTrace(Trace::TRC_ERROR, "Too many bot number numBot:%0% -> %1%, maxPlayer:%2%", numBot, maxPlayer-1, maxPlayer);
			numBot = maxPlayer - 1;
		}

		m_TotalJoinedPlayer = 0;
		m_NumBot = numBot;
		m_MaxPlayer = maxPlayer;

		// set kill timer
		SetGameKillTimer(DurationMS(Const::TIME_DELETE_GAMEINSTANCE_NOPLAYER));

		memset(m_PlayerCharacter, 0xFF, sizeof(m_PlayerCharacter));

		// randomize player character
		for (INT character = 0; character < GameConst::MAX_GAMEPLAYER; character++)
		{
			UINT player = (UINT)Util::Random.Rand(m_MaxPlayer);
			for (UINT iPlayer = 0; iPlayer < m_MaxPlayer; iPlayer++)
			{
				if (m_PlayerCharacter[player] == 0xFF)
				{
					m_PlayerCharacter[player] = character;
					break;
				}
				player++;
				player = player % m_MaxPlayer;
			}
		}

		m_TimeToKill.SetTimerFunc( [&]()  { OnGameKillTimer(); } );


		// add fake bot player
		for (UINT iBot = 0; iBot < m_NumBot; iBot++)
		{
			PlayerInformation playerInfo;
			playerInfo.PlayerID = iBot + 1;
			playerInfo.Level = 1;
			StrUtil::Format(playerInfo.NickName, "Bot%0%", iBot);
			pPlayer = new GamePlayer(this, playerInfo);
			pPlayer->SetIsBot(true);
			svrChk(AddPlayerToJoin(pPlayer));
		}


		m_RoleRequestSeer = 0;
		m_RoleRequestWerewolf = 0;


	Proc_End:

		Util::SafeDelete(pPlayer);

		return hr;
	}




	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game Player
	//

	HRESULT GameInstanceEntity::GetPlayerIndex( PlayerID playerID, UINT &playerIndex )
	{
		GamePlayer* pGamePlayer = nullptr;

		if( FAILED(FindPlayer( playerID, pGamePlayer )) )
			return E_INVALID_PLAYERID;

		playerIndex = pGamePlayer->GetIndex();
		return S_OK;
	}

	HRESULT GameInstanceEntity::GetPlayerByIndex( INT playerIndex, GamePlayer* &pGamePlayer )
	{
		if( playerIndex < 0 || playerIndex >= (INT)m_MaxPlayer )
			return E_INVALID_PLAYERID;

		pGamePlayer = m_PlayerByIndex[playerIndex];

		return pGamePlayer ? S_OK : E_FAIL;
	}


	// Register new player to join
	HRESULT GameInstanceEntity::AddPlayerToJoin(GamePlayer* &pPlayer)
	{
		HRESULT hr = S_OK;
		GamePlayer* pFound = nullptr;

		svrChkPtr( pPlayer );
		if (SUCCEEDED(m_GamePlayerByUID.Find(pPlayer->GetPlayerID(), pFound)))
		{
			svrErr(E_GAME_ALREADY_IN_GAME);
		}

		m_TotalJoinedPlayer++;

		// Check role availability
		if (pPlayer->GetRequestedRole() != PlayerRole::None)
		{
			switch (pPlayer->GetRequestedRole())
			{
			case PlayerRole::Seer:
				if (m_RoleRequestSeer == 0) m_RoleRequestSeer++;
				else                        svrErr(E_GAME_INVALID_ROLE);
				break;
			case PlayerRole::Werewolf:
				if (m_RoleRequestWerewolf < 2) m_RoleRequestWerewolf++;
				else                           svrErr(E_GAME_INVALID_ROLE);
				break;
			}
		}


		svrChk(m_GamePlayerByUID.Insert(pPlayer->GetPlayerID(), pPlayer));

		// search empty player index from start
		UINT playerIndex = (UINT)Util::Random.Rand() % m_MaxPlayer;
		for( UINT search = 0; search < m_MaxPlayer; search++ )
		{
			if( m_PlayerByIndex[playerIndex] == nullptr )
			{
				m_PlayerByIndex[playerIndex] = pPlayer;
				pPlayer->SetIndex(playerIndex);
				break;
			}
			playerIndex++;
			if( playerIndex >= m_MaxPlayer ) playerIndex = 0;
		}

		pPlayer->SetPlayerState( PlayerState::Playing );
		pPlayer->SetCharacter( m_PlayerCharacter[playerIndex] );


		// clear game killing timer
		m_TimeToKill.ClearTimer();

		pPlayer = nullptr;

	Proc_End:


		return hr;
	}


	// Player leave
	HRESULT GameInstanceEntity::LeavePlayer( GamePlayer* &pPlayer )
	{
		HRESULT hr = S_OK;
		Net::Connection *pCon = nullptr;

		pPlayer->SetServerEntity(nullptr,0);

		// We will leave him as an inactive player so the clean-up and any notify aren't needed


	Proc_End:

		svrTrace(Trace::TRC_TRACE, "LeavePlayer, remain:%0%", m_GamePlayerByUID.GetItemCount());

		if (m_GamePlayerByUID.GetItemCount() == 0) // if no player remain
		{
			SetGameKillTimer(DurationMS(Const::TIME_DELETE_GAMEINSTANCE_NOPLAYER));
		}


		return hr;
	}

	HRESULT GameInstanceEntity::LeavePlayer( PlayerID pltID )
	{
		HRESULT hr = S_OK;

		svrChk( m_PendingReleasePlayer.Enqueue( pltID ) );

	Proc_End:

		return hr;
	}

	// Leave all player
	HRESULT GameInstanceEntity::LeaveAllPlayerForGameDelete()
	{
		m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GamePlayer* pPlayer)-> bool
		{
			auto pPolicy = pPlayer->GetPolicy<Policy::ISvrPolicyGameInstance>();
			if (pPolicy != nullptr && pPlayer->GetPlayerEntityUID() != 0)
				pPolicy->PlayerKickedS2CEvt(RouteContext(GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayer->GetPlayerID());

			LeavePlayer( pPlayer );

			Util::SafeDelete(pPlayer);

			return true;
		});

		m_GamePlayerByUID.ClearMap();

		return S_OK;
	}

	// Find Player pilotid
	HRESULT GameInstanceEntity::FindPlayer( PlayerID pltID, GamePlayer* &pGamePlayer )
	{
		HRESULT hr = S_OK;

		if (FAILED(m_GamePlayerByUID.Find(pltID, pGamePlayer)))
		{
			return E_PLAYER_NOT_FOUND;
		}

	Proc_End:

		return hr;
	}


	// Called when a player get out of game
	HRESULT GameInstanceEntity::OnPlayerGetOutOfGame( GamePlayer* pPlayer )
	{
		HRESULT hr = S_OK;

		// update exit status
		svrChk( GetComponent<GamePlaySystem>()->OnPlayerGetOutOfGame(pPlayer) );
		svrChk( GetComponent<GameStateSystem>()->OnPlayerGetOutOfGame(pPlayer) );

	Proc_End:

		return hr;
	}

}; // ConspiracyGameInstanceServer
}; // namespace BR


