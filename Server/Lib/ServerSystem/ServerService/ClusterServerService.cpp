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
#include "Common/BRBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
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
			static_assert(Policy::IPolicyClusterServer::ID_POLICY == ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyClusterServer());
		}


		// Cmd: Cluster member list query
		HRESULT ClusterServerService::GetClusterMemberListCmd( const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->GetClusterMemberListCmd( InContext, InRouteContext, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::GetClusterMemberListCmd( const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		HRESULT ClusterServerService::JoinClusterCmd( const Context &InContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->JoinClusterCmd( InContext, InRouteContext, InRouteHopCount, InSender, InSenderNetClass, InSenderAddress, InClusterID, InClusterType, InClusterMembership ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::JoinClusterCmd( const Context &InContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		// C2S: Do not let it broadcasted while it's manual broadcast packet
		HRESULT ClusterServerService::NewServerServiceJoinedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->NewServerServiceJoinedC2SEvt( InRouteContext, InRouteHopCount, InJoinedServiceUID, InJoinedServiceNetClass, InJoinedServiceAddress, InClusterID, InClusterType, InJoinedServiceMembership ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::NewServerServiceJoinedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		// C2S: Sync cluster service informations
		HRESULT ClusterServerService::SyncClusterServiceC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->SyncClusterServiceC2SEvt( InRouteContext, InRouteHopCount, InClusterID, InClusterType, InMemberList ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::SyncClusterServiceC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		HRESULT ClusterServerService::RequestDataSyncCmd( const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->RequestDataSyncCmd( InContext, InRouteContext, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::RequestDataSyncCmd( const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Master vote
		HRESULT ClusterServerService::ClusterMasterVoteC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->ClusterMasterVoteC2SEvt( InRouteContext, InRouteHopCount, InClusterID, InVoteToUID, InVotedUpTime ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::ClusterMasterVoteC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime )
		// C2S: Update cluster service status
		HRESULT ClusterServerService::ClusterUpdateStatusC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->ClusterUpdateStatusC2SEvt( InRouteContext, InRouteHopCount, InSender, InClusterID, InMemberStatus ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::ClusterUpdateStatusC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		// C2S: Update cluster service workload
		HRESULT ClusterServerService::ClusterUpdateWorkloadC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->ClusterUpdateWorkloadC2SEvt( InRouteContext, InRouteHopCount, InSender, InClusterID, InWorkload ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::ClusterUpdateWorkloadC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload )
		// Cmd: Get lowest workloaded cluster member
		HRESULT ClusterServerService::GetLowestWorkloadClusterMemberCmd( const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->GetLowestWorkloadClusterMemberCmd( InContext, InRouteContext, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::GetLowestWorkloadClusterMemberCmd( const Context &InContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Called when a player entity is created
		HRESULT ClusterServerService::GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->GamePlayerEntityCreatedC2SEvt( InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		// C2S: Called when a player entity is deleted
		HRESULT ClusterServerService::GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyClusterServer()->GamePlayerEntityDeletedC2SEvt( InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID ) );

		Proc_End:

			return hr;

		}; // HRESULT ClusterServerService::GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )


	}; // namespace Svr
}; // namespace BR


