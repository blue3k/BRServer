////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
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



class VariableTable;



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyClusterServer 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyClusterServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyClusterServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Cluster member list query
			Result GetClusterMemberListCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );
			// Cmd: Join to the cluster
			Result RequestDataSyncCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );

		}; // class NetPolicyClusterServer 


		class NetSvrPolicyClusterServer
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyClusterServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyClusterServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Cluster member list query
			Result GetClusterMemberListRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList );
			// Cmd: Join to the cluster
			Result RequestDataSyncRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyClusterServer


	}; // namespace Policy
}; // namespace SF


