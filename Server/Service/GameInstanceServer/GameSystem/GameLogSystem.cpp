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

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"


#include "GameSystem/GameLogSystem.h"
#include "GameInstance/GameInstanceEntity.h"


SF_MEMORYPOOL_IMPLEMENT(BR::ConspiracyGameInstanceServer::GameLogSystem);


namespace SF {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	// Constructor 
	GameLogSystem::GameLogSystem( GameInstanceEntity* pEntity )
		:GameSystem(pEntity)
		,m_CurrentVoteLogItem(nullptr)
	{
	}

	GameLogSystem::~GameLogSystem()
	{
	}
	

	// Initialzie system
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
	Result GameLogSystem::AddGameStateChange(TimeStampSec timeStamp, GameStateID gameState )
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

	Result GameLogSystem::AddGameVote( TimeStampSec timeStamp, GameVoteType type, uint voterCount )
	{
		Result hr = ResultCode::SUCCESS;
		GameLogVote *logItem;

		if( type < GameVoteType(0) || type >= GameVoteType::Max )
			return ResultCode::INVALID_ARG;


		if( voterCount <= 0 || voterCount > GameConst::MAX_GAMEPLAYER )
		{
			return ResultCode::E_GAME_INVALID_PLAYER_COUNT;
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
			return ResultCode::E_GAME_INVALID_VOTETYPE;

		svrChk( m_CurrentVoteLogItem->SetVoteStatus( voter, voted ) );

	Proc_End:

		return hr;
	}

	Result GameLogSystem::AddGameVoteResult(TimeStampSec timeStamp, uint numRankers, const PlayerID* ranker )
	{
		Result hr = ResultCode::SUCCESS;
		GameLogVoteResult *logItem;

		if( numRankers > 2 )
			return ResultCode::INVALID_ARG;

		if( ranker == nullptr )
			return ResultCode::INVALID_POINTER;

		Assert(numRankers > 0 );
		if( numRankers <= 0 || numRankers > GameConst::MAX_GAMEPLAYER )
		{
			return ResultCode::E_GAME_INVALID_PLAYER_COUNT;
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
	
	Result GameLogSystem::AddGamePlayerKilled(TimeStampSec timeStamp, PlayerKilledReason reason, PlayerID killedPlayerID )
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

	Result GameLogSystem::AddGameEnd(TimeStampSec timeStamp, GameWinner winner )
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







