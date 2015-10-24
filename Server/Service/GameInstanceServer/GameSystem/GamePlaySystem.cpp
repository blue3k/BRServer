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

#include "Common/TimeUtil.h"
#include "Common/BrRandom.h"
#include "Common/HRESSvrSys.h"
#include "Common/HRESGame.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BRServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"

#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "ConspiracyGameInstanceServerClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"
#include "GameSystem/GameLogSystem.h"
#include "GameInstance/GameInstanceEntity.h"


BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GamePlaySystem);


namespace BR {
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
		:GameSystem(pEntity)
	{
	}

	GamePlaySystem::~GamePlaySystem()
	{
	}
	

	// Initialzie system
	HRESULT GamePlaySystem::InitializeComponent()
	{
		HRESULT hr = S_OK;

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

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) ->HRESULT {
			pPlayer->SetRevealedBySeer( false );
			pPlayer->SetPlayerState( PlayerState::Playing );
			pPlayer->SetReviveCount(0);
			return S_OK;
		});

		svrChk( __super::InitializeComponent() );

	Proc_End:

		return hr;
	}

	HRESULT GamePlaySystem::PlayerRoleAssigned(GamePlayer* pPlayer)
	{
		HRESULT hr = S_OK;

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
		};


	Proc_End:

		return hr;
	}
	
	HRESULT GamePlaySystem::AssignRole()
	{
		HRESULT hr = S_OK;

		bool ShufflingBuffer[GameConst::MAX_GAMEPLAYER];
		size_t numPlayer = GetOwner().GetNumPlayer();

		m_Werewolves.Clear();

		memset( ShufflingBuffer, 0, sizeof(ShufflingBuffer) );

		// assign requested player first
		GetOwner().ForeachPlayer([&](GamePlayer* pPlayer) ->HRESULT {

			pPlayer->SetRole(PlayerRole::None);

			if (pPlayer->GetRequestedRole() == PlayerRole::None)
			{
				return S_OK;
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
				svrTrace(Trace::TRC_WARN, "Failed to find requested role, player:%0%, requested:%1%", pPlayer->GetPlayerID(), (UINT)pPlayer->GetRequestedRole());
				return S_OK;
			}

			ShufflingBuffer[iSlot] = true;
			pPlayer->SetRole(pPlayer->GetRequestedRole());

			Assert(pPlayer->GetRole() != PlayerRole::None);

			PlayerRoleAssigned(pPlayer);

			return S_OK;
		});


		// assign remain
		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) ->HRESULT {

			if (pPlayer->GetRole() != PlayerRole::None)
				return S_OK;

			int iRandom = (int)Util::Random.Rand(0, (int)numPlayer - 1);
			for( int count = 0; count < GameConst::MAX_GAMEPLAYER; count++, iRandom = (iRandom+1)%numPlayer )
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

			return S_OK;
		});


		GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->HRESULT {
			if( pPlayer->GetPlayerEntityUID() != 0 )
				pPolicy->RoleAssignedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayer->GetRole() );
			return S_OK;
		});


		// notify other's role
		for( int iwolf = 0; iwolf < m_Werewolves.GetSize(); iwolf++ )
		{
			for( int iother = 0; iother < m_Werewolves.GetSize(); iother++ )
			{
				if( iwolf == iother ) continue;

				if( m_Werewolves[iwolf] != nullptr && m_Werewolves[iother] != nullptr )
				{
					auto pPolicy = m_Werewolves[iwolf]->GetPolicy<Policy::ISvrPolicyGameInstance>();
					if (m_Werewolves[iwolf]->GetPlayerEntityUID() != 0 && pPolicy != nullptr)
					{
						pPolicy->PlayerRevealedS2CEvt(RouteContext(GetOwner().GetEntityUID(), m_Werewolves[iwolf]->GetPlayerEntityUID()), m_Werewolves[iother]->GetPlayerID(), m_Werewolves[iother]->GetRole(), PlayerRevealedReason::Werewolf);
					}
				}
			}
		}

	Proc_End:

		return hr;
	}

	// Set suspect list
	void GamePlaySystem::SetSuspect( PlayerID suspect1, PlayerID suspect2 )
	{
		m_Suspects[0] = suspect1;
		m_Suspects[1] = suspect2;
	}


	// Update Villager and werewolf count
	HRESULT GamePlaySystem::UpdateVillagerNWerewolfCount()
	{
		m_NumVillager = 0;
		m_NumWereWolf = 0;

		HRESULT hr = S_OK;
		GetOwner().ForeachPlayer( [&]( GamePlayer* pPlayer )->HRESULT
		{
			if( pPlayer->GetPlayerState() != PlayerState::Playing )
				return S_OK;

			switch( pPlayer->GetRole() )
			{
			case PlayerRole::None:
				return E_GAME_INVALID_ROLE;
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
				hr = E_GAME_INVALID_ROLE;
				return hr;
			};
			return S_OK;
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

	HRESULT GamePlaySystem::UpdateGameEnd()
	{
		HRESULT hr = S_OK;

		GameWinner winner = GetGameWinner();
		if( winner == GameWinner::None )
			return hr;

		GetOwner().GetComponent<GameStateSystem>()->SetGameEnd(winner);

	Proc_End:

		return hr;
	}


	PlayerID GamePlaySystem::PickAnyoneCloseToHuntedPlayer( PlayerID exculdePlayer )
	{
		GamePlayer *pGamePlayer = nullptr;

		if( FAILED(GetOwner().FindPlayer(GetHuntedPlayer(), pGamePlayer)) )
		{
			if( FAILED(GetOwner().FindPlayer(exculdePlayer, pGamePlayer)) )
			{
				// pick random
				svrTrace( Trace::TRC_ERROR, "Invalid hunted player: PlayerID:%0%", GetHuntedPlayer() );
				GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer ) -> HRESULT
				{
					if( pGamePlayer != nullptr )
						return E_FAIL;

					if( pPlayer != nullptr && pPlayer->IsInGame() && pPlayer->GetPlayerState() != PlayerState::Ghost )
					{
						pGamePlayer = pPlayer;
					}

					return S_OK;
				});
			}
		}


		int playerIndexToCheck = pGamePlayer->GetIndex();
		GamePlayer* pCheckPlayer = nullptr;
		for( int iPlayer = 1; iPlayer < (int)(GetOwner().GetMaxPlayer()>>1); iPlayer++ )
		{
			// search right
			int checkIndex = (playerIndexToCheck + iPlayer) % GetOwner().GetMaxPlayer();
			if( SUCCEEDED(GetOwner().GetPlayerByIndex(checkIndex, pCheckPlayer))
				&& pCheckPlayer != nullptr && pCheckPlayer->IsInGame() && pCheckPlayer->GetPlayerState() != PlayerState::Ghost
				&& pCheckPlayer->GetPlayerID() != exculdePlayer )
			{
				return pCheckPlayer->GetPlayerID();
			}

			// search left
			checkIndex = (playerIndexToCheck - iPlayer) % GetOwner().GetMaxPlayer();
			if( SUCCEEDED(GetOwner().GetPlayerByIndex(checkIndex, pCheckPlayer))
				&& pCheckPlayer != nullptr && pCheckPlayer->IsInGame() && pCheckPlayer->GetPlayerState() != PlayerState::Ghost
				&& pCheckPlayer->GetPlayerID() != exculdePlayer )
			{
				return pCheckPlayer->GetPlayerID();
			}
		}


		svrTrace( Trace::TRC_ERROR, "Failed to find player PickAnyoneCloseToHuntedPlayer Hunted PlayerID:%0%", GetHuntedPlayer() );
		return 0;
	}


	// Kill Player
	HRESULT GamePlaySystem::KillPlayer( PlayerID playerToKill, PlayerKilledReason reason )
	{
		HRESULT hr = S_OK;
		GamePlayer *pPlayer = nullptr;


		svrChk( GetOwner().FindPlayer( playerToKill, pPlayer ) );

		svrChk( KillPlayer( pPlayer, reason ) );

	Proc_End:

		if( FAILED(hr) )
			svrTrace( Trace::TRC_ERROR, "Failed to find player KillPlayer PlayerID:%0%, reason:%1%", playerToKill, (int)reason );

		return hr;
	}

	HRESULT GamePlaySystem::KillPlayer( GamePlayer* pPlayerToKill, PlayerKilledReason reason )
	{
		HRESULT hr = S_OK;
		if( pPlayerToKill == nullptr )
			return E_INVALIDARG;

		if( pPlayerToKill->GetPlayerState() == PlayerState::Ghost )
		{
			hr = E_GAME_INVALID_PLAYER_STATE;
			goto Proc_End;
		}

		switch( reason )
		{
		case PlayerKilledReason::ByHanging:
			// Update lynched player to send it to the medium later
			m_LynchedPlayer = pPlayerToKill->GetPlayerID();
			m_LynchedRole = pPlayerToKill->GetRole();
			break;
		}

		pPlayerToKill->SetPlayerState( PlayerState::Ghost );

		switch( pPlayerToKill->GetRole() )
		{
		case PlayerRole::None:
			return E_GAME_INVALID_ROLE;
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
			svrErr(E_GAME_INVALID_ROLE);
			break;
		}

		GetOwner().OnPlayerGetOutOfGame( pPlayerToKill );

		GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->HRESULT {
			if( pPlayer->GetPlayerEntityUID() != 0 )
				pPolicy->PlayerKilledS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayerToKill->GetPlayerID(), reason);
			return S_OK;
		});

		//svrChk( UpdateGameEnd() );

	Proc_End:

		if( FAILED(hr) )
		{
			svrTrace( Trace::TRC_ERROR, "Failed to kill a player PlayerID:%0%, reason:%1%, HRESULT:%2%", pPlayerToKill->GetPlayerID(), (int)reason, ArgHex32(hr) );
		}
		else
		{
			svrTrace( Trace::TRC_TRACE, "Player PlayerID:%0% is killed by reason:%1%", pPlayerToKill->GetPlayerID(), (int)reason );
		}

		return hr;
	}


	HRESULT GamePlaySystem::RevivePlayer(GamePlayer* pPlayerToRevive)
	{
		HRESULT hr = S_OK;
		if (pPlayerToRevive == nullptr)
			return E_INVALIDARG;

		if (pPlayerToRevive->GetPlayerState() != PlayerState::Ghost)
		{
			hr = E_GAME_INVALID_PLAYER_STATE;
			goto Proc_End;
		}

		pPlayerToRevive->SetPlayerState(PlayerState::Playing);

		switch (pPlayerToRevive->GetRole())
		{
		case PlayerRole::None:
			return E_GAME_INVALID_ROLE;
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
			svrErr(E_GAME_INVALID_ROLE);
			break;
		}

		//GetOwner().OnPlayerGetOutOfGame(pPlayerToRevive);
		switch (pPlayerToRevive->GetRole())
		{
		case PlayerRole::Seer:
			SetSeer(1);
			break;
		};


		GetOwner().ForeachPlayerSvrGameInstance([&](GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy)->HRESULT {
			if (pPlayer->GetPlayerEntityUID() != 0)
				pPolicy->GamePlayerRevivedS2CEvt(RouteContext(GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayerToRevive->GetPlayerID());
			return S_OK;
		});


	Proc_End:

		if (FAILED(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to revive a player PlayerID:%0%, HRESULT:%1%", pPlayerToRevive->GetPlayerID(), ArgHex32(hr));
		}
		else
		{
			svrTrace(Trace::TRC_TRACE, "Player PlayerID:%0% is revived", pPlayerToRevive->GetPlayerID());
		}

		return hr;
	}


	// Called when a player get out of game
	HRESULT GamePlaySystem::OnPlayerGetOutOfGame( GamePlayer* pPlayer )
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
		};

		return S_OK;
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
		}

		return otherRole;
	}

	
}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







