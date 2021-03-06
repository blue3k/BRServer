////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Vote
//	
////////////////////////////////////////////////////////////////////////////////




#include "GameInstancePCH.h"

#include "Util/SFTimeUtil.h"
#include "Util/SFRandom.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeGame.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"

#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "ConspiracyGameInstanceServerClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "GameSystem/GameVote.h"
#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"
#include "GameInstance/GameInstanceEntity.h"




namespace SF {
namespace ConspiracyGameInstanceServer {


	
	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlay state calss
	//

	GamePlaySystem& GameVote::GetGamePlaySystem()
	{
		GamePlaySystem* pSystem = m_Owner->GetComponent<GamePlaySystem>();
		Assert(pSystem);
		return *pSystem;
	}

	GameVote::GameVote(GameInstanceEntity* Owner)
		:m_Owner(Owner)
		,m_VoteState(VoteState::None)
	{
	}


	// Iniciate vote
	Result GameVote::IniciateVote()
	{
		m_VoteState = VoteState::Voting;

		GetOwner().ForeachPlayer( [&]( GamePlayer* pPlayer )->Result {
			pPlayer->SetVote(0);
			pPlayer->SetVoted(0);
			return ResultCode::SUCCESS;
		});
		return ResultCode::SUCCESS;
	}

	Result GameVote::Vote( GamePlayer* pVoter, GamePlayer *pVoteTarget )
	{
		if( pVoter == nullptr || pVoteTarget == nullptr )
			return ResultCode::INVALID_POINTER;

		if( !pVoter->IsInGame() )
			return ResultCode::GAME_NO_SUFFRAGE;

		if( !pVoteTarget->IsInGame() )
			return ResultCode::GAME_INVALID_VOTE_TARGET;

		// Increase voted count
		pVoteTarget->SetVoted( pVoteTarget->GetVoted() + 1 );

		if( pVoter->GetVote() != 0 )
		{
			GamePlayer *pPreviousVoteTarget = nullptr;
			if( (GetOwner().FindPlayer( pVoter->GetVote(), pPreviousVoteTarget )) )
			{
				uint numVoted = pPreviousVoteTarget->GetVoted();
				Assert(numVoted>0);
				if( numVoted > 0 )
				{
					numVoted--;
					pPreviousVoteTarget->SetVoted( numVoted );
				}
			}
		}

		// Set vote 
		pVoter->SetVote( pVoteTarget->GetPlayerID() );

		return ResultCode::SUCCESS;
	}

	// Update vote state
	void GameVote::UpdateVote(DurationMS ulTimeInState)
	{
		switch( m_VoteState )
		{
		case VoteState::None:
			break;
		case VoteState::Voting:
			if( ulTimeInState > DurationMS(GameConst::AI_VOTE_TIMEOUT) )
			{
				ForceAIVoted();
				m_VoteState = VoteState::InHurry;
			}
			break;
		case VoteState::InHurry:
			//if( ulTimeInState > GameConst::VOTE_TIMEOUT )
			//{
			//	ForceAllVoted();
			//	m_VoteState = VoteState::ForceVote;
			//}
			break;
		default:
			break;
		}
	}


	void GameVote::VoteRandomTarget( GamePlayer* pVoter, Array<GamePlayer*>& voteTargets )
	{
		uint randTarget = (uint)(Util::Random.Rand() % voteTargets.size());
		if( voteTargets[randTarget] == pVoter ) randTarget = (uint)((randTarget+1) % voteTargets.size());
		
		Result voteRes = GetOwner().GetComponent<GameStateSystem>()->Vote( pVoter, voteTargets[randTarget] );
		if( !(voteRes) )
		{
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// Vote for suspect
	//

	PlayerID GameVoteSuspect::GetVoteRanker( int index )
	{
		AssertRel(index>=0 && index<(int)countof(m_VoteRankers));
		return m_VoteRankers[index];
	}

	void GameVoteSuspect::SetVoteRanker( PlayerID playerID, uint rate )
	{
		for( int index = 0; index < (int)countof(m_VoteRankers); index++ )
		{
			if( m_VoteRates[index] <= rate )
			{
				std::swap( m_VoteRankers[index], playerID ); 
				std::swap( m_VoteRates[index], rate ); 

				if( m_VoteRankers[index] == playerID || playerID == 0 )
					break;
			}
		}
	}

	// Iniciate vote
	Result GameVoteSuspect::IniciateVote()
	{
		m_IsInVoting = true;

		memset( m_VoteRankers, 0, sizeof(m_VoteRankers) );
		memset( m_VoteRates, 0, sizeof(m_VoteRates) );

		return GameVote::IniciateVote();
	}

	Result GameVoteSuspect::Vote( GamePlayer* pVoter, GamePlayer *pVoteTarget )
	{
		Result hr = ResultCode::SUCCESS;
		
		if( !pVoter->IsInGame() || pVoter->GetPlayerState() == PlayerState::Ghost )
			return ResultCode::GAME_INVALID_PLAYER_STATE;

		if( !pVoteTarget->IsInGame() || pVoteTarget->GetPlayerState() == PlayerState::Ghost )
			return ResultCode::GAME_INVALID_VOTE_TARGET;

		svrChk(GameVote::Vote(pVoter,pVoteTarget) );

		// Update vote ranker
		SetVoteRanker( pVoteTarget->GetPlayerID(), pVoteTarget->GetVoted() );

		// broadcast vote result
		GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, NetSvrPolicyGameInstance &pPolicy )->Result {
			if( pPlayer->GetPlayerEntityUID().UID != 0 )
				pPolicy.VotedS2CEvt( RouteContext(GetOwner().GetEntityUID(),pPlayer->GetPlayerEntityUID()), pVoter->GetPlayerID(), pVoteTarget->GetPlayerID()  );

			return ResultCode::SUCCESS;
		});

	Proc_End:

		return hr;
	}

	uint GameVoteSuspect::GatherVoteTarget(Array<GamePlayer*>& voteTargets)
	{
		voteTargets.Clear();
		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result {
			if( pPlayer->IsInGame() && pPlayer->GetPlayerState() != PlayerState::Ghost )
			{
				voteTargets.push_back(pPlayer);
			}
			return ResultCode::SUCCESS;
		});

		return (uint)voteTargets.size();
	}
	
	// Make all AI(left players and sleepers) be voted
	void GameVoteSuspect::ForceAIVoted()
	{
		if( !m_IsInVoting )
			return;

		StaticArray<GamePlayer*, GameLogItem::LEGACY_MAX_GAMEPLAYER> voteTargets(GetOwner().GetHeap());

		// Nothing to do
		if( GatherVoteTarget(voteTargets) == 0 ) return;

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result {
			if( pPlayer->IsInGame() && pPlayer->GetVote() == 0
				&& !pPlayer->GetIsActivePlayer() )
			{
				// Select random target except himself
				VoteRandomTarget(pPlayer,voteTargets);
			}
			return ResultCode::SUCCESS;
		});
	}

	// Make all player be voted
	void GameVoteSuspect::ForceAllVoted()
	{
		if( !m_IsInVoting )
			return;
		
		StaticArray<GamePlayer*, GameLogItem::LEGACY_MAX_GAMEPLAYER> voteTargets(GetOwner().GetHeap());

		// Nothing to do
		if( GatherVoteTarget(voteTargets) == 0 ) return;

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result {
			if( pPlayer->IsInGame() && pPlayer->GetVote() == 0 )
			{
				VoteRandomTarget(pPlayer, voteTargets);
			}
			return ResultCode::SUCCESS;
		});
	}

	bool GameVoteSuspect::IsVoteEnd()
	{
		if( !m_IsInVoting )
			return true;

		m_IsInVoting = false;
		// Check vote end
		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result
		{
			if (pPlayer->IsInGame()
				&& pPlayer->GetPlayerState() != PlayerState::Ghost
				&& pPlayer->GetVote() == 0)
			{
				m_IsInVoting = true;
				return ResultCode::FAIL;
			}
				return ResultCode::SUCCESS;
		});

		return !m_IsInVoting;
	}


	////////////////////////////////////////////////////////////////////////////////////
	//
	// Vote for hanging
	//

	PlayerID GameVoteHanging::GetPlayerToHang()
	{
		GamePlaySystem& gamePlaySystem = GetGamePlaySystem();

		GamePlayer *pSuspect0 = nullptr, *pSuspect1 = nullptr;
		GetOwner().FindPlayer( gamePlaySystem.GetSuspect(0), pSuspect0 );
		GetOwner().FindPlayer( gamePlaySystem.GetSuspect(1), pSuspect1 );

		GamePlayer *pHuntedPlayer = nullptr;
		GetOwner().FindPlayer( gamePlaySystem.GetHuntedPlayer(), pHuntedPlayer );
		if( pSuspect0 != nullptr && pSuspect1 != nullptr && pHuntedPlayer != nullptr
			&& pSuspect0->GetVoted() == pSuspect1->GetVoted() )
		{
			if( std::abs(((INT)pSuspect0->GetIndex() - (INT)pHuntedPlayer->GetIndex())) > std::abs(((INT)pSuspect1->GetIndex() - (INT)pHuntedPlayer->GetIndex())) )
				return pSuspect1->GetPlayerID();
		}

		if( pSuspect0 != nullptr )
			return pSuspect0->GetPlayerID();

		if( pSuspect1 != nullptr )
			return pSuspect1->GetPlayerID();

		return gamePlaySystem.GetSuspect(0);
	}

	// Iniciate vote
	Result GameVoteHanging::IniciateVote()
	{
		m_IsInVoting = true;

		m_PlayerToHang = 0;
		m_TopRate = 0;

		return GameVote::IniciateVote();
	}

	Result GameVoteHanging::Vote( GamePlayer* pVoter, GamePlayer *pVoteTarget )
	{
		Result hr = ResultCode::SUCCESS;

		if(GetGamePlaySystem().IsSuspect(pVoter->GetPlayerID()) )
			return ResultCode::GAME_NO_SUFFRAGE;

		if( !pVoter->IsInGame() || pVoter->GetPlayerState() == PlayerState::Ghost )
			return ResultCode::GAME_INVALID_PLAYER_STATE;

		if( !GetGamePlaySystem().IsSuspect(pVoteTarget->GetPlayerID()) )
			return ResultCode::GAME_INVALID_VOTE_TARGET;

		if( !pVoteTarget->IsInGame() || pVoteTarget->GetPlayerState() == PlayerState::Ghost )
			return ResultCode::GAME_INVALID_VOTE_TARGET;

		svrChk(GameVote::Vote(pVoter,pVoteTarget) );

		// Update vote ranker
		if( m_TopRate < pVoteTarget->GetVoted() )
		{
			m_PlayerToHang = pVoteTarget->GetPlayerID();
			m_TopRate = pVoteTarget->GetVoted();
		}

		// broadcast vote result
		GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, NetSvrPolicyGameInstance &pPolicy )->Result {
			if( pPlayer->GetPlayerEntityUID().UID != 0 )
				pPolicy.VotedS2CEvt( RouteContext(GetOwner().GetEntityUID(),pPlayer->GetPlayerEntityUID()), pVoter->GetPlayerID(), pVoteTarget->GetPlayerID()  );
			return ResultCode::SUCCESS;
		});

	Proc_End:

		return hr;
	}
	
	uint GameVoteHanging::GatherVoteTarget(Array<GamePlayer*>& voteTargets)
	{
		GamePlaySystem& gamePlaySystem = GetGamePlaySystem();

		for(uint iSuspect = 0; iSuspect < gamePlaySystem.GetNumberOfSuspects(); iSuspect++ )
		{
			GamePlayer *pFound = nullptr;
			GetOwner().FindPlayer( gamePlaySystem.GetSuspect(iSuspect), pFound );
			voteTargets.push_back( pFound );
		}

		return (uint)voteTargets.size();
	}
	
	// Make all AI(left players and sleepers) be voted
	void GameVoteHanging::ForceAIVoted()
	{
		GamePlaySystem& gamePlaySystem = GetGamePlaySystem();

		if( !m_IsInVoting )
			return;

		StaticArray<GamePlayer*, GameLogItem::LEGACY_MAX_GAMEPLAYER> voteTargets(GetOwner().GetHeap());

		// Nothing to do
		if( GatherVoteTarget(voteTargets) == 0 ) return;

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result {
			if( pPlayer->IsInGame() && pPlayer->GetVote() == 0 && !gamePlaySystem.IsSuspect(pPlayer->GetPlayerID())
				&& !pPlayer->GetIsActivePlayer()
				)
			{
				VoteRandomTarget(pPlayer, voteTargets);
			}
			return ResultCode::SUCCESS;
		});
	}

	// Make all player be voted
	void GameVoteHanging::ForceAllVoted()
	{
		GamePlaySystem& gamePlaySystem = GetGamePlaySystem();

		if( !m_IsInVoting )
			return;
		
		StaticArray<GamePlayer*, GameLogItem::LEGACY_MAX_GAMEPLAYER> voteTargets(GetOwner().GetHeap());

		// Nothing to do
		if( GatherVoteTarget(voteTargets) == 0 ) return;

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result {
			if( pPlayer->IsInGame() && pPlayer->GetVote() == 0 && !gamePlaySystem.IsSuspect(pPlayer->GetPlayerID()) )
			{
				VoteRandomTarget(pPlayer, voteTargets);
			}
			return ResultCode::SUCCESS;
		});
	}

	bool GameVoteHanging::IsVoteEnd()
	{
		if( !m_IsInVoting )
			return true;

		// Check vote end
		m_IsInVoting = false;
		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result
		{
			if( pPlayer->IsInGame() && !GetGamePlaySystem().IsSuspect(pPlayer->GetPlayerID()) && pPlayer->GetVote() == 0 )
			{
				m_IsInVoting = true;
				return ResultCode::FAIL;
			}
			return ResultCode::SUCCESS;
		});

		return !m_IsInVoting;
	}


	////////////////////////////////////////////////////////////////////////////////////
	//
	// Vote for night
	//

	GameVoteNight::GameVoteNight(GameInstanceEntity* Owner,uint votingFlags)
		:GameVote(Owner)
		, m_VotingFlags(votingFlags)
		, m_SeersChoice(0)
		, m_BodyGuardsChoice(0)
		, m_PlayerToKill(0)
		, m_TopRate(0)
		, m_IsInVoting(false)
	{
	}

	// Iniciate vote
	Result GameVoteNight::IniciateVote()
	{
		m_IsInVoting = true;

		m_SeersChoice = 0;
		m_BodyGuardsChoice = 0;
		GetGamePlaySystem().SetOwlmansChoice(0);
		m_PlayerToKill = 0;
		m_TopRate = 0;

		return GameVote::IniciateVote();
	}

	Result GameVoteNight::Vote( GamePlayer* pVoter, GamePlayer *pVoteTarget )
	{
		Result hr = ResultCode::SUCCESS;

		if( pVoter == nullptr || pVoteTarget == nullptr )
			return ResultCode::INVALID_POINTER;

		if( !pVoter->IsInGame() || pVoter->GetPlayerState() == PlayerState::Ghost )
			return ResultCode::GAME_NO_SUFFRAGE;

		if( !pVoteTarget->IsInGame() || pVoteTarget->GetPlayerState() == PlayerState::Ghost )
			return ResultCode::GAME_INVALID_VOTE_TARGET;

		// Seer
		if( pVoter->GetPlayerID() == GetGamePlaySystem().GetSeer() )
		{
			// We can't allow to vote again when he got the notice
			if (GetSeersChoice() != 0)
				return ResultCode::GAME_NO_SUFFRAGE;

			if (GetGamePlaySystem().GetSeer() == 0)
				return ResultCode::UNEXPECTED;

			if (pVoteTarget->GetRevealedBySeer())
				return ResultCode::GAME_INVALID_VOTE_TARGET;


			// send result as soon as he vote
			GamePlayer *pSeers = nullptr;
			auto pSeersChoice = pVoteTarget;
			svrChk(GetOwner().FindPlayer(GetGamePlaySystem().GetSeer(), pSeers));

			SetSeersChoice(pVoteTarget->GetPlayerID());

			pSeersChoice->SetRevealedBySeer(true);

			if (pSeers->GetPlayerEntityUID().UID != 0)
				NetSvrPolicyGameInstance(pSeers->GetRemoteEndpoint()).PlayerRevealedS2CEvt(RouteContext(GetOwner().GetEntityUID(), pSeers->GetPlayerEntityUID()), pSeersChoice->GetPlayerID(), (uint8_t)pSeersChoice->GetRole(), (uint8_t)PlayerRevealedReason::SeersChoice);

			return ResultCode::SUCCESS;
		}

		// Bodyguard
		if( IsFlagSet(BODYGUARD) && pVoter->GetPlayerID() == GetGamePlaySystem().GetBodyGuard() )
		{
			SetBodyGuardsChoice( pVoteTarget->GetPlayerID() );
			return ResultCode::SUCCESS;
		}

		// Owlman
		if( IsFlagSet(OWLMAN) && pVoter->GetPlayerID() == GetGamePlaySystem().GetOwlman() )
		{
			GetGamePlaySystem().SetOwlmansChoice( pVoteTarget->GetPlayerID() );
			return ResultCode::SUCCESS;
		}

		if( pVoter->GetRole() != PlayerRole::Werewolf )
			return ResultCode::GAME_NO_SUFFRAGE;

		if( pVoteTarget->GetRole() == PlayerRole::Werewolf )
			return ResultCode::GAME_INVALID_VOTE_TARGET;

		svrChk(GameVote::Vote(pVoter,pVoteTarget) );

		// broadcast vote result
		GetOwner().ForeachPlayerSvrGameInstance([&](GamePlayer* pPlayer, NetSvrPolicyGameInstance &pPolicy)->Result {
			if (pPlayer->GetPlayerEntityUID().UID != 0
				&& pPlayer->GetRole() == PlayerRole::Werewolf
				&& pPlayer->GetPlayerID() != pVoter->GetPlayerID())
			{
				pPolicy.VotedS2CEvt(RouteContext(GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pVoter->GetPlayerID(), pVoteTarget->GetPlayerID());
			}

			return ResultCode::SUCCESS;
		});

		// Update vote ranker
		if( m_TopRate < pVoteTarget->GetVoted() )
		{
			m_PlayerToKill = pVoteTarget->GetPlayerID();
			m_TopRate = pVoteTarget->GetVoted();
		}

	Proc_End:

		return hr;
	}

	bool GameVoteNight::IsVoteEnd()
	{
		if( !m_IsInVoting )
			return true;

		if( GetGamePlaySystem().GetSeer() != 0 && GetSeersChoice() == 0 )
			return false;

		// Bodygard
		if( IsFlagSet(BODYGUARD) && GetGamePlaySystem().GetBodyGuard() != 0 && GetBodyGuardsChoice() == 0 )
			return false;

		// Owlman
		if( IsFlagSet(OWLMAN) && GetGamePlaySystem().GetOwlman() != 0 && GetGamePlaySystem().GetOwlmansChoice() == 0 )
			return false;

		// Check vote end
		m_IsInVoting = false;
		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result 
		{
			if (pPlayer->IsInGame() && pPlayer->GetRole() == PlayerRole::Werewolf && pPlayer->GetVote() == 0)
			{
				m_IsInVoting = true;
				return ResultCode::FAIL;// we don't need to proceed anymore
			}

			return ResultCode::SUCCESS;
		});

		return !m_IsInVoting;
	}

	uint GameVoteNight::GatherVoteTarget(Array<GamePlayer*>& voteTargets)
	{
		voteTargets.Clear();
		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result {
			if( pPlayer->IsInGame() && pPlayer->GetPlayerState() != PlayerState::Ghost )
			{
				voteTargets.push_back(pPlayer);
			}
			return ResultCode::SUCCESS;
		});

		return (uint)voteTargets.size();
	}

	uint GameVoteNight::GatherVoteTargetForHunting(Array<GamePlayer*>& voteTargets)
	{
		voteTargets.Clear();

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result {
			if( pPlayer->IsInGame() 
				&& pPlayer->GetRole() != PlayerRole::Werewolf
				&&  pPlayer->GetPlayerState() != PlayerState::Ghost)
			{
				voteTargets.push_back(pPlayer);
			}
			return ResultCode::SUCCESS;
		});

		return (uint)voteTargets.size();
	}

	uint GameVoteNight::GatherVoteTargetForSeer(Array<GamePlayer*>& voteTargets)
	{
		voteTargets.Clear();

		GetOwner().ForeachPlayer( [&](GamePlayer* pPlayer) -> Result {
			if( pPlayer->IsInGame() && pPlayer->GetVote() == 0
				&& !pPlayer->GetRevealedBySeer()
				&& pPlayer->GetRole() != PlayerRole::Seer
				&& pPlayer->GetPlayerState() != PlayerState::Ghost )
			{
				voteTargets.push_back(pPlayer);
			}
			return ResultCode::SUCCESS;
		});

		return (uint)voteTargets.size();
	}

	// Make all AI(left players and sleepers) be voted
	void GameVoteNight::ForceAIVoted()
	{
		GamePlaySystem& gamePlaySystem = GetGamePlaySystem();
		GamePlayer* pVoter = nullptr;

		StaticArray<GamePlayer*, GameLogItem::LEGACY_MAX_GAMEPLAYER> voteTargets(GetOwner().GetHeap());

		// Seer random vote
		if( GetGamePlaySystem().GetSeer() != 0 && GetSeersChoice() == 0
			&& (GetOwner().FindPlayer(gamePlaySystem.GetSeer(), pVoter )) 
			&& !pVoter->GetIsActivePlayer()									// The seer must be inactive
			&& (GatherVoteTargetForSeer(voteTargets) > 0) )
		{
			VoteRandomTarget(pVoter, voteTargets);
		}

		// Bodygard
		voteTargets.Clear();
		if( IsFlagSet(BODYGUARD) && gamePlaySystem.GetBodyGuard() != 0 && GetBodyGuardsChoice() == 0
			&& (GetOwner().FindPlayer(gamePlaySystem.GetBodyGuard(), pVoter )) 
			&& !pVoter->GetIsActivePlayer()									// The bodyguard must be inactive
			&& (GatherVoteTarget(voteTargets) > 0) )
		{
			VoteRandomTarget(pVoter, voteTargets);
		}

		// Owlman
		voteTargets.Clear();
		if( IsFlagSet(OWLMAN) && gamePlaySystem.GetOwlman() != 0 && gamePlaySystem.GetOwlmansChoice() == 0
			&& (GetOwner().FindPlayer(gamePlaySystem.GetOwlman(), pVoter )) 
			&& !pVoter->GetIsActivePlayer()									// The seer must be inactive
			&& (GatherVoteTarget(voteTargets) > 0) )
		{
			VoteRandomTarget(pVoter, voteTargets);
		}

		// Werewolves random vote
		voteTargets.Clear();
		if( GatherVoteTargetForHunting(voteTargets) > 0 )
		{
			auto& werewolves = gamePlaySystem.GetWerewolves();
			for (GamePlayer* pPlayer : werewolves)
			{
				if( pPlayer->IsInGame() && pPlayer->GetVote() == 0
					&& !pPlayer->GetIsActivePlayer()						// The seer must be inactive
					)
				{
					VoteRandomTarget(pPlayer, voteTargets);
				}
			}
		}
	}

	// Make all player be voted
	void GameVoteNight::ForceAllVoted()
	{
		GamePlaySystem& gamePlaySystem = GetGamePlaySystem();
		GamePlayer* pVoter = nullptr;

		if( !m_IsInVoting )
			return;

		StaticArray<GamePlayer*, GameLogItem::LEGACY_MAX_GAMEPLAYER> voteTargets(GetOwner().GetHeap());

		// Seer random vote
		if( GetGamePlaySystem().GetSeer() != 0 && GetSeersChoice() == 0
			&& (GetOwner().FindPlayer(gamePlaySystem.GetSeer(), pVoter )) 
			&& (GatherVoteTargetForSeer(voteTargets) > 0) )
		{
			VoteRandomTarget(pVoter, voteTargets);
		}

		// Bodygard
		voteTargets.Clear();
		if( IsFlagSet(BODYGUARD) && gamePlaySystem.GetBodyGuard() != 0 && GetBodyGuardsChoice() == 0
			&& (GetOwner().FindPlayer(gamePlaySystem.GetBodyGuard(), pVoter )) 
			&& (GatherVoteTarget(voteTargets) > 0) )
		{
			VoteRandomTarget(pVoter, voteTargets);
		}

		// Owlman
		voteTargets.Clear();
		if( IsFlagSet(OWLMAN) && gamePlaySystem.GetOwlman() != 0 && gamePlaySystem.GetOwlmansChoice() == 0
			&& (GetOwner().FindPlayer(gamePlaySystem.GetOwlman(), pVoter )) 
			&& (GatherVoteTarget(voteTargets) > 0) )
		{
			VoteRandomTarget(pVoter, voteTargets);
		}

		// Werewolves random vote
		voteTargets.Clear();
		if( GatherVoteTargetForHunting(voteTargets) > 0 )
		{
			auto& werewolves = gamePlaySystem.GetWerewolves();
			for (GamePlayer* pPlayer : werewolves)
			{
				if( pPlayer->IsInGame() && pPlayer->GetVote() == 0
					)
				{
					VoteRandomTarget(pPlayer, voteTargets);
				}
			}
		}
	}


}; // namespace ConspiracyGameInstanceServer
}; // namespace SF


