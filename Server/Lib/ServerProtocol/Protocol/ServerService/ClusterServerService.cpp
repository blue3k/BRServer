////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ClusterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
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
		Result ClusterServerService::GetClusterMemberListCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).GetClusterMemberListCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::GetClusterMemberListCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// Cmd: Join to the cluster
		Result ClusterServerService::RequestDataSyncCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyClusterServer(GetConnection()).RequestDataSyncCmd( InRouteContext, InTransactionID, InRouteHopCount, InClusterID ) );

		Proc_End:

			return hr;

		}; // Result ClusterServerService::RequestDataSyncCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )


	}; // namespace Svr
}; // namespace SF


