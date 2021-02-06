////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : ClusterServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"



namespace SF
{
 	class ClusterServerService : public ServerServiceBase
	{
 		public:
		// Constructor
		ClusterServerService ( ServerServiceInformation* pService );

		// Cmd: Cluster member list query
		Result GetClusterMemberListCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );
		// Cmd: Join to the cluster
		Result RequestDataSyncCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );

	}; // class ClusterServerService : public ServerServiceBase


}; // namespace SF


