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

#include "SFTypedefs.h"
#include "Types/BrGameTypes.h"
#include "MemoryManager/SFMemoryPool.h"

#include "ServiceEntity/Game/GameSystem.h"
#include "ServerSystem/GameLog/GameLog.h"
#include "Container/SFArray.h"
#include "GameConst.h"
#include "GameSystemComponentIDs.h"

namespace SF {
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
		static constexpr StringCrc32 ComponentID = "GameLog"_crc32c;

	private:
		typedef Svr::GameSystem<GameInstanceEntity, GameLogSystem> superGameSystem;
		
		// Current voting item
		GameLogVote *m_CurrentVoteLogItem;

	public:
		
		// Constructor 
		GameLogSystem( GameInstanceEntity* pEntity );
		~GameLogSystem();

		// Initialize server component
		virtual Result InitializeComponent() override;

		virtual void TerminateComponent() override;

		void ClearGameLog();

		// Add log items
		Result AddGameStateChange(UTCTimeStampSec timeStamp, GameStateID gameState );

		Result AddGameVote(UTCTimeStampSec timeStamp, GameVoteType type, uint voterCount );
		Result UpdateGameVote( PlayerID voter, PlayerID voted );

		Result AddGameVoteResult(UTCTimeStampSec timeStamp, uint numRankers, const PlayerID* ranker );

		Result AddGamePlayerKilled(UTCTimeStampSec timeStamp, PlayerKilledReason reason, PlayerID killedPlayerID );

		Result AddGameEnd(UTCTimeStampSec timeStamp, GameWinner winner );

	};



}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







