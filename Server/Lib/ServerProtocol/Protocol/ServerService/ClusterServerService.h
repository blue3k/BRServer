////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : ClusterServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class ClusterServerService : public ServerServiceBase
		{
 			public:
			// Constructor
			ClusterServerService ( ServerServiceInformation* pService );

			// Cmd: Cluster member list query
			Result GetClusterMemberListCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			Result JoinClusterCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership );
			// C2S: Do not let it broadcasted while it's manual broadcast packet
			Result NewServerServiceJoinedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership );
			// C2S: Sync cluster service informations
			Result SyncClusterServiceC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster
			Result RequestDataSyncCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );
			// C2S: Master vote
			Result ClusterMasterVoteC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const uint64_t &InVoteToUID, const uint64_t &InVotedUpTime );
			// C2S: Update cluster service status
			Result ClusterUpdateStatusC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus );
			// C2S: Update cluster service workload
			Result ClusterUpdateWorkloadC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload );
			// Cmd: Get lowest workloaded cluster member
			Result GetLowestWorkloadClusterMemberCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );
			// C2S: Called when a player entity is created
			Result GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID );
			// C2S: Called when a player entity is deleted
			Result GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID );

		}; // class ClusterServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


