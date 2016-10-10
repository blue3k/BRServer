////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BrBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/ClusterServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		ClusterServerService::ClusterServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyClusterServer::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyClusterServer::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyClusterServer());
		}


		// Cmd: Cluster member list query
		Result ClusterServerService::GetClusterMemberListCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->GetClusterMemberListCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GetClusterMemberListCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		Result ClusterServerService::JoinClusterCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->JoinClusterCmd( InRouteContext, InTransactionID, InRouteHopCount, InSender, InSenderNetClass, InSenderAddress, InClusterID, InClusterType, InClusterMembership ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::JoinClusterCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		// C2S: Do not let it broadcasted while it's manual broadcast packet
		Result ClusterServerService::NewServerServiceJoinedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->NewServerServiceJoinedC2SEvt( InRouteContext, InRouteHopCount, InJoinedServiceUID, InJoinedServiceNetClass, InJoinedServiceAddress, InClusterID, InClusterType, InJoinedServiceMembership ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::NewServerServiceJoinedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		// C2S: Sync cluster service informations
		Result ClusterServerService::SyncClusterServiceC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->SyncClusterServiceC2SEvt( InRouteContext, InRouteHopCount, InClusterID, InClusterType, InMemberList ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::SyncClusterServiceC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		Result ClusterServerService::RequestDataSyncCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->RequestDataSyncCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::RequestDataSyncCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Master vote
		Result ClusterServerService::ClusterMasterVoteC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->ClusterMasterVoteC2SEvt( InRouteContext, InRouteHopCount, InClusterID, InVoteToUID, InVotedUpTime ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::ClusterMasterVoteC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime )
		// C2S: Update cluster service status
		Result ClusterServerService::ClusterUpdateStatusC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->ClusterUpdateStatusC2SEvt( InRouteContext, InRouteHopCount, InSender, InClusterID, InMemberStatus ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::ClusterUpdateStatusC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		// C2S: Update cluster service workload
		Result ClusterServerService::ClusterUpdateWorkloadC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->ClusterUpdateWorkloadC2SEvt( InRouteContext, InRouteHopCount, InSender, InClusterID, InWorkload ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::ClusterUpdateWorkloadC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload )
		// Cmd: Get lowest workloaded cluster member
		Result ClusterServerService::GetLowestWorkloadClusterMemberCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->GetLowestWorkloadClusterMemberCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GetLowestWorkloadClusterMemberCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Called when a player entity is created
		Result ClusterServerService::GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->GamePlayerEntityCreatedC2SEvt( InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		// C2S: Called when a player entity is deleted
		Result ClusterServerService::GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->GamePlayerEntityDeletedC2SEvt( InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )


	}; // namespace Svr
}; // namespace BR


