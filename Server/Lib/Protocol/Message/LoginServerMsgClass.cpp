////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : LoginServer Message parser implementations
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
#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace LoginServer
		{
 			// Cmd: Notify user joind and see it's valid authticket instance
			const MessageID PlayerJoinedToGameServerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_LOGINSERVER, 0);
			HRESULT PlayerJoinedToGameServerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_AuthTicket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerJoinedToGameServerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerJoinedToGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( LoginServer::PlayerJoinedToGameServerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InAuthTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerJoinedToGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )

			HRESULT PlayerJoinedToGameServerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT PlayerJoinedToGameServerCmd::OverrideRouteContextDestination( EntityUID to )


			void PlayerJoinedToGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerJoinedToGameServerCmd:{1}:{2} , RouteContext:{3}, Context:{4}, PlayerID:{5}, AuthTicket:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_PlayerID, m_AuthTicket); 
			}; // void PlayerJoinedToGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PlayerJoinedToGameServerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_LOGINSERVER, 0);
			HRESULT PlayerJoinedToGameServerRes::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT PlayerJoinedToGameServerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerJoinedToGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( LoginServer::PlayerJoinedToGameServerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerJoinedToGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )

			HRESULT PlayerJoinedToGameServerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT PlayerJoinedToGameServerRes::OverrideRouteContextDestination( EntityUID to )


			void PlayerJoinedToGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerJoinedToGameServerRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result); 
			}; // void PlayerJoinedToGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick logged in player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_LOGINSERVER, 1);
			HRESULT KickPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InKickedPlayerID )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( LoginServer::KickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InKickedPlayerID )

			HRESULT KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			void KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:KickPlayerCmd:{1}:{2} , RouteContext:{3}, Context:{4}, KickedPlayerID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_KickedPlayerID); 
			}; // void KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_LOGINSERVER, 1);
			HRESULT KickPlayerRes::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT KickPlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( LoginServer::KickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )

			HRESULT KickPlayerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT KickPlayerRes::OverrideRouteContextDestination( EntityUID to )


			void KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:KickPlayerRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result); 
			}; // void KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace LoginServer
	}; // namespace Message
}; // namespace BR


