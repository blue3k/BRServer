////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class ClusterServerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyClusterServer* GetPolicyClusterServer() { return GetPolicy<Policy::IPolicyClusterServer>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyClusterServer::ID_POLICY };
			// Constructor
			ClusterServerService ( ServerServiceInformation* pService );

			// Cmd: Cluster member list query
			Result GetClusterMemberListCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			Result JoinClusterCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership );
			// C2S: Do not let it broadcasted while it's manual broadcast packet
			Result NewServerServiceJoinedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership );
			// C2S: Sync cluster service informations
			Result SyncClusterServiceC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster
			Result RequestDataSyncCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// C2S: Master vote
			Result ClusterMasterVoteC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime );
			// C2S: Update cluster service status
			Result ClusterUpdateStatusC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus );
			// C2S: Update cluster service workload
			Result ClusterUpdateWorkloadC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload );
			// Cmd: Get lowest workloaded cluster member
			Result GetLowestWorkloadClusterMemberCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// C2S: Called when a player entity is created
			Result GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID );
			// C2S: Called when a player entity is deleted
			Result GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID );

		}; // class ClusterServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


