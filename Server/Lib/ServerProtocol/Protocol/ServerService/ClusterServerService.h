////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
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
 	namespace Svr
	{
 		class ClusterServerService : public ServerServiceBase
		{
 			public:
			// Constructor
			ClusterServerService ( ServerServiceInformation* pService );

			// Cmd: Cluster member list query
			Result GetClusterMemberListCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );
			// Cmd: Join to the cluster
			Result RequestDataSyncCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );

		}; // class ClusterServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


