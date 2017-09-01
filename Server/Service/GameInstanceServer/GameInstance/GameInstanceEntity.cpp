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
#include "Util/TimeUtil.h"
#include "Types/BrBaseTypes.h"
#include "ResultCode/SFResultCodeCommon.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Net/Message.h"
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
#include "Table/TableSystem.h"
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
		: m_TableVersion(-1)
		, m_PresetGameConfigID(1) // 1 is default
		, m_PresetGameConfig(nullptr)
		, m_pBotTalk(nullptr)
	{
		memset(m_PlayerByIndex,0, sizeof(m_PlayerByIndex));
		SetTickInterval(DurationMS(ConspiracyGameInstanceServer::Const::GAMEINSTANCE_TICK_TIME));
		SetEmptyInstanceKillTimeOut(DurationMS(Const::TIME_DELETE_GAMEINSTANCE_NOPLAYER));


		////////////////////////////////////////////////////////////////////////////////////////////////////
		// To game instance
		BR_ENTITY_MESSAGE(Message::GameInstance::DeleteGameC2SEvt) { svrMemReturn(pNewTrans = new GameEntityTransDeleteGame(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::JoinGameCmd) { svrMemReturn(pNewTrans = new GameEntityTransJoinGame(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::LeaveGameCmd) { svrMemReturn(pNewTrans = new GameEntityTransLeaveGame(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::KickPlayerCmd) { svrMemReturn(pNewTrans = new GameEntityTransKickPlayer(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::AssignRoleCmd) { svrMemReturn(pNewTrans = new GameEntityTransAssignRole(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::ChatMessageC2SEvt) { svrMemReturn(pNewTrans = new GameEntityTransChatMessage(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::VoteGameAdvanceCmd) { svrMemReturn(pNewTrans = new GameEntityTransVoteGameAdvance(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::VoteCmd) { svrMemReturn(pNewTrans = new GameEntityTransVote(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::AdvanceGameCmd) { svrMemReturn(pNewTrans = new GameEntityTransAdvanceGame(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayAgainCmd) { svrMemReturn(pNewTrans = new GameEntityTransGamePlayAgain(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GameRevealPlayerCmd) { svrMemReturn(pNewTrans = new GameEntityTransGameRevealPlayer(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstance::GamePlayerReviveCmd) { svrMemReturn(pNewTrans = new GameEntityTransGamePlayerRevive(pMsgData)); return ResultCode::SUCCESS; } );
	}

	GameInstanceEntity::~GameInstanceEntity()
	{
	}

	// Initialize game system
	Result GameInstanceEntity::InitializeSystem()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(UpdateGameTable() );

		svrChk( AddComponent<GamePlaySystem>( this, false ) );
		svrChk( AddComponent<GameStateSystem>( this, false ) );
		svrChk( AddComponent<GameLogSystem>( this, false ) );
		svrChk( AddComponent<ChattingLogSystem>( this, false ) );

		svrChk( InitializeComponents() );

	Proc_End:

		return hr;
	}

	// Update game config
	Result GameInstanceEntity::UpdateGameTable()
	{
		Result hr = ResultCode::SUCCESS;
		conspiracy::GameConfigTbl::GameConfigItem *pGameConfig = nullptr;

		m_TableVersion = GameTable::GetTableVersion();

		if (!( conspiracy::GameConfigTbl::FindItem(m_PresetGameConfigID, pGameConfig) ))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to find Gameconfig");
			goto Proc_End;
		}

		// set value only if it succeeded
		m_PresetGameConfig = pGameConfig;

		if (!(conspiracy::BotTalkTbl::FindItem(1, m_pBotTalk)))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to find bot talk item");
		}


	Proc_End:

		return hr;
	}

	// Initialize entity to proceed new connection
	Result GameInstanceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::InitializeEntity( newEntityID ) );

		svrChk( InitializeSystem() );

	Proc_End:	

		return hr;
	}

	// Close entity and clear transaction
	Result GameInstanceEntity::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		svrChk(super::TerminateEntity());

		ClearComponents();

	Proc_End:


		return hr;
	}


	////////////////////////////////////////////////////////////
	//
	//	Update processes
	//



	// Update Game status
	Result GameInstanceEntity::UpdateGameStatus( TimeStampMS ulCurTime )
	{
		Result hr = ResultCode::SUCCESS;

		if (m_TableVersion != GameTable::GetTableVersion())
		{
			UpdateGameTable();
		}

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
	Result GameInstanceEntity::InitializeGameEntity(UINT numBot, UINT maxPlayer)
	{
		Result hr = ResultCode::SUCCESS;
		GamePlayer *pPlayer = nullptr;

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
					m_PlayerCharacter[player] = (uint8_t)character;
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

	Result GameInstanceEntity::GetPlayerIndex( PlayerID playerID, UINT &playerIndex )
	{
		GamePlayer* pGamePlayer = nullptr;

		if( !(FindPlayer( playerID, pGamePlayer )) )
			return ResultCode::E_INVALID_PLAYERID;

		playerIndex = pGamePlayer->GetIndex();
		return ResultCode::SUCCESS;
	}

	Result GameInstanceEntity::GetPlayerByIndex( INT playerIndex, GamePlayer* &pGamePlayer )
	{
		if( playerIndex < 0 || playerIndex >= (INT)GetMaxPlayer() )
			return ResultCode::E_INVALID_PLAYERID;

		pGamePlayer = m_PlayerByIndex[playerIndex];

		return pGamePlayer ? ResultCode::SUCCESS : ResultCode::FAIL;
	}


	Result GameInstanceEntity::CreatePlayerInstance(const PlayerInformation& playerInfo, Svr::GameInstancePlayer* &pPlayer)
	{
		pPlayer = new GamePlayer(this, playerInfo);

		return pPlayer != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
	}


	// Register new player to join
	Result GameInstanceEntity::AddPlayerToJoin(Svr::GameInstancePlayer* &pInsPlayer)
	{
		Result hr = ResultCode::SUCCESS;
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
				else                        svrErr(ResultCode::E_GAME_INVALID_ROLE);
				break;
			case PlayerRole::Werewolf:
				if (m_RoleRequestWerewolf < 2) m_RoleRequestWerewolf++;
				else                           svrErr(ResultCode::E_GAME_INVALID_ROLE);
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
	Result GameInstanceEntity::FindPlayer( PlayerID pltID, GamePlayer* &pGamePlayer )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::GameInstancePlayer* pGameInsPlayer = nullptr;

		hr = super::FindPlayer(pltID, pGameInsPlayer);
		if (!hr) return hr;

		pGamePlayer = (GamePlayer*)pGameInsPlayer;

	//Proc_End:

		return hr;
	}


	// Called when a player get out of game
	Result GameInstanceEntity::OnPlayerGetOutOfGame( Svr::GameInstancePlayer* pPlayer )
	{
		Result hr = ResultCode::SUCCESS;

		// update exit status
		svrChk(super::OnPlayerGetOutOfGame(pPlayer));

		svrChk( GetComponent<GamePlaySystem>()->OnPlayerGetOutOfGame((GamePlayer*)pPlayer) );
		svrChk( GetComponent<GameStateSystem>()->OnPlayerGetOutOfGame((GamePlayer*)pPlayer) );

	Proc_End:

		return hr;
	}

}; // ConspiracyGameInstanceServer
}; // namespace BR


