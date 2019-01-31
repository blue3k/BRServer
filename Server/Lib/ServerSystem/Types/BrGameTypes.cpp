////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Types/BrGameTypes.h"
#include "ServerLog/SvrLog.h"





namespace SF
{

	
	GameLogItem::GameLogItem(GameLogType logType, TimeStampSec timeStamp, size_t logItemSize)
		: LogType(logType), LogItemSize((UINT16)logItemSize), TimeStamp(timeStamp)
	{
	}




	
    GameLogChatMessage::GameLogChatMessage(TimeStampSec timeStamp, uint messageBufferSize)
        : GameLogItem(GameLogType::ChatMessage, timeStamp, sizeof(GameLogChatMessage)+messageBufferSize)
		,MessageBufferSize((decltype(MessageBufferSize))messageBufferSize)
    {
    }
        
    Result GameLogChatMessage::SetChatMessage( PlayerID player, BYTE playerStatus, ChatType type, const char* message)
    {
		Result hr = ResultCode::SUCCESS;

        Player = player;
		PlayerStatus = playerStatus;
        MessageType = type;
		defChk( StrUtil::StringCopy((char*)MessageText,(INT)MessageBufferSize, message) );

		// force null terminate regardless of the size of message
		MessageText[MessageBufferSize - 1] = 0;

	Proc_End:

		return hr;
    }

	
    GameLogGameStateChange::GameLogGameStateChange(TimeStampSec timeStamp)
        :GameLogItem(GameLogType::GameStateChange,timeStamp,sizeof(GameLogGameStateChange))
    {
    }

    void GameLogGameStateChange::SetState( GameStateID gameState )
    {
        GameState = gameState;
    }

	
    GameLogVote::GameLogVote(TimeStampSec timeStamp, uint numVoter)
		: GameLogItem(GameLogType::Vote,timeStamp,sizeof(GameLogVote) + sizeof(VoteInfo)*std::min((uint)GameConst::MAX_GAMEPLAYER,numVoter-1))
		,NumberOfVoter((decltype(NumberOfVoter))numVoter)
    {
		Assert(numVoter <= GameConst::MAX_GAMEPLAYER);
    }

    Result GameLogVote::InitializeVote(GameVoteType type, int voterCount)
    {
		Assert( voterCount == NumberOfVoter );
		if( voterCount > NumberOfVoter )
			voterCount = NumberOfVoter;

        Type = type;
		for (int vote = 0; vote < voterCount; vote++)
        {
            VoteStatus[vote].Voter = 0;
            VoteStatus[vote].Voted = 0;
        }

		return ResultCode::SUCCESS;
    }

	Result GameLogVote::SetVoteStatus( PlayerID voter, PlayerID voted )
    {
        for (int vote = 0; vote < NumberOfVoter; vote++)
        {
            if (VoteStatus[vote].Voter == 0
				|| VoteStatus[vote].Voter == voter)
            {
                VoteStatus[vote].Voter = voter;
                VoteStatus[vote].Voted = voted;

                return ResultCode::SUCCESS;
            }
        }
        // can't find slot for the voter
        Assert(false);
		return ResultCode::FAIL;
    }


	
    GameLogVoteResult::GameLogVoteResult(TimeStampSec timeStamp, uint numRankers)
		: GameLogItem(GameLogType::VoteResult,timeStamp,sizeof(GameLogVoteResult) + sizeof(PlayerID)*std::min((uint)GameConst::MAX_GAMEPLAYER,numRankers-1))
		, NumberOfRanker(numRankers)
    {
    }

    void GameLogVoteResult::SetVotedRankers(uint numRankers, const PlayerID* ranker)
    {
		Assert( NumberOfRanker == numRankers );
		if( NumberOfRanker < numRankers )
			numRankers = NumberOfRanker;

        memcpy( VotedRankers, ranker, sizeof(PlayerID)*numRankers );
    }

	
    GameLogPlayerKilled::GameLogPlayerKilled(TimeStampSec timeStamp)
		: GameLogItem(GameLogType::PlayerKilled,timeStamp,sizeof(GameLogPlayerKilled))
    {
    }

    void GameLogPlayerKilled::SetPlayerKilled(PlayerKilledReason reason, PlayerID killedPlayerID)
    {
        killedReason = reason;
		KilledPlayerID = killedPlayerID;
    }



	
    GameLogGameEnd::GameLogGameEnd(TimeStampSec timeStamp)
		: GameLogItem(GameLogType::GameEnd,timeStamp,sizeof(GameLogGameEnd))
    {
    }

    void GameLogGameEnd::SetWinner( GameWinner winner )
    {
        Winner = winner;
    }

};	// namespace SF


