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
#include "Common/BrBaseTypes.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/Message.h"
#include "Common/BrRandom.h"

#include "ServerSystem/BrServer.h"
#include "ServerSystem/EntityManager.h"

#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"

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
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "Table/conspiracy/BotTalkTbl.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Transaction/GameInstanceTransPlayer.h"



BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameInstanceEntity);


namespace BR {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameInstanceEntity::GameInstanceEntity()
		: m_PresetGameConfigID(1) // 1 is default
		, m_PresetGameConfig(nullptr)
		, m_pBotTalk(nullptr)
	{
		memset(m_PlayerByIndex,0, sizeof(m_PlayerByIndex));
		SetTickInterval(DurationMS(ConspiracyGameInstanceServer::Const::GAMEINSTANCE_TICK_TIME));
		SetEmptyInstanceKillTimeOut(DurationMS(Const::TIME_DELETE_GAMEINSTANCE_NOPLAYER));


		////////////////////////////////////////////////////////////////////////////////////////////////////
		// To game instance
		BR_ENTITY_MESSAGE(Message::GameInstance::DeleteGameC2SEvt) { svrMemReturn(pNewTrans = new GameEntityTransDeleteGame(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::JoinGameCmd) { svrMemReturn(pNewTrans = new GameEntityTransJoinGame(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::LeaveGameCmd) { svrMemReturn(pNewTrans = new GameEntityTransLeaveGame(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::KickPlayerCmd) { svrMemReturn(pNewTrans = new GameEntityTransKickPlayer(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::AssignRoleCmd) { svrMemReturn(pNewTrans = new GameEntityTransAssignRole(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::ChatMessageC2SEvt) { svrMemReturn(pNewTrans = new GameEntityTransChatMessage(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::VoteGameAdvanceCmd) { svrMemReturn(pNewTrans = new GameEntityTransVoteGameAdvance(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::VoteCmd) { svrMemReturn(pNewTrans = new GameEntityTransVote(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::AdvanceGameCmd) { svrMemReturn(pNewTrans = new GameEntityTransAdvanceGame(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayAgainCmd) { svrMemReturn(pNewTrans = new GameEntityTransGamePlayAgain(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GameRevealPlayerCmd) { svrMemReturn(pNewTrans = new GameEntityTransGameRevealPlayer(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayerReviveCmd) { svrMemReturn(pNewTrans = new GameEntityTransGamePlayerRevive(pMsgData)); return S_SYSTEM_OK; } );
	}

	GameInstanceEntity::~GameInstanceEntity()
	{
	}

	// Initialize game system
	HRESULT GameInstanceEntity::InitializeSystem()
	{
		HRESULT hr = S_SYSTEM_OK;

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
		HRESULT hr = S_SYSTEM_OK;

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
		HRESULT hr = S_SYSTEM_OK;

		svrChk( super::InitializeEntity( newEntityID ) );

		svrChk( InitializeSystem() );

	Proc_End:	

		return hr;
	}

	// Close entity and clear transaction
	HRESULT GameInstanceEntity::TerminateEntity()
	{
		HRESULT hr = S_SYSTEM_OK;

		if( GetEntityState() == EntityState::FREE )
			return S_SYSTEM_OK;

		svrChk(super::TerminateEntity());

		ClearComponents();

	Proc_End:


		return hr;
	}

	//// Run entity
	//HRESULT GameInstanceEntity::TickUpdate(Svr::TimerAction *pAction)
	//{
	//	HRESULT hr = S_SYSTEM_OK;
	//	auto CurTime = Util::Time.GetTimeMs();
	//	StaticArray<PlayerID,64> LeaverList;
	//	Svr::GameInstancePlayer *pGamePlayer = nullptr;
	//	PlayerID pltID;
	//	INT playerCount = 0;

	//	hr = super::TickUpdate(pAction);
	//	if (FAILED(hr) || hr == S_SYSTEM_FALSE)
	//		return hr;

	//	// Update Players
	//	m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&CurTime, &playerCount](const PlayerID& playerID, Svr::GameInstancePlayer* pPlayer)-> bool
	//	{
	//		if (pPlayer->GetServerEntity() != nullptr)
	//			playerCount++;

	//		pPlayer->UpdateGamePlayer(CurTime);
	//		return true;
	//	});

	//	UpdateGameStatus( CurTime );

	//	// Leave player
	//	while( SUCCEEDED(m_PendingReleasePlayer.Dequeue( pltID )) )
	//	{
	//		if (SUCCEEDED(m_GamePlayerByUID.Find(pltID, pGamePlayer)))
	//		{
	//			playerCount--;
	//			LeavePlayer( pGamePlayer );

	//			if (playerCount <= 0)
	//			{
	//				CloseGameInstance();
	//			}

	//		}
	//	}

	//	if (GetEntityState() == EntityState::FREE)
	//		return S_SYSTEM_FALSE;

	////Proc_End:

	//	return hr;
	//}


	////////////////////////////////////////////////////////////
	//
	//	Update processes
	//



	// Update Game status
	HRESULT GameInstanceEntity::UpdateGameStatus( TimeStampMS ulCurTime )
	{
		HRESULT hr = S_SYSTEM_OK;

		// Call check timer to update
		svrChk(super::UpdateGameStatus(ulCurTime));

		svrChk( GetComponent<GameStateSystem>()->UpdateSystem() );

	Proc_End:

		return hr;
	}







	////////////////////////////////////////////////////////////
	//
	//	Game entity
	//

	// Initialize entity to proceed new connection
	HRESULT GameInstanceEntity::InitializeGameEntity(UINT numBot, UINT maxPlayer)
	{
		HRESULT hr = S_SYSTEM_OK;
		GamePlayer *pPlayer = nullptr;

		if (FAILED(conspiracy::BotTalkTbl::FindItem(1, m_pBotTalk)))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to find bot talk item");
			svrErr(E_GAME_INVALID_BOTTALK_TABLE);
		}

		// initialize
		memset(m_PlayerCharacter, 0xFF, sizeof(m_PlayerCharacter));

		// randomize player character
		for (INT character = 0; character < GameConst::MAX_GAMEPLAYER; character++)
		{
			UINT player = (UINT)Util::Random.Rand() % maxPlayer;
			for (UINT iPlayer = 0; iPlayer < maxPlayer; iPlayer++)
			{
				if (m_PlayerCharacter[player] == 0xFF)
				{
					m_PlayerCharacter[player] = character;
					break;
				}
				player++;
				player = player % maxPlayer;
			}
		}


		m_RoleRequestSeer = 0;
		m_RoleRequestWerewolf = 0;

		svrChk(super::InitializeGameEntity(numBot, maxPlayer));


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
		return S_SYSTEM_OK;
	}

	HRESULT GameInstanceEntity::GetPlayerByIndex( INT playerIndex, GamePlayer* &pGamePlayer )
	{
		if( playerIndex < 0 || playerIndex >= (INT)GetMaxPlayer() )
			return E_INVALID_PLAYERID;

		pGamePlayer = m_PlayerByIndex[playerIndex];

		return pGamePlayer ? S_SYSTEM_OK : E_SYSTEM_FAIL;
	}


	HRESULT GameInstanceEntity::CreatePlayerInstance(const PlayerInformation& playerInfo, Svr::GameInstancePlayer* &pPlayer)
	{
		pPlayer = new GamePlayer(this, playerInfo);

		return pPlayer != nullptr ? S_SYSTEM_OK : E_SYSTEM_OUTOFMEMORY;
	}


	// Register new player to join
	HRESULT GameInstanceEntity::AddPlayerToJoin(Svr::GameInstancePlayer* &pInsPlayer)
	{
		HRESULT hr = S_SYSTEM_OK;
		//GamePlayer* pFound = nullptr;
		GamePlayer* pPlayer = nullptr;
		UINT playerIndex;


		pPlayer = (GamePlayer*)pInsPlayer;

		svrChk(super::AddPlayerToJoin(pInsPlayer));
		pInsPlayer = nullptr;


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
			default:
				break;
			}
		}


		// search empty player index from start
		playerIndex = (UINT)Util::Random.Rand() % GetMaxPlayer();
		for( UINT search = 0; search < GetMaxPlayer(); search++ )
		{
			if( m_PlayerByIndex[playerIndex] == nullptr )
			{
				m_PlayerByIndex[playerIndex] = pPlayer;
				pPlayer->SetIndex(playerIndex);
				break;
			}
			playerIndex++;
			if( playerIndex >= GetMaxPlayer() ) playerIndex = 0;
		}

		pPlayer->SetPlayerState( PlayerState::Playing );
		pPlayer->SetCharacter( m_PlayerCharacter[playerIndex] );


	Proc_End:


		return hr;
	}



	// Find Player pilotid
	HRESULT GameInstanceEntity::FindPlayer( PlayerID pltID, GamePlayer* &pGamePlayer )
	{
		HRESULT hr = S_SYSTEM_OK;
		Svr::GameInstancePlayer* pGameInsPlayer = nullptr;

		svrChk(super::FindPlayer(pltID, pGameInsPlayer));

		pGamePlayer = (GamePlayer*)pGameInsPlayer;

	Proc_End:

		return hr;
	}


	// Called when a player get out of game
	HRESULT GameInstanceEntity::OnPlayerGetOutOfGame( Svr::GameInstancePlayer* pPlayer )
	{
		HRESULT hr = S_SYSTEM_OK;

		// update exit status
		svrChk(super::OnPlayerGetOutOfGame(pPlayer));

		svrChk( GetComponent<GamePlaySystem>()->OnPlayerGetOutOfGame((GamePlayer*)pPlayer) );
		svrChk( GetComponent<GameStateSystem>()->OnPlayerGetOutOfGame((GamePlayer*)pPlayer) );

	Proc_End:

		return hr;
	}

}; // ConspiracyGameInstanceServer
}; // namespace BR


