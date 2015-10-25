////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Log system
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/TypeDefs.h"
#include "Common/BrGameTypes.h"
#include "Common/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/GameLog/GameLog.h"
#include "Common/ArrayUtil.h"
#include "Common/GameConst.h"
#include "GameSystemComponentIDs.h"

namespace BR {
namespace ConspiracyGameInstanceServer {

	class GameInstanceEntity;
	class GamePlayer;


	//////////////////////////////////////////////////////////////////////////
	//
	//	GameLogSystem class
	//



	class GameLogSystem : public Svr::GameSystem<GameInstanceEntity,GameLogSystem>, public Svr::GameLog<GameConst::MAX_GAMELOG_BUFFER,GameConst::MAX_GAMELOG>
	{
	public:
		enum {
			ComponentID = GameSystemComponentID_GameLog,

			
		};

	private:
		
		// Current voting item
		GameLogVote *m_CurrentVoteLogItem;

	public:
		
		// Constructor 
		GameLogSystem( GameInstanceEntity* pEntity );
		~GameLogSystem();

		// Initialize server component
		virtual HRESULT InitializeComponent() override;

		virtual void TerminateComponent() override;

		void ClearGameLog();

		// Add log items
		HRESULT AddGameStateChange( ULONG timeStamp, GameStateID gameState );

		HRESULT AddGameVote( ULONG timeStamp, GameVoteType type, UINT voterCount );
		HRESULT UpdateGameVote( PlayerID voter, PlayerID voted );

		HRESULT AddGameVoteResult( ULONG timeStamp, UINT numRankers, const PlayerID* ranker );

		HRESULT AddGamePlayerKilled( ULONG timeStamp, PlayerKilledReason reason, PlayerID killedPlayerID );

		HRESULT AddGameEnd( ULONG timeStamp, GameWinner winner );

	};



}; // namespace ConspiracyGameInstanceServer
}; // namespace BR






