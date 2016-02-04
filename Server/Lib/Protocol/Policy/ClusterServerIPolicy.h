////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyClusterServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_CLUSTERSERVER };

			// Constructor
			ISvrPolicyClusterServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Cluster member list query
			virtual HRESULT GetClusterMemberListRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const Array<ServiceInformation>& InMemberList ) = 0;
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			virtual HRESULT JoinClusterRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const Array<ServiceInformation>& InMemberList ) = 0;
			// C2S: Do not let it broadcasted while it's manual broadcast packet
			// C2S: Sync cluster service informations
			// Cmd: Join to the cluster
			virtual HRESULT RequestDataSyncRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult ) = 0;
			// S2C: Master instance of the cluster is assigned
			virtual HRESULT ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID ) = 0;
			// C2S: Master vote
			// C2S: Update cluster service status
			// C2S: Update cluster service workload
			// Cmd: Get lowest workloaded cluster member
			virtual HRESULT GetLowestWorkloadClusterMemberRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const ServiceInformation &InMember ) = 0;
			// C2S: Called when a player entity is created
			// C2S: Called when a player entity is deleted

		}; // class ISvrPolicyClusterServer : public Net::INetPolicy


		class IPolicyClusterServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_CLUSTERSERVER };

			// Constructor
			IPolicyClusterServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Cluster member list query
			virtual HRESULT GetClusterMemberListCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID ) = 0;
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			virtual HRESULT JoinClusterCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership ) = 0;
			// C2S: Do not let it broadcasted while it's manual broadcast packet
			virtual HRESULT NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership ) = 0;
			// C2S: Sync cluster service informations
			virtual HRESULT SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList ) = 0;
			// Cmd: Join to the cluster
			virtual HRESULT RequestDataSyncCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID ) = 0;
			// S2C: Master instance of the cluster is assigned
			// C2S: Master vote
			virtual HRESULT ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime ) = 0;
			// C2S: Update cluster service status
			virtual HRESULT ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus ) = 0;
			// C2S: Update cluster service workload
			virtual HRESULT ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload ) = 0;
			// Cmd: Get lowest workloaded cluster member
			virtual HRESULT GetLowestWorkloadClusterMemberCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID ) = 0;
			// C2S: Called when a player entity is created
			virtual HRESULT GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID ) = 0;
			// C2S: Called when a player entity is deleted
			virtual HRESULT GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID ) = 0;

		}; // class IPolicyClusterServer : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


