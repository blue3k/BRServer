////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Brave base types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "String/StrUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "Types/BrGameTypes.h"


namespace SF {


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity State
	//
	enum class EntityState
	{
		FREE,
		WORKING,
		CLOSING,
	};


#pragma pack(push)
#pragma pack(4)

	enum class DBClusterType
	{
		Normal,
		Sharding,
	};

	
	// server cluster ID
	//typedef uint32_t ClusterID;
	enum class ClusterID : uint32_t
	{
		EntityManager				= 1,
		ClusterManager,
		Monitoring,
		Login,
		Game,
		Game_Conspiracy,
		Game_MyTownHero,
		Game_Max,
		GameInstanceManager = Game_Max,
		GamePartyManager,
		Ranking,

		PurchaseValidateGoogle,
		PurchaseValidateIOS,

		Matching_Game_4				= 80,
		Matching_Game_8,
		//Matching_Game_10,
		//Matching_Game_12,

		MatchingQueue_Game_4x1		= 100,
		MatchingQueue_Game_4x2,
		MatchingQueue_Game_4x3,
		MatchingQueue_Game_4x1S,
		MatchingQueue_Game_4x1W,

		MatchingQueue_Game_8x1,
		MatchingQueue_Game_8x2,
		MatchingQueue_Game_8x3,
		MatchingQueue_Game_8x4,
		MatchingQueue_Game_8x5,
		MatchingQueue_Game_8x6,
		MatchingQueue_Game_8x7,
		MatchingQueue_Game_8x1S,
		MatchingQueue_Game_8x1W,

		//MatchingQueue_Game_10x1,
		//MatchingQueue_Game_10x2,
		//MatchingQueue_Game_10x3,
		//MatchingQueue_Game_10x4,
		//MatchingQueue_Game_10x5,
		//MatchingQueue_Game_10x6,
		//MatchingQueue_Game_10x7,
		//MatchingQueue_Game_10x8,
		//MatchingQueue_Game_10x9,

		//MatchingQueue_Game_12x1,
		//MatchingQueue_Game_12x2,
		//MatchingQueue_Game_12x3,
		//MatchingQueue_Game_12x4,
		//MatchingQueue_Game_12x5,
		//MatchingQueue_Game_12x6,
		//MatchingQueue_Game_12x7,
		//MatchingQueue_Game_12x8,
		//MatchingQueue_Game_12x9,
		//MatchingQueue_Game_12x10,
		//MatchingQueue_Game_12x11,

		Max,
		MatchingQueue_Max = MatchingQueue_Game_8x1W,
	};

	inline ClusterID operator++(ClusterID clusterID)
	{
		switch( clusterID )
		{
		case ClusterID::Matching_Game_8:
			return ClusterID::MatchingQueue_Game_4x1;
		default:
			return static_cast<ClusterID>((INT)clusterID + 1);
		};
	}
	
	inline ClusterID operator++(ClusterID& clusterID, int)
	{
		ClusterID oldValue = clusterID;
		clusterID = ++clusterID;
		return oldValue;
	}


	// Clustering model
	enum class ClusterType : uint32_t
	{
		Replication,		// Replica with master. master - write, others - read
		FreeReplication,	// Replica without master. plat relationship
		Ring,				// Circular queue cluster
		Shard,				// Sharded. only shares status
	};

	// Cluster Member ship mode
	enum class ClusterMembership : uint32_t
	{
		Master,
		Slave,
		StatusWatcher,	// Service status watcher
		Watcher,			// Service data watcher, Only effect with replica model
	};
	
	// Cluster Service status
	enum class ServiceStatus : uint32_t
	{
		Offline,
		Online,
		Stopping,
		Ready,
	};


	// cluster service information
	struct ServiceInformation
	{
		EntityUID			UID;
		ClusterMembership	Membership;
		ServiceStatus		Status;
		NetClass			ServerClass;
		NetAddress			ServerAddress;
		TimeStampSec		ServerUpTime;
		uint32_t				Workload;

		inline ServiceInformation();
		inline ServiceInformation( const ServiceInformation& src );
		inline ServiceInformation( EntityUID entityUID, ClusterMembership membership, ServiceStatus status, NetClass netClass, const NetAddress& address, TimeStampSec serverUpTime, uint32_t workload );
		inline ServiceInformation( int initValue );

		inline ServiceInformation& operator = ( const ServiceInformation& src );

		inline bool operator == ( const ServiceInformation& op ) const;
	};

	
	// Global ID in single machine
	union GlobalUID
	{
		enum {
			BIT_SERVERID		= 8,
			BIT_ID				= 24,
		};

		struct {
			uint32_t		Time;
			uint32_t		SvrID		: BIT_SERVERID;
			uint32_t		ID			: BIT_ID;
		};
		uint64_t			UID;

		inline GlobalUID();
		inline GlobalUID( const GlobalUID& src );
		inline GlobalUID( uint serverID, uint32_t time, uint32_t id );
		inline GlobalUID( uint64_t initValue );

		inline GlobalUID& operator = ( const GlobalUID& src );

		inline bool operator == ( const GlobalUID& op ) const;
		inline bool operator != ( const GlobalUID& op ) const;

#if !defined(SWIG)
		inline operator uint64_t () const;
#endif
	};


	// Matching queue player information
	struct MatchingPlayerInformation
	{
		EntityUID		PlayerUID;
		AccountID		PlayerID;
		PlayerRole      RequestedRole;

		inline MatchingPlayerInformation();
		inline MatchingPlayerInformation( const MatchingPlayerInformation& src );
		inline MatchingPlayerInformation( EntityUID playerUID, AccountID playerID, PlayerRole requestedRole );
		inline MatchingPlayerInformation( int initValue );

		inline MatchingPlayerInformation& operator = ( const MatchingPlayerInformation& src );

		inline bool operator == ( const MatchingPlayerInformation& op ) const;
	};

	// Matching queue item
	struct MatchingQueueItem
	{
		enum { MAX_NUM_PLAYER = 20 };

		EntityUID					RegisterUID;				// Who registered
		PlayerID					RegisterID;					// Who registered
		uint32_t						NumPlayers;					// Cancel information is marked here
		MatchingPlayerInformation	Players[MAX_NUM_PLAYER];

		inline MatchingQueueItem();
		inline MatchingQueueItem( const MatchingQueueItem& src );
		inline MatchingQueueItem( EntityUID RegisterUID, PlayerID RegisterID, uint numPlayer, const MatchingPlayerInformation* playerInformations );
		inline MatchingQueueItem( int initValue );

		inline Result SetQueueItem( EntityUID RegisterUID, PlayerID RegisterID, uint numPlayer, const MatchingPlayerInformation* playerInformations );

		inline MatchingQueueItem& operator = ( const MatchingQueueItem& src );

		inline bool operator == ( const MatchingQueueItem& op ) const;
	};


	struct PerformanceCounterInfo
	{
		uint32_t DateType;
		char CounterName[128];

		bool operator == (const PerformanceCounterInfo& op) const;
	};

	struct PerformanceCounterInstanceInfo
	{
		char InstanceName[128];
		EntityUID InstanceUID;

		bool operator == (const PerformanceCounterInstanceInfo& op) const;
	};

#pragma pack(pop)

#include "BrSvrTypes.inl"


};


