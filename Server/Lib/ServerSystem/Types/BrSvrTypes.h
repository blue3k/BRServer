////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Brave base types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "String/SFStrUtil.h"
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


#pragma pack(push, 4)

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
		EntityUID			UID = 0;
		ClusterMembership	Membership = ClusterMembership::Slave;
		ServiceStatus		Status = ServiceStatus::Offline;
		NetClass			ServerClass = NetClass::Unknown;
		NetAddress			ServerAddress{};
		UTCTimeStampSec		ServerUpTime = UTCTimeStampSec::min();
		uint32_t				Workload = 0;

		inline ServiceInformation();
		inline ServiceInformation( const ServiceInformation& src );
		inline ServiceInformation( EntityUID entityUID, ClusterMembership membership, ServiceStatus status, NetClass netClass, const NetAddress& address, UTCTimeStampSec serverUpTime, uint32_t workload );

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
		uint64_t			UID = 0;

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


}


