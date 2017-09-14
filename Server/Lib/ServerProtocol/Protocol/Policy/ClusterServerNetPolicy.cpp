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
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		Result NetPolicyClusterServer::JoinClusterCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::JoinClusterCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InSender, InSenderNetClass, InSenderAddress, InClusterID, InClusterType, InClusterMembership);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::JoinClusterCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		// C2S: Do not let it broadcasted while it's manual broadcast packet
		Result NetPolicyClusterServer::NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::NewServerServiceJoinedC2SEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InJoinedServiceUID, InJoinedServiceNetClass, InJoinedServiceAddress, InClusterID, InClusterType, InJoinedServiceMembership);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		// C2S: Sync cluster service informations
		Result NetPolicyClusterServer::SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::SyncClusterServiceC2SEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InClusterID, InClusterType, InMemberList);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
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
		// C2S: Master vote
		Result NetPolicyClusterServer::ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const uint64_t &InVoteToUID, const uint64_t &InVotedUpTime )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::ClusterMasterVoteC2SEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InClusterID, InVoteToUID, InVotedUpTime);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const uint64_t &InVoteToUID, const uint64_t &InVotedUpTime )
		// C2S: Update cluster service status
		Result NetPolicyClusterServer::ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::ClusterUpdateStatusC2SEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InSender, InClusterID, InMemberStatus);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		// C2S: Update cluster service workload
		Result NetPolicyClusterServer::ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::ClusterUpdateWorkloadC2SEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InSender, InClusterID, InWorkload);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )
		// Cmd: Get lowest workloaded cluster member
		Result NetPolicyClusterServer::GetLowestWorkloadClusterMemberCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::GetLowestWorkloadClusterMemberCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InClusterID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::GetLowestWorkloadClusterMemberCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Called when a player entity is created
		Result NetPolicyClusterServer::GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID )
		// C2S: Called when a player entity is deleted
		Result NetPolicyClusterServer::GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID )


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
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		Result NetSvrPolicyClusterServer::JoinClusterRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::JoinClusterRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult, InMemberList);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::JoinClusterRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
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
		// S2C: Master instance of the cluster is assigned
		Result NetSvrPolicyClusterServer::ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint64_t &InMasterUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::ClusterMasterAssignedS2CEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InSender, InClusterID, InMasterUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint64_t &InMasterUID )
		// Cmd: Get lowest workloaded cluster member
		Result NetSvrPolicyClusterServer::GetLowestWorkloadClusterMemberRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ClusterServer::GetLowestWorkloadClusterMemberRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult, InMember);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::GetLowestWorkloadClusterMemberRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember )


	}; // namespace Policy
}; // namespace SF


