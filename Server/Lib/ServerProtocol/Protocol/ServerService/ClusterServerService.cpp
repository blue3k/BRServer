////////////////////////////////////////////////////////////////////////////////
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
		// Cmd: Join to the cluster
		Result ClusterServerService::RequestDataSyncCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).RequestDataSyncCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::RequestDataSyncCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
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


