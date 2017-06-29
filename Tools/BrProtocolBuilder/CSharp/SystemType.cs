using System;
using System.Collections.Generic;
using ProtocolXml;

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
    using EntityID = UInt32;
    using EntityUID = UInt64;
    using ServerID = UInt32;
    using ServerUID = UInt64;

    
	// Game ID
	enum GameID// : UINT16
	{
		Invalid,				// Not used
		Conspiracy,
	};

    // Ranking type
	enum RankingType// : UINT8
	{
		Friend,
		World,
	};


    // Player Role
	enum PlayerRole// : UINT8
	{
		None,
		Villager,
		Werewolf,
		Seer,
		Medium,
		Possessed,
		Bodyguard,
		Owlman,

		Freemason,
		Werehamster,
		Mythomaniac,
		Hunter,
		Wizard,
		Sorceress,
		Prostitute,
		Chosen,
		AngelOfVillagers,
		AngelOfWolves,
		Paladin,
		Sorcerer,
		Condemned,
		Redeemed,
		Devil,
	};


	// Game flow state for Lupus in Tabula
    enum GameStateID// : UINT8
	{
		None,
		FreeDebate,
		//SeersChoice,
		//FreeMason,
		//BodyguardsChoice,
		//Owlman,
		FirstNightVote,
		SecondNightVote,
		NightVote,
		Mythomaniac,
		MorningDebate,
		VoteForSuspects,
		DefenceOfSuspects,
		VoteForHanging,
		End,
		Max,
	};
	
	// Game Winner
	enum GameWinner// : UINT8
	{
		None,
		Werewolf,
		Villager,
		WereHamster,
	};
	
	// 
	enum PlayerKilledReason// : UINT8
	{
		Unknown,
		BlockedByBodyguard,
		ByWerewolf,
		ByHanging,
	};
	
	// 
	enum PlayerRevealedReason// : UINT8
	{
		None,
		Werewolf,
		SeersChoice,
		Medium,
		GameEnd,
	};
	
	// Clustering model
	enum ClusterType
	{
		Replication,
		Ring,
		Shard,
	};

	// Cluster Member ship mode
	enum ClusterMembership
	{
		Master,
		Slave,
		StatusWatcher,	// Service status watcher
		Watcher,			// Service data watcher, Only effect with replica model
	};
	
	// Cluster Service status
	enum ServiceStatus
	{
		Offline,
		Online,
		Ready,
	};


    // structure data type attribute, this is used for message builder
    public class StructAttribute : Attribute
    {
    }

    [Struct()]
    public struct Vector2
    {
        public float x;
        public float y;
    };

    [Struct()]
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;
    };

    [Struct()]
    public struct NetAddress
    {
        public string Address;
        public UInt16 Port;
    };

    [Struct()]
    public struct RouteContext
    {
        public ServerID From;
        public ServerID To;
    }

    [Struct()]
    public struct PlayerInformation
    {
        public const int Size = 8 + 8 + 64 + 8 + 4 + 4;
        public AccountID FriendID;
        public AccountID FriendFacebookID;
    }

    [Struct()]
    public struct RankingPlayerInformation
    {
        public const int Size = PlayerInformation.Size + 4 + 4;
        public AccountID FriendID;
        public AccountID FriendFacebookID;
    }


    [Struct()]
    public struct TotalRankingPlayerInformation
    {
        public const int Size = 4 + 4 + 8 + 8 + 64 + 4 + 4 + 4;
        public AccountID FriendID;
        public AccountID FriendFacebookID;
    }

    [Struct()]
    public struct FriendInformation
    {
        public const int Size = RankingPlayerInformation.Size + 4;
        public AccountID FriendID;
        public AccountID FriendFacebookID;
    }

    [Struct()]
    public struct ServerFriendInformation
    {
        public const int Size = FriendInformation.Size + 4;
        // Dummy
        public AccountID FriendID;
    }

    [Struct()]
    public struct PerformanceCounterInfo
    {
        public const int Size = 4 + 128;
        // Dummy
        public AccountID FriendID;
    }

    [Struct()]
    public struct PerformanceCounterInstanceInfo
    {
        public const int Size = 128 + 8;
        // Dummy
        public AccountID FriendID;
    }

    [Struct()]
    public struct ServiceInformation
    {
        public EntityUID EntityUID;
    }

    [Struct()]
    public struct MatchingPlayerInformation
    {
        public EntityUID EntityUID;
    }

    [Struct()]
    public struct MatchingQueueTicket
    {
        public EntityUID EntityUID;
    }

    [Struct()]
    public struct LocalUID
    {
        public EntityUID EntityUID;
    }

    public class SystemTypeMap
    {

        public class TypeMap
        {
            public ParameterType XMLType;
            public int ByteSize;
            public Type CSharpType;
            public string CPPType;

            public TypeMap(ParameterType xmlType, int byteSize, Type chType, string cpptype = null)
            {
                XMLType = xmlType;
                ByteSize = byteSize;
                CSharpType = chType;
                CPPType = string.IsNullOrEmpty(cpptype) ? XMLType.ToString() : cpptype;
            }
        };

        static TypeMap[] TypesMapList = new TypeMap[]{
            new TypeMap( ParameterType.String, -1, typeof(String) ),
            new TypeMap( ParameterType.int8, 1, typeof(SByte), "int8_t" ),
            new TypeMap( ParameterType.uint8, 1, typeof(Byte), "uint8_t" ),
            new TypeMap( ParameterType.int16, 2, typeof(Int16), "int16_t" ),
            new TypeMap( ParameterType.uint16, 2, typeof(UInt16), "uint16_t" ),
            new TypeMap( ParameterType.int32, 4, typeof(Int32), "int32_t" ),
            new TypeMap( ParameterType.uint32, 4, typeof(UInt32), "uint32_t" ),
            new TypeMap( ParameterType.int64, 8, typeof(Int64), "int64_t" ),
            new TypeMap( ParameterType.uint64, 8, typeof(UInt64), "uint64_t" ),
            new TypeMap( ParameterType.BYTE, 1, typeof(Byte), "uint8_t" ),
            new TypeMap( ParameterType.WORD, 2, typeof(UInt16) ),
            new TypeMap( ParameterType.DWORD, 4, typeof(UInt32) ),
            new TypeMap( ParameterType.QWORD, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.FLOAT, 4, typeof(float) ),
            new TypeMap( ParameterType.TransactionID, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.AccountID, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.PlayerID, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.FacebookUID, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.AuthTicket, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.GameInsID, 4, typeof(UInt32) ),
            new TypeMap( ParameterType.GameInsUID, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.EntityID, 4, typeof(UInt32) ),
            new TypeMap( ParameterType.EntityUID, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.PartyUID, 8, typeof(UInt64) ),
            new TypeMap( ParameterType.ServerID, 4, typeof(UInt32) ),
            new TypeMap( ParameterType.ClusterID, 4, typeof(UInt32) ),
            new TypeMap( ParameterType.Result, 4, typeof(Int32) ),
            new TypeMap( ParameterType.NotificationType, 2, typeof(Int16) ),
            new TypeMap( ParameterType.NetClass, 4, typeof(UInt32) ),

            new TypeMap( ParameterType.RankingType, 1, typeof(RankingType) ),
            new TypeMap( ParameterType.ServerFriendInformation, ServerFriendInformation.Size, typeof(ServerFriendInformation) ),
            new TypeMap( ParameterType.PerformanceCounterInfo, PerformanceCounterInfo.Size, typeof(PerformanceCounterInfo) ),
            new TypeMap( ParameterType.PerformanceCounterInstanceInfo, PerformanceCounterInstanceInfo.Size, typeof(PerformanceCounterInstanceInfo) ),

            new TypeMap( ParameterType.PlayerRole, 1, typeof(PlayerRole) ),
            new TypeMap( ParameterType.GameStateID, 1, typeof(GameStateID) ),
            new TypeMap( ParameterType.GameWinner, 1, typeof(GameWinner) ),
            new TypeMap( ParameterType.PlayerKilledReason, 1, typeof(PlayerKilledReason) ),
            new TypeMap( ParameterType.PlayerRevealedReason, 1, typeof(PlayerRevealedReason) ),

            new TypeMap( ParameterType.GameID, 2, typeof(GameID) ),

            new TypeMap( ParameterType.ClusterType, 4, typeof(ClusterType) ),
            new TypeMap( ParameterType.ClusterMembership, 4, typeof(ClusterMembership) ),
            new TypeMap( ParameterType.ServiceStatus, 4, typeof(ServiceStatus) ),

            new TypeMap( ParameterType.MatchingPlayerInformation, 16, typeof(MatchingPlayerInformation) ),
            new TypeMap( ParameterType.MatchingQueueTicket, 8, typeof(MatchingQueueTicket) ),
            new TypeMap( ParameterType.LocalUID, 8, typeof(LocalUID) ),

            new TypeMap( ParameterType.Vector2, 8, typeof(Vector2) ),
            new TypeMap( ParameterType.Vector3, 12, typeof(Vector3) ),
            new TypeMap( ParameterType.NetAddress, 18, typeof(NetAddress) ),
            new TypeMap( ParameterType.RouteContext, 8, typeof(RouteContext) ),
            new TypeMap( ParameterType.ServiceInformation, 18+8, typeof(ServiceInformation) ),
            new TypeMap( ParameterType.PlayerInformation, PlayerInformation.Size, typeof(PlayerInformation) ),
            new TypeMap( ParameterType.RankingPlayerInformation, RankingPlayerInformation.Size, typeof(RankingPlayerInformation) ),
            new TypeMap( ParameterType.FriendInformation, FriendInformation.Size, typeof(FriendInformation) ),
            new TypeMap( ParameterType.TotalRankingPlayerInformation, TotalRankingPlayerInformation.Size, typeof(TotalRankingPlayerInformation) ),
        };

        static Dictionary<ParameterType, TypeMap> MapToTypeInfo = new Dictionary<ParameterType, TypeMap>();
        //static Dictionary<Type, TypeMap> MapToCPP = new Dictionary<Type, TypeMap>();

        static SystemTypeMap()
        {
            foreach (TypeMap typeMap in TypesMapList)
            {
                MapToTypeInfo.Add(typeMap.XMLType, typeMap);
            }
        }

        public static Type ToCSharpType(ParameterType type)
        {
            return MapToTypeInfo[type].CSharpType;
        }

        public static string ToCPPType(ParameterType type)
        {
            return MapToTypeInfo[type].CPPType;
        }

        public static TypeMap GetParameterInfo(ParameterType type)
        {
            return MapToTypeInfo[type];
        }
    }

}
