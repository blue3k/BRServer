////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Cluster member list query
		Result NetPolicyClusterServer::GetClusterMemberListCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GetClusterMemberListCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InClusterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::GetClusterMemberListCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		Result NetPolicyClusterServer::JoinClusterCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::JoinClusterCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InSender, InSenderNetClass, InSenderAddress, InClusterID, InClusterType, InClusterMembership));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::JoinClusterCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		// C2S: Do not let it broadcasted while it's manual broadcast packet
		Result NetPolicyClusterServer::NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::NewServerServiceJoinedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InJoinedServiceUID, InJoinedServiceNetClass, InJoinedServiceAddress, InClusterID, InClusterType, InJoinedServiceMembership));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		// C2S: Sync cluster service informations
		Result NetPolicyClusterServer::SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::SyncClusterServiceC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InClusterID, InClusterType, InMemberList));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		Result NetPolicyClusterServer::RequestDataSyncCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::RequestDataSyncCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InClusterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::RequestDataSyncCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Master vote
		Result NetPolicyClusterServer::ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const uint64_t &InVotedUpTime )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::ClusterMasterVoteC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InClusterID, InVoteToUID, InVotedUpTime));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const uint64_t &InVotedUpTime )
		// C2S: Update cluster service status
		Result NetPolicyClusterServer::ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::ClusterUpdateStatusC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InSender, InClusterID, InMemberStatus));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		// C2S: Update cluster service workload
		Result NetPolicyClusterServer::ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InSender, InClusterID, InWorkload));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )
		// Cmd: Get lowest workloaded cluster member
		Result NetPolicyClusterServer::GetLowestWorkloadClusterMemberCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InClusterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::GetLowestWorkloadClusterMemberCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Called when a player entity is created
		Result NetPolicyClusterServer::GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		// C2S: Called when a player entity is deleted
		Result NetPolicyClusterServer::GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyClusterServer::GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )


		// Cmd: Cluster member list query
		Result NetSvrPolicyClusterServer::GetClusterMemberListRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GetClusterMemberListRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InMemberList));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::GetClusterMemberListRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		Result NetSvrPolicyClusterServer::JoinClusterRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::JoinClusterRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InMemberList));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::JoinClusterRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		Result NetSvrPolicyClusterServer::RequestDataSyncRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::RequestDataSyncRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::RequestDataSyncRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Master instance of the cluster is assigned
		Result NetSvrPolicyClusterServer::ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::ClusterMasterAssignedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InSender, InClusterID, InMasterUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )
		// Cmd: Get lowest workloaded cluster member
		Result NetSvrPolicyClusterServer::GetLowestWorkloadClusterMemberRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GetLowestWorkloadClusterMemberRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InMember));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyClusterServer::GetLowestWorkloadClusterMemberRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember )


	}; // namespace Policy
}; // namespace BR


