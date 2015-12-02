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

#include "Common/TypeDefs.h"
#include "Common/BrGameTypes.h"
#include "Common/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/GameSystem.h"
#include "GameSystem/GameVote.h"
#include "Common/ArrayUtil.h"
#include "Common/GameConst.h"
#include "GameSystemComponentIDs.h"

namespace BR {
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
		BRCLASS_ATTRIBUTE_READONLY(GameStateID,GameState);

		GameInstanceEntity* m_Owner;

		// State start time
		TimeStampMS m_StateStartTime;
		TimeStampSec m_StateStartTimeUTC;

	

	public:
		GamePlayState(GameInstanceEntity* Owner, GameStateID gameState)
			:m_Owner(Owner),m_GameState(gameState), m_StateStartTime(TimeStampMS(DurationMS(0)))
		{}
		virtual ~GamePlayState() {}

		// Get time in this state in ms
		FORCEINLINE DurationMS GetTimeInState()						{ return Util::Time.GetTimeMs() - m_StateStartTime; }
		FORCEINLINE TimeStampSec GetStateTimeUTC()					{ return m_StateStartTimeUTC; }

		GamePlaySystem& GetGamePlaySystem();
		GameStateSystem& GetGameStateSystem();
		FORCEINLINE GameInstanceEntity& GetOwner()					{ return *m_Owner; }

		virtual HRESULT Vote(GamePlayer* pVoter, GamePlayer* pVoteTarget )		{ return E_GAME_INVALID_VOTE_STATE; }

		// Check whether it can be proceeded or not
		virtual bool CanAdvanceToNext()								{ return true; }

		virtual bool CanBeEntered()									{ return true; }

		// regular state change event handling
		virtual HRESULT OnPlayerGetOutOfGame( GamePlayer* pPlayer ) { return S_OK; }
		virtual HRESULT OnEnter();
		virtual HRESULT OnUpdate()									{ return S_OK; }
		virtual HRESULT OnLeave()									{ return S_OK; }

		friend class GamePlaySystem;
	};

	

	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	class GameStateSystem : public Svr::GameSystem<GameInstanceEntity,GameStateSystem>
	{
	public:
		enum {
			ComponentID = GameSystemComponentID_GameState,
		};


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
		BRCLASS_ATTRIBUTE_READONLY(GameStateID,CurrentGameState);
		BRCLASS_ATTRIBUTE_READONLY(UINT,CurrentGameStateIndex);
		GamePlayState* m_GamePlayStates[GameStateID::Max];

		BRCLASS_ATTRIBUTE_READONLY(UINT,CurrentDay);

		// number of player who voted to game advance
		BRCLASS_ATTRIBUTE_READONLY(UINT,GameAdvanceVoted);

		BRCLASS_ATTRIBUTE_READONLY(bool,IsInStateChanging);

	public:

		// Constructor 
		GameStateSystem( GameInstanceEntity* pEntity );
		~GameStateSystem();

		// Get current game play state
		FORCEINLINE GamePlayState* GetCurrentGamePlayState();
		FORCEINLINE GameStateID GetCurrentGameState();

		// increase day
		FORCEINLINE void NextDay();

		// Initialzie system
		HRESULT InitializeComponent();

		// Update system
		HRESULT UpdateSystem();

		// Advance State if can
		HRESULT AdvanceState();

		// Vote game advance
		HRESULT VoteGameAdvance( GamePlayer* pVoter );

		// process vote
		HRESULT Vote( GamePlayer* pVoter, GamePlayer* pPlayer );

		// Update state if can, no error is generated even if the state can't be advanced
		HRESULT AdvanceStateIfCan();

		// Set game end
		HRESULT SetGameEnd(GameWinner winner);

		// Start Game
		HRESULT OnStartGame();
		
		// Called when a player get out of game
		HRESULT OnPlayerGetOutOfGame( GamePlayer* pPlayer );
	};


	
	GamePlayState* GameStateSystem::GetCurrentGamePlayState()
	{
		return m_GamePlayStates[(UINT)m_CurrentGameState];
	}

	GameStateID GameStateSystem::GetCurrentGameState()
	{
		return m_CurrentGameState;
	}

	void GameStateSystem::NextDay()
	{
		m_CurrentDay++;
	}

}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







