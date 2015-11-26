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
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Common/Message.h"
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
		HRESULT NetPolicyClusterServer::GetClusterMemberListCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GetClusterMemberListCmd::BuildIMsg(pMsg, InContext, InRouteContext, InRouteHopCount, InClusterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::GetClusterMemberListCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		HRESULT NetPolicyClusterServer::JoinClusterCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::JoinClusterCmd::BuildIMsg(pMsg, InContext, InRouteContext, InRouteHopCount, InSender, InSenderNetClass, InSenderAddress, InClusterID, InClusterType, InClusterMembership));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::JoinClusterCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
		// C2S: Do not let it broadcasted while it's manual broadcast packet
		HRESULT NetPolicyClusterServer::NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::NewServerServiceJoinedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InJoinedServiceUID, InJoinedServiceNetClass, InJoinedServiceAddress, InClusterID, InClusterType, InJoinedServiceMembership));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::NewServerServiceJoinedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
		// C2S: Sync cluster service informations
		HRESULT NetPolicyClusterServer::SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::SyncClusterServiceC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InClusterID, InClusterType, InMemberList));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::SyncClusterServiceC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		HRESULT NetPolicyClusterServer::RequestDataSyncCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::RequestDataSyncCmd::BuildIMsg(pMsg, InContext, InRouteContext, InRouteHopCount, InClusterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::RequestDataSyncCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Master vote
		HRESULT NetPolicyClusterServer::ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::ClusterMasterVoteC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InClusterID, InVoteToUID, InVotedUpTime));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::ClusterMasterVoteC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime )
		// C2S: Update cluster service status
		HRESULT NetPolicyClusterServer::ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::ClusterUpdateStatusC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InSender, InClusterID, InMemberStatus));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::ClusterUpdateStatusC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
		// C2S: Update cluster service workload
		HRESULT NetPolicyClusterServer::ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InSender, InClusterID, InWorkload));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::ClusterUpdateWorkloadC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload )
		// Cmd: Get lowest workloaded cluster member
		HRESULT NetPolicyClusterServer::GetLowestWorkloadClusterMemberCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd::BuildIMsg(pMsg, InContext, InRouteContext, InRouteHopCount, InClusterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::GetLowestWorkloadClusterMemberCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
		// C2S: Called when a player entity is created
		HRESULT NetPolicyClusterServer::GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::GamePlayerEntityCreatedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		// C2S: Called when a player entity is deleted
		HRESULT NetPolicyClusterServer::GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InPlayerID, InPlayerUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyClusterServer::GamePlayerEntityDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )


		// Cmd: Cluster member list query
		HRESULT NetSvrPolicyClusterServer::GetClusterMemberListRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GetClusterMemberListRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InMemberList));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyClusterServer::GetClusterMemberListRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
		HRESULT NetSvrPolicyClusterServer::JoinClusterRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::JoinClusterRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InMemberList));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyClusterServer::JoinClusterRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList )
		// Cmd: Join to the cluster
		HRESULT NetSvrPolicyClusterServer::RequestDataSyncRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::RequestDataSyncRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyClusterServer::RequestDataSyncRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Master instance of the cluster is assigned
		HRESULT NetSvrPolicyClusterServer::ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::ClusterMasterAssignedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InSender, InClusterID, InMasterUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyClusterServer::ClusterMasterAssignedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )
		// Cmd: Get lowest workloaded cluster member
		HRESULT NetSvrPolicyClusterServer::GetLowestWorkloadClusterMemberRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const ServiceInformation &InMember )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::ClusterServer::GetLowestWorkloadClusterMemberRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InMember));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyClusterServer::GetLowestWorkloadClusterMemberRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const ServiceInformation &InMember )


	}; // namespace Policy
}; // namespace BR


