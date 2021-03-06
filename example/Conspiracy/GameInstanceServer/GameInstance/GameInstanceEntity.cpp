////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//
////////////////////////////////////////////////////////////////////////////////



#include "GameInstancePCH.h"
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

#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"

#include "GameInstanceEntity.h"

#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"
#include "GameSystem/GameLogSystem.h"
#include "GameSystem/ChattingLogSystem.h"

#include "ConspiracyGameInstanceSvrConst.h"

#include "GamePlayer.h"
#include "Protocol/ServerService/GameInstanceManagerService.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "conspiracy/BotTalkTbl.h"
#include "TableSystem.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Transaction/GameInstanceTransPlayer.h"





namespace SF {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameInstanceEntity::GameInstanceEntity()
	{
		SetTickInterval(DurationMS(Const::GAMEINSTANCE_TICK_TIME));
		SetEmptyInstanceKillTimeOut(DurationMS(Const::TIME_DELETE_GAMEINSTANCE_NOPLAYER));

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// To game instance

		RegisterMessageHandler<GameEntityTransJoinGame>();
		RegisterMessageHandler<GameEntityTransLeaveGame>();
		RegisterMessageHandler<GameEntityTransKickPlayer>();
		RegisterMessageHandler<GameEntityTransAssignRole>();
		RegisterMessageHandler<GameEntityTransChatMessage>();
		RegisterMessageHandler<GameEntityTransVoteGameAdvance>();
		RegisterMessageHandler<GameEntityTransVote>();
		RegisterMessageHandler<GameEntityTransAdvanceGame>();
		RegisterMessageHandler<GameEntityTransGamePlayAgain>();
		RegisterMessageHandler<GameEntityTransGameRevealPlayer>();
		RegisterMessageHandler<GameEntityTransGamePlayerRevive>();
	}

	GameInstanceEntity::~GameInstanceEntity()
	{
	}

	// Initialize game system
	Result GameInstanceEntity::InitializeSystem()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(UpdateGameTable() );

		svrChk( GetComponentManager().AddComponent<GamePlaySystem>( this, false ) );
		svrChk( GetComponentManager().AddComponent<GameStateSystem>( this, false ) );
		svrChk( GetComponentManager().AddComponent<GameLogSystem>( this, false ) );
		svrChk( GetComponentManager().AddComponent<ChattingLogSystem>( this, false ) );

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
			svrTrace(Error, "Failed to find Gameconfig");
			goto Proc_End;
		}

		// set value only if it succeeded
		m_PresetGameConfig = pGameConfig;

		if (!(conspiracy::BotTalkTbl::FindItem(1, m_pBotTalk)))
		{
			svrTrace(Error, "Failed to find bot talk item");
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

	Proc_End:

		return hr;
	}




	////////////////////////////////////////////////////////////
	//
	//	Game entity
	//

	// Initialize entity to proceed new connection
	Result GameInstanceEntity::InitializeGameEntity(const ServerConfig::NetPublic& netPublic, const VariableTable& attributes)
	{
		Result hr = ResultCode::SUCCESS;

		uint numBot = attributes.GetValue<uint>("NumBot"_crc);
		uint maxPlayer = attributes.GetValue<uint>("MaxPlayer"_crc);

		// initialize
		memset(m_PlayerCharacter, 0xFF, sizeof(m_PlayerCharacter));

		// randomize player character
		for (INT character = 0; character < GameLogItem::LEGACY_MAX_GAMEPLAYER; character++)
		{
			uint player = (uint)Util::Random.Rand() % maxPlayer;
			for (uint iPlayer = 0; iPlayer < maxPlayer; iPlayer++)
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

		if (numBot > maxPlayer)
		{
			// Too many boot number
			svrTrace(Error, "Too many bot number numBot:{0} -> {1}, maxPlayer:{2}", numBot, maxPlayer - 1, maxPlayer);
			numBot = maxPlayer - 1;
		}

		m_NumBot = numBot;

		m_RoleRequestSeer = 0;
		m_RoleRequestWerewolf = 0;

		svrCheck(super::InitializeGameEntity(netPublic, attributes));


		// add fake bot player
		for (uint iBot = 0; iBot < m_NumBot; iBot++)
		{
			SFUniquePtr<Svr::GameInstancePlayer> pPlayer;
			PlayerInformation playerInfo;
			playerInfo.PlayerID = iBot + 1;
			playerInfo.Level = 1;
			StrUtil::Format(playerInfo.NickName, "Bot{0}", iBot);
			//svrCheck(CreatePlayerInstance(playerInfo, pPlayer));
			pPlayer->SetIsBot(true);
			//svrCheck(AddPlayerToJoin(pPlayer));
		}

		return hr;
	}




	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game Player
	//

	Result GameInstanceEntity::GetPlayerIndex( PlayerID playerID, uint &playerIndex )
	{
		GamePlayer* pGamePlayer = nullptr;

		if( !(FindPlayer( playerID, pGamePlayer )) )
			return ResultCode::INVALID_PLAYERID;

		playerIndex = pGamePlayer->GetIndex();
		return ResultCode::SUCCESS;
	}

	Result GameInstanceEntity::GetPlayerByIndex( INT playerIndex, GamePlayer* &pGamePlayer )
	{
		if( playerIndex < 0 || playerIndex >= (INT)GetMaxPlayer() )
			return ResultCode::INVALID_PLAYERID;

		pGamePlayer = m_PlayerByIndex[playerIndex];

		return pGamePlayer ? ResultCode::SUCCESS : ResultCode::FAIL;
	}


	//Result GameInstanceEntity::CreatePlayerInstance(const PlayerInformation& playerInfo, SFUniquePtr<Svr::GameInstancePlayer>& pPlayer)
	//{
	//	pPlayer.reset(new(GetHeap()) GamePlayer(this, playerInfo));

	//	return pPlayer != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
	//}


	//// Register new player to join
	//Result GameInstanceEntity::AddPlayerToJoin(SFUniquePtr<Svr::GameInstancePlayer>& pInsPlayer)
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	//GamePlayer* pFound = nullptr;
	//	GamePlayer* pPlayer = nullptr;
	//	uint playerIndex;


	//	pPlayer = (GamePlayer*)pInsPlayer.get();

	//	svrChk(super::AddPlayerToJoin(pInsPlayer));
	//	pInsPlayer = nullptr;


	//	// Check role availability
	//	if (pPlayer->GetRequestedRole() != PlayerRole::None)
	//	{
	//		switch (pPlayer->GetRequestedRole())
	//		{
	//		case PlayerRole::Seer:
	//			if (m_RoleRequestSeer == 0) m_RoleRequestSeer++;
	//			else                        svrErr(ResultCode::GAME_INVALID_ROLE);
	//			break;
	//		case PlayerRole::Werewolf:
	//			if (m_RoleRequestWerewolf < 2) m_RoleRequestWerewolf++;
	//			else                           svrErr(ResultCode::GAME_INVALID_ROLE);
	//			break;
	//		default:
	//			break;
	//		}
	//	}


	//	// search empty player index from start
	//	playerIndex = (uint)Util::Random.Rand() % GetMaxPlayer();
	//	for( uint search = 0; search < GetMaxPlayer(); search++ )
	//	{
	//		if( m_PlayerByIndex[playerIndex] == nullptr )
	//		{
	//			m_PlayerByIndex[playerIndex] = pPlayer;
	//			pPlayer->SetIndex(playerIndex);
	//			break;
	//		}
	//		playerIndex++;
	//		if( playerIndex >= GetMaxPlayer() ) playerIndex = 0;
	//	}

	//	pPlayer->SetPlayerState( PlayerState::Playing );
	//	pPlayer->SetCharacter( m_PlayerCharacter[playerIndex] );


	//Proc_End:


	//	return hr;
	//}



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

		svrChk(GetComponentManager().GetComponent<GamePlaySystem>()->OnPlayerGetOutOfGame((GamePlayer*)pPlayer));
		svrChk(GetComponentManager().GetComponent<GameStateSystem>()->OnPlayerGetOutOfGame((GamePlayer*)pPlayer));

	Proc_End:

		return hr;
	}

}; // ConspiracyGameInstanceServer
}; // namespace SF


