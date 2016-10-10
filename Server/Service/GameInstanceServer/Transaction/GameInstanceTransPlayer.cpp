////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "GameServer.h"
#include "ConspiracyGameInstanceServerClass.h"

#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Common/BrRandom.h"
#include "Common/GameConst.h"

#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"


#include "GameInstanceTransPlayer.h"
#include "GameInstance/GamePlayer.h"

#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"
#include "GameSystem/ChattingLogSystem.h"
#include "GameSystem/GameLogSystem.h"

#include "ServerSystem/EntityManager.h"



BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransDeleteGame);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransJoinGame);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransLeaveGame);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransKickPlayer);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransAssignRole);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransChatMessage);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransVoteGameAdvance);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransVote);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransAdvanceGame);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransGamePlayAgain);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransGameRevealPlayer);
BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameEntityTransGamePlayerRevive);


namespace BR {
namespace ConspiracyGameInstanceServer {



	GameEntityTransDeleteGame::GameEntityTransDeleteGame(Message::MessageData* &pIMsg)
		: ServerEntityMessageTransaction(pIMsg)
	{
		SetWorkOnServerEntity(false);
		SetExclusive(true);
	}

	// Start Transaction
	Result GameEntityTransDeleteGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		
		svrChk(super::StartTransaction());

		GetMyOwner()->CloseGameInstance();

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	// Start Transaction
	Result GameEntityTransJoinGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePlayer *pMyPlayer = nullptr;
		Policy::ISvrPolicyGameInstance *pMyPolicy = nullptr;
		Svr::GameInstancePlayer* pNewInsPlayer = nullptr;
		auto pGameStateSystem = GetMyOwner()->GetComponent<GameStateSystem>();

		svrChk( super::StartTransaction() );

		m_Day = 0;
		m_GameInsUID = GameInsUID( GetMyOwner()->GetEntityUID() );
		m_TimeStamp = pGameStateSystem->GetCurrentGamePlayState()->GetStateTimeUTC();
		m_GameState = pGameStateSystem->GetCurrentGameState();
		m_Day = pGameStateSystem->GetCurrentDay();
		m_PlayerIndex = 0;
		m_Dead = false;
		m_bIsFirstJoin = false;

		if( FAILED(GetMyOwner()->FindPlayer( GetPlayer().PlayerID, pMyPlayer )) )
		{
			svrChk(GetMyOwner()->CreatePlayerInstance(GetPlayer(), pNewInsPlayer));
			pMyPlayer =  dynamic_cast<GamePlayer*>(pNewInsPlayer);
			svrChk(pMyPlayer->InitializePlayer( GetMyOwner() ) );
			pMyPlayer->SetRequestedRole(GetRequestedRole());

			svrChk( GetMyOwner()->AddPlayerToJoin(pNewInsPlayer) );
			pNewInsPlayer = nullptr;
			svrTrace(Svr::TRC_INFO, "GameUID:{0} Join player {1}, NumPlayer:{2}, ", GetMyOwner()->GetEntityUID(), GetPlayer().PlayerID, GetMyOwner()->GetNumPlayer());

			m_bIsFirstJoin = true;
		}
		else
		{
			svrTrace(Svr::TRC_INFO, "GameUID:{0} Rejoin player {1}, NumPlayer:{2}, ", GetMyOwner()->GetEntityUID(), GetPlayer().PlayerID, GetMyOwner()->GetNumPlayer());

			// Player ID should be bigger than 10, 1~10 is used by bot
			Assert(GetPlayer().PlayerID > 10);

			// Just update name and ticket
			pMyPlayer->SetPlayerName(GetPlayer().NickName);
		}

		m_PlayerIndex = pMyPlayer->GetIndex();

		svrChk( pMyPlayer->SetServerEntity( GetServerEntity<Svr::ServerEntity>(), GetRouteContext().GetFrom()) );

		m_Role = pMyPlayer->GetRole();
		m_Dead = pMyPlayer->GetPlayerState() != PlayerState::Playing;
		m_PlayerCharacter = pMyPlayer->GetCharacter();

		svrChk( GetMyOwner()->GetComponent<ChattingLogSystem>()->GetGameLogBinary( m_ChatHistoryBuffer, 0, GameConst::MAX_CHATSYNC ) );
		svrChk( GetMyOwner()->GetComponent<GameLogSystem>()->GetGameLogBinary( m_GameLogBuffer, 0, GameConst::MAX_GAMELOG ) );

		// Earlier close so that the player can get JoinGameRes first
		CloseTransaction( hr );

		pMyPolicy = pMyPlayer->GetPolicy<Policy::ISvrPolicyGameInstance>();
		// Send all other player to me
		if (pMyPolicy != nullptr)
		{
			GetMyOwner()->ForeachPlayer([&](GamePlayer* pPlayer)->Result {

				PlayerRole otherRole = GetMyOwner()->GetComponent<GamePlaySystem>()->GetRevealedRole(pMyPlayer, pPlayer);
				pMyPolicy->PlayerJoinedS2CEvt(RouteContext(GetOwnerEntityUID(), pMyPlayer->GetPlayerEntityUID()), pPlayer->GetPlayerInformation(), otherRole, pPlayer->GetPlayerState() != PlayerState::Playing, pPlayer->GetIndex(), pPlayer->GetCharacter());

				return ResultCode::SUCCESS;
			});
		}

		// Send my info to others
		GetMyOwner()->ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->Result {
			if( pMyPlayer != pPlayer )
			{
				PlayerRole myRoleToOther = GetMyOwner()->GetComponent<GamePlaySystem>()->GetRevealedRole( pPlayer, pMyPlayer );
				pPolicy->PlayerJoinedS2CEvt( RouteContext( GetOwnerEntityUID(), pPlayer->GetPlayerEntityUID()), pMyPlayer->GetPlayerInformation(), myRoleToOther, pMyPlayer->GetPlayerState() != PlayerState::Playing, pMyPlayer->GetIndex(), pMyPlayer->GetCharacter()  );
			}
			return ResultCode::SUCCESS;
		});


		// Role assign when the last player is joined
		if( m_bIsFirstJoin && GetMyOwner()->GetNumPlayer() == GetMyOwner()->GetMaxPlayer() )
		{
			svrChk( GetMyOwner()->InitializeSystem() );

			svrChk( GetMyOwner()->GetComponent<GamePlaySystem>()->AssignRole() );
		
			svrChk(pGameStateSystem->OnStartGame());
		}


	Proc_End:

		Util::SafeDelete(pNewInsPlayer);

		CloseTransaction( hr );

		return hr;
	}
	
	

	// Start Transaction
	Result GameEntityTransLeaveGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->LeavePlayer( GetPlayerID() ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	

	// Start Transaction
	Result GameEntityTransKickPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetPlayerToKick() == (PlayerID)(-1) )
		{
			svrChk( GetMyOwner()->ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->Result {
				pPolicy->PlayerKickedS2CEvt( RouteContext( GetOwnerEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayer->GetPlayerID()  );
				return GetMyOwner()->LeavePlayer( pPlayer->GetPlayerID() );
			}) );
		}
		else
		{
			Policy::ISvrPolicyGameInstance *pPolicy = nullptr;
			GamePlayer *pPlayerToKick = nullptr;

			svrChk( GetMyOwner()->FindPlayer( GetPlayerToKick(), pPlayerToKick ) );
			pPolicy = pPlayerToKick->GetPolicy<Policy::ISvrPolicyGameInstance>();
			if (pPolicy != nullptr)
				pPolicy->PlayerKickedS2CEvt( RouteContext( GetOwnerEntityUID(), pPlayerToKick->GetPlayerEntityUID()), pPlayerToKick->GetPlayerID()  );
			svrChk( GetMyOwner()->LeavePlayer( pPlayerToKick->GetPlayerID() ) );
		}

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	

	// Start Transaction
	Result GameEntityTransAssignRole::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		// NOTE: for now, restart the game
		svrChk( GetMyOwner()->InitializeSystem() );

		svrChk( GetMyOwner()->GetComponent<GamePlaySystem>()->AssignRole() );
		
		svrChk( GetMyOwner()->GetComponent<GameStateSystem>()->OnStartGame() );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	

	// Start Transaction
	Result GameEntityTransChatMessage::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		GamePlayer *pMyPlayer = nullptr;
		//const char* message = nullptr;
		//ChatType charType;
		//GameStateID gameState;
		//bool bIsGhost;

		svrChk( super::StartTransaction() );

		svrChk( GetMyPlayer( pMyPlayer ) );

		hr = GetMyOwner()->GetComponent<GamePlaySystem>()->BroadCastChatMessage(pMyPlayer, GetRole(), GetChatMessage());

		//if( GetRole() != PlayerRole::None && GetRole() != PlayerRole::Werewolf  )
		//	svrErr(ResultCode::E_GAME_INVALID_ROLE);

		//gameState = GetMyOwner()->GetComponent<GameStateSystem>()->GetCurrentGameState();
		//bIsGhost = gameState != GameStateID::None && gameState != GameStateID::End && pMyPlayer->GetPlayerState() == PlayerState::Ghost;

		//// prevent chatting during night
		//switch( gameState )
		//{
		////case GameStateID::FirstNightVote:
		////case GameStateID::SecondNightVote:
		//case GameStateID::NightVote:
		//	if( !bIsGhost && GetRole() != PlayerRole::Werewolf )
		//		svrErrClose(ResultCode::E_GAME_INVALID_GAMESTATE);
		//	break;
		//default:
		//	break;
		//};

		//charType = GetRole() != PlayerRole::None ? ChatType::Role : ChatType::Normal;
		//svrChk( GetMyOwner()->GetComponent<ChattingLogSystem>()->AddChattingLog( Util::Time.GetTimeUTCSec(), pMyPlayer->GetPlayerID(), pMyPlayer->GetPlayerState() == PlayerState::Ghost, charType, GetChatMessage() ) );

		//GetMyOwner()->ForeachPlayerGameServer( [&]( GamePlayer* pPlayer, Policy::IPolicyGameServer *pPolicy )->Result {
		//	if( GetRole() == PlayerRole::None || GetRole() == pPlayer->GetRole() )
		//	{
		//		message = GetChatMessage();
		//		if( bIsGhost && pPlayer->GetPlayerState() != PlayerState::Ghost )
		//		{
		//			// Skip broadcast from ghost to others
		//			return ResultCode::SUCCESS;
		//		}
		//		pPolicy->ChatMessageC2SEvt( RouteContext( GetOwnerEntityUID(), pPlayer->GetPlayerEntityUID()), pMyPlayer->GetPlayerID(), GetRole(), pMyPlayer->GetPlayerName(), message );
		//	}
		//	return ResultCode::SUCCESS;
		//});

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}

	

	// Start Transaction
	Result GameEntityTransVoteGameAdvance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePlayer *pMyPlayer = nullptr;

		svrChk( super::StartTransaction() );

		svrChk( GetMyPlayer( pMyPlayer ) );

		svrChkClose( GetMyOwner()->GetComponent<GameStateSystem>()->VoteGameAdvance( pMyPlayer ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	

	// Start Transaction
	Result GameEntityTransVote::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePlayer *pMyPlayer = nullptr;
		GamePlayer *pVoteTarget = nullptr;

		if( GetVoteTarget() == 0 )
			svrErrClose(ResultCode::E_INVALID_PLAYERID);

		svrChk( super::StartTransaction() );

		svrChk( GetMyPlayer( pMyPlayer ) );
		svrChkClose( GetMyOwner()->FindPlayer( GetVoteTarget(), pVoteTarget ) );

		svrChkClose( GetMyOwner()->GetComponent<GameStateSystem>()->Vote( pMyPlayer, pVoteTarget ) );

	Proc_End:

		CloseTransaction( hr );

		return hr;
	}
	
	// Start Transaction
	Result GameEntityTransAdvanceGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChkClose( GetMyOwner()->GetComponent<GameStateSystem>()->AdvanceState() );

	Proc_End:

		CloseTransaction( hr );

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result GameEntityTransGamePlayAgain::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		EntityUID ownerUID = GetOwnerEntityUID();

		m_MemberCount = 0;

		svrChk(super::StartTransaction());

		GetMyOwner()->ForeachPlayerSvrGameInstance([&](GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy)->Result
		{
			if (pPlayer->GetPlayerEntityUID() == 0)
				return ResultCode::SUCCESS;

			m_MemberCount++;

			pPolicy->GamePlayAgainS2CEvt(RouteContext(ownerUID, pPlayer->GetPlayerEntityUID()), pPlayer->GetPlayerID(), GetPartyUID(), GetLeadPlayer());
			return ResultCode::SUCCESS;
		});

		GetMyOwner()->CloseGameInstance();

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}





	// Start Transaction
	Result GameEntityTransGameRevealPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePlayer* pMyPlayer = nullptr;
		GamePlayer* pTargetPlayer = nullptr;
		size_t numReveal;

		svrChk(super::StartTransaction());

		svrChk(GetMyPlayer(pMyPlayer));

		if (GetMyOwner()->GetComponent<GameStateSystem>()->GetCurrentGameState() != GameStateID::MorningDebate)
			svrErrClose(ResultCode::E_GAME_INVALID_GAMESTATE);

		if (pMyPlayer->GetPlayerState() != PlayerState::Ghost)
			svrErrClose(ResultCode::E_GAME_INVALID_PLAYER_STATE);

		numReveal = std::min((size_t)GameConst::MAX_PLAYER_REVEAL, GetTargetPlayerID().GetSize());
		for (UINT iTargetPlayer = 0; iTargetPlayer < numReveal; iTargetPlayer++)
		{
			svrChk(GetMyOwner()->FindPlayer(GetTargetPlayerID()[iTargetPlayer], pTargetPlayer));

			m_RevealedPlayerID.push_back(pTargetPlayer->GetPlayerID());
			m_RevealedPlayerRole.push_back(pTargetPlayer->GetRole());
		}


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}




	// Start Transaction
	Result GameEntityTransGamePlayerRevive::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		GamePlayer* pMyPlayer = nullptr;

		svrChk(super::StartTransaction());

		svrChk(GetMyPlayer(pMyPlayer));

		if (pMyPlayer->GetReviveCount() > 0)
			svrErrClose(ResultCode::E_GAME_MAX_TRY);

		if (GetMyOwner()->GetComponent<GameStateSystem>()->GetCurrentGameState() != GameStateID::MorningDebate)
			svrErrClose(ResultCode::E_GAME_INVALID_GAMESTATE);

		if (pMyPlayer->GetPlayerState() != PlayerState::Ghost)
			svrErrClose(ResultCode::E_GAME_INVALID_PLAYER_STATE);

		svrChkClose(GetMyOwner()->GetComponent<GamePlaySystem>()->RevivePlayer(pMyPlayer));

		pMyPlayer->SetReviveCount(pMyPlayer->GetReviveCount() + 1);

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}




};// namespace ConspiracyGameInstanceServer 
};// namespace BR 

