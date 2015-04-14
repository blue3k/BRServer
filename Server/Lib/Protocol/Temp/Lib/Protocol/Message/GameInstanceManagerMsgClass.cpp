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
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NumberOfBotPlayer, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(UINT16) ) );


			Proc_End:

				return hr;

			}; // HRESULT CreateGameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT CreateGameCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(UINT16)
					+ sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstanceManager::CreateGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InNumberOfBotPlayer, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(UINT16));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CreateGameCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer )

			HRESULT CreateGameCmd::OverrideRouteContextDestination( EntityUID to )
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
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT CreateGameCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT CreateGameCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT CreateGameCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			VOID CreateGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:CreateGameCmd:%1%:%2% , Context:%3%, RouteContext:%4%, RouteHopCount:%5%, NumberOfBotPlayer:%6%, MaxPlayer:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_RouteHopCount, m_NumberOfBotPlayer, m_MaxPlayer); 
			}; // VOID CreateGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CreateGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCEMANAGER, 0);
			HRESULT CreateGameRes::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT CreateGameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT CreateGameRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstanceManager::CreateGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CreateGameRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT CreateGameRes::OverrideRouteContextDestination( EntityUID to )
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
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT CreateGameRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT CreateGameRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);


			Proc_End:

				return hr;

			}; // HRESULT CreateGameRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			VOID CreateGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:CreateGameRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext); 
			}; // VOID CreateGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Game instance notify of deletion
			const MessageID GameDeletedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCEMANAGER, 1);
			HRESULT GameDeletedC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

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
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16));

				MessageData *pNewMsg = NULL;

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
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GameDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT GameDeletedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= sizeof(UINT16) );
				*(UINT16*)pCur = hopCount;
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);


			Proc_End:

				return hr;

			}; // HRESULT GameDeletedC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			VOID GameDeletedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameDeletedC2SEvt:%1%:%2% , RouteContext:%3%, RouteHopCount:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount); 
			}; // VOID GameDeletedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace GameInstanceManager
	}; // namespace Message
}; // namespace BR


