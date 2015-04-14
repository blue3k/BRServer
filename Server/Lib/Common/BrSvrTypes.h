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

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/MathUtil.h"
#include "Common/NullUtil.h"
#include "Common/StrUtil.h"
#include "Common/BrBaseTypes.h"


namespace BR {

#pragma pack(push)
#pragma pack(4)

	enum class DBClusterType
	{
		Normal,
		Sharding,
	};

	
	// server cluster ID
	//typedef UINT32 ClusterID;
	enum class ClusterID : UINT32
	{
		EntityManager				= 1,
		ClusterManager,
		Monitoring,
		LoginServers,
		Game,
		Game_Conspiracy,
		Game_Max,
		GameInstanceManager = Game_Max,
		GamePartyManager,
		Ranking,

		Matching_Game_4,
		Matching_Game_8,
		Matching_Game_10,
		Matching_Game_12,

		MatchingQueue_Game_4x1		= 50,
		MatchingQueue_Game_4x2,
		MatchingQueue_Game_4x3,

		MatchingQueue_Game_8x1		= 55,
		MatchingQueue_Game_8x2,
		MatchingQueue_Game_8x3,
		MatchingQueue_Game_8x4,
		MatchingQueue_Game_8x5,
		MatchingQueue_Game_8x6,
		MatchingQueue_Game_8x7,

		MatchingQueue_Game_10x1		= 65,
		MatchingQueue_Game_10x2,
		MatchingQueue_Game_10x3,
		MatchingQueue_Game_10x4,
		MatchingQueue_Game_10x5,
		MatchingQueue_Game_10x6,
		MatchingQueue_Game_10x7,
		MatchingQueue_Game_10x8,
		MatchingQueue_Game_10x9,

		MatchingQueue_Game_12x1		= 75,
		MatchingQueue_Game_12x2,
		MatchingQueue_Game_12x3,
		MatchingQueue_Game_12x4,
		MatchingQueue_Game_12x5,
		MatchingQueue_Game_12x6,
		MatchingQueue_Game_12x7,
		MatchingQueue_Game_12x8,
		MatchingQueue_Game_12x9,
		MatchingQueue_Game_12x10,
		MatchingQueue_Game_12x11,

		Max
	};

	inline ClusterID operator++(ClusterID clusterID)
	{
		switch( clusterID )
		{
		case ClusterID::Matching_Game_12:
			return ClusterID::MatchingQueue_Game_4x1;
		case ClusterID::MatchingQueue_Game_4x3:
			return ClusterID::MatchingQueue_Game_8x1;
		case ClusterID::MatchingQueue_Game_8x7:
			return ClusterID::MatchingQueue_Game_10x1;
		case ClusterID::MatchingQueue_Game_10x9:
			return ClusterID::MatchingQueue_Game_12x1;
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


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Transaction ID
	//

	union TransactionID
	{
		struct {
			EntityID	EntityID;
			UINT32		TransID;
		};
		UINT64 ID;

		inline TransactionID();
		inline TransactionID( const TransactionID& transID );
		inline TransactionID( BR::EntityID entityID, UINT32 transID );
		inline TransactionID( BR::Context context );

		inline bool IsValid() const;

		inline TransactionID& operator = ( const TransactionID& transID );

		inline bool operator == ( const TransactionID& src ) const;

#if !defined(SWIG)
		inline operator BR::Context() const;
#endif
	};

	


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Server Route context
	//

	typedef union tag_RouteContext
	{
		struct {
			EntityUID	From;
			EntityUID	To;
		};
		UINT64 ContextValue[2];

		inline tag_RouteContext();
		inline tag_RouteContext( const tag_RouteContext& routeContext );
		inline tag_RouteContext( EntityUID InFromID, EntityUID InToID );
		inline tag_RouteContext( int initValue );

		inline tag_RouteContext& operator = ( const tag_RouteContext& src );

		inline bool operator == ( const tag_RouteContext& routeContext ) const;

		// Get swaped context( From <==> To )
		inline tag_RouteContext GetSwaped() const;

	} RouteContext;



	// Clustering model
	enum class ClusterType : UINT32
	{
		Replication,		// Replica with master. master - write, others - read
		FreeReplication,	// Replica without master. plat relationship
		Ring,				// Circular queue cluster
		Shard,				// Sharded. only shares status
	};

	// Cluster Member ship mode
	enum class ClusterMembership : UINT32
	{
		Master,
		Slave,
		StatusWatcher,	// Service status watcher
		Watcher,			// Service data watcher, Only effect with replica model
	};
	
	// Cluster Service status
	enum class ServiceStatus : UINT32
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
		ULONGLONG			ServerUpTime;
		UINT32				Workload;

		inline ServiceInformation();
		inline ServiceInformation( const ServiceInformation& src );
		inline ServiceInformation( EntityUID entityUID, ClusterMembership membership, ServiceStatus status, BR::NetClass netClass, const NetAddress& address, ULONGLONG serverUpTime, UINT32 workload );
		inline ServiceInformation( int initValue );

		inline ServiceInformation& operator = ( const ServiceInformation& src );

		inline bool operator == ( const ServiceInformation& op ) const;
	};

	
	// Global ID in single machine
	union GlobalUID
	{
		struct {
			UINT32		Time;
			UINT32		ServerID	: 8;
			UINT32		ID			: 24;
		};
		UINT64			UID;

		inline GlobalUID();
		inline GlobalUID( const GlobalUID& src );
		inline GlobalUID( UINT serverID, UINT32 time, UINT32 id );
		inline GlobalUID( UINT64 initValue );

		inline GlobalUID& operator = ( const GlobalUID& src );

		inline bool operator == ( const GlobalUID& op ) const;
		inline bool operator != ( const GlobalUID& op ) const;

#if !defined(SWIG)
		inline operator UINT64 () const;
#endif
	};


	// Unique ID in single machine
	union LocalUID
	{
		struct {
			UINT32		Time;
			UINT32		ID;
		};
		UINT64			UID;

		inline LocalUID();
		inline LocalUID( const LocalUID& src );
		inline LocalUID( UINT32 time, UINT32 id );
		inline LocalUID( UINT64 initValue );

		inline LocalUID& operator = ( const LocalUID& src );

		inline bool operator == ( const LocalUID& op ) const;
		inline bool operator != ( const LocalUID& op ) const;
#if !defined(SWIG)
		inline operator UINT64 () const;
#endif
	};

	// Matching queue player information
	struct MatchingQueueTicket
	{
		EntityUID		QueueUID;
		LocalUID		QueueItemID;

		inline MatchingQueueTicket();
		inline MatchingQueueTicket( const MatchingQueueTicket& src );
		inline MatchingQueueTicket( EntityUID queueUID, LocalUID itemID );
		inline MatchingQueueTicket( int initValue );

		inline MatchingQueueTicket& operator = ( const MatchingQueueTicket& src );

		inline bool operator == ( const MatchingQueueTicket& op ) const;
		inline bool operator != ( const MatchingQueueTicket& op ) const;
	};

	// Matching queue player information
	struct MatchingPlayerInformation
	{
		EntityUID		PlayerUID;
		AccountID		PlayerID;

		inline MatchingPlayerInformation();
		inline MatchingPlayerInformation( const MatchingPlayerInformation& src );
		inline MatchingPlayerInformation( EntityUID playerUID, AccountID playerID );
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
		UINT32						NumPlayers;					// Cancel information is marked here
		MatchingPlayerInformation	Players[MAX_NUM_PLAYER];

		inline MatchingQueueItem();
		inline MatchingQueueItem( const MatchingQueueItem& src );
		inline MatchingQueueItem( EntityUID RegisterUID, PlayerID RegisterID, UINT numPlayer, const MatchingPlayerInformation* playerInformations );
		inline MatchingQueueItem( int initValue );

		inline HRESULT SetQueueItem( EntityUID RegisterUID, PlayerID RegisterID, UINT numPlayer, const MatchingPlayerInformation* playerInformations );

		inline MatchingQueueItem& operator = ( const MatchingQueueItem& src );

		inline bool operator == ( const MatchingQueueItem& op ) const;
	};


	struct PerformanceCounterInfo
	{
		UINT32 DateType;
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


