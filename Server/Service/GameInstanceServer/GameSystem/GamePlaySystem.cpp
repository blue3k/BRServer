////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////




#include "stdafx.h"

#include "Util/TimeUtil.h"
#include "Common/BrRandom.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeGame.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"

#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "ConspiracyGameInstanceServerClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"
#include "GameSystem/GameLogSystem.h"
#include "GameSystem/ChattingLogSystem.h"
#include "GameInstance/GameInstanceEntity.h"


SF_MEMORYPOOL_IMPLEMENT(BR::ConspiracyGameInstanceServer::GamePlaySystem);


namespace SF {
namespace ConspiracyGameInstanceServer {


	

	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	const PlayerRole GamePlaySystem::stm_PlayerRoleByCount[GameConst::MAX_GAMEPLAYER] = 
	{
		PlayerRole::Villager,
		PlayerRole::Villager,
		PlayerRole::Werewolf,
		PlayerRole::Seer,
		PlayerRole::Villager,
		PlayerRole::Villager,
		PlayerRole::Werewolf,
		PlayerRole::Villager,
		//PlayerRole::Medium,
		//PlayerRole::Possessed,
		//PlayerRole::Bodyguard,
		//PlayerRole::Owlman,
	};



	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	// Constructor 
	GamePlaySystem::GamePlaySystem( GameInstanceEntity* pEntity )
		: GameSystem(pEntity)
		, m_Werewolves(pEntity->GetMemoryManager())
	{
	}

	GamePlaySystem::~GamePlaySystem()
	{
	}
	

	// Initialzie system
	Result GamePlaySystem::InitializeComponent()
	{
		Result hr = ResultCode::SUCCESS;

		m_NumVillager = 0;
		m_NumWereWolf = 0;
		m_HuntedPlayer = 0;

		memset(m_Suspects,0,sizeof(m_Suspects) );

		m_LynchedRole = PlayerRole::None;
		m_LynchedPlayer = 0;

		SetSeer(0);

		SetBodyGuard( 0 );

		SetOwlman( 0 );
		SetOwlmansChoice( 0 );

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) ->Result {
			pPlayer->SetRevealedBySeer( false );
			pPlayer->SetPlayerState( PlayerState::Playing );
			pPlayer->SetReviveCount(0);
			return ResultCode::SUCCESS;
		});

		svrChk( super::InitializeComponent() );

	Proc_End:

		return hr;
	}

	Result GamePlaySystem::PlayerRoleAssigned(GamePlayer* pPlayer)
	{
		Result hr = ResultCode::SUCCESS;

		switch (pPlayer->GetRole())
		{
		case PlayerRole::Seer:
			SetSeer(pPlayer->GetPlayerID());
			break;
			//case PlayerRole::Bodyguard:
			//	SetBodyGuard( pPlayer->GetPlayerID() );
			//	break;
		case PlayerRole::Werewolf:
			if (m_Werewolves.GetSize() > MAX_WEREWOLF)
			{
				Assert(!"Too many werewolfs are assigned This mustn't be happened");
				pPlayer->SetRole(PlayerRole::Villager);
				break;
			}
			m_Werewolves.push_back(pPlayer);
			break;
		default:
			break;
		};


	//Proc_End:

		return hr;
	}
	
	Result GamePlaySystem::AssignRole()
	{
		Result hr = ResultCode::SUCCESS;

		bool ShufflingBuffer[GameConst::MAX_GAMEPLAYER];
		size_t numPlayer = GetOwner().GetNumPlayer();

		m_Werewolves.Clear();

		memset( ShufflingBuffer, 0, sizeof(ShufflingBuffer) );

		// assign requested player first
		GetOwner().ForeachPlayer([&](GamePlayer* pPlayer) ->Result {

			pPlayer->SetRole(PlayerRole::None);

			if (pPlayer->GetRequestedRole() == PlayerRole::None)
			{
				return ResultCode::SUCCESS;
			}

			// Find empty slot
			int iSlot = 0;
			for (; iSlot < GameConst::MAX_GAMEPLAYER; iSlot++)
			{
				if (pPlayer->GetRequestedRole() == GamePlaySystem::stm_PlayerRoleByCount[iSlot])
					break;
			}

			// skip errnous player
			if (iSlot >= GameConst::MAX_GAMEPLAYER)
			{
				svrTrace(Trace::TRC_WARN, "Failed to find requested role, player:{0}, requested:{1}", pPlayer->GetPlayerID(), (uint)pPlayer->GetRequestedRole());
				return ResultCode::SUCCESS;
			}

			ShufflingBuffer[iSlot] = true;
			pPlayer->SetRole(pPlayer->GetRequestedRole());

			Assert(pPlayer->GetRole() != PlayerRole::None);

			PlayerRoleAssigned(pPlayer);

			return ResultCode::SUCCESS;
		});


		// assign remain
		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) ->Result {

			if (pPlayer->GetRole() != PlayerRole::None)
				return ResultCode::SUCCESS;

			int iRandom = (int)(Util::Random.Rand() % numPlayer);
			for( int count = 0; count < GameConst::MAX_GAMEPLAYER; count++, iRandom = (int)((iRandom+1)%numPlayer) )
			{
				if( !ShufflingBuffer[iRandom] )
				{
					ShufflingBuffer[iRandom] = true;
					pPlayer->SetRole(GamePlaySystem::stm_PlayerRoleByCount[iRandom]);
					break;
				}
			}
			Assert(pPlayer->GetRole()!=PlayerRole::None);

			PlayerRoleAssigned(pPlayer);

			return ResultCode::SUCCESS;
		});


		GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance *pPolicy )->Result {
			if( pPlayer->GetPlayerEntityUID() != 0 )
				pPolicy->RoleAssignedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayer->GetRole() );
			return ResultCode::SUCCESS;
		});


		// notify other's role
		for( uint iwolf = 0; iwolf < m_Werewolves.GetSize(); iwolf++ )
		{
			for( uint iother = 0; iother < m_Werewolves.GetSize(); iother++ )
			{
				if( iwolf == iother ) continue;

				if( m_Werewolves[iwolf] != nullptr && m_Werewolves[iother] != nullptr )
				{
					auto pPolicy = m_Werewolves[iwolf]->GetInterface<Policy::NetSvrPolicyGameInstance>();
					if (m_Werewolves[iwolf]->GetPlayerEntityUID() != 0 && pPolicy != nullptr)
					{
						pPolicy->PlayerRevealedS2CEvt(RouteContext(GetOwner().GetEntityUID(), m_Werewolves[iwolf]->GetPlayerEntityUID()), m_Werewolves[iother]->GetPlayerID(), m_Werewolves[iother]->GetRole(), PlayerRevealedReason::Werewolf);
					}
				}
			}
		}

	//Proc_End:

		return hr;
	}

	// Set suspect list
	void GamePlaySystem::SetSuspect( PlayerID suspect1, PlayerID suspect2 )
	{
		m_Suspects[0] = suspect1;
		m_Suspects[1] = suspect2;
	}


	// Update Villager and werewolf count
	Result GamePlaySystem::UpdateVillagerNWerewolfCount()
	{
		m_NumVillager = 0;
		m_NumWereWolf = 0;

		Result hr = ResultCode::SUCCESS;
		GetOwner().ForeachPlayer( [&]( GamePlayer* pPlayer )->Result
		{
			if( pPlayer->GetPlayerState() != PlayerState::Playing )
				return ResultCode::SUCCESS;

			switch( pPlayer->GetRole() )
			{
			case PlayerRole::None:
				return ResultCode::E_GAME_INVALID_ROLE;
				break;
			case PlayerRole::Villager:
			case PlayerRole::Seer:
			case PlayerRole::Medium:
			//case PlayerRole::Bodyguard:
			//case PlayerRole::Owlman:
			//case PlayerRole::Freemason:
				m_NumVillager++;
				break;

			case PlayerRole::Werewolf:
			//case PlayerRole::Possessed:
				m_NumWereWolf++;
				break;

			//case PlayerRole::Werehamster:
			//case PlayerRole::Mythomaniac:
			//case PlayerRole::Hunter:
			//case PlayerRole::Wizard:
			//case PlayerRole::Sorceress:
			//case PlayerRole::Prostitute:
			//case PlayerRole::Chosen:
			//case PlayerRole::AngelOfVillagers:
			//case PlayerRole::AngelOfWolves:
			//case PlayerRole::Paladin:
			//case PlayerRole::Sorcerer:
			//case PlayerRole::Condemned:
			//case PlayerRole::Redeemed:
			//case PlayerRole::Devil:
			default:
				// I Have no idea for these now
				hr = ResultCode::E_GAME_INVALID_ROLE;
				return hr;
			};
			return ResultCode::SUCCESS;
		});

		return hr;
	}

	// Check game end condition
	GameWinner GamePlaySystem::GetGameWinner()
	{
		if( m_NumWereWolf == 0 || m_NumWereWolf == m_NumVillager )
		{
			// update count for safety
			UpdateVillagerNWerewolfCount();
			if( m_NumWereWolf == 0 )
				return GameWinner::Villager;
			else if( m_NumWereWolf == m_NumVillager )
				return GameWinner::Werewolf;
		}

		return GameWinner::None;
	}

	Result GamePlaySystem::UpdateGameEnd()
	{
		Result hr = ResultCode::SUCCESS;

		GameWinner winner = GetGameWinner();
		if( winner == GameWinner::None )
			return hr;

		GetOwner().GetComponent<GameStateSystem>()->SetGameEnd(winner);

	//Proc_End:

		return hr;
	}


	PlayerID GamePlaySystem::PickAnyoneCloseToHuntedPlayer( PlayerID exculdePlayer )
	{
		GamePlayer *pGamePlayer = nullptr;

		if( !(GetOwner().FindPlayer(GetHuntedPlayer(), pGamePlayer)) )
		{
			if( !(GetOwner().FindPlayer(exculdePlayer, pGamePlayer)) )
			{
				// pick random
				svrTrace( Trace::TRC_ERROR, "Invalid hunted player: PlayerID:{0}", GetHuntedPlayer() );
				GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer ) -> Result
				{
					if( pGamePlayer != nullptr )
						return ResultCode::FAIL;

					if( pPlayer != nullptr && pPlayer->IsInGame() && pPlayer->GetPlayerState() != PlayerState::Ghost )
					{
						pGamePlayer = pPlayer;
					}

					return ResultCode::SUCCESS;
				});
			}
		}


		int playerIndexToCheck = pGamePlayer->GetIndex();
		GamePlayer* pCheckPlayer = nullptr;
		for( int iPlayer = 1; iPlayer < (int)(GetOwner().GetMaxPlayer()>>1); iPlayer++ )
		{
			// search right
			int checkIndex = (playerIndexToCheck + iPlayer) % GetOwner().GetMaxPlayer();
			if( (GetOwner().GetPlayerByIndex(checkIndex, pCheckPlayer))
				&& pCheckPlayer != nullptr && pCheckPlayer->IsInGame() && pCheckPlayer->GetPlayerState() != PlayerState::Ghost
				&& pCheckPlayer->GetPlayerID() != exculdePlayer )
			{
				return pCheckPlayer->GetPlayerID();
			}

			// search left
			checkIndex = (playerIndexToCheck - iPlayer) % GetOwner().GetMaxPlayer();
			if( (GetOwner().GetPlayerByIndex(checkIndex, pCheckPlayer))
				&& pCheckPlayer != nullptr && pCheckPlayer->IsInGame() && pCheckPlayer->GetPlayerState() != PlayerState::Ghost
				&& pCheckPlayer->GetPlayerID() != exculdePlayer )
			{
				return pCheckPlayer->GetPlayerID();
			}
		}


		svrTrace( Trace::TRC_ERROR, "Failed to find player PickAnyoneCloseToHuntedPlayer Hunted PlayerID:{0}", GetHuntedPlayer() );
		return 0;
	}


	// Kill Player
	Result GamePlaySystem::KillPlayer( PlayerID playerToKill, PlayerKilledReason reason )
	{
		Result hr = ResultCode::SUCCESS;
		GamePlayer *pPlayer = nullptr;


		svrChk( GetOwner().FindPlayer( playerToKill, pPlayer ) );

		svrChk( KillPlayer( pPlayer, reason ) );

	Proc_End:

		if( !(hr) )
			svrTrace( Svr::TRC_GAME1, "Failed to find player KillPlayer PlayerID:{0}, reason:{1}", playerToKill, (int)reason );

		return hr;
	}

	Result GamePlaySystem::KillPlayer( GamePlayer* pPlayerToKill, PlayerKilledReason reason )
	{
		Result hr = ResultCode::SUCCESS;

		if( pPlayerToKill == nullptr )
			return ResultCode::INVALID_ARG;

		if( pPlayerToKill->GetPlayerState() == PlayerState::Ghost )
		{
			hr = ResultCode::E_GAME_INVALID_PLAYER_STATE;
			goto Proc_End;
		}

		switch( reason )
		{
		case PlayerKilledReason::ByHanging:
			// Update lynched player to send it to the medium later
			m_LynchedPlayer = pPlayerToKill->GetPlayerID();
			m_LynchedRole = pPlayerToKill->GetRole();
			break;
		default:
			break;
		}

		pPlayerToKill->SetPlayerState( PlayerState::Ghost );

		switch( pPlayerToKill->GetRole() )
		{
		case PlayerRole::None:
			return ResultCode::E_GAME_INVALID_ROLE;
			break;
		case PlayerRole::Villager:
		case PlayerRole::Seer:
		case PlayerRole::Medium:
		//case PlayerRole::Possessed:
		//case PlayerRole::Bodyguard:
		//case PlayerRole::Owlman:
		//case PlayerRole::Freemason:
			m_NumVillager--;
			break;
			
		case PlayerRole::Werewolf:
			m_NumWereWolf--;
			break;
		default:
			svrErr(ResultCode::E_GAME_INVALID_ROLE);
			break;
		}

		GetOwner().OnPlayerGetOutOfGame( pPlayerToKill );

		GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance *pPolicy )->Result {
			if( pPlayer->GetPlayerEntityUID() != 0 )
				pPolicy->PlayerKilledS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayerToKill->GetPlayerID(), reason);
			return ResultCode::SUCCESS;
		});

		//svrChk( UpdateGameEnd() );

	Proc_End:

		if( !(hr) )
		{
			svrTrace(Svr::TRC_GAME1, "Failed to kill a player PlayerID:{0}, reason:{1}, Result:{2:X8}", pPlayerToKill->GetPlayerID(), (int)reason, hr );
		}
		else
		{
			svrTrace(Svr::TRC_GAME1, "Player PlayerID:{0} is killed by reason:{1}", pPlayerToKill->GetPlayerID(), (int)reason );
		}

		return hr;
	}


	Result GamePlaySystem::RevivePlayer(GamePlayer* pPlayerToRevive)
	{
		Result hr = ResultCode::SUCCESS;
		if (pPlayerToRevive == nullptr)
			return ResultCode::INVALID_ARG;

		if (pPlayerToRevive->GetPlayerState() != PlayerState::Ghost)
		{
			hr = ResultCode::E_GAME_INVALID_PLAYER_STATE;
			goto Proc_End;
		}

		pPlayerToRevive->SetPlayerState(PlayerState::Playing);

		switch (pPlayerToRevive->GetRole())
		{
		case PlayerRole::None:
			return ResultCode::E_GAME_INVALID_ROLE;
			break;
		case PlayerRole::Villager:
		case PlayerRole::Seer:
		case PlayerRole::Medium:
			//case PlayerRole::Possessed:
			//case PlayerRole::Bodyguard:
			//case PlayerRole::Owlman:
			//case PlayerRole::Freemason:
			m_NumVillager++;
			break;

		case PlayerRole::Werewolf:
			m_NumWereWolf++;
			break;
		default:
			svrErr(ResultCode::E_GAME_INVALID_ROLE);
			break;
		}

		//GetOwner().OnPlayerGetOutOfGame(pPlayerToRevive);
		switch (pPlayerToRevive->GetRole())
		{
		case PlayerRole::Seer:
			SetSeer(1);
			break;
		default:
			break;
		};


		GetOwner().ForeachPlayerSvrGameInstance([&](GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance *pPolicy)->Result {
			if (pPlayer->GetPlayerEntityUID() != 0)
				pPolicy->GamePlayerRevivedS2CEvt(RouteContext(GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayerToRevive->GetPlayerID());
			return ResultCode::SUCCESS;
		});


	Proc_End:

		if (!(hr))
		{
			svrTrace(Svr::TRC_GAME1, "Failed to revive a player PlayerID:{0}, Result:{1:X8}", pPlayerToRevive->GetPlayerID(), hr);
		}
		else
		{
			svrTrace(Svr::TRC_GAME1, "Player PlayerID:{0} is revived", pPlayerToRevive->GetPlayerID());
		}

		return hr;
	}


	// Called when a player get out of game
	Result GamePlaySystem::OnPlayerGetOutOfGame( GamePlayer* pPlayer )
	{
		// remove flags so that the role effect isn't affected
		switch( pPlayer->GetRole() )
		{
		case PlayerRole::Seer:
			SetSeer( 0 );
			break;
		//case PlayerRole::Bodyguard:
		//	SetBodyGuard( 0 );
		//	break;
		//case PlayerRole::Owlman:
		//	SetOwlman( 0 );
		//	SetOwlmansChoice( 0 );
		//	break;
		default:
			break;
		};

		return ResultCode::SUCCESS;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	// Get revealed role
	PlayerRole GamePlaySystem::GetRevealedRole( GamePlayer* pPlayer, GamePlayer* pOtherPlayer )
	{
		if( pPlayer == nullptr || pOtherPlayer == nullptr )
			return PlayerRole::None;

		// Give other player role regardless the player's role if 
		if( pPlayer == pOtherPlayer // Same player
			|| GetOwner().GetComponent<GameStateSystem>()->GetCurrentGameState() == GameStateID::End ) // Game end
			return pOtherPlayer->GetRole();

		PlayerRole otherRole = PlayerRole::None;
		switch( pPlayer->GetRole() )
		{
		case PlayerRole::Werewolf:
			if( pOtherPlayer->GetRole() == PlayerRole::Werewolf )
				otherRole = PlayerRole::Werewolf;
			break;
		case PlayerRole::Seer:
			if( pOtherPlayer->GetRevealedBySeer() )
				otherRole = pOtherPlayer->GetRole();
			break;
		default:
			break;
		}

		return otherRole;
	}

	Result GamePlaySystem::BroadCastChatMessage(GamePlayer *pMyPlayer, PlayerRole role, const char* message)
	{
		Result hr = ResultCode::SUCCESS;
		ChatType charType;
		GameStateID gameState;
		bool bIsGhost;
		auto ownerEntityUID = GetOwner().GetEntityUID();


		if (role != PlayerRole::None && role != PlayerRole::Werewolf)
			svrErr(ResultCode::E_GAME_INVALID_ROLE);

		gameState = GetOwner().GetComponent<GameStateSystem>()->GetCurrentGameState();
		bIsGhost = gameState != GameStateID::None && gameState != GameStateID::End && pMyPlayer->GetPlayerState() == PlayerState::Ghost;

		// prevent chatting during night
		switch (gameState)
		{
		//case GameStateID::FirstNightVote:
		//case GameStateID::SecondNightVote:
		case GameStateID::NightVote:
			if (!bIsGhost && role != PlayerRole::Werewolf)
			{
				hr = ResultCode::E_GAME_INVALID_GAMESTATE;
				goto Proc_End;
			}
			break;
		default:
			break;
		};

		charType = role != PlayerRole::None ? ChatType::Role : ChatType::Normal;
		svrChk(GetOwner().GetComponent<ChattingLogSystem>()->AddChattingLog(Util::Time.GetTimeUTCSec(), pMyPlayer->GetPlayerID(), pMyPlayer->GetPlayerState() == PlayerState::Ghost, charType, message));

		GetOwner().ForeachPlayerGameServer([&](GamePlayer* pPlayer, Policy::IPolicyGameServer *pPolicy)->Result {
			if (role == PlayerRole::None || role == pPlayer->GetRole())
			{
				if (bIsGhost && pPlayer->GetPlayerState() != PlayerState::Ghost)
				{
					// Skip broadcast from ghost to others
					return ResultCode::SUCCESS;
				}
				pPolicy->ChatMessageC2SEvt(RouteContext(ownerEntityUID, pPlayer->GetPlayerEntityUID()), pMyPlayer->GetPlayerID(), role, pMyPlayer->GetPlayerName(), message);
			}
			return ResultCode::SUCCESS;
		});



	Proc_End:

		return hr;
	}

	Result GamePlaySystem::BroadCastRandomBotMessage(int minID, int maxID, int& index)
	{
		return BroadCastRandomBotMessage(PlayerRole::None, minID, maxID, index);
	}

	
	Result GamePlaySystem::BroadCastRandomBotMessage(PlayerRole roleToTalk, int minID, int maxID, int& index)
	{
		int botIndex = -1;
		char strBuffer[1024];

		if(index >= (int)GetOwner().GetNumBot())
			return ResultCode::SUCCESS;

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayerFrom) ->Result
		{
			if (!pPlayerFrom->GetIsBot()) return ResultCode::SUCCESS;

			botIndex++;

			if(botIndex < index)
				return ResultCode::SUCCESS;

			if (botIndex > index)
				return ResultCode::FAIL;

			if(pPlayerFrom->GetPlayerState() != PlayerState::Playing)
				return ResultCode::SUCCESS;

			if (roleToTalk != PlayerRole::None && pPlayerFrom->GetRole() != roleToTalk) return ResultCode::SUCCESS;

			int randTalkPossibility = Util::Random.Rand(1000);
			if (randTalkPossibility > 700) return ResultCode::SUCCESS;

			int randTalkID = Util::Random.Rand(minID, maxID);
			StrUtil::Format(strBuffer, "<chatmsg><n><t>b</t><v>{0}</v></n></chatmsg>", randTalkID);

			GetOwner().GetComponent<GamePlaySystem>()->BroadCastChatMessage(pPlayerFrom, PlayerRole::None, strBuffer);

			return ResultCode::SUCCESS;
		});

		index++;
		return ResultCode::SUCCESS;
	}

	Result GamePlaySystem::BroadCastRandomBotMessageSuspect(int minID, int maxID, int& index)
	{
		int botIndex = -1;
		char strBuffer[1024];

		if (index >= (int)GetOwner().GetNumBot())
			return ResultCode::SUCCESS;

		GetOwner().ForeachPlayer([&](GamePlayer* pPlayerFrom) ->Result
		{
			if (!pPlayerFrom->GetIsBot()) return ResultCode::SUCCESS;

			if (!IsSuspect(pPlayerFrom->GetPlayerID()))
				return ResultCode::SUCCESS;

			botIndex++;

			if (botIndex < index)
				return ResultCode::SUCCESS;

			if (botIndex > index)
				return ResultCode::FAIL;

			int randTalkPossibility = Util::Random.Rand(1000);
			if (randTalkPossibility > 700) return ResultCode::SUCCESS;

			int randTalkID = Util::Random.Rand(minID, maxID);
			StrUtil::Format(strBuffer, "<chatmsg><n><t>b</t><v>{0}</v></n></chatmsg>", randTalkID);

			GetOwner().GetComponent<GamePlaySystem>()->BroadCastChatMessage(pPlayerFrom, PlayerRole::None, strBuffer);

			return ResultCode::SUCCESS;
		});

		index++;
		return ResultCode::SUCCESS;
	}



}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







