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
#include "ServerSystem/BrServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"


#include "GameSystem/GameLogSystem.h"
#include "GameInstance/GameInstanceEntity.h"


BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GameLogSystem);


namespace BR {
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
	HRESULT GameLogSystem::InitializeComponent()
	{
		HRESULT hr = S_OK;

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
	HRESULT GameLogSystem::AddGameStateChange(TimeStampSec timeStamp, GameStateID gameState )
	{
		HRESULT hr = S_OK;
		GameLogGameStateChange *pLogItem;

		if( gameState < GameStateID::None || gameState >= GameStateID::Max )
			return E_INVALIDARG;

		BYTE* itemBuffer = NewLogItemBuffer<GameLogGameStateChange>();
		svrMem( itemBuffer );
		pLogItem = new(itemBuffer) GameLogGameStateChange(timeStamp);
		svrMem( pLogItem );

		pLogItem->SetState(gameState);

		svrChk( AddLogItem( pLogItem ) );

	Proc_End:

		return hr;
	}

	HRESULT GameLogSystem::AddGameVote( TimeStampSec timeStamp, GameVoteType type, UINT voterCount )
	{
		HRESULT hr = S_OK;
		GameLogVote *logItem;

		if( type < GameVoteType(0) || type >= GameVoteType::Max )
			return E_INVALIDARG;


		if( voterCount <= 0 || voterCount > GameConst::MAX_GAMEPLAYER )
		{
			return E_GAME_INAVLID_PLAYER_COUNT;
		}

		size_t allocationSize = sizeof(GameLogVote) + (voterCount-1)*sizeof(GameLogVote::VoteInfo);
		BYTE* itemBuffer = NewLogItemBuffer<GameLogVote>( allocationSize );
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
	HRESULT GameLogSystem::UpdateGameVote( PlayerID voter, PlayerID voted )
	{
		HRESULT hr = S_OK;

		if( m_CurrentVoteLogItem == nullptr /*|| m_CurrentVoteLogItem->Type != type*/ )
			return E_GAME_INVALID_VOTETYPE;

		svrChk( m_CurrentVoteLogItem->SetVoteStatus( voter, voted ) );

	Proc_End:

		return hr;
	}

	HRESULT GameLogSystem::AddGameVoteResult(TimeStampSec timeStamp, UINT numRankers, const PlayerID* ranker )
	{
		HRESULT hr = S_OK;
		GameLogVoteResult *logItem;

		if( numRankers > 2 )
			return E_INVALIDARG;

		if( ranker == nullptr )
			return E_POINTER;

		Assert(numRankers > 0 );
		if( numRankers <= 0 || numRankers > GameConst::MAX_GAMEPLAYER )
		{
			return E_GAME_INAVLID_PLAYER_COUNT;
		}

		size_t allocationSize = sizeof(GameLogVoteResult) + (numRankers-1)*sizeof(PlayerID);
		BYTE* itemBuffer = NewLogItemBuffer<GameLogVoteResult>( allocationSize );
		svrMem( itemBuffer );
		logItem = new(itemBuffer) GameLogVoteResult(timeStamp,numRankers);
		svrMem( logItem );

		Assert( logItem->LogItemSize == allocationSize );

		logItem->SetVotedRankers(numRankers, ranker);

		svrChk( AddLogItem( logItem ) );

	Proc_End:

		return hr;
	}
	
	HRESULT GameLogSystem::AddGamePlayerKilled(TimeStampSec timeStamp, PlayerKilledReason reason, PlayerID killedPlayerID )
	{
		HRESULT hr = S_OK;
		GameLogPlayerKilled *logItem;

		BYTE* itemBuffer = NewLogItemBuffer<GameLogPlayerKilled>();
		svrMem( itemBuffer );
		logItem = new(itemBuffer) GameLogPlayerKilled(timeStamp);
		svrMem( logItem );

		logItem->SetPlayerKilled(reason, killedPlayerID);

		svrChk( AddLogItem( logItem ) );

	Proc_End:

		return hr;
	}

	HRESULT GameLogSystem::AddGameEnd(TimeStampSec timeStamp, GameWinner winner )
	{
		HRESULT hr = S_OK;
		GameLogGameEnd *logItem;

		if( winner < GameWinner(0) || winner >= GameWinner::Max )
			return E_INVALIDARG;

		BYTE* itemBuffer = NewLogItemBuffer<GameLogGameEnd>();
		svrMem( itemBuffer );
		logItem = new(itemBuffer) GameLogGameEnd(timeStamp);
		svrMem( logItem );

		logItem->SetWinner(winner);

		svrChk( AddLogItem( logItem ) );

	Proc_End:

		return hr;
	}

	
}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







