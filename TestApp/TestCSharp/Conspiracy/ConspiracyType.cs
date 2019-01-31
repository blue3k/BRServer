////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : system types
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;


namespace BR
{
    using String = String;
    using INT8 = SByte;
    using UINT8 = Byte;
    using INT16 = Int16;
    using UINT16 = UInt16;
    using INT32 = Int32;
    using UINT32 = UInt32;
    using INT64 = Int64;
    using UINT64 = UInt64;
    using BYTE = Byte;
    using WORD = UInt16;
    using DWORD = UInt32;
    using QWORD = UInt64;
    using FLOAT = Single;
    using Context = UInt64;
    using AccountID = UInt64;
    using AuthTicket = UInt64;
    using GameInstanceID = UInt32;
    using GameInstanceUID = UInt64;
    using HRESULT = Int32;
    using EntityID = UInt32;
    using EntityUID = UInt64;
    using ServerID = UInt32;
    using ServerUID = UInt64;


    ////[Struct()]
    public class PlayerInformation
    {
        public const int MAX_NAME = 64;

        public AccountID PlayerID;
        public AccountID FacebookUID;
        public string NickName;
        public UInt32 Level;
        public bool IsPlayingGame;
        public UINT64 LastActiveTime;
    }

    ////[Struct()]
    public class RankingPlayerInformation : PlayerInformation
    {
        public UINT32 WeeklyWin;
        public UINT32 WeeklyLose;
    }

    //[Struct()]
    public class FriendInformation : RankingPlayerInformation
    {
        public UINT64 StaminaTime;
    }

    //[Struct()]
    public class TotalRankingPlayerInformation
    {
        public const int MAX_NAME = 64;
        public UINT32 RankingID;
        public UINT32 Ranking;
        public AccountID PlayerID;
        public AccountID FacebookUID;
        public string NickName;
        public UInt32 Level;
        public UINT32 Win;
        public UINT32 Lose;
    }

    //[Struct()]
    public struct MatchingQueueTicket
    {
        public UInt64 QueueUID;
        public UInt64 QueueItemID;
    }


    // Ranking type
	public enum RankingType
	{
		//Friend,
		World,
	};

    public enum GameID
    {
        Invalid,
        Conspiracy
    }

    // Player Role
    public enum PlayerRole
    {
        None,
        Villager,
        Werewolf,
        Seer,
//        Medium,
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
    public enum GameStateID
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
    public enum GameWinner
    {
        None,
        Werewolf,
        Villager,
        WereHamster,
    };

    // 
    public enum PlayerKilledReason
    {
        Unknown,
        BlockedByBoardGuard,
        ByWerewolf,
        ByHanging,
    };

    // 
    public enum PlayerRevealedReason
    {
        None,
        Werewolf,
        SeersChoice,
        Medium,
        GameEnd,
    };


    // Notification message
    public enum NotificationType
    {
        None,				// Not used
        MessageNotify,
        FriendRequest,
        GiftStamina,
    };
    
    public enum ChatType// : UINT16
    {
        Normal,         // Normal chat message
        Whisper,        // Whisper
        Info,           // Information
        System,         // System
        Role,           // Role chat in game
    };

	// Game log type
    public enum GameLogType// : UINT32
	{
		ChatMessage,            // Chatting message
        GameStateChange,        // Role chat in game
        Vote,                   // Vote
        VoteResult,             // Vote result
        PlayerKilled,           // Player killed
        GameEnd,                // Game End
	};

    public enum GameVoteType//  : UINT16
    {
        Suspect,
        Hanging,
        Hunting,
		Max
    };


    // for 
	public enum DebugGameResource// : UINT8
    {
        Gem,
        GameMoney,
        Stamina,
		Max
    };

}
