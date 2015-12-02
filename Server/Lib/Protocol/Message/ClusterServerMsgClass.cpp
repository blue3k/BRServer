////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/PolicyID.h"
#include "Common/ToStringSvr.h"
#include "Common/ToStringGame.h"
#include "Common/ArrayUtil.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace ClusterServer
		{
 			// Cmd: Cluster member list query
			const MessageID GetClusterMemberListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 0);
			HRESULT GetClusterMemberListCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetClusterMemberListCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetClusterMemberListCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(ClusterID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GetClusterMemberListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetClusterMemberListCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )

			HRESULT GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GetClusterMemberListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:GetClusterMemberListCmd:{1}:{2} , Context:{3}, RouteContext:{4}, RouteHopCount:{5}, ClusterID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_RouteHopCount, m_ClusterID); 
			}; // void GetClusterMemberListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetClusterMemberListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 0);
			HRESULT GetClusterMemberListRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // HRESULT GetClusterMemberListRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetClusterMemberListRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext)
					+ sizeof(ServiceInformation)*InMemberList.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				UINT16 numberOfInMemberList = (UINT16)InMemberList.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GetClusterMemberListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetClusterMemberListRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList )

			HRESULT GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);
				pCur += sizeof(ServiceInformation)*m_MemberList.GetSize(); iMsgSize -= (INT)(sizeof(ServiceInformation)*m_MemberList.GetSize());


			Proc_End:

				return hr;

			}; // HRESULT GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GetClusterMemberListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:GetClusterMemberListRes:{1}:{2} , Context:{3}, Result:{4:X8}, RouteContext:{5}, MemberList:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result, m_RouteContext, m_MemberList); 
			}; // void GetClusterMemberListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			const MessageID JoinClusterCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 1);
			HRESULT JoinClusterCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderNetClass, pCur, iMsgSize, sizeof(NetClass) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderAddress, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterMembership, pCur, iMsgSize, sizeof(ClusterMembership) ) );


			Proc_End:

				return hr;

			}; // HRESULT JoinClusterCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinClusterCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(EntityUID)
					+ sizeof(NetClass)
					+ sizeof(NetAddress)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ClusterMembership));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::JoinClusterCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InSenderNetClass, sizeof(NetClass));
				Protocol::PackParamCopy( pMsgData, &InSenderAddress, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &InClusterMembership, sizeof(ClusterMembership));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinClusterCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )

			HRESULT JoinClusterCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT JoinClusterCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT JoinClusterCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT JoinClusterCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void JoinClusterCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:JoinClusterCmd:{1}:{2} , Context:{3}, RouteContext:{4}, RouteHopCount:{5}, Sender:{6}, SenderNetClass:{7}, SenderAddress:{8}, ClusterID:{9}, ClusterType:{10}, ClusterMembership:{11}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_RouteHopCount, m_Sender, m_SenderNetClass, m_SenderAddress, m_ClusterID, m_ClusterType, m_ClusterMembership); 
			}; // void JoinClusterCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinClusterRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 1);
			HRESULT JoinClusterRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // HRESULT JoinClusterRes::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinClusterRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext)
					+ sizeof(ServiceInformation)*InMemberList.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				UINT16 numberOfInMemberList = (UINT16)InMemberList.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::JoinClusterRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinClusterRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList )

			HRESULT JoinClusterRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT JoinClusterRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT JoinClusterRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);
				pCur += sizeof(ServiceInformation)*m_MemberList.GetSize(); iMsgSize -= (INT)(sizeof(ServiceInformation)*m_MemberList.GetSize());


			Proc_End:

				return hr;

			}; // HRESULT JoinClusterRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void JoinClusterRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:JoinClusterRes:{1}:{2} , Context:{3}, Result:{4:X8}, RouteContext:{5}, MemberList:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result, m_RouteContext, m_MemberList); 
			}; // void JoinClusterRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Do not let it broadcasted while it's manual broadcast packet
			const MessageID NewServerServiceJoinedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 2);
			HRESULT NewServerServiceJoinedC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceUID, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceNetClass, pCur, iMsgSize, sizeof(NetClass) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceAddress, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceMembership, pCur, iMsgSize, sizeof(ClusterMembership) ) );


			Proc_End:

				return hr;

			}; // HRESULT NewServerServiceJoinedC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT NewServerServiceJoinedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(EntityUID)
					+ sizeof(NetClass)
					+ sizeof(NetAddress)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ClusterMembership));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::NewServerServiceJoinedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceNetClass, sizeof(NetClass));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceAddress, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceMembership, sizeof(ClusterMembership));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT NewServerServiceJoinedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )

			HRESULT NewServerServiceJoinedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT NewServerServiceJoinedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT NewServerServiceJoinedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT NewServerServiceJoinedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void NewServerServiceJoinedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:NewServerServiceJoinedC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, JoinedServiceUID:{5}, JoinedServiceNetClass:{6}, JoinedServiceAddress:{7}, ClusterID:{8}, ClusterType:{9}, JoinedServiceMembership:{10}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_JoinedServiceUID, m_JoinedServiceNetClass, m_JoinedServiceAddress, m_ClusterID, m_ClusterType, m_JoinedServiceMembership); 
			}; // void NewServerServiceJoinedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Sync cluster service informations
			const MessageID SyncClusterServiceC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 3);
			HRESULT SyncClusterServiceC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // HRESULT SyncClusterServiceC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT SyncClusterServiceC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ServiceInformation)*InMemberList.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				UINT16 numberOfInMemberList = (UINT16)InMemberList.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::SyncClusterServiceC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SyncClusterServiceC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )

			HRESULT SyncClusterServiceC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT SyncClusterServiceC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT SyncClusterServiceC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT SyncClusterServiceC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void SyncClusterServiceC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:SyncClusterServiceC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, ClusterID:{5}, ClusterType:{6}, MemberList:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_ClusterID, m_ClusterType, m_MemberList); 
			}; // void SyncClusterServiceC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join to the cluster
			const MessageID RequestDataSyncCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 4);
			HRESULT RequestDataSyncCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // HRESULT RequestDataSyncCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RequestDataSyncCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(ClusterID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::RequestDataSyncCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RequestDataSyncCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )

			HRESULT RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void RequestDataSyncCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:RequestDataSyncCmd:{1}:{2} , Context:{3}, RouteContext:{4}, RouteHopCount:{5}, ClusterID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_RouteHopCount, m_ClusterID); 
			}; // void RequestDataSyncCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestDataSyncRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 4);
			HRESULT RequestDataSyncRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT RequestDataSyncRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RequestDataSyncRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::RequestDataSyncRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RequestDataSyncRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT RequestDataSyncRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);


			Proc_End:

				return hr;

			}; // HRESULT RequestDataSyncRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void RequestDataSyncRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:RequestDataSyncRes:{1}:{2} , Context:{3}, Result:{4:X8}, RouteContext:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result, m_RouteContext); 
			}; // void RequestDataSyncRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Master instance of the cluster is assigned
			const MessageID ClusterMasterAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 5);
			HRESULT ClusterMasterAssignedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MasterUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT ClusterMasterAssignedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT ClusterMasterAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::ClusterMasterAssignedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InMasterUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ClusterMasterAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )

			HRESULT ClusterMasterAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT ClusterMasterAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT ClusterMasterAssignedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT ClusterMasterAssignedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ClusterMasterAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ClusterMasterAssignedS2CEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, Sender:{5}, ClusterID:{6}, MasterUID:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_Sender, m_ClusterID, m_MasterUID); 
			}; // void ClusterMasterAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Master vote
			const MessageID ClusterMasterVoteC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 6);
			HRESULT ClusterMasterVoteC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VoteToUID, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VotedUpTime, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT ClusterMasterVoteC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT ClusterMasterVoteC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(ClusterID)
					+ sizeof(EntityUID)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::ClusterMasterVoteC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InVoteToUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InVotedUpTime, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ClusterMasterVoteC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime )

			HRESULT ClusterMasterVoteC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT ClusterMasterVoteC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT ClusterMasterVoteC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT ClusterMasterVoteC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ClusterMasterVoteC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ClusterMasterVoteC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, ClusterID:{5}, VoteToUID:{6}, VotedUpTime:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_ClusterID, m_VoteToUID, m_VotedUpTime); 
			}; // void ClusterMasterVoteC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Update cluster service status
			const MessageID ClusterUpdateStatusC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 7);
			HRESULT ClusterUpdateStatusC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MemberStatus, pCur, iMsgSize, sizeof(ServiceStatus) ) );


			Proc_End:

				return hr;

			}; // HRESULT ClusterUpdateStatusC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT ClusterUpdateStatusC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(ServiceStatus));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::ClusterUpdateStatusC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InMemberStatus, sizeof(ServiceStatus));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ClusterUpdateStatusC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )

			HRESULT ClusterUpdateStatusC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT ClusterUpdateStatusC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT ClusterUpdateStatusC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT ClusterUpdateStatusC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ClusterUpdateStatusC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ClusterUpdateStatusC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, Sender:{5}, ClusterID:{6}, MemberStatus:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_Sender, m_ClusterID, m_MemberStatus); 
			}; // void ClusterUpdateStatusC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Update cluster service workload
			const MessageID ClusterUpdateWorkloadC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 8);
			HRESULT ClusterUpdateWorkloadC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Workload, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT ClusterUpdateWorkloadC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT ClusterUpdateWorkloadC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::ClusterUpdateWorkloadC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InWorkload, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ClusterUpdateWorkloadC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload )

			HRESULT ClusterUpdateWorkloadC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT ClusterUpdateWorkloadC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT ClusterUpdateWorkloadC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT ClusterUpdateWorkloadC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ClusterUpdateWorkloadC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ClusterUpdateWorkloadC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, Sender:{5}, ClusterID:{6}, Workload:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_Sender, m_ClusterID, m_Workload); 
			}; // void ClusterUpdateWorkloadC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get lowest workloaded cluster member
			const MessageID GetLowestWorkloadClusterMemberCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 9);
			HRESULT GetLowestWorkloadClusterMemberCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetLowestWorkloadClusterMemberCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetLowestWorkloadClusterMemberCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(ClusterID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GetLowestWorkloadClusterMemberCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetLowestWorkloadClusterMemberCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID )

			HRESULT GetLowestWorkloadClusterMemberCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GetLowestWorkloadClusterMemberCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT GetLowestWorkloadClusterMemberCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT GetLowestWorkloadClusterMemberCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GetLowestWorkloadClusterMemberCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:GetLowestWorkloadClusterMemberCmd:{1}:{2} , Context:{3}, RouteContext:{4}, RouteHopCount:{5}, ClusterID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_RouteHopCount, m_ClusterID); 
			}; // void GetLowestWorkloadClusterMemberCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetLowestWorkloadClusterMemberRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 9);
			HRESULT GetLowestWorkloadClusterMemberRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Member, pCur, iMsgSize, sizeof(ServiceInformation) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetLowestWorkloadClusterMemberRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetLowestWorkloadClusterMemberRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const ServiceInformation &InMember )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext)
					+ sizeof(ServiceInformation));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GetLowestWorkloadClusterMemberRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InMember, sizeof(ServiceInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetLowestWorkloadClusterMemberRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const ServiceInformation &InMember )

			HRESULT GetLowestWorkloadClusterMemberRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GetLowestWorkloadClusterMemberRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT GetLowestWorkloadClusterMemberRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(ServiceInformation); iMsgSize -= sizeof(ServiceInformation);


			Proc_End:

				return hr;

			}; // HRESULT GetLowestWorkloadClusterMemberRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GetLowestWorkloadClusterMemberRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:GetLowestWorkloadClusterMemberRes:{1}:{2} , Context:{3}, Result:{4:X8}, RouteContext:{5}, Member:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result, m_RouteContext, m_Member); 
			}; // void GetLowestWorkloadClusterMemberRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Called when a player entity is created
			const MessageID GamePlayerEntityCreatedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 10);
			HRESULT GamePlayerEntityCreatedC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerEntityCreatedC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerEntityCreatedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(PlayerID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GamePlayerEntityCreatedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerEntityCreatedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )

			HRESULT GamePlayerEntityCreatedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerEntityCreatedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT GamePlayerEntityCreatedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerEntityCreatedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GamePlayerEntityCreatedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerEntityCreatedC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, PlayerID:{5}, PlayerUID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_PlayerID, m_PlayerUID); 
			}; // void GamePlayerEntityCreatedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Called when a player entity is deleted
			const MessageID GamePlayerEntityDeletedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 11);
			HRESULT GamePlayerEntityDeletedC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerEntityDeletedC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerEntityDeletedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(PlayerID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GamePlayerEntityDeletedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerEntityDeletedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )

			HRESULT GamePlayerEntityDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerEntityDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT GamePlayerEntityDeletedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerEntityDeletedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GamePlayerEntityDeletedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerEntityDeletedC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, PlayerID:{5}, PlayerUID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_PlayerID, m_PlayerUID); 
			}; // void GamePlayerEntityDeletedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace ClusterServer
	}; // namespace Message
}; // namespace BR


