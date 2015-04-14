////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Brave game types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrBaseTypes.h"
#include "Common/BrCommonTypes.h"
#include "Common/GameConst.h"

namespace BR
{
	// for 
	enum class DebugGameResource : UINT8
    {
        Gem,
        GameMoney,
        Stamina,
		Max
    };

	
	// Ranking type
	enum class RankingType : UINT8
	{
		//Friend,
		World,
	};


	// Player Role
	enum class PlayerRole : UINT8
	{
		None,
		Villager,
		Werewolf,
		Seer,

		Medium,
		//Possessed,
		//Bodyguard,
		//Owlman,
		//Freemason,
		//Werehamster,
		//Mythomaniac,
		//Hunter,
		//Wizard,
		//Sorceress,
		//Prostitute,
		//Chosen,
		//AngelOfVillagers,
		//AngelOfWolves,
		//Paladin,
		//Sorcerer,
		//Condemned,
		//Redeemed,
		//Devil,
	};


	// Game flow state for Lupus in Tabula
	enum class GameStateID : UINT8
	{
		None,
		FreeDebate,
		//FirstNightVote,
		//SecondNightVote,
		NightVote,
		//Mythomaniac,
		MorningDebate,
		VoteForSuspects,
		DefenceOfSuspects,
		VoteForHanging,
		End,
		Max,
	};
	
	// Game Winner
	enum class GameWinner : UINT8
	{
		None,
		Werewolf,
		Villager,
		WereHamster,
		Max
	};
	
	// 
	enum class PlayerKilledReason : UINT8
	{
		Unknown,
		BlockedByBodyguard,
		ByWerewolf,
		ByHanging,
	};
	
	// 
	enum class PlayerRevealedReason : UINT8
	{
		None,
		Werewolf,
		SeersChoice,
		Medium,
		GameEnd,
	};

    enum class ChatType : UINT8
    {
        Normal,         // Normal chat message
        Whisper,        // Whisper
        Info,           // Information
        System,         // System
        Role,           // Role chat in game
    };

	// Game log type
	enum class GameLogType : UINT16
	{
		ChatMessage,            // Chatting message
        GameStateChange,        // Role chat in game
        Vote,                   // Vote
        VoteResult,             // Vote result
        PlayerKilled,           // Player killed
        GameEnd,                // Game End
	};

	enum class GameVoteType : UINT16
    {
        Suspect,
        Hanging,
        Hunting,
		Max
    };


#pragma pack(push)
#pragma pack(2)

	// Game log item base item
	struct GameLogItem {
		GameLogType	LogType;		// Log type
		UINT16		LogItemSize;	// Actual log item size
		UINT32		TimeStamp;		// time32_t

		GameLogItem(GameLogType logType, ULONG timeStamp, size_t logItemSize);
	};

    struct GameLogChatMessage : GameLogItem
    {

        PlayerID Player;
		BYTE     PlayerStatus;
        ChatType MessageType;
		UINT16 MessageBufferSize;
        char MessageText[1];

        GameLogChatMessage(ULONG timeStamp, UINT messageBufferSize);
        
        HRESULT SetChatMessage( PlayerID player, BYTE playerStatus, ChatType type, const char* message);
    };


    struct GameLogGameStateChange : GameLogItem
    {
        GameStateID GameState;

        GameLogGameStateChange(ULONG timeStamp);

        void SetState( GameStateID gameState );
    };

	struct GameLogVote : GameLogItem
    {
        GameVoteType Type;
		UINT16 NumberOfVoter;

        struct VoteInfo
        {
            PlayerID Voter;
            PlayerID Voted;
        };
        VoteInfo VoteStatus[1];

        GameLogVote(ULONG timeStamp, UINT numVoter);

        HRESULT InitializeVote(GameVoteType type, int voterCount);

		HRESULT SetVoteStatus( PlayerID voter, PlayerID voted );
    };

    struct GameLogVoteResult : GameLogItem
    {
		UINT32 NumberOfRanker;
        PlayerID VotedRankers[1];

        GameLogVoteResult(ULONG timeStamp, UINT numRankers);

        void SetVotedRankers(UINT numRankers, const PlayerID* ranker);
    };
	
    struct GameLogPlayerKilled : GameLogItem
    {
        PlayerID KilledPlayerID;
		PlayerKilledReason killedReason;

        GameLogPlayerKilled(ULONG timeStamp);

        void SetPlayerKilled(PlayerKilledReason reason, PlayerID killedPlayerID);
    };

    struct GameLogGameEnd : GameLogItem
    {
        GameWinner Winner;

        GameLogGameEnd(ULONG timeStamp);

        void SetWinner( GameWinner winner );
    }; 


	enum class TransLogCategory : char
	{
		Account		= 'A',
		Buy = 'B',
		DbgGain = 'D',
		Give = 'S',
		Game = 'G',
		GameResult = 'R',
	};


#pragma pack(pop)

#include "BrGameTypes.inl"


};


