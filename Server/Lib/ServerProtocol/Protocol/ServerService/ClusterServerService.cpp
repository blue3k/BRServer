////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : ClusterServer Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/ServerService/ClusterServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	ClusterServerService::ClusterServerService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Cluster member list query
	Result ClusterServerService::GetClusterMemberListCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyClusterServer(GetTargetEndpoint()).GetClusterMemberListCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		return hr;

	}; // Result ClusterServerService::GetClusterMemberListCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
	// Cmd: Join to the cluster
	Result ClusterServerService::RequestDataSyncCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyClusterServer(GetTargetEndpoint()).RequestDataSyncCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		return hr;

	}; // Result ClusterServerService::RequestDataSyncCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )


}; // namespace SF


