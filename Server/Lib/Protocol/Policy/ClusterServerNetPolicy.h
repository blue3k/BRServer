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
#include "Net/Netdef.h"
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
			HRESULT GetClusterMemberListCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			HRESULT JoinClusterCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership );
			// C2S: Do not let it broadcasted while it's manual broadcast packet
			HRESULT NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership );
			// C2S: Sync cluster service informations
			HRESULT SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster
			HRESULT RequestDataSyncCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// C2S: Master vote
			HRESULT ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime );
			// C2S: Update cluster service status
			HRESULT ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus );
			// C2S: Update cluster service workload
			HRESULT ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload );
			// Cmd: Get lowest workloaded cluster member
			HRESULT GetLowestWorkloadClusterMemberCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );
			// C2S: Called when a player entity is created
			HRESULT GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID );
			// C2S: Called when a player entity is deleted
			HRESULT GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID );

		}; // class NetPolicyClusterServer : public IPolicyClusterServer


		class NetSvrPolicyClusterServer : public ISvrPolicyClusterServer
		{
 			public:
			// Constructor
			NetSvrPolicyClusterServer ( Net::IConnection* pConn ) : ISvrPolicyClusterServer(pConn)
			{}

			// Cmd: Cluster member list query
			HRESULT GetClusterMemberListRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			HRESULT JoinClusterRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster
			HRESULT RequestDataSyncRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );
			// S2C: Master instance of the cluster is assigned
			HRESULT ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID );
			// Cmd: Get lowest workloaded cluster member
			HRESULT GetLowestWorkloadClusterMemberRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const ServiceInformation &InMember );

		}; // class NetSvrPolicyClusterServer : public ISvrPolicyClusterServer


	}; // namespace Policy
}; // namespace BR


