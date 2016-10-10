////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyClusterServer : public IPolicyClusterServer
		{
 			public:
			// Constructor
			NetPolicyClusterServer ( Net::IConnection* pConn ) : IPolicyClusterServer(pConn)
			{}

			// Cmd: Cluster member list query
			Result GetClusterMemberListCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			Result JoinClusterCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership );
			// C2S: Do not let it broadcasted while it's manual broadcast packet
			Result NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership );
			// C2S: Sync cluster service informations
			Result SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster
			Result RequestDataSyncCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// C2S: Master vote
			Result ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime );
			// C2S: Update cluster service status
			Result ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus );
			// C2S: Update cluster service workload
			Result ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload );
			// Cmd: Get lowest workloaded cluster member
			Result GetLowestWorkloadClusterMemberCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// C2S: Called when a player entity is created
			Result GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID );
			// C2S: Called when a player entity is deleted
			Result GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID );

		}; // class NetPolicyClusterServer : public IPolicyClusterServer


		class NetSvrPolicyClusterServer : public ISvrPolicyClusterServer
		{
 			public:
			// Constructor
			NetSvrPolicyClusterServer ( Net::IConnection* pConn ) : ISvrPolicyClusterServer(pConn)
			{}

			// Cmd: Cluster member list query
			Result GetClusterMemberListRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			Result JoinClusterRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster
			Result RequestDataSyncRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Master instance of the cluster is assigned
			Result ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID );
			// Cmd: Get lowest workloaded cluster member
			Result GetLowestWorkloadClusterMemberRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember );

		}; // class NetSvrPolicyClusterServer : public ISvrPolicyClusterServer


	}; // namespace Policy
}; // namespace BR


