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


#include "StdAfx.h"
#include "Common/BrGameTypes.h"
#include "Common\Trace.h"





namespace BR
{

	
	GameLogItem::GameLogItem(GameLogType logType, ULONG timeStamp, size_t logItemSize)
		: LogType(logType), TimeStamp(timeStamp), LogItemSize((UINT32)logItemSize)
	{
	}




	
    GameLogChatMessage::GameLogChatMessage(ULONG timeStamp, UINT messageBufferSize)
        : GameLogItem(GameLogType::ChatMessage, timeStamp, sizeof(GameLogChatMessage)+messageBufferSize)
		,MessageBufferSize(messageBufferSize)
    {
    }
        
    HRESULT GameLogChatMessage::SetChatMessage( PlayerID player, BYTE playerStatus, ChatType type, const char* message)
    {
		HRESULT hr = S_OK;

        Player = player;
		PlayerStatus = playerStatus;
        MessageType = type;
		defChk( StrUtil::StringCpy((char*)MessageText,(INT)MessageBufferSize, message) );

		// force null terminate regardless of the size of message
		MessageText[MessageBufferSize - 1] = 0;

	Proc_End:

		return hr;
    }

	
    GameLogGameStateChange::GameLogGameStateChange(ULONG timeStamp)
        :GameLogItem(GameLogType::GameStateChange,timeStamp,sizeof(GameLogGameStateChange))
    {
    }

    void GameLogGameStateChange::SetState( GameStateID gameState )
    {
        GameState = gameState;
    }

	
    GameLogVote::GameLogVote(ULONG timeStamp, UINT numVoter)
		: GameLogItem(GameLogType::Vote,timeStamp,sizeof(GameLogVote) + sizeof(VoteInfo)*Util::Min((UINT)GameConst::MAX_GAMEPLAYER,numVoter-1))
		,NumberOfVoter(numVoter)
    {
		Assert(numVoter <= GameConst::MAX_GAMEPLAYER);
    }

    HRESULT GameLogVote::InitializeVote(GameVoteType type, int voterCount)
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

		return S_OK;
    }

	HRESULT GameLogVote::SetVoteStatus( PlayerID voter, PlayerID voted )
    {
        for (int vote = 0; vote < NumberOfVoter; vote++)
        {
            if (VoteStatus[vote].Voter == 0
				|| VoteStatus[vote].Voter == voter)
            {
                VoteStatus[vote].Voter = voter;
                VoteStatus[vote].Voted = voted;

                return S_OK;
            }
        }
        // can't find slot for the voter
        Assert(false);
		return E_FAIL;
    }


	
    GameLogVoteResult::GameLogVoteResult(ULONG timeStamp, UINT numRankers)
		: GameLogItem(GameLogType::VoteResult,timeStamp,sizeof(GameLogVoteResult) + sizeof(PlayerID)*Util::Min((UINT)GameConst::MAX_GAMEPLAYER,numRankers-1))
		, NumberOfRanker(numRankers)
    {
    }

    void GameLogVoteResult::SetVotedRankers(UINT numRankers, const PlayerID* ranker)
    {
		Assert( NumberOfRanker == numRankers );
		if( NumberOfRanker < numRankers )
			numRankers = NumberOfRanker;

        memcpy( VotedRankers, ranker, sizeof(PlayerID)*numRankers );
    }

	
    GameLogPlayerKilled::GameLogPlayerKilled(ULONG timeStamp)
		: GameLogItem(GameLogType::PlayerKilled,timeStamp,sizeof(GameLogPlayerKilled))
    {
    }

    void GameLogPlayerKilled::SetPlayerKilled(PlayerKilledReason reason, PlayerID killedPlayerID)
    {
        killedReason = reason;
		KilledPlayerID = killedPlayerID;
    }



	
    GameLogGameEnd::GameLogGameEnd(ULONG timeStamp)
		: GameLogItem(GameLogType::GameEnd,timeStamp,sizeof(GameLogGameEnd))
    {
    }

    void GameLogGameEnd::SetWinner( GameWinner winner )
    {
        Winner = winner;
    }

};	// namespace BR


