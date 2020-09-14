////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
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
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Cluster member list query
		Result NetPolicyClusterServer::GetClusterMemberListCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::GetClusterMemberListCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InClusterID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyClusterServer::GetClusterMemberListCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// Cmd: Join to the cluster
		Result NetPolicyClusterServer::RequestDataSyncCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::RequestDataSyncCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InClusterID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyClusterServer::RequestDataSyncCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )


		// Cmd: Cluster member list query
		Result NetSvrPolicyClusterServer::GetClusterMemberListRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::GetClusterMemberListRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InMemberList);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyClusterServer::GetClusterMemberListRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		Result NetSvrPolicyClusterServer::RequestDataSyncRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::RequestDataSyncRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyClusterServer::RequestDataSyncRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


