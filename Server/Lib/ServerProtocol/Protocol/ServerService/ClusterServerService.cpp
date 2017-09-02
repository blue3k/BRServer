﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : ClusterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/ClusterServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		ClusterServerService::ClusterServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Cluster member list query
		Result ClusterServerService::GetClusterMemberListCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).GetClusterMemberListCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GetClusterMemberListCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		Result ClusterServerService::JoinClusterCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).JoinClusterCmd( InRouteContext, InTransactionID, InRouteHopCount, InSender, InSenderNetClass, InSenderAddress, InClusterID, InClusterType, InClusterMembership ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::JoinClusterCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		// C2S: Do not let it broadcasted while it's manual broadcast packet
		Result ClusterServerService::NewServerServiceJoinedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).NewServerServiceJoinedC2SEvt( InRouteContext, InRouteHopCount, InJoinedServiceUID, InJoinedServiceNetClass, InJoinedServiceAddress, InClusterID, InClusterType, InJoinedServiceMembership ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::NewServerServiceJoinedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		// C2S: Sync cluster service informations
		Result ClusterServerService::SyncClusterServiceC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).SyncClusterServiceC2SEvt( InRouteContext, InRouteHopCount, InClusterID, InClusterType, InMemberList ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::SyncClusterServiceC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		Result ClusterServerService::RequestDataSyncCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).RequestDataSyncCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::RequestDataSyncCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Master vote
		Result ClusterServerService::ClusterMasterVoteC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const uint64_t &InVoteToUID, const uint64_t &InVotedUpTime )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).ClusterMasterVoteC2SEvt( InRouteContext, InRouteHopCount, InClusterID, InVoteToUID, InVotedUpTime ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::ClusterMasterVoteC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const uint64_t &InVoteToUID, const uint64_t &InVotedUpTime )
		// C2S: Update cluster service status
		Result ClusterServerService::ClusterUpdateStatusC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).ClusterUpdateStatusC2SEvt( InRouteContext, InRouteHopCount, InSender, InClusterID, InMemberStatus ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::ClusterUpdateStatusC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		// C2S: Update cluster service workload
		Result ClusterServerService::ClusterUpdateWorkloadC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).ClusterUpdateWorkloadC2SEvt( InRouteContext, InRouteHopCount, InSender, InClusterID, InWorkload ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::ClusterUpdateWorkloadC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )
		// Cmd: Get lowest workloaded cluster member
		Result ClusterServerService::GetLowestWorkloadClusterMemberCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).GetLowestWorkloadClusterMemberCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GetLowestWorkloadClusterMemberCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Called when a player entity is created
		Result ClusterServerService::GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).GamePlayerEntityCreatedC2SEvt( InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID )
		// C2S: Called when a player entity is deleted
		Result ClusterServerService::GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).GamePlayerEntityDeletedC2SEvt( InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID )


	}; // namespace Svr
}; // namespace SF


