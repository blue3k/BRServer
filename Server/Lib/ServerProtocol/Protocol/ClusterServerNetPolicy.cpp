////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/ClusterServerNetPolicy.h"
#include "Protocol/ClusterServerMsgClass.h"



namespace SF
{
 	// Cmd: Cluster member list query
	Result NetPolicyClusterServer::GetClusterMemberListCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ClusterServer::GetClusterMemberListCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InRouteHopCount, InClusterID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyClusterServer::GetClusterMemberListCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
	// Cmd: Join to the cluster
	Result NetPolicyClusterServer::RequestDataSyncCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ClusterServer::RequestDataSyncCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InRouteHopCount, InClusterID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyClusterServer::RequestDataSyncCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )


	// Cmd: Cluster member list query
	Result NetSvrPolicyClusterServer::GetClusterMemberListRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ClusterServer::GetClusterMemberListRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InMemberList);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyClusterServer::GetClusterMemberListRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
	// Cmd: Join to the cluster
	Result NetSvrPolicyClusterServer::RequestDataSyncRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ClusterServer::RequestDataSyncRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyClusterServer::RequestDataSyncRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


}; // namespace SF


