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
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
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
			// C2S: Called when a player entity is created
			Result GamePlayerEntityCreatedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID );
			// C2S: Called when a player entity is deleted
			Result GamePlayerEntityDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID );

		}; // class ClusterServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


