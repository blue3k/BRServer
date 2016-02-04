////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Server Message parser implementations
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
#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace Server
		{
 			// Cmd: 
			const MessageID GenericFailureCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_SERVER, 0);
			HRESULT GenericFailureCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );


			Proc_End:

				return hr;

			}; // HRESULT GenericFailureCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GenericFailureCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Server::GenericFailureCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GenericFailureCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext )

			HRESULT GenericFailureCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_SYSTEM_OK;

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

			}; // HRESULT GenericFailureCmd::OverrideRouteContextDestination( EntityUID to )


			void GenericFailureCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GenericFailureCmd:{1}:{2} , RouteContext:{3}, Context:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context); 
			}; // void GenericFailureCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GenericFailureRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_SERVER, 0);
			HRESULT GenericFailureRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT GenericFailureRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GenericFailureRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Server::GenericFailureRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GenericFailureRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )

			HRESULT GenericFailureRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_SYSTEM_OK;

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

			}; // HRESULT GenericFailureRes::OverrideRouteContextDestination( EntityUID to )


			void GenericFailureRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GenericFailureRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result); 
			}; // void GenericFailureRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Server Started or Connected
			const MessageID ServerConnectedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_SERVER, 1);
			HRESULT ServerConnectedC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterManagerServiceInformation, pCur, iMsgSize, sizeof(ServiceInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_StartUpTime, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PrivateAddress, pCur, iMsgSize, sizeof(NetAddress) ) );


			Proc_End:

				return hr;

			}; // HRESULT ServerConnectedC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT ServerConnectedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPrivateAddress )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(ServiceInformation)
					+ sizeof(UINT32)
					+ sizeof(NetAddress));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Server::ServerConnectedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InClusterManagerServiceInformation, sizeof(ServiceInformation));
				Protocol::PackParamCopy( pMsgData, &InStartUpTime, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InPrivateAddress, sizeof(NetAddress));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ServerConnectedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPrivateAddress )

			HRESULT ServerConnectedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_SYSTEM_OK;

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

			}; // HRESULT ServerConnectedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			void ServerConnectedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:ServerConnectedC2SEvt:{1}:{2} , RouteContext:{3}, ClusterManagerServiceInformation:{4}, StartUpTime:{5}, PrivateAddress:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_ClusterManagerServiceInformation, m_StartUpTime, m_PrivateAddress); 
			}; // void ServerConnectedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Server
	}; // namespace Message
}; // namespace BR


