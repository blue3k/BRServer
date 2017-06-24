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
			Result GetClusterMemberListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::ParseMessage( MessageData* pIMsg )

			Result GetClusterMemberListCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GetClusterMemberListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )

			Result GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= (int)sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GetClusterMemberListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:GetClusterMemberListCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, RouteHopCount:{5}, ClusterID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_RouteHopCount, m_ClusterID); 
			}; // void GetClusterMemberListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetClusterMemberListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 0);
			Result GetClusterMemberListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, (int)sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::ParseMessage( MessageData* pIMsg )

			Result GetClusterMemberListRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(ServiceInformation)*InMemberList.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInMemberList = (UINT16)InMemberList.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GetClusterMemberListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )

			Result GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )

			Result GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= (int)sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= (int)sizeof(Result);
				pCur += sizeof(UINT16); iMsgSize -= (int)sizeof(UINT16);
				pCur += sizeof(ServiceInformation)*m_MemberList.GetSize(); iMsgSize -= (INT)(sizeof(ServiceInformation)*m_MemberList.GetSize());


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GetClusterMemberListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:GetClusterMemberListRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, MemberList:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_MemberList); 
			}; // void GetClusterMemberListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			const MessageID JoinClusterCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 1);
			Result JoinClusterCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderNetClass, pCur, iMsgSize, (int)sizeof(NetClass) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderAddress, pCur, iMsgSize, (int)sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, (int)sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterMembership, pCur, iMsgSize, (int)sizeof(ClusterMembership) ) );


			Proc_End:

				return hr;

			}; // Result JoinClusterCmd::ParseMessage( MessageData* pIMsg )

			Result JoinClusterCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(NetClass)
					+ sizeof(NetAddress)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ClusterMembership));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::JoinClusterCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InSenderNetClass, sizeof(NetClass));
				Protocol::PackParamCopy( pMsgData, &InSenderAddress, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &InClusterMembership, sizeof(ClusterMembership));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinClusterCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )

			Result JoinClusterCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result JoinClusterCmd::OverrideRouteContextDestination( EntityUID to )

			Result JoinClusterCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= (int)sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result JoinClusterCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void JoinClusterCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:JoinClusterCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, RouteHopCount:{5}, Sender:{6}, SenderNetClass:{7}, SenderAddress:{8}, ClusterID:{9}, ClusterType:{10}, ClusterMembership:{11}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_RouteHopCount, m_Sender, m_SenderNetClass, m_SenderAddress, m_ClusterID, m_ClusterType, m_ClusterMembership); 
			}; // void JoinClusterCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinClusterRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 1);
			Result JoinClusterRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, (int)sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // Result JoinClusterRes::ParseMessage( MessageData* pIMsg )

			Result JoinClusterRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(ServiceInformation)*InMemberList.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInMemberList = (UINT16)InMemberList.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::JoinClusterRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinClusterRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )

			Result JoinClusterRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result JoinClusterRes::OverrideRouteContextDestination( EntityUID to )

			Result JoinClusterRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= (int)sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= (int)sizeof(Result);
				pCur += sizeof(UINT16); iMsgSize -= (int)sizeof(UINT16);
				pCur += sizeof(ServiceInformation)*m_MemberList.GetSize(); iMsgSize -= (INT)(sizeof(ServiceInformation)*m_MemberList.GetSize());


			Proc_End:

				return hr;

			}; // Result JoinClusterRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void JoinClusterRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:JoinClusterRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, MemberList:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_MemberList); 
			}; // void JoinClusterRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Do not let it broadcasted while it's manual broadcast packet
			const MessageID NewServerServiceJoinedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 2);
			Result NewServerServiceJoinedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceNetClass, pCur, iMsgSize, (int)sizeof(NetClass) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceAddress, pCur, iMsgSize, (int)sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, (int)sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceMembership, pCur, iMsgSize, (int)sizeof(ClusterMembership) ) );


			Proc_End:

				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result NewServerServiceJoinedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(NetClass)
					+ sizeof(NetAddress)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ClusterMembership));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::NewServerServiceJoinedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceNetClass, sizeof(NetClass));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceAddress, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceMembership, sizeof(ClusterMembership));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )

			Result NewServerServiceJoinedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result NewServerServiceJoinedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void NewServerServiceJoinedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:NewServerServiceJoinedC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, JoinedServiceUID:{5}, JoinedServiceNetClass:{6}, JoinedServiceAddress:{7}, ClusterID:{8}, ClusterType:{9}, JoinedServiceMembership:{10}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_JoinedServiceUID, m_JoinedServiceNetClass, m_JoinedServiceAddress, m_ClusterID, m_ClusterType, m_JoinedServiceMembership); 
			}; // void NewServerServiceJoinedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Sync cluster service informations
			const MessageID SyncClusterServiceC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 3);
			Result SyncClusterServiceC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, (int)sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, (int)sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // Result SyncClusterServiceC2SEvt::ParseMessage( MessageData* pIMsg )

			Result SyncClusterServiceC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ServiceInformation)*InMemberList.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInMemberList = (UINT16)InMemberList.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::SyncClusterServiceC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SyncClusterServiceC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )

			Result SyncClusterServiceC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result SyncClusterServiceC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result SyncClusterServiceC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result SyncClusterServiceC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void SyncClusterServiceC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:SyncClusterServiceC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, ClusterID:{5}, ClusterType:{6}, MemberList:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_ClusterID, m_ClusterType, m_MemberList); 
			}; // void SyncClusterServiceC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join to the cluster
			const MessageID RequestDataSyncCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 4);
			Result RequestDataSyncCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::ParseMessage( MessageData* pIMsg )

			Result RequestDataSyncCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::RequestDataSyncCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )

			Result RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= (int)sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void RequestDataSyncCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:RequestDataSyncCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, RouteHopCount:{5}, ClusterID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_RouteHopCount, m_ClusterID); 
			}; // void RequestDataSyncCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestDataSyncRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 4);
			Result RequestDataSyncRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::ParseMessage( MessageData* pIMsg )

			Result RequestDataSyncRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::RequestDataSyncRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )

			Result RequestDataSyncRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= (int)sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= (int)sizeof(Result);


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void RequestDataSyncRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:RequestDataSyncRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result); 
			}; // void RequestDataSyncRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Master instance of the cluster is assigned
			const MessageID ClusterMasterAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 5);
			Result ClusterMasterAssignedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MasterUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result ClusterMasterAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::ClusterMasterAssignedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InMasterUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )

			Result ClusterMasterAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			Result ClusterMasterAssignedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ClusterMasterAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ClusterMasterAssignedS2CEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, Sender:{5}, ClusterID:{6}, MasterUID:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_Sender, m_ClusterID, m_MasterUID); 
			}; // void ClusterMasterAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Master vote
			const MessageID ClusterMasterVoteC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 6);
			Result ClusterMasterVoteC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VoteToUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VotedUpTime, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result ClusterMasterVoteC2SEvt::ParseMessage( MessageData* pIMsg )

			Result ClusterMasterVoteC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const uint64_t &InVotedUpTime )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID)
					+ sizeof(EntityUID)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::ClusterMasterVoteC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InVoteToUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InVotedUpTime, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ClusterMasterVoteC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const uint64_t &InVotedUpTime )

			Result ClusterMasterVoteC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ClusterMasterVoteC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result ClusterMasterVoteC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result ClusterMasterVoteC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ClusterMasterVoteC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ClusterMasterVoteC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, ClusterID:{5}, VoteToUID:{6}, VotedUpTime:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_ClusterID, m_VoteToUID, m_VotedUpTime); 
			}; // void ClusterMasterVoteC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Update cluster service status
			const MessageID ClusterUpdateStatusC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 7);
			Result ClusterUpdateStatusC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MemberStatus, pCur, iMsgSize, (int)sizeof(ServiceStatus) ) );


			Proc_End:

				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::ParseMessage( MessageData* pIMsg )

			Result ClusterUpdateStatusC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(ServiceStatus));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::ClusterUpdateStatusC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InMemberStatus, sizeof(ServiceStatus));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )

			Result ClusterUpdateStatusC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result ClusterUpdateStatusC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ClusterUpdateStatusC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ClusterUpdateStatusC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, Sender:{5}, ClusterID:{6}, MemberStatus:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_Sender, m_ClusterID, m_MemberStatus); 
			}; // void ClusterUpdateStatusC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Update cluster service workload
			const MessageID ClusterUpdateWorkloadC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 8);
			Result ClusterUpdateWorkloadC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Workload, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::ParseMessage( MessageData* pIMsg )

			Result ClusterUpdateWorkloadC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::ClusterUpdateWorkloadC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InWorkload, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )

			Result ClusterUpdateWorkloadC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result ClusterUpdateWorkloadC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ClusterUpdateWorkloadC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ClusterUpdateWorkloadC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, Sender:{5}, ClusterID:{6}, Workload:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_Sender, m_ClusterID, m_Workload); 
			}; // void ClusterUpdateWorkloadC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get lowest workloaded cluster member
			const MessageID GetLowestWorkloadClusterMemberCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 9);
			Result GetLowestWorkloadClusterMemberCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, (int)sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::ParseMessage( MessageData* pIMsg )

			Result GetLowestWorkloadClusterMemberCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GetLowestWorkloadClusterMemberCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result GetLowestWorkloadClusterMemberCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::OverrideRouteContextDestination( EntityUID to )

			Result GetLowestWorkloadClusterMemberCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= (int)sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GetLowestWorkloadClusterMemberCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:GetLowestWorkloadClusterMemberCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, RouteHopCount:{5}, ClusterID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_RouteHopCount, m_ClusterID); 
			}; // void GetLowestWorkloadClusterMemberCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetLowestWorkloadClusterMemberRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 9);
			Result GetLowestWorkloadClusterMemberRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Member, pCur, iMsgSize, (int)sizeof(ServiceInformation) ) );


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::ParseMessage( MessageData* pIMsg )

			Result GetLowestWorkloadClusterMemberRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(ServiceInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GetLowestWorkloadClusterMemberRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InMember, sizeof(ServiceInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember )

			Result GetLowestWorkloadClusterMemberRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::OverrideRouteContextDestination( EntityUID to )

			Result GetLowestWorkloadClusterMemberRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= (int)sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= (int)sizeof(Result);
				pCur += sizeof(ServiceInformation); iMsgSize -= (int)sizeof(ServiceInformation);


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GetLowestWorkloadClusterMemberRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:GetLowestWorkloadClusterMemberRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, Member:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_Member); 
			}; // void GetLowestWorkloadClusterMemberRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Called when a player entity is created
			const MessageID GamePlayerEntityCreatedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 10);
			Result GamePlayerEntityCreatedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result GamePlayerEntityCreatedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(PlayerID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GamePlayerEntityCreatedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )

			Result GamePlayerEntityCreatedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result GamePlayerEntityCreatedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GamePlayerEntityCreatedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerEntityCreatedC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, PlayerID:{5}, PlayerUID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_PlayerID, m_PlayerUID); 
			}; // void GamePlayerEntityCreatedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Called when a player entity is deleted
			const MessageID GamePlayerEntityDeletedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_CLUSTERSERVER, 11);
			Result GamePlayerEntityDeletedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result GamePlayerEntityDeletedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(PlayerID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( ClusterServer::GamePlayerEntityDeletedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )

			Result GamePlayerEntityDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result GamePlayerEntityDeletedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= (int)sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = (uint16_t)hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= (int)sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GamePlayerEntityDeletedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerEntityDeletedC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, PlayerID:{5}, PlayerUID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_PlayerID, m_PlayerUID); 
			}; // void GamePlayerEntityDeletedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace ClusterServer
	}; // namespace Message
}; // namespace BR


