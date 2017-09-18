////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : ClusterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Cluster member list query
		Result NetPolicyClusterServer::GetClusterMemberListCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::GetClusterMemberListCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InClusterID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::GetClusterMemberListCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// Cmd: Join to the cluster
		Result NetPolicyClusterServer::RequestDataSyncCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::RequestDataSyncCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InClusterID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::RequestDataSyncCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )


		// Cmd: Cluster member list query
		Result NetSvrPolicyClusterServer::GetClusterMemberListRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::GetClusterMemberListRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult, InMemberList);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::GetClusterMemberListRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		Result NetSvrPolicyClusterServer::RequestDataSyncRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::RequestDataSyncRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::RequestDataSyncRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


