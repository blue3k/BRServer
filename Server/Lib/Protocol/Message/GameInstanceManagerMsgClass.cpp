////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Message parser implementations
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
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace GameInstanceManager
		{
 			// Cmd: Create a game instance
			const MessageID CreateGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCEMANAGER, 0);
			HRESULT CreateGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NumberOfBotPlayer, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(UINT16) ) );


			Proc_End:

				return hr;

			}; // HRESULT CreateGameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT CreateGameCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(UINT16)
					+ sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstanceManager::CreateGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InNumberOfBotPlayer, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(UINT16));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CreateGameCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )

			HRESULT CreateGameCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT CreateGameCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT CreateGameCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT CreateGameCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void CreateGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CreateGameCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, NumberOfBotPlayer:{6}, MaxPlayer:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_NumberOfBotPlayer, m_MaxPlayer); 
			}; // void CreateGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CreateGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCEMANAGER, 0);
			HRESULT CreateGameRes::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT CreateGameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT CreateGameRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstanceManager::CreateGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CreateGameRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )

			HRESULT CreateGameRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT CreateGameRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT CreateGameRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);


			Proc_End:

				return hr;

			}; // HRESULT CreateGameRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void CreateGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CreateGameRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result); 
			}; // void CreateGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Game instance notify of deletion
			const MessageID GameDeletedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCEMANAGER, 1);
			HRESULT GameDeletedC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameDeletedC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameDeletedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstanceManager::GameDeletedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameDeletedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )

			HRESULT GameDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT GameDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT GameDeletedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT GameDeletedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void GameDeletedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameDeletedC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount); 
			}; // void GameDeletedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace GameInstanceManager
	}; // namespace Message
}; // namespace BR


