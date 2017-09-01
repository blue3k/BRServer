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

#include "SFTypedefs.h"
#include "Types/BrBaseTypes.h"
#include "GameConst.h"

namespace BR
{

	struct ServerFriendInformation : public FriendInformation
	{
		int32_t ShardID;

		ServerFriendInformation() : ShardID(0) {}
		ServerFriendInformation(const ServerFriendInformation& src) : FriendInformation(src), ShardID(src.ShardID) {}
		ServerFriendInformation(AccountID playerID, int32_t shardID, FacebookUID fbUID, const char* nickName, UINT level, UINT weeklyWin, UINT weeklyLose, BYTE isPlayingGame, uint64_t lastActiveTime, uint64_t LastStaminaSent)
			: FriendInformation(playerID, fbUID, nickName, level, weeklyWin, weeklyLose, isPlayingGame, lastActiveTime, LastStaminaSent)
			, ShardID(shardID)
		{
		}
		ServerFriendInformation& operator = (const ServerFriendInformation& src)
		{
			FriendInformation::operator = (src);
			ShardID = src.ShardID;
			return *this;
		}
		bool operator == (const ServerFriendInformation& src) const { return FriendInformation::operator ==(src); }
	};


	// for 
	enum class DebugGameResource : uint8_t
    {
        Gem,
        GameMoney,
        Stamina,
		Max
    };

	
	// Ranking type
	enum class RankingType : uint8_t
	{
		World,
		//Friend,
	};


	// Player Role
	enum class PlayerRole : uint8_t
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
	enum class GameStateID : uint8_t
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
	enum class GameWinner : uint8_t
	{
		None,
		Werewolf,
		Villager,
		WereHamster,
		Max
	};
	
	// 
	enum class PlayerKilledReason : uint8_t
	{
		Unknown,
		BlockedByBodyguard,
		ByWerewolf,
		ByHanging,
	};
	
	// 
	enum class PlayerRevealedReason : uint8_t
	{
		None,
		Werewolf,
		SeersChoice,
		Medium,
		GameEnd,
	};

    enum class ChatType : uint8_t
    {
        Normal,         // Normal chat message
        Whisper,        // Whisper
        Info,           // Information
        System,         // System
        Role,           // Role chat in game
    };

	// Game log type
	enum class GameLogType : uint16_t
	{
		ChatMessage,            // Chatting message
        GameStateChange,        // Role chat in game
        Vote,                   // Vote
        VoteResult,             // Vote result
        PlayerKilled,           // Player killed
        GameEnd,                // Game End
	};

	enum class GameVoteType : uint16_t
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
		GameLogType		LogType;		// Log type
		uint16_t			LogItemSize;	// Actual log item size
		TimeStampSec	TimeStamp;		// time32_t

		GameLogItem(GameLogType logType, TimeStampSec timeStamp, size_t logItemSize);
	};

    struct GameLogChatMessage : GameLogItem
    {

        PlayerID Player;
		uint8_t     PlayerStatus;
        ChatType MessageType;
		uint16_t MessageBufferSize;
        char MessageText[1];

        GameLogChatMessage(TimeStampSec timeStamp, UINT messageBufferSize);
        
        Result SetChatMessage( PlayerID player, uint8_t playerStatus, ChatType type, const char* message);
    };


    struct GameLogGameStateChange : GameLogItem
    {
        GameStateID GameState;

        GameLogGameStateChange(TimeStampSec timeStamp);

        void SetState( GameStateID gameState );
    };

	struct GameLogVote : GameLogItem
    {
        GameVoteType Type;
		uint16_t NumberOfVoter;

        struct VoteInfo
        {
            PlayerID Voter;
            PlayerID Voted;
        };
        VoteInfo VoteStatus[1];

        GameLogVote(TimeStampSec timeStamp, uint numVoter);

        Result InitializeVote(GameVoteType type, int voterCount);

		Result SetVoteStatus( PlayerID voter, PlayerID voted );
    };

    struct GameLogVoteResult : GameLogItem
    {
		uint32_t NumberOfRanker;
        PlayerID VotedRankers[1];

        GameLogVoteResult(TimeStampSec timeStamp, uint numRankers);

        void SetVotedRankers(uint numRankers, const PlayerID* ranker);
    };
	
    struct GameLogPlayerKilled : GameLogItem
    {
        PlayerID KilledPlayerID;
		PlayerKilledReason killedReason;

        GameLogPlayerKilled(TimeStampSec timeStamp);

        void SetPlayerKilled(PlayerKilledReason reason, PlayerID killedPlayerID);
    };

    struct GameLogGameEnd : GameLogItem
    {
        GameWinner Winner;

        GameLogGameEnd(TimeStampSec timeStamp);

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


