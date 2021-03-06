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


#include "GameSystem/GameLogSystem.h"
#include "GameInstance/GameInstanceEntity.h"




namespace SF {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	constexpr StringCrc32 GameLogSystem::ComponentID;

	// Constructor 
	GameLogSystem::GameLogSystem( GameInstanceEntity* pEntity )
		: GameSystem(pEntity)
		, GameLog(GetHeap())
		, m_CurrentVoteLogItem(nullptr)
	{
	}

	GameLogSystem::~GameLogSystem()
	{
	}
	

	// Initialize system
	Result GameLogSystem::InitializeComponent()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( InitializeLog() );

		m_CurrentVoteLogItem = nullptr;

		svrChk(superGameSystem::InitializeComponent() );

	Proc_End:

		return hr;
	}

	void GameLogSystem::TerminateComponent()
	{
		TerminateLog();

		m_CurrentVoteLogItem = nullptr;

		superGameSystem::TerminateComponent();
	}

	void GameLogSystem::ClearGameLog()
	{
		m_CurrentVoteLogItem = nullptr;
		InitializeLog();
	}

	// Add log items
	Result GameLogSystem::AddGameStateChange(UTCTimeStampSec timeStamp, GameStateID gameState )
	{
		Result hr = ResultCode::SUCCESS;
		GameLogGameStateChange *pLogItem;

		if( gameState < GameStateID::None || gameState >= GameStateID::Max )
			return ResultCode::INVALID_ARG;

		uint8_t* itemBuffer = NewLogItemBuffer<GameLogGameStateChange>();
		svrMem( itemBuffer );
		pLogItem = new(itemBuffer) GameLogGameStateChange(timeStamp);
		svrMem( pLogItem );

		pLogItem->SetState(gameState);

		svrChk( AddLogItem( pLogItem ) );

	Proc_End:

		return hr;
	}

	Result GameLogSystem::AddGameVote( UTCTimeStampSec timeStamp, GameVoteType type, uint voterCount )
	{
		Result hr = ResultCode::SUCCESS;
		GameLogVote *logItem;

		if( type < GameVoteType(0) || type >= GameVoteType::Max )
			return ResultCode::INVALID_ARG;


		if( voterCount <= 0 || voterCount > GameLogItem::LEGACY_MAX_GAMEPLAYER )
		{
			return ResultCode::GAME_INVALID_PLAYER_COUNT;
		}

		size_t allocationSize = sizeof(GameLogVote) + (voterCount-1)*sizeof(GameLogVote::VoteInfo);
		uint8_t* itemBuffer = NewLogItemBuffer<GameLogVote>( allocationSize );
		svrMem(itemBuffer);
		logItem = new(itemBuffer) GameLogVote(timeStamp, voterCount);

		svrMem( logItem );
		Assert( logItem->LogItemSize == allocationSize );

		svrChk( logItem->InitializeVote(type,voterCount) );

		svrChk( AddLogItem( logItem ) );

		m_CurrentVoteLogItem = logItem;

	Proc_End:

		return hr;
	}
	Result GameLogSystem::UpdateGameVote( PlayerID voter, PlayerID voted )
	{
		Result hr = ResultCode::SUCCESS;

		if( m_CurrentVoteLogItem == nullptr /*|| m_CurrentVoteLogItem->Type != type*/ )
			return ResultCode::GAME_INVALID_VOTETYPE;

		svrChk( m_CurrentVoteLogItem->SetVoteStatus( voter, voted ) );

	Proc_End:

		return hr;
	}

	Result GameLogSystem::AddGameVoteResult(UTCTimeStampSec timeStamp, uint numRankers, const PlayerID* ranker )
	{
		Result hr = ResultCode::SUCCESS;
		GameLogVoteResult *logItem;

		if( numRankers > 2 )
			return ResultCode::INVALID_ARG;

		if( ranker == nullptr )
			return ResultCode::INVALID_POINTER;

		Assert(numRankers > 0 );
		if( numRankers <= 0 || numRankers > GameLogItem::LEGACY_MAX_GAMEPLAYER )
		{
			return ResultCode::GAME_INVALID_PLAYER_COUNT;
		}

		size_t allocationSize = sizeof(GameLogVoteResult) + (numRankers-1)*sizeof(PlayerID);
		uint8_t* itemBuffer = NewLogItemBuffer<GameLogVoteResult>( allocationSize );
		svrMem( itemBuffer );
		logItem = new(itemBuffer) GameLogVoteResult(timeStamp,numRankers);
		svrMem( logItem );

		Assert( logItem->LogItemSize == allocationSize );

		logItem->SetVotedRankers(numRankers, ranker);

		svrChk( AddLogItem( logItem ) );

	Proc_End:

		return hr;
	}
	
	Result GameLogSystem::AddGamePlayerKilled(UTCTimeStampSec timeStamp, PlayerKilledReason reason, PlayerID killedPlayerID )
	{
		Result hr = ResultCode::SUCCESS;
		GameLogPlayerKilled *logItem;

		uint8_t* itemBuffer = NewLogItemBuffer<GameLogPlayerKilled>();
		svrMem( itemBuffer );
		logItem = new(itemBuffer) GameLogPlayerKilled(timeStamp);
		svrMem( logItem );

		logItem->SetPlayerKilled(reason, killedPlayerID);

		svrChk( AddLogItem( logItem ) );

	Proc_End:

		return hr;
	}

	Result GameLogSystem::AddGameEnd(UTCTimeStampSec timeStamp, GameWinner winner )
	{
		Result hr = ResultCode::SUCCESS;
		GameLogGameEnd *logItem;

		if( winner < GameWinner(0) || winner >= GameWinner::Max )
			return ResultCode::INVALID_ARG;

		uint8_t* itemBuffer = NewLogItemBuffer<GameLogGameEnd>();
		svrMem( itemBuffer );
		logItem = new(itemBuffer) GameLogGameEnd(timeStamp);
		svrMem( logItem );

		logItem->SetWinner(winner);

		svrChk( AddLogItem( logItem ) );

	Proc_End:

		return hr;
	}

	
}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







