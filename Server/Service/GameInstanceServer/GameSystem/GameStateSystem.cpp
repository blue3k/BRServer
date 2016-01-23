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
#include "Common/ResultCode/BRResultCodeSvr.h"
#include "Common/ResultCode/BRResultCodeGame.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"

#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "ConspiracyGameInstanceServerClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"
#include "GameSystem/GameLogSystem.h"
#include "GameSystem/GameVote.h"
#include "GameInstance/GameInstanceEntity.h"

#include "Table/conspiracy/RewardTbl.h"



BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameStateSystem);


namespace BR {
namespace ConspiracyGameInstanceServer {


	
	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlay state calss
	//

	GamePlaySystem& GamePlayState::GetGamePlaySystem()
	{
		GamePlaySystem* pSystem = m_Owner->GetComponent<GamePlaySystem>();
		Assert(pSystem);
		return *pSystem;
	}

	GameStateSystem& GamePlayState::GetGameStateSystem()
	{
		GameStateSystem *pSystem = m_Owner->GetComponent<GameStateSystem>();
		Assert(pSystem);
		return *pSystem;
	}

	HRESULT GamePlayState::OnEnter()
	{
		m_StateStartTime = Util::Time.GetTimeMs();
		m_StateStartTimeUTC = Util::Time.GetTimeUTCSec();

		UINT day = GetOwner().GetComponent<GameStateSystem>()->GetCurrentDay();
		GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->HRESULT
		{
			pPlayer->SetVotedGameAdvance(false);

			if( pPlayer->GetPlayerEntityUID() != 0 )
				pPolicy->GameAdvancedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), m_StateStartTimeUTC.time_since_epoch().count(), GetGameState(), day );

			return S_SYSTEM_OK;
		});

		return S_SYSTEM_OK;
	}


	class GamePlayState_None : public GamePlayState
	{
	public:
		GamePlayState_None(GameInstanceEntity* Owner):GamePlayState(Owner,GameStateID::None) {}
		virtual ~GamePlayState_None() {}

		virtual bool CanAdvanceToNext() override				{ return false; }
		virtual bool CanBeEntered() override					{ return false; }

		HRESULT virtual OnEnter() { return GamePlayState::OnEnter(); }
		HRESULT virtual OnLeave() { return GamePlayState::OnLeave(); }
	};

	
	// Game state with time limit
	class GamePlayState_TimeLimit : public GamePlayState
	{
	protected:
		Util::TimeStampTimer m_TimeToNext;

	public:
		GamePlayState_TimeLimit(GameInstanceEntity* Owner,GameStateID state):GamePlayState(Owner,state) {}
		virtual ~GamePlayState_TimeLimit() {}

		virtual HRESULT OnEnter()
		{
			m_TimeToNext.SetTimer( DurationMS(3*60*1000) );
			return GamePlayState::OnEnter();
		}
		virtual HRESULT OnUpdate()
		{
			m_TimeToNext.CheckTimer();
			return GamePlayState::OnUpdate();
		}
		virtual HRESULT OnLeave()
		{
			m_TimeToNext.ClearTimer();
			return GamePlayState::OnLeave();
		}
	};


	class GamePlayState_FirstFreeDebate : public GamePlayState_TimeLimit
	{
	private:

	public:
		GamePlayState_FirstFreeDebate(GameInstanceEntity* Owner):GamePlayState_TimeLimit(Owner,GameStateID::FreeDebate) {}
		virtual ~GamePlayState_FirstFreeDebate() {}

		virtual HRESULT OnEnter()
		{
			HRESULT hr = S_SYSTEM_OK;
			auto pBotTalkTbl = GetOwner().GetBotTalkTbl();

			svrChk(GamePlayState_TimeLimit::OnEnter() );

			GetGamePlaySystem().SetHuntedPlayer(0);

			m_TimeToNext.SetTimerFunc( [&](){ GetGameStateSystem().AdvanceState(); } );
			m_TimeToNext.SetTimer( DurationMS(GetOwner().GetPresetGameConfig()->FreeDiscussion*1000) );

			svrChkPtr(pBotTalkTbl);
			GetGamePlaySystem().BroadCastRandomBotMessage(pBotTalkTbl->FirstDay_Begin, pBotTalkTbl->FirstDay_End);

		Proc_End:

			return hr;
		}
	};


	// thrid or later
	class GamePlayState_Night : public GamePlayState_TimeLimit
	{
	private:
		GameVoteNight m_vote;

	public:
		GamePlayState_Night(GameInstanceEntity* Owner,GameStateID gameState,UINT votingFlags)
			:GamePlayState_TimeLimit(Owner,gameState)
			,m_vote(Owner,votingFlags)
		{}
		virtual ~GamePlayState_Night() {}
		
		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext() override
		{
			return m_vote.IsVoteEnd();
		}

		virtual HRESULT Vote(GamePlayer* pVoter, GamePlayer* pVoteTarget ) override
		{
			return m_vote.Vote( pVoter, pVoteTarget );
		}

		virtual HRESULT OnUpdate() override
		{
			m_vote.UpdateVote(GetTimeInState());
			return GamePlayState_TimeLimit::OnUpdate();
		}

		virtual HRESULT OnEnter() override
		{
			HRESULT hr = S_SYSTEM_OK;
			UINT numVotePlayer;

			auto stateTime = GetGameStateSystem().GetCurrentDay() == 1 ? 
				GetOwner().GetPresetGameConfig()->RolePlayTime : 
				GetOwner().GetPresetGameConfig()->RolePlayAndKillingTime;

			svrChk(GamePlayState_TimeLimit::OnEnter() );

			// Set vote timer
			m_TimeToNext.SetTimerFunc( [&](){ m_vote.ForceAllVoted(); } );
			m_TimeToNext.SetTimer(DurationMS(stateTime * 1000));


			// Reveal to medium 
			if( GetGamePlaySystem().GetLynchedPlayer() != 0 && GetGamePlaySystem().GetLynchedRole() != PlayerRole::None ) // execpt the first night, the lynched role will be specified
			{
				GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->HRESULT {
					if( pPlayer->GetRole() != PlayerRole::Medium )
						return S_SYSTEM_OK;

					if( pPlayer->GetPlayerEntityUID() != 0 )
						pPolicy->PlayerRevealedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), GetGamePlaySystem().GetLynchedPlayer(), GetGamePlaySystem().GetLynchedRole(), PlayerRevealedReason::Medium );

					return E_SYSTEM_FAIL;
				});
			}

			svrChk( m_vote.IniciateVote() );

			numVotePlayer = GetOwner().GetComponent<GamePlaySystem>()->GetNumWereWolf();
			if( GetOwner().GetComponent<GamePlaySystem>()->GetSeer() != 0 ) numVotePlayer++;
			if( GetOwner().GetComponent<GamePlaySystem>()->GetBodyGuard() != 0 ) numVotePlayer++;

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameVote( Util::Time.GetTimeUTCSec(), GameVoteType::Hunting, numVotePlayer ) );

		Proc_End:

			return hr;
		}
		//virtual HRESULT OnUpdate()	{ return S_SYSTEM_OK; }
		virtual HRESULT OnLeave() override
		{
			HRESULT hr = S_SYSTEM_OK;

			// vote system will send result when he voted
			// process seer
			//if( GetGamePlaySystem().GetSeer() != 0 && m_vote.GetSeersChoice() != 0 )
			//{
			//	GamePlayer *pSeers = nullptr;
			//	GamePlayer *pSeersChoice = nullptr;
			//	svrChk( GetOwner().FindPlayer( GetGamePlaySystem().GetSeer(), pSeers ) );
			//	svrChk( GetOwner().FindPlayer( m_vote.GetSeersChoice(), pSeersChoice ) );

			//	pSeersChoice->SetRevealedBySeer(true);
			//	auto pPolicy = pSeers->GetPolicy<Policy::ISvrPolicyGameInstance>();
			//	if( pSeers->GetPlayerEntityUID() != 0 pPolicy != nullptr)
			//		pPolicy->PlayerRevealedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pSeers->GetPlayerEntityUID()), pSeersChoice->GetPlayerID(), pSeersChoice->GetRole(), PlayerRevealedReason::SeersChoice );
			//}

			// process owlman
			if( m_vote.IsFlagSet(GameVoteNight::OWLMAN) && GetGamePlaySystem().GetOwlman() != 0 && GetGamePlaySystem().GetOwlmansChoice() != 0 )
			{
				// ??
			}

			if( m_vote.GetPlayerToKill() != 0 )
			{
				// process werewolf
				// if he is protected by a boardguard
				if( m_vote.IsFlagSet(GameVoteNight::BODYGUARD) && GetGamePlaySystem().GetBodyGuard() != 0 && m_vote.GetBodyGuardsChoice() == m_vote.GetPlayerToKill() )
				{
					GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->HRESULT {
						if( pPlayer->GetPlayerEntityUID() != 0 )
							pPolicy->PlayerKilledS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), m_vote.GetPlayerToKill(), PlayerKilledReason::BlockedByBodyguard );
						return S_SYSTEM_OK;
					});
					svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGamePlayerKilled( Util::Time.GetTimeUTCSec(), PlayerKilledReason::BlockedByBodyguard, m_vote.GetPlayerToKill() ) );
				}
				else
				{
					svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGamePlayerKilled( Util::Time.GetTimeUTCSec(), PlayerKilledReason::ByWerewolf, m_vote.GetPlayerToKill() ) );
					svrChk( GetGamePlaySystem().KillPlayer( m_vote.GetPlayerToKill(), PlayerKilledReason::ByWerewolf ) );
					GetGamePlaySystem().SetHuntedPlayer(m_vote.GetPlayerToKill());
				}
			}


			svrChk(GamePlayState_TimeLimit::OnLeave() );

		Proc_End:

			return hr;
		}

	};
	

	//

	//class GamePlayState_Mythomaniac : public GamePlayState
	//{
	//private:


	//public:
	//	GamePlayState_Mythomaniac(GameInstanceEntity* Owner):GamePlayState(Owner,GameStateID::Mythomaniac) {}
	//	virtual ~GamePlayState_Mythomaniac() {}


	//	// not implemented yet
	//	virtual bool CanBeEntered()						{ return false; }

	//};


	class GamePlayState_MorningDebate : public GamePlayState_TimeLimit
	{
	private:

	public:
		GamePlayState_MorningDebate(GameInstanceEntity* Owner):GamePlayState_TimeLimit(Owner,GameStateID::MorningDebate) {}
		virtual ~GamePlayState_MorningDebate() {}
		
		virtual HRESULT OnEnter()
		{
			HRESULT hr = S_SYSTEM_OK;

			GetOwner().GetComponent<GameStateSystem>()->NextDay();

			svrChk(GamePlayState_TimeLimit::OnEnter() );

			// Set vote timer
			m_TimeToNext.SetTimerFunc( [&](){ GetGameStateSystem().AdvanceState(); } );
			m_TimeToNext.SetTimer( DurationMS(GetOwner().GetPresetGameConfig()->MorningDiscussion*1000) );

			{
				auto pBotTalkTbl = GetOwner().GetBotTalkTbl();
				svrChkPtr(pBotTalkTbl);
				GetGamePlaySystem().BroadCastRandomBotMessage(pBotTalkTbl->FreeTalk_Begin, pBotTalkTbl->FreeTalk_End);
			}

		Proc_End:

			return hr;
		}
	};


	class GamePlayState_VoteForSuspects : public GamePlayState_TimeLimit
	{
	private:
		GameVoteSuspect m_vote;

	public:
		GamePlayState_VoteForSuspects(GameInstanceEntity* Owner)
			:GamePlayState_TimeLimit(Owner,GameStateID::VoteForSuspects)
			,m_vote(Owner)
		{}
		virtual ~GamePlayState_VoteForSuspects() {}
		
		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext()		{ return m_vote.IsVoteEnd(); }

		virtual HRESULT Vote(GamePlayer* pVoter, GamePlayer* pVoteTarget ) override
		{
			return m_vote.Vote( pVoter, pVoteTarget );
		}

		virtual HRESULT OnEnter() override
		{
			HRESULT hr = S_SYSTEM_OK;
			UINT totalAlives;

			svrChk(GamePlayState_TimeLimit::OnEnter() );

			// Set vote timer
			m_TimeToNext.SetTimerFunc( [&](){ m_vote.ForceAllVoted(); } );
			m_TimeToNext.SetTimer( DurationMS(GetOwner().GetPresetGameConfig()->FirstVote*1000) );


			svrChk( m_vote.IniciateVote() );

			totalAlives = GetOwner().GetComponent<GamePlaySystem>()->GetNumWereWolf() + GetOwner().GetComponent<GamePlaySystem>()->GetNumVillager();
			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameVote( Util::Time.GetTimeUTCSec(), GameVoteType::Suspect, totalAlives ) );


		Proc_End:

			return hr;
		}

		//virtual HRESULT OnUpdate()	{ return S_SYSTEM_OK; }
		virtual HRESULT OnLeave() override
		{
			HRESULT hr = S_SYSTEM_OK;
			
			// put someone who sit next to the latest dead player
			if( m_vote.GetVoteRanker(1) == 0 )
			{
				PlayerID nearPlayerID = GetGamePlaySystem().PickAnyoneCloseToHuntedPlayer(m_vote.GetVoteRanker(0));
				m_vote.SetVoteRanker(nearPlayerID, 1);
			}


			GetGamePlaySystem().SetSuspect(m_vote.GetVoteRanker(0),m_vote.GetVoteRanker(1));

			StaticArray<PlayerID, 2> rankers;
			rankers.push_back(m_vote.GetVoteRanker(0));
			rankers.push_back(m_vote.GetVoteRanker(1));
			//PlayerID rankers[2] = {m_vote.GetVoteRanker(0),m_vote.GetVoteRanker(1)};
			GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->HRESULT {
				if( pPlayer->GetPlayerEntityUID() != 0 )
					pPolicy->VoteEndS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), rankers );
				return S_SYSTEM_OK;
			});

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameVoteResult( Util::Time.GetTimeUTCSec(), 2, rankers.data() ) );

		Proc_End:

			return hr;
		}

		virtual HRESULT OnUpdate() override
		{
			m_vote.UpdateVote(GetTimeInState());
			return GamePlayState_TimeLimit::OnUpdate();
		}

	};


	class GamePlayState_DefenceOfSuspects : public GamePlayState_TimeLimit
	{
	private:

	public:
		GamePlayState_DefenceOfSuspects(GameInstanceEntity* Owner):GamePlayState_TimeLimit(Owner,GameStateID::DefenceOfSuspects) {}
		virtual ~GamePlayState_DefenceOfSuspects() {}
		
		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext() override		{ return true; }

		virtual HRESULT OnEnter() override
		{
			HRESULT hr = S_SYSTEM_OK;
			svrChk(GamePlayState_TimeLimit::OnEnter() );

			// Set vote timer
			m_TimeToNext.SetTimerFunc( [&](){ GetGameStateSystem().AdvanceState(); } );
			m_TimeToNext.SetTimer(DurationMS(GetOwner().GetPresetGameConfig()->DefenceTime*1000) );

			{
				auto pBotTalkTbl = GetOwner().GetBotTalkTbl();
				svrChkPtr(pBotTalkTbl);
				GetGamePlaySystem().BroadCastRandomBotMessageSuspect(pBotTalkTbl->Defense_Begin, pBotTalkTbl->Defense_End);
			}

		Proc_End:

			return hr;
		}

		virtual HRESULT OnLeave() override
		{
			return GamePlayState_TimeLimit::OnLeave();
		}
	};


	class GamePlayState_VoteForHanging : public GamePlayState_TimeLimit
	{
	private:
		GameVoteHanging m_vote;

	public:
		GamePlayState_VoteForHanging(GameInstanceEntity* Owner)
			:GamePlayState_TimeLimit(Owner,GameStateID::VoteForHanging)
			,m_vote(Owner)
		{}
		virtual ~GamePlayState_VoteForHanging() {}
		
		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext() override		{ return m_vote.IsVoteEnd(); }

		virtual HRESULT Vote(GamePlayer* pVoter, GamePlayer* pVoteTarget ) override
		{
			return m_vote.Vote( pVoter, pVoteTarget );
		}

		virtual HRESULT OnEnter() override
		{
			HRESULT hr = S_SYSTEM_OK;
			UINT totalAlives, totalSuspect;

			svrChk(GamePlayState_TimeLimit::OnEnter() );

			// Set vote timer
			m_TimeToNext.SetTimerFunc( [&](){ m_vote.ForceAllVoted(); } );
			m_TimeToNext.SetTimer(DurationMS(GetOwner().GetPresetGameConfig()->SecondVote*1000) );


			svrChk( m_vote.IniciateVote() );

			totalAlives = GetOwner().GetComponent<GamePlaySystem>()->GetNumWereWolf() + GetOwner().GetComponent<GamePlaySystem>()->GetNumVillager();
			totalSuspect = GetOwner().GetComponent<GamePlaySystem>()->GetNumberOfSuspects();
			Assert( totalAlives > totalSuspect );
			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameVote( Util::Time.GetTimeUTCSec(), GameVoteType::Hanging, totalAlives - totalSuspect ) );

		Proc_End:

			return hr;
		}

		virtual HRESULT OnLeave() override
		{
			HRESULT hr = S_SYSTEM_OK;

			// kill the top vote ranker
			if( m_vote.GetPlayerToHang() == 0 )
				return E_INVALID_PLAYERID;

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGamePlayerKilled( Util::Time.GetTimeUTCSec(), PlayerKilledReason::ByHanging, m_vote.GetPlayerToHang() ) );

			svrChk( GetGamePlaySystem().KillPlayer( m_vote.GetPlayerToHang(), PlayerKilledReason::ByHanging ) );

		Proc_End:

			return hr;
		}
		
		virtual HRESULT OnUpdate() override
		{
			m_vote.UpdateVote(GetTimeInState());
			return GamePlayState_TimeLimit::OnUpdate();
		}

	};


	class GamePlayState_End : public GamePlayState
	{
	private:


	public:
		GamePlayState_End(GameInstanceEntity* Owner):GamePlayState(Owner,GameStateID::End) {}
		virtual ~GamePlayState_End() {}
		
		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext() override		{ return false; }

		
		virtual HRESULT OnEnter() override
		{
			HRESULT hr = S_SYSTEM_OK;
			GameWinner winner = GetGamePlaySystem().GetGameWinner();
			conspiracy::RewardTbl::RewardItem *pItem = nullptr;
			char strBuffer[1024];
			auto pBotTalkTbl = GetOwner().GetBotTalkTbl();

			svrChk(GamePlayState::OnEnter() );

			// Broad cast game end
			GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::ISvrPolicyGameInstance *pPolicy )->HRESULT {

				UINT expGain = 0;
				bool isWinner = pPlayer->IsWinnerSide(winner);

				// Get expgain by role
				switch( pPlayer->GetRole() )
				{
				default:
				case PlayerRole::None:
					break;
				case PlayerRole::Seer:
					if( FAILED(conspiracy::RewardTbl::FindItem( (int)conspiracy::RewardTbl::ERole::Enum::Seer, pItem )) )
					{
						svrTrace( Trace::TRC_ERROR, "Can't find expericend for the seer, no exp for PID:{0}", pPlayer->GetPlayerID() );
						break;
					}
					expGain = isWinner ? pItem->WinExp : pItem->LoseExp;
					break;
				case PlayerRole::Villager:
				case PlayerRole::Medium:
				//case PlayerRole::Bodyguard:
				//case PlayerRole::Owlman:
					if( FAILED(conspiracy::RewardTbl::FindItem( (int)conspiracy::RewardTbl::ERole::Enum::Villager, pItem )) )
					{
						svrTrace( Trace::TRC_ERROR, "Can't find expericend for villager side, no exp for PID:{0}", pPlayer->GetPlayerID() );
						break;
					}
					expGain = isWinner ? pItem->WinExp : pItem->LoseExp;
					break;
				case PlayerRole::Werewolf:
				//case PlayerRole::Possessed:
					if (FAILED(conspiracy::RewardTbl::FindItem((int)conspiracy::RewardTbl::ERole::Enum::Monster, pItem)))
					{
						svrTrace( Trace::TRC_ERROR, "Can't find expericend for werewolf side, no exp for PID:{0}", pPlayer->GetPlayerID() );
						break;
					}
					expGain = isWinner ? pItem->WinExp : pItem->LoseExp;
					break;
				}

				// Broadcast other player's role to me
				GetOwner().ForeachPlayer( [&]( GamePlayer* pOtherPlayer )->HRESULT {
					pPolicy->PlayerRevealedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pOtherPlayer->GetPlayerID(), pOtherPlayer->GetRole(), PlayerRevealedReason::GameEnd );
					return S_SYSTEM_OK;
				});


				UINT gainedMoney = isWinner ? pItem->WinMoney : pItem->LoseMoney;

				// send game end
				pPolicy->GameEndedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), winner, expGain, gainedMoney, pPlayer->GetRole(), isWinner );

				return S_SYSTEM_OK;
			});


			// bot talking
			svrChkPtr(pBotTalkTbl);
			GetOwner().ForeachPlayer([&](GamePlayer* pPlayerFrom) ->HRESULT
			{
				int randTalkID = 0;
				if (!pPlayerFrom->GetIsBot()) return S_SYSTEM_OK;

				int randTalkPossibility = Util::Random.Rand(1000);
				if (randTalkPossibility > 700) return S_SYSTEM_OK;

				bool isWinner = pPlayerFrom->IsWinnerSide(winner);


				if(isWinner)
					randTalkID = Util::Random.Rand(pBotTalkTbl->GameResultWinner_Begin, pBotTalkTbl->GameResultWinner_End);
				else
					randTalkID = Util::Random.Rand(pBotTalkTbl->GameResultLoser_Begin, pBotTalkTbl->GameResultLoser_End);


				StrUtil::Format(strBuffer, "<chatmsg><n><t>b</t><v>{0}</v></n></chatmsg>", randTalkID);

				GetOwner().GetComponent<GamePlaySystem>()->BroadCastChatMessage(pPlayerFrom, PlayerRole::None, strBuffer);

				return S_SYSTEM_OK;
			});



		Proc_End:

			return hr;
		}

	};


	

	
	const GameStateID GameStateSystem::stm_GameStateFlow[] = {
		GameStateID::None,

		// first night
		GameStateID::FreeDebate,
		//GameStateID::SeersChoice,
		//GameStateID::FreeMason,
		//GameStateID::Owlman,
		//GameStateID::FirstNightVote,
		GameStateID::NightVote,

		// first day
		GameStateID::MorningDebate,
		GameStateID::VoteForSuspects,
		GameStateID::DefenceOfSuspects,
		GameStateID::VoteForHanging,

		// second night
		//GameStateID::SeersChoice,
		//GameStateID::BodyguardsChoice,
		//GameStateID::Owlman,
		//GameStateID::SecondNightVote,
		GameStateID::NightVote,
		//GameStateID::Mythomaniac,

		// second day
		GameStateID::MorningDebate,
		GameStateID::VoteForSuspects,
		GameStateID::DefenceOfSuspects,
		GameStateID::VoteForHanging,

		// third night
		//GameStateID::SeersChoice,
		//GameStateID::BodyguardsChoice,
		//GameStateID::Owlman,
		GameStateID::NightVote,
		//GameStateID::Mythomaniac,

		// third day
		GameStateID::MorningDebate,
		GameStateID::VoteForSuspects,
		GameStateID::DefenceOfSuspects,
		GameStateID::VoteForHanging,
	};

	#define GAMESTATE_INDEX_THIRDDAY_START		12
	#define GAMESTATE_INDEX_MAX					((INT)countof(GameStateSystem::stm_GameStateFlow))


	// Constructor 
	GameStateSystem::GameStateSystem( GameInstanceEntity* pEntity )
		:GameSystem(pEntity)
		,m_GameAdvanceVoted(0)
	{
		memset( m_GamePlayStates, 0, sizeof(m_GamePlayStates) );
		m_GamePlayStates[(UINT)GameStateID::None] = new GamePlayState_None(&GetOwner());
		m_GamePlayStates[(UINT)GameStateID::FreeDebate] = new GamePlayState_FirstFreeDebate(&GetOwner());
		//m_GamePlayStates[(UINT)GameStateID::FirstNightVote] = new GamePlayState_Night(&GetOwner(), GameStateID::FirstNightVote, GameVoteNight::FREEMASON | GameVoteNight::OWLMAN);
		//m_GamePlayStates[(UINT)GameStateID::SecondNightVote] = new GamePlayState_Night(&GetOwner(), GameStateID::SecondNightVote, GameVoteNight::BODYGUARD | GameVoteNight::OWLMAN | GameVoteNight::MEDIUM );
		m_GamePlayStates[(UINT)GameStateID::NightVote] = new GamePlayState_Night(&GetOwner(), GameStateID::NightVote, GameVoteNight::BODYGUARD | GameVoteNight::OWLMAN | GameVoteNight::MEDIUM );
		//m_GamePlayStates[(UINT)GameStateID::Mythomaniac] = new GamePlayState_Mythomaniac(&GetOwner());
		m_GamePlayStates[(UINT)GameStateID::MorningDebate] = new GamePlayState_MorningDebate(&GetOwner());
		m_GamePlayStates[(UINT)GameStateID::VoteForSuspects] = new GamePlayState_VoteForSuspects(&GetOwner());
		m_GamePlayStates[(UINT)GameStateID::DefenceOfSuspects] = new GamePlayState_DefenceOfSuspects(&GetOwner());
		m_GamePlayStates[(UINT)GameStateID::VoteForHanging] = new GamePlayState_VoteForHanging(&GetOwner());
		m_GamePlayStates[(UINT)GameStateID::End] = new GamePlayState_End(&GetOwner());

	}

	GameStateSystem::~GameStateSystem()
	{
		for( int iState = 0; iState < (INT)GameStateID::Max; iState++ )
		{
			Util::SafeDelete( m_GamePlayStates[iState] );
		}
	}


	// Initialzie system
	HRESULT GameStateSystem::InitializeComponent()
	{
		m_IsInStateChanging = false;
		m_GameAdvanceVoted = 0;
		m_CurrentGameStateIndex = 0;
		m_CurrentDay = 1;

		if( m_CurrentGameState != GameStateID::None )
		{
			// Do not call leave when the state is reset
			//m_GamePlayStates[m_CurrentGameState]->OnLeave();
			m_CurrentGameState = GameStateID::None;
			m_GamePlayStates[(UINT)m_CurrentGameState]->OnEnter();
		}

		return super::InitializeComponent();
	}

	// Update system
	HRESULT GameStateSystem::UpdateSystem()
	{
		return m_GamePlayStates[(UINT)m_CurrentGameState]->OnUpdate();
	}

	// Advance State if can
	HRESULT GameStateSystem::AdvanceState()
	{
		HRESULT hr = S_SYSTEM_OK;

		if( !m_GamePlayStates[(UINT)m_CurrentGameState]->CanAdvanceToNext() )
			return E_GAME_NOT_READY_FOR_NEXT_STATE;

		{
		LockStateChangingScope stateChangingLock(m_IsInStateChanging);

		svrChk( m_GamePlayStates[(UINT)m_CurrentGameState]->OnLeave() );

		// reset game advance vote count
		m_GameAdvanceVoted = 0;

		// If a player died the game play state already changed, we need to check wether it can be advanced or not
		if( !m_GamePlayStates[(UINT)m_CurrentGameState]->CanAdvanceToNext() )
		{
			return S_SYSTEM_OK;
		}

		GameWinner winner = GetOwner().GetComponent<GamePlaySystem>()->GetGameWinner();
		if( winner == GameWinner::None )
		{

			do {
				m_CurrentGameStateIndex++;
				//int Max = GAMESTATE_INDEX_MAX;
				//int ThirdDay = GAMESTATE_INDEX_THIRDDAY_START;
				if( m_CurrentGameStateIndex >= GAMESTATE_INDEX_MAX )
					m_CurrentGameStateIndex = GAMESTATE_INDEX_THIRDDAY_START;

				m_CurrentGameState = stm_GameStateFlow[m_CurrentGameStateIndex];
			}while( !m_GamePlayStates[(UINT)m_CurrentGameState]->CanBeEntered() );

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameStateChange(Util::Time.GetTimeUTCSec(), m_CurrentGameState) );
		}
		else
		{
			//SetGameEnd(winner);
			m_CurrentGameState = GameStateID::End;

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameEnd(Util::Time.GetTimeUTCSec(), winner ) );
		}

		hr = m_GamePlayStates[(UINT)m_CurrentGameState]->OnEnter();
		if(FAILED(hr)) svrErr(hr);

		}
		//svrChk( GetOwner().GetComponent<GamePlaySystem>()->UpdateGameEnd() );

	Proc_End:

		return hr;
	}

	// Vote game advance
	HRESULT GameStateSystem::VoteGameAdvance( GamePlayer* pVoter )
	{
		HRESULT hr = S_SYSTEM_OK;

		if( pVoter == nullptr )
			return E_SYSTEM_INVALIDARG;

		if( !pVoter->GetIsActivePlayer() )
			return E_GAME_INVALID_PLAYER_STATE;

		// state with vote can't advance
		if( !m_GamePlayStates[(UINT)m_CurrentGameState]->CanAdvanceToNext() )
			return E_GAME_INVALID_GAMESTATE;

		if( pVoter->GetVotedGameAdvance() )
			return hr;

		pVoter->SetVotedGameAdvance(true);
		m_GameAdvanceVoted++;
		GetOwner().ForeachPlayerSvrGameInstance( [&](GamePlayer *pPlayer, Policy::ISvrPolicyGameInstance *pPolicy)
		{
			pPolicy->GameAdvanceVotedS2CEvt( BR::RouteContext(GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pVoter->GetPlayerID() );
			return S_SYSTEM_OK;
		});

		// if the majority of the people are voted
		if( (m_GameAdvanceVoted*3) >= (GetOwner().GetNumPlayer()*2) )
		{
			AdvanceState();
		}

	//Proc_End:

		return hr;
	}

	// process vote
	HRESULT GameStateSystem::Vote( GamePlayer* pVoter, GamePlayer* pPlayer )
	{
		HRESULT hr = S_SYSTEM_OK;

		hr = m_GamePlayStates[(UINT)m_CurrentGameState]->Vote(pVoter,pPlayer);
		if( hr != E_GAME_INVALID_PLAYER_STATE )
			svrChk(hr);

		svrChk( GetOwner().GetComponent<GameLogSystem>()->UpdateGameVote( pVoter->GetPlayerID(), pPlayer->GetPlayerID() ) );

		svrChk( GetOwner().GetComponent<GameStateSystem>()->AdvanceStateIfCan() );

	Proc_End:

		return hr;
	}

	// Update state if can
	HRESULT GameStateSystem::AdvanceStateIfCan()
	{
		HRESULT hr = S_SYSTEM_OK;

		if( m_GamePlayStates[(UINT)m_CurrentGameState]->CanAdvanceToNext() )
		{
			svrChk( AdvanceState() );
		}

	Proc_End:

		return hr;
	}

	
	// Set game end
	HRESULT GameStateSystem::SetGameEnd(GameWinner winner)
	{
		HRESULT hr = S_SYSTEM_OK;

		if( m_CurrentGameState == GameStateID::End )
			return S_SYSTEM_OK;

		{
		LockStateChangingScope stateChangingLock(m_IsInStateChanging);

		m_GamePlayStates[(UINT)m_CurrentGameState]->OnLeave();

		m_CurrentGameState = GameStateID::End;

		m_GamePlayStates[(UINT)m_CurrentGameState]->OnEnter();

		}

		svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameEnd(Util::Time.GetTimeUTCSec(), winner ) );

	Proc_End:

		return hr;
	}

	// Start Game
	HRESULT GameStateSystem::OnStartGame()
	{
		HRESULT hr = S_SYSTEM_OK;

		{
		LockStateChangingScope stateChangingLock(m_IsInStateChanging);

		// Change game state to Free debate
		svrChk( m_GamePlayStates[(UINT)m_CurrentGameState]->OnLeave() );

		GetOwner().GetComponent<GameLogSystem>()->ClearGameLog();

		m_CurrentDay = 1;
		m_GameAdvanceVoted = 0;
		m_CurrentGameStateIndex = 1;
		m_CurrentGameState = GameStateID::FreeDebate;

		svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameStateChange(Util::Time.GetTimeUTCSec(), m_CurrentGameState) );

		hr = m_GamePlayStates[(UINT)m_CurrentGameState]->OnEnter();
		if(FAILED(hr)) svrErr(hr);
		}

		svrChk( GetOwner().GetComponent<GamePlaySystem>()->UpdateGameEnd() );

	Proc_End:

		return hr;
	}

	// Called when a player get out of game
	HRESULT GameStateSystem::OnPlayerGetOutOfGame( GamePlayer* pPlayer )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk( m_GamePlayStates[(UINT)m_CurrentGameState]->OnPlayerGetOutOfGame( pPlayer ) );

	Proc_End:

		return hr;
	}

}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







