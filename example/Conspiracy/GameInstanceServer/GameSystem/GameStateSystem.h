////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Types/BrGameTypes.h"
#include "MemoryManager/SFMemoryPool.h"

#include "ServiceEntity/Game/GameSystem.h"
#include "GameSystem/GameVote.h"
#include "Container/SFArray.h"
#include "GameConst.h"
#include "GameSystemComponentIDs.h"

namespace SF {
namespace ConspiracyGameInstanceServer {

	class GameInstanceEntity;
	class GamePlayer;
	class GamePlaySystem;
	class GameStateSystem;


	//////////////////////////////////////////////////////////////////////////
	//
	//	GameState class
	//

	class GamePlayState
	{
	public:

	private:
		GameInstanceEntity* m_Owner;

		GameStateID m_GameState;

		// State start time
		UTCTimeStampMS m_StateStartTimeUTC;

	

	public:
		GamePlayState(GameInstanceEntity* Owner, GameStateID gameState)
			: m_Owner(Owner)
			, m_GameState(gameState)
		{}
		virtual ~GamePlayState() {}


		GameStateID GetGameState() { return m_GameState; }
		void SetGameState(GameStateID value) { m_GameState = value; }


		// Get time in this state in ms
		DurationMS GetTimeInState()						{ return Util::TimeSinceUTC(m_StateStartTimeUTC); }
		UTCTimeStampMS GetStateTimeUTC()					{ return m_StateStartTimeUTC; }

		GamePlaySystem& GetGamePlaySystem();
		GameStateSystem& GetGameStateSystem();
		GameInstanceEntity& GetOwner()					{ return *m_Owner; }

		virtual Result Vote(GamePlayer* pVoter, GamePlayer* pVoteTarget )		{ return ResultCode::GAME_INVALID_VOTE_STATE; }

		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext()								{ return true; }

		virtual bool CanBeEntered()									{ return true; }

		// regular state change event handling
		virtual Result OnPlayerGetOutOfGame( GamePlayer* pPlayer ) { return ResultCode::SUCCESS; }
		virtual Result OnEnter();
		virtual Result OnUpdate()									{ return ResultCode::SUCCESS; }
		virtual Result OnLeave()									{ return ResultCode::SUCCESS; }

		friend class GamePlaySystem;
	};

	

	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	class GameStateSystem : public Svr::GameSystem<GameInstanceEntity,GameStateSystem>
	{
	public:
		
		static constexpr StringCrc32 ComponentID = "GameStateSystem"_crc32c;


	private:
		typedef Svr::GameSystem<GameInstanceEntity, GameStateSystem> super;

		// game state flow table
		static const GameStateID stm_GameStateFlow[];

		class LockStateChangingScope
		{
			bool &m_IsInStateChanging;
		public:
			LockStateChangingScope( bool &IsInStateChanging)
				: m_IsInStateChanging(IsInStateChanging)		{ Assert(!m_IsInStateChanging); m_IsInStateChanging = true; }
			~LockStateChangingScope()							{ m_IsInStateChanging = false; }
		};

		// Game state
		GameStateID m_CurrentGameState;
		uint m_CurrentGameStateIndex;
		GamePlayState* m_GamePlayStates[(int)GameStateID::Max];

		uint m_CurrentDay;

		// number of player who voted to game advance
		uint m_GameAdvanceVoted;

		bool m_IsInStateChanging;

	public:

		// Constructor 
		GameStateSystem( GameInstanceEntity* pEntity );
		~GameStateSystem();

		IHeap& GetHeap();

		GameStateID GetCurrentGameState() { return m_CurrentGameState; }

		uint GetCurrentDay() { return m_CurrentDay; }

		uint GetGameAdvanceVoted() { return m_GameAdvanceVoted; }
		uint GetIsInStateChanging() { return m_IsInStateChanging; }


		// Get current game play state
		GamePlayState* GetCurrentGamePlayState();

		// increase day
		void NextDay();

		// Initialzie system
		Result InitializeComponent();

		// Update system
		Result UpdateSystem();

		// Advance State if can
		Result AdvanceState();

		// Vote game advance
		Result VoteGameAdvance( GamePlayer* pVoter );

		// process vote
		Result Vote( GamePlayer* pVoter, GamePlayer* pPlayer );

		// Update state if can, no error is generated even if the state can't be advanced
		Result AdvanceStateIfCan();

		// Set game end
		Result SetGameEnd(GameWinner winner);

		// Start Game
		Result OnStartGame();
		
		// Called when a player get out of game
		Result OnPlayerGetOutOfGame( GamePlayer* pPlayer );
	};


	
	inline GamePlayState* GameStateSystem::GetCurrentGamePlayState()
	{
		return m_GamePlayStates[(uint)m_CurrentGameState];
	}

	inline void GameStateSystem::NextDay()
	{
		m_CurrentDay++;
	}

}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







