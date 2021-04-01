////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
    class MessageEndpoint;
   class VariableTable;
   struct ActorMovement;


	class NetPolicyClusterServer 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyClusterServer (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Cluster member list query
		Result GetClusterMemberListCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );
		// Cmd: Join to the cluster
		Result RequestDataSyncCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );

	}; // class NetPolicyClusterServer 


	class NetSvrPolicyClusterServer
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyClusterServer ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Cluster member list query
		Result GetClusterMemberListRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList );
		// Cmd: Join to the cluster
		Result RequestDataSyncRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

	}; // class NetSvrPolicyClusterServer


}; // namespace SF


