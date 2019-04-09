////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
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
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "ConspiracyGameInstanceServerClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"
#include "GameSystem/GameLogSystem.h"
#include "GameSystem/GameVote.h"
#include "GameInstance/GameInstanceEntity.h"

#include "Table/conspiracy/RewardTbl.h"



SF_MEMORYPOOL_IMPLEMENT(BR::ConspiracyGameInstanceServer::GameStateSystem);


namespace SF {
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

	Result GamePlayState::OnEnter()
	{
		m_StateStartTime = Util::Time.GetTimeMs();
		m_StateStartTimeUTC = Util::Time.GetTimeUTCSec();

		uint day = GetOwner().GetComponent<GameStateSystem>()->GetCurrentDay();
		GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance &pPolicy )->Result
		{
			pPlayer->SetVotedGameAdvance(false);

			if( pPlayer->GetPlayerEntityUID().UID != 0 )
				pPolicy.GameAdvancedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), m_StateStartTimeUTC.time_since_epoch().count(), (uint8_t)GetGameState(), (uint8_t)day );

			return ResultCode::SUCCESS;
		});

		return ResultCode::SUCCESS;
	}


	class GamePlayState_None : public GamePlayState
	{
	public:
		GamePlayState_None(GameInstanceEntity* Owner):GamePlayState(Owner,GameStateID::None) {}
		virtual ~GamePlayState_None() {}

		virtual bool CanAdvanceToNext() override				{ return false; }
		virtual bool CanBeEntered() override					{ return false; }

		Result virtual OnEnter() { return GamePlayState::OnEnter(); }
		Result virtual OnLeave() { return GamePlayState::OnLeave(); }
	};

	
	// Game state with time limit
	class GamePlayState_TimeLimit : public GamePlayState
	{
	protected:
		Util::TimeStampTimer m_TimeToNext;

	public:
		GamePlayState_TimeLimit(GameInstanceEntity* Owner,GameStateID state):GamePlayState(Owner,state) {}
		virtual ~GamePlayState_TimeLimit() {}

		virtual Result OnEnter()
		{
			m_TimeToNext.SetTimer( DurationMS(3*60*1000) );
			return GamePlayState::OnEnter();
		}
		virtual Result OnUpdate()
		{
			m_TimeToNext.CheckTimer();
			return GamePlayState::OnUpdate();
		}
		virtual Result OnLeave()
		{
			m_TimeToNext.ClearTimer();
			return GamePlayState::OnLeave();
		}
	};


	class GamePlayState_TimeLimitBotTalk : public GamePlayState_TimeLimit
	{
	protected:
		Util::TimeStampTimer m_TimeToNextBotTalk;
		int m_BotTalkBegin;
		int m_BotTalkEnd;
		float m_PlayDelay;
		float m_BotDelay1;
		float m_BotDelay2;
		int m_BotIndex;

	public:
		GamePlayState_TimeLimitBotTalk(GameInstanceEntity* Owner, GameStateID state) :GamePlayState_TimeLimit(Owner, state) {}
		virtual ~GamePlayState_TimeLimitBotTalk() {}

		virtual Result OnEnter()
		{
			m_BotIndex = 0;

			float delay = ((m_BotDelay2 - m_BotDelay1) * (float)Util::Random.Rand(0, 100)) / 100.0f;
			m_TimeToNextBotTalk.SetTimer(DurationMS((int)((m_PlayDelay + delay) * 1000)));
			return GamePlayState_TimeLimit::OnEnter();
		}

		virtual Result OnUpdate()
		{
			if (m_TimeToNextBotTalk.CheckTimer())
			{
				BroadCastRandomBotMessage(m_BotTalkBegin, m_BotTalkEnd, m_BotIndex);

				float delay = ((m_BotDelay2 - m_BotDelay1) * (float)Util::Random.Rand(0, 100)) / 100.0f;
				m_TimeToNextBotTalk.SetTimer(DurationMS((int)(delay * 1000)));
			}
			return GamePlayState_TimeLimit::OnUpdate();
		}

		virtual Result OnLeave()
		{
			m_TimeToNextBotTalk.ClearTimer();
			return GamePlayState_TimeLimit::OnLeave();
		}

		virtual Result BroadCastRandomBotMessage(int minID, int maxID, int& index)
		{
			return GetGamePlaySystem().BroadCastRandomBotMessage(m_BotTalkBegin, m_BotTalkEnd, m_BotIndex);
		}
	};


	class GamePlayState_FirstFreeDebate : public GamePlayState_TimeLimitBotTalk
	{
	private:

	public:
		GamePlayState_FirstFreeDebate(GameInstanceEntity* Owner):GamePlayState_TimeLimitBotTalk(Owner,GameStateID::FreeDebate)
		{
			auto pBotTalkTbl = GetOwner().GetBotTalkTbl();
			m_BotTalkBegin = pBotTalkTbl->FirstDay_Begin;
			m_BotTalkEnd = pBotTalkTbl->FirstDay_End;
			m_PlayDelay = pBotTalkTbl->FirstDay_PlayDelay;
			m_BotDelay1 = pBotTalkTbl->FirstDay_BotDelay1;
			m_BotDelay2 = pBotTalkTbl->FirstDay_BotDelay2;
		}

		virtual ~GamePlayState_FirstFreeDebate() {}

		virtual Result OnEnter()
		{
			Result hr = ResultCode::SUCCESS;

			svrChk(GamePlayState_TimeLimitBotTalk::OnEnter() );

			GetGamePlaySystem().SetHuntedPlayer(0);

			m_TimeToNext.SetTimerFunc( [&](){ GetGameStateSystem().AdvanceState(); } );
			m_TimeToNext.SetTimer( DurationMS(GetOwner().GetPresetGameConfig()->FreeDiscussion*1000) );

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
		GamePlayState_Night(GameInstanceEntity* Owner,GameStateID gameState,uint votingFlags)
			:GamePlayState_TimeLimit(Owner,gameState)
			,m_vote(Owner,votingFlags)
		{}
		virtual ~GamePlayState_Night() {}
		
		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext() override
		{
			return m_vote.IsVoteEnd();
		}

		virtual Result Vote(GamePlayer* pVoter, GamePlayer* pVoteTarget ) override
		{
			return m_vote.Vote( pVoter, pVoteTarget );
		}

		virtual Result OnUpdate() override
		{
			m_vote.UpdateVote(GetTimeInState());
			return GamePlayState_TimeLimit::OnUpdate();
		}

		virtual Result OnEnter() override
		{
			Result hr = ResultCode::SUCCESS;
			uint numVotePlayer;

			auto stateTime = GetGameStateSystem().GetCurrentDay() == 1 ? 
				GetOwner().GetPresetGameConfig()->RolePlayTime : 
				GetOwner().GetPresetGameConfig()->RolePlayAndKillingTime;

			svrChk(GamePlayState_TimeLimit::OnEnter() );

			// Set vote timer
			m_TimeToNext.SetTimerFunc( [&](){ m_vote.ForceAllVoted(); } );
			m_TimeToNext.SetTimer(DurationMS(stateTime * 1000));


			// Reveal to medium 
			if( GetGamePlaySystem().GetLynchedPlayer() != 0 && GetGamePlaySystem().GetLynchedRole() != PlayerRole::None ) // except the first night, the lynched role will be specified
			{
				GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance &pPolicy )->Result {
					if( pPlayer->GetRole() != PlayerRole::Medium )
						return ResultCode::SUCCESS;

					if( pPlayer->GetPlayerEntityUID().UID != 0 )
						pPolicy.PlayerRevealedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), GetGamePlaySystem().GetLynchedPlayer(), (uint8_t)GetGamePlaySystem().GetLynchedRole(), (uint8_t)PlayerRevealedReason::Medium );

					return ResultCode::FAIL;
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
		//virtual Result OnUpdate()	{ return ResultCode::SUCCESS; }
		virtual Result OnLeave() override
		{
			Result hr = ResultCode::SUCCESS;

			// vote system will send result when he voted
			// process seer
			//if( GetGamePlaySystem().GetSeer() != 0 && m_vote.GetSeersChoice() != 0 )
			//{
			//	GamePlayer *pSeers = nullptr;
			//	GamePlayer *pSeersChoice = nullptr;
			//	svrChk( GetOwner().FindPlayer( GetGamePlaySystem().GetSeer(), pSeers ) );
			//	svrChk( GetOwner().FindPlayer( m_vote.GetSeersChoice(), pSeersChoice ) );

			//	pSeersChoice->SetRevealedBySeer(true);
			//	auto pPolicy = pSeers->GetInterface<Policy::NetSvrPolicyGameInstance>();
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
					GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance &pPolicy )->Result {
						if( pPlayer->GetPlayerEntityUID().UID != 0 )
							pPolicy.PlayerKilledS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), m_vote.GetPlayerToKill(), (uint8_t)PlayerKilledReason::BlockedByBodyguard );
						return ResultCode::SUCCESS;
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


	class GamePlayState_MorningDebate : public GamePlayState_TimeLimitBotTalk
	{
	private:

	public:
		GamePlayState_MorningDebate(GameInstanceEntity* Owner):GamePlayState_TimeLimitBotTalk(Owner,GameStateID::MorningDebate)
		{
			auto pBotTalkTbl = GetOwner().GetBotTalkTbl();
			m_BotTalkBegin = pBotTalkTbl->FreeTalk_Begin;
			m_BotTalkEnd = pBotTalkTbl->FreeTalk_End;
			m_PlayDelay = pBotTalkTbl->FreeTalk_PlayDelay;
			m_BotDelay1 = pBotTalkTbl->FreeTalk_BotDelay1;
			m_BotDelay2 = pBotTalkTbl->FreeTalk_BotDelay2;
		}
		virtual ~GamePlayState_MorningDebate() {}
		
		virtual Result OnEnter()
		{
			Result hr = ResultCode::SUCCESS;

			GetOwner().GetComponent<GameStateSystem>()->NextDay();

			svrChk(GamePlayState_TimeLimitBotTalk::OnEnter() );

			// Set vote timer
			m_TimeToNext.SetTimerFunc( [&](){ GetGameStateSystem().AdvanceState(); } );
			m_TimeToNext.SetTimer( DurationMS(GetOwner().GetPresetGameConfig()->MorningDiscussion*1000) );

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

		virtual Result Vote(GamePlayer* pVoter, GamePlayer* pVoteTarget ) override
		{
			return m_vote.Vote( pVoter, pVoteTarget );
		}

		virtual Result OnEnter() override
		{
			Result hr = ResultCode::SUCCESS;
			uint totalAlives;

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

		//virtual Result OnUpdate()	{ return ResultCode::SUCCESS; }
		virtual Result OnLeave() override
		{
			Result hr = ResultCode::SUCCESS;
			
			// put someone who sit next to the latest dead player
			if( m_vote.GetVoteRanker(1) == 0 )
			{
				PlayerID nearPlayerID = GetGamePlaySystem().PickAnyoneCloseToHuntedPlayer(m_vote.GetVoteRanker(0));
				m_vote.SetVoteRanker(nearPlayerID, 1);
			}


			GetGamePlaySystem().SetSuspect(m_vote.GetVoteRanker(0),m_vote.GetVoteRanker(1));

			StaticArray<PlayerID, 2> rankers(GetOwner().GetHeap());
			rankers.push_back(m_vote.GetVoteRanker(0));
			rankers.push_back(m_vote.GetVoteRanker(1));
			//PlayerID rankers[2] = {m_vote.GetVoteRanker(0),m_vote.GetVoteRanker(1)};
			GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance &pPolicy )->Result {
				if( pPlayer->GetPlayerEntityUID().UID != 0 )
					pPolicy.VoteEndS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), rankers );
				return ResultCode::SUCCESS;
			});

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameVoteResult( Util::Time.GetTimeUTCSec(), 2, rankers.data() ) );

		Proc_End:

			return hr;
		}

		virtual Result OnUpdate() override
		{
			m_vote.UpdateVote(GetTimeInState());
			return GamePlayState_TimeLimit::OnUpdate();
		}

	};


	class GamePlayState_DefenceOfSuspects : public GamePlayState_TimeLimitBotTalk
	{
	private:

	public:
		GamePlayState_DefenceOfSuspects(GameInstanceEntity* Owner):GamePlayState_TimeLimitBotTalk(Owner,GameStateID::DefenceOfSuspects)
		{
			auto pBotTalkTbl = GetOwner().GetBotTalkTbl();
			m_BotTalkBegin = pBotTalkTbl->Defense_Begin;
			m_BotTalkEnd = pBotTalkTbl->Defense_End;
			m_PlayDelay = pBotTalkTbl->Defense_PlayDelay;
			m_BotDelay1 = pBotTalkTbl->Defense_BotDelay1;
			m_BotDelay2 = pBotTalkTbl->Defense_BotDelay2;
		}
		virtual ~GamePlayState_DefenceOfSuspects() {}
		
		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext() override		{ return true; }

		virtual Result OnEnter() override
		{
			Result hr = ResultCode::SUCCESS;
			svrChk(GamePlayState_TimeLimitBotTalk::OnEnter() );

			// Set vote timer
			m_TimeToNext.SetTimerFunc( [&](){ GetGameStateSystem().AdvanceState(); } );
			m_TimeToNext.SetTimer(DurationMS(GetOwner().GetPresetGameConfig()->DefenceTime*1000) );

		Proc_End:

			return hr;
		}

		virtual Result BroadCastRandomBotMessage(int minID, int maxID, int& index) override
		{
			return GetGamePlaySystem().BroadCastRandomBotMessageSuspect(m_BotTalkBegin, m_BotTalkEnd, m_BotIndex);
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

		virtual Result Vote(GamePlayer* pVoter, GamePlayer* pVoteTarget ) override
		{
			return m_vote.Vote( pVoter, pVoteTarget );
		}

		virtual Result OnEnter() override
		{
			Result hr = ResultCode::SUCCESS;
			uint totalAlives, totalSuspect;

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

		virtual Result OnLeave() override
		{
			Result hr = ResultCode::SUCCESS;

			// kill the top vote ranker
			if( m_vote.GetPlayerToHang() == 0 )
				return ResultCode::INVALID_PLAYERID;

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGamePlayerKilled( Util::Time.GetTimeUTCSec(), PlayerKilledReason::ByHanging, m_vote.GetPlayerToHang() ) );

			svrChk( GetGamePlaySystem().KillPlayer( m_vote.GetPlayerToHang(), PlayerKilledReason::ByHanging ) );

		Proc_End:

			return hr;
		}
		
		virtual Result OnUpdate() override
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

		
		virtual Result OnEnter() override
		{
			Result hr = ResultCode::SUCCESS;
			GameWinner winner = GetGamePlaySystem().GetGameWinner();
			conspiracy::RewardTbl::RewardItem *pItem = nullptr;
			char strBuffer[1024];
			auto pBotTalkTbl = GetOwner().GetBotTalkTbl();

			svrChk(GamePlayState::OnEnter() );

			// Broad cast game end
			GetOwner().ForeachPlayerSvrGameInstance( [&]( GamePlayer* pPlayer, Policy::NetSvrPolicyGameInstance &pPolicy )->Result {

				uint expGain = 0;
				bool isWinner = pPlayer->IsWinnerSide(winner);

				// Get exp gain by role
				switch( pPlayer->GetRole() )
				{
				default:
				case PlayerRole::None:
					break;
				case PlayerRole::Seer:
					if( !(conspiracy::RewardTbl::FindItem( (int)conspiracy::RewardTbl::ERole::Enum::Seer, pItem )) )
					{
						svrTrace( Error, "Can't find expericend for the seer, no exp for PID:{0}", pPlayer->GetPlayerID() );
						break;
					}
					expGain = isWinner ? pItem->WinExp : pItem->LoseExp;
					break;
				case PlayerRole::Villager:
				case PlayerRole::Medium:
				//case PlayerRole::Bodyguard:
				//case PlayerRole::Owlman:
					if( !(conspiracy::RewardTbl::FindItem( (int)conspiracy::RewardTbl::ERole::Enum::Villager, pItem )) )
					{
						svrTrace( Error, "Can't find expericend for villager side, no exp for PID:{0}", pPlayer->GetPlayerID() );
						break;
					}
					expGain = isWinner ? pItem->WinExp : pItem->LoseExp;
					break;
				case PlayerRole::Werewolf:
				//case PlayerRole::Possessed:
					if (!(conspiracy::RewardTbl::FindItem((int)conspiracy::RewardTbl::ERole::Enum::Monster, pItem)))
					{
						svrTrace( Error, "Can't find expericend for werewolf side, no exp for PID:{0}", pPlayer->GetPlayerID() );
						break;
					}
					expGain = isWinner ? pItem->WinExp : pItem->LoseExp;
					break;
				}

				// Broadcast other player's role to me
				GetOwner().ForeachPlayer( [&]( GamePlayer* pOtherPlayer )->Result {
					pPolicy.PlayerRevealedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pOtherPlayer->GetPlayerID(), (uint8_t)pOtherPlayer->GetRole(), (uint8_t)PlayerRevealedReason::GameEnd );
					return ResultCode::SUCCESS;
				});


				uint gainedMoney = isWinner ? pItem->WinMoney : pItem->LoseMoney;

				// send game end
				pPolicy.GameEndedS2CEvt( RouteContext( GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), (uint8_t)winner, expGain, gainedMoney, (uint8_t)pPlayer->GetRole(), isWinner );

				return ResultCode::SUCCESS;
			});


			// bot talking
			svrChkPtr(pBotTalkTbl);
			GetOwner().ForeachPlayer([&](GamePlayer* pPlayerFrom) ->Result
			{
				int randTalkID = 0;
				if (!pPlayerFrom->GetIsBot()) return ResultCode::SUCCESS;

				int randTalkPossibility = Util::Random.Rand(1000);
				if (randTalkPossibility > 700) return ResultCode::SUCCESS;

				bool isWinner = pPlayerFrom->IsWinnerSide(winner);


				if(isWinner)
					randTalkID = Util::Random.Rand(pBotTalkTbl->GameResultWinner_Begin, pBotTalkTbl->GameResultWinner_End);
				else
					randTalkID = Util::Random.Rand(pBotTalkTbl->GameResultLoser_Begin, pBotTalkTbl->GameResultLoser_End);


				StrUtil::Format(strBuffer, "<chatmsg><n><t>b</t><v>{0}</v></n></chatmsg>", randTalkID);

				GetOwner().GetComponent<GamePlaySystem>()->BroadCastChatMessage(pPlayerFrom, PlayerRole::None, strBuffer);

				return ResultCode::SUCCESS;
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
		m_GamePlayStates[(uint)GameStateID::None] = new(GetHeap()) GamePlayState_None(&GetOwner());
		m_GamePlayStates[(uint)GameStateID::FreeDebate] = new(GetHeap()) GamePlayState_FirstFreeDebate(&GetOwner());
		//m_GamePlayStates[(uint)GameStateID::FirstNightVote] = new(GetHeap()) GamePlayState_Night(&GetOwner(), GameStateID::FirstNightVote, GameVoteNight::FREEMASON | GameVoteNight::OWLMAN);
		//m_GamePlayStates[(uint)GameStateID::SecondNightVote] = new(GetHeap()) GamePlayState_Night(&GetOwner(), GameStateID::SecondNightVote, GameVoteNight::BODYGUARD | GameVoteNight::OWLMAN | GameVoteNight::MEDIUM );
		m_GamePlayStates[(uint)GameStateID::NightVote] = new(GetHeap()) GamePlayState_Night(&GetOwner(), GameStateID::NightVote, GameVoteNight::BODYGUARD | GameVoteNight::OWLMAN | GameVoteNight::MEDIUM );
		//m_GamePlayStates[(uint)GameStateID::Mythomaniac] = new(GetHeap()) GamePlayState_Mythomaniac(&GetOwner());
		m_GamePlayStates[(uint)GameStateID::MorningDebate] = new(GetHeap()) GamePlayState_MorningDebate(&GetOwner());
		m_GamePlayStates[(uint)GameStateID::VoteForSuspects] = new(GetHeap()) GamePlayState_VoteForSuspects(&GetOwner());
		m_GamePlayStates[(uint)GameStateID::DefenceOfSuspects] = new(GetHeap()) GamePlayState_DefenceOfSuspects(&GetOwner());
		m_GamePlayStates[(uint)GameStateID::VoteForHanging] = new(GetHeap()) GamePlayState_VoteForHanging(&GetOwner());
		m_GamePlayStates[(uint)GameStateID::End] = new(GetHeap()) GamePlayState_End(&GetOwner());

	}

	GameStateSystem::~GameStateSystem()
	{
		for( int iState = 0; iState < (INT)GameStateID::Max; iState++ )
		{
			Util::SafeDelete( m_GamePlayStates[iState] );
		}
	}

	IHeap& GameStateSystem::GetHeap() 
	{
		return GetOwner().GetHeap();
	}

	// Initialize system
	Result GameStateSystem::InitializeComponent()
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
			m_GamePlayStates[(uint)m_CurrentGameState]->OnEnter();
		}

		return super::InitializeComponent();
	}

	// Update system
	Result GameStateSystem::UpdateSystem()
	{
		return m_GamePlayStates[(uint)m_CurrentGameState]->OnUpdate();
	}

	// Advance State if can
	Result GameStateSystem::AdvanceState()
	{
		Result hr = ResultCode::SUCCESS;

		if( !m_GamePlayStates[(uint)m_CurrentGameState]->CanAdvanceToNext() )
			return ResultCode::GAME_NOT_READY_FOR_NEXT_STATE;

		{
		LockStateChangingScope stateChangingLock(m_IsInStateChanging);

		svrChk( m_GamePlayStates[(uint)m_CurrentGameState]->OnLeave() );

		// reset game advance vote count
		m_GameAdvanceVoted = 0;

		// If a player died the game play state already changed, we need to check wether it can be advanced or not
		if( !m_GamePlayStates[(uint)m_CurrentGameState]->CanAdvanceToNext() )
		{
			return ResultCode::SUCCESS;
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
			}while( !m_GamePlayStates[(uint)m_CurrentGameState]->CanBeEntered() );

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameStateChange(Util::Time.GetTimeUTCSec(), m_CurrentGameState) );
		}
		else
		{
			//SetGameEnd(winner);
			m_CurrentGameState = GameStateID::End;

			svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameEnd(Util::Time.GetTimeUTCSec(), winner ) );
		}

		hr = m_GamePlayStates[(uint)m_CurrentGameState]->OnEnter();
		if(!(hr)) svrErr(hr);

		}
		//svrChk( GetOwner().GetComponent<GamePlaySystem>()->UpdateGameEnd() );

	Proc_End:

		return hr;
	}

	// Vote game advance
	Result GameStateSystem::VoteGameAdvance( GamePlayer* pVoter )
	{
		Result hr = ResultCode::SUCCESS;

		if( pVoter == nullptr )
			return ResultCode::INVALID_ARG;

		if( !pVoter->GetIsActivePlayer() )
			return ResultCode::GAME_INVALID_PLAYER_STATE;

		// state with vote can't advance
		if( !m_GamePlayStates[(uint)m_CurrentGameState]->CanAdvanceToNext() )
			return ResultCode::GAME_INVALID_GAMESTATE;

		if( pVoter->GetVotedGameAdvance() )
			return hr;

		pVoter->SetVotedGameAdvance(true);
		m_GameAdvanceVoted++;
		GetOwner().ForeachPlayerSvrGameInstance( [&](GamePlayer *pPlayer, Policy::NetSvrPolicyGameInstance &pPolicy)
		{
			pPolicy.GameAdvanceVotedS2CEvt( RouteContext(GetOwner().GetEntityUID(), pPlayer->GetPlayerEntityUID()), pVoter->GetPlayerID() );
			return ResultCode::SUCCESS;
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
	Result GameStateSystem::Vote( GamePlayer* pVoter, GamePlayer* pPlayer )
	{
		Result hr = ResultCode::SUCCESS;

		hr = m_GamePlayStates[(uint)m_CurrentGameState]->Vote(pVoter,pPlayer);
		if( hr != Result(ResultCode::GAME_INVALID_PLAYER_STATE) )
			svrChk(hr);

		svrChk( GetOwner().GetComponent<GameLogSystem>()->UpdateGameVote( pVoter->GetPlayerID(), pPlayer->GetPlayerID() ) );

		svrChk( GetOwner().GetComponent<GameStateSystem>()->AdvanceStateIfCan() );

	Proc_End:

		return hr;
	}

	// Update state if can
	Result GameStateSystem::AdvanceStateIfCan()
	{
		Result hr = ResultCode::SUCCESS;

		if( m_GamePlayStates[(uint)m_CurrentGameState]->CanAdvanceToNext() )
		{
			svrChk( AdvanceState() );
		}

	Proc_End:

		return hr;
	}

	
	// Set game end
	Result GameStateSystem::SetGameEnd(GameWinner winner)
	{
		Result hr = ResultCode::SUCCESS;

		if( m_CurrentGameState == GameStateID::End )
			return ResultCode::SUCCESS;

		{
		LockStateChangingScope stateChangingLock(m_IsInStateChanging);

		m_GamePlayStates[(uint)m_CurrentGameState]->OnLeave();

		m_CurrentGameState = GameStateID::End;

		m_GamePlayStates[(uint)m_CurrentGameState]->OnEnter();

		}

		svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameEnd(Util::Time.GetTimeUTCSec(), winner ) );

	Proc_End:

		return hr;
	}

	// Start Game
	Result GameStateSystem::OnStartGame()
	{
		Result hr = ResultCode::SUCCESS;

		{
		LockStateChangingScope stateChangingLock(m_IsInStateChanging);

		// Change game state to Free debate
		svrChk( m_GamePlayStates[(uint)m_CurrentGameState]->OnLeave() );

		GetOwner().GetComponent<GameLogSystem>()->ClearGameLog();

		m_CurrentDay = 1;
		m_GameAdvanceVoted = 0;
		m_CurrentGameStateIndex = 1;
		m_CurrentGameState = GameStateID::FreeDebate;

		svrChk( GetOwner().GetComponent<GameLogSystem>()->AddGameStateChange(Util::Time.GetTimeUTCSec(), m_CurrentGameState) );

		hr = m_GamePlayStates[(uint)m_CurrentGameState]->OnEnter();
		if(!(hr)) svrErr(hr);
		}

		svrChk( GetOwner().GetComponent<GamePlaySystem>()->UpdateGameEnd() );

	Proc_End:

		return hr;
	}

	// Called when a player get out of game
	Result GameStateSystem::OnPlayerGetOutOfGame( GamePlayer* pPlayer )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( m_GamePlayStates[(uint)m_CurrentGameState]->OnPlayerGetOutOfGame( pPlayer ) );

	Proc_End:

		return hr;
	}

}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







