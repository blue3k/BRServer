////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message parser implementations
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
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace PartyMatchingQueue
		{
 			// Cmd: Register match by party
			const MessageID RegisterPartyMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 0);
			HRESULT RegisterPartyMatchingCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofPlayers = 0; MatchingPlayerInformation* pPlayers = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPlayers, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pPlayers, pCur, iMsgSize, sizeof(MatchingPlayerInformation)*numberofPlayers ) );
				m_Players.SetLinkedBuffer(numberofPlayers, numberofPlayers, pPlayers);


			Proc_End:

				return hr;

			}; // HRESULT RegisterPartyMatchingCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RegisterPartyMatchingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(MatchingPlayerInformation)*InPlayers.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInPlayers = (UINT16)InPlayers.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::RegisterPartyMatchingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &numberOfInPlayers, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InPlayers.data(), (INT)(sizeof(MatchingPlayerInformation)*InPlayers.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RegisterPartyMatchingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )

			HRESULT RegisterPartyMatchingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT RegisterPartyMatchingCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT RegisterPartyMatchingCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT RegisterPartyMatchingCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void RegisterPartyMatchingCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterPartyMatchingCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, Players:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_Players); 
			}; // void RegisterPartyMatchingCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterPartyMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 0);
			HRESULT RegisterPartyMatchingRes::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT RegisterPartyMatchingRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RegisterPartyMatchingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::RegisterPartyMatchingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RegisterPartyMatchingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket )

			HRESULT RegisterPartyMatchingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT RegisterPartyMatchingRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT RegisterPartyMatchingRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				pCur += sizeof(MatchingQueueTicket); iMsgSize -= sizeof(MatchingQueueTicket);


			Proc_End:

				return hr;

			}; // HRESULT RegisterPartyMatchingRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void RegisterPartyMatchingRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterPartyMatchingRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}, MatchingTicket:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result, m_MatchingTicket); 
			}; // void RegisterPartyMatchingRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Register match alone
			const MessageID RegisterPlayerMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 1);
			HRESULT RegisterPlayerMatchingCmd::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT RegisterPlayerMatchingCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RegisterPlayerMatchingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::RegisterPlayerMatchingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RegisterPlayerMatchingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID )

			HRESULT RegisterPlayerMatchingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT RegisterPlayerMatchingCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT RegisterPlayerMatchingCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT RegisterPlayerMatchingCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void RegisterPlayerMatchingCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterPlayerMatchingCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, PlayerID:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_PlayerID); 
			}; // void RegisterPlayerMatchingCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterPlayerMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 1);
			HRESULT RegisterPlayerMatchingRes::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT RegisterPlayerMatchingRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RegisterPlayerMatchingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::RegisterPlayerMatchingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RegisterPlayerMatchingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket )

			HRESULT RegisterPlayerMatchingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT RegisterPlayerMatchingRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT RegisterPlayerMatchingRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				pCur += sizeof(MatchingQueueTicket); iMsgSize -= sizeof(MatchingQueueTicket);


			Proc_End:

				return hr;

			}; // HRESULT RegisterPlayerMatchingRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void RegisterPlayerMatchingRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterPlayerMatchingRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}, MatchingTicket:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result, m_MatchingTicket); 
			}; // void RegisterPlayerMatchingRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: update registration information
			const MessageID UpdateMatchingEntityUIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 2);
			HRESULT UpdateMatchingEntityUIDCmd::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PreviousUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT UpdateMatchingEntityUIDCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT UpdateMatchingEntityUIDCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(MatchingQueueTicket)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::UpdateMatchingEntityUIDCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));
				Protocol::PackParamCopy( pMsgData, &InPreviousUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UpdateMatchingEntityUIDCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID )

			HRESULT UpdateMatchingEntityUIDCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT UpdateMatchingEntityUIDCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT UpdateMatchingEntityUIDCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT UpdateMatchingEntityUIDCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void UpdateMatchingEntityUIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UpdateMatchingEntityUIDCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, MatchingTicket:{6}, PreviousUID:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_MatchingTicket, m_PreviousUID); 
			}; // void UpdateMatchingEntityUIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UpdateMatchingEntityUIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 2);
			HRESULT UpdateMatchingEntityUIDRes::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT UpdateMatchingEntityUIDRes::ParseIMsg( MessageData* pIMsg )

			HRESULT UpdateMatchingEntityUIDRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::UpdateMatchingEntityUIDRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UpdateMatchingEntityUIDRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )

			HRESULT UpdateMatchingEntityUIDRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT UpdateMatchingEntityUIDRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT UpdateMatchingEntityUIDRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT UpdateMatchingEntityUIDRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void UpdateMatchingEntityUIDRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UpdateMatchingEntityUIDRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result); 
			}; // void UpdateMatchingEntityUIDRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: calcel registration
			const MessageID UnregisterMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 3);
			HRESULT UnregisterMatchingCmd::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT UnregisterMatchingCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT UnregisterMatchingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::UnregisterMatchingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UnregisterMatchingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			HRESULT UnregisterMatchingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT UnregisterMatchingCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT UnregisterMatchingCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT UnregisterMatchingCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void UnregisterMatchingCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UnregisterMatchingCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, MatchingTicket:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_MatchingTicket); 
			}; // void UnregisterMatchingCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UnregisterMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 3);
			HRESULT UnregisterMatchingRes::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT UnregisterMatchingRes::ParseIMsg( MessageData* pIMsg )

			HRESULT UnregisterMatchingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::UnregisterMatchingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UnregisterMatchingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )

			HRESULT UnregisterMatchingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT UnregisterMatchingRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT UnregisterMatchingRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT UnregisterMatchingRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void UnregisterMatchingRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UnregisterMatchingRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result); 
			}; // void UnregisterMatchingRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: For some reason, matching is canceled
			const MessageID PartyMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 4);
			HRESULT PartyMatchingCanceledS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyMatchingCanceledS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(PartyUID)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::PartyMatchingCanceledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InDestPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )

			HRESULT PartyMatchingCanceledS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT PartyMatchingCanceledS2CEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT PartyMatchingCanceledS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT PartyMatchingCanceledS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void PartyMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyMatchingCanceledS2CEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, DestPartyUID:{5}, MatchingTicket:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_DestPartyUID, m_MatchingTicket); 
			}; // void PartyMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: For some reason, matching is canceled
			const MessageID PlayerMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 5);
			HRESULT PlayerMatchingCanceledS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerMatchingCanceledS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(PlayerID)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )

			HRESULT PlayerMatchingCanceledS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT PlayerMatchingCanceledS2CEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT PlayerMatchingCanceledS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT PlayerMatchingCanceledS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void PlayerMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerMatchingCanceledS2CEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, DestPlayerID:{5}, MatchingTicket:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_DestPlayerID, m_MatchingTicket); 
			}; // void PlayerMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Enqueued item is dequeued
			const MessageID PartyMatchingItemDequeuedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 6);
			HRESULT PartyMatchingItemDequeuedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyMatchingItemDequeuedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyMatchingItemDequeuedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyMatchingItemDequeuedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			HRESULT PartyMatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT PartyMatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT PartyMatchingItemDequeuedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT PartyMatchingItemDequeuedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void PartyMatchingItemDequeuedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyMatchingItemDequeuedS2CEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, MatchingTicket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_MatchingTicket); 
			}; // void PartyMatchingItemDequeuedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Enqueued item is dequeued
			const MessageID PlayerMatchingItemDequeuedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 7);
			HRESULT PlayerMatchingItemDequeuedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerMatchingItemDequeuedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerMatchingItemDequeuedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerMatchingItemDequeuedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			HRESULT PlayerMatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT PlayerMatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT PlayerMatchingItemDequeuedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT PlayerMatchingItemDequeuedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void PlayerMatchingItemDequeuedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerMatchingItemDequeuedS2CEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, MatchingTicket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_MatchingTicket); 
			}; // void PlayerMatchingItemDequeuedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Reserve a item
			const MessageID ReserveItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 8);
			HRESULT ReserveItemCmd::ParseIMsg( MessageData* pIMsg )
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


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT ReserveItemCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::ReserveItemCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount )

			HRESULT ReserveItemCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT ReserveItemCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT ReserveItemCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT ReserveItemCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ReserveItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ReserveItemCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount); 
			}; // void ReserveItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID ReserveItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 8);
			HRESULT ReserveItemRes::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_NumberOfPlayersInTheItem, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemRes::ParseIMsg( MessageData* pIMsg )

			HRESULT ReserveItemRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(UINT32)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::ReserveItemRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InNumberOfPlayersInTheItem, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )

			HRESULT ReserveItemRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT ReserveItemRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT ReserveItemRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				pCur += sizeof(UINT32); iMsgSize -= sizeof(UINT32);
				pCur += sizeof(MatchingQueueTicket); iMsgSize -= sizeof(MatchingQueueTicket);


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ReserveItemRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ReserveItemRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}, NumberOfPlayersInTheItem:{6}, MatchingTicket:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result, m_NumberOfPlayersInTheItem, m_MatchingTicket); 
			}; // void ReserveItemRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Reserve a item
			const MessageID ReserveItemsCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 9);
			HRESULT ReserveItemsCmd::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_NumberOfItemsToReserve, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemsCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT ReserveItemsCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::ReserveItemsCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InNumberOfItemsToReserve, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemsCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve )

			HRESULT ReserveItemsCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT ReserveItemsCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT ReserveItemsCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT ReserveItemsCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ReserveItemsCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ReserveItemsCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, NumberOfItemsToReserve:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_NumberOfItemsToReserve); 
			}; // void ReserveItemsCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID ReserveItemsRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 9);
			HRESULT ReserveItemsRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofNumberOfPlayersInTheItem = 0; UINT32* pNumberOfPlayersInTheItem = nullptr;
				UINT16 numberofMatchingTicket = 0; MatchingQueueTicket* pMatchingTicket = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofNumberOfPlayersInTheItem, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pNumberOfPlayersInTheItem, pCur, iMsgSize, sizeof(UINT32)*numberofNumberOfPlayersInTheItem ) );
				m_NumberOfPlayersInTheItem.SetLinkedBuffer(numberofNumberOfPlayersInTheItem, numberofNumberOfPlayersInTheItem, pNumberOfPlayersInTheItem);
				protocolChk( Protocol::StreamParamCopy( &numberofMatchingTicket, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pMatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket)*numberofMatchingTicket ) );
				m_MatchingTicket.SetLinkedBuffer(numberofMatchingTicket, numberofMatchingTicket, pMatchingTicket);


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemsRes::ParseIMsg( MessageData* pIMsg )

			HRESULT ReserveItemsRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const Array<UINT32>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(UINT32)*InNumberOfPlayersInTheItem.GetSize() + sizeof(UINT16)
					+ sizeof(MatchingQueueTicket)*InMatchingTicket.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInNumberOfPlayersInTheItem = (UINT16)InNumberOfPlayersInTheItem.GetSize(); 
				UINT16 numberOfInMatchingTicket = (UINT16)InMatchingTicket.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::ReserveItemsRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &numberOfInNumberOfPlayersInTheItem, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InNumberOfPlayersInTheItem.data(), (INT)(sizeof(UINT32)*InNumberOfPlayersInTheItem.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInMatchingTicket, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InMatchingTicket.data(), (INT)(sizeof(MatchingQueueTicket)*InMatchingTicket.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemsRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const Array<UINT32>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )

			HRESULT ReserveItemsRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT ReserveItemsRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT ReserveItemsRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);
				pCur += sizeof(UINT32)*m_NumberOfPlayersInTheItem.GetSize(); iMsgSize -= (INT)(sizeof(UINT32)*m_NumberOfPlayersInTheItem.GetSize());
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);
				pCur += sizeof(MatchingQueueTicket)*m_MatchingTicket.GetSize(); iMsgSize -= (INT)(sizeof(MatchingQueueTicket)*m_MatchingTicket.GetSize());


			Proc_End:

				return hr;

			}; // HRESULT ReserveItemsRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void ReserveItemsRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ReserveItemsRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}, NumberOfPlayersInTheItem:{6}, MatchingTicket:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result, m_NumberOfPlayersInTheItem, m_MatchingTicket); 
			}; // void ReserveItemsRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Cancel reservation
			const MessageID CancelReservationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 10);
			HRESULT CancelReservationCmd::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_TicketToCancel, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT CancelReservationCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT CancelReservationCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::CancelReservationCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InTicketToCancel, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CancelReservationCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )

			HRESULT CancelReservationCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT CancelReservationCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT CancelReservationCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT CancelReservationCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void CancelReservationCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CancelReservationCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, TicketToCancel:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_TicketToCancel); 
			}; // void CancelReservationCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CancelReservationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 10);
			HRESULT CancelReservationRes::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT CancelReservationRes::ParseIMsg( MessageData* pIMsg )

			HRESULT CancelReservationRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::CancelReservationRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CancelReservationRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )

			HRESULT CancelReservationRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT CancelReservationRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT CancelReservationRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT CancelReservationRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void CancelReservationRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CancelReservationRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result); 
			}; // void CancelReservationRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Cancel reservation
			const MessageID CancelReservationsCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 11);
			HRESULT CancelReservationsCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofTicketToCancel = 0; MatchingQueueTicket* pTicketToCancel = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofTicketToCancel, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pTicketToCancel, pCur, iMsgSize, sizeof(MatchingQueueTicket)*numberofTicketToCancel ) );
				m_TicketToCancel.SetLinkedBuffer(numberofTicketToCancel, numberofTicketToCancel, pTicketToCancel);


			Proc_End:

				return hr;

			}; // HRESULT CancelReservationsCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT CancelReservationsCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(MatchingQueueTicket)*InTicketToCancel.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInTicketToCancel = (UINT16)InTicketToCancel.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::CancelReservationsCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &numberOfInTicketToCancel, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InTicketToCancel.data(), (INT)(sizeof(MatchingQueueTicket)*InTicketToCancel.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CancelReservationsCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )

			HRESULT CancelReservationsCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT CancelReservationsCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT CancelReservationsCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT CancelReservationsCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void CancelReservationsCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CancelReservationsCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, TicketToCancel:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_TicketToCancel); 
			}; // void CancelReservationsCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CancelReservationsRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 11);
			HRESULT CancelReservationsRes::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT CancelReservationsRes::ParseIMsg( MessageData* pIMsg )

			HRESULT CancelReservationsRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::CancelReservationsRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CancelReservationsRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )

			HRESULT CancelReservationsRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT CancelReservationsRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT CancelReservationsRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT CancelReservationsRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void CancelReservationsRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CancelReservationsRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result); 
			}; // void CancelReservationsRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Dequeue a reserved item
			const MessageID DequeueItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 12);
			HRESULT DequeueItemCmd::ParseIMsg( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT DequeueItemCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT DequeueItemCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(UINT16)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::DequeueItemCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT DequeueItemCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			HRESULT DequeueItemCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT DequeueItemCmd::OverrideRouteContextDestination( EntityUID to )

			HRESULT DequeueItemCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT DequeueItemCmd::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void DequeueItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:DequeueItemCmd:{1}:{2} , RouteContext:{3}, Context:{4}, RouteHopCount:{5}, MatchingTicket:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_RouteHopCount, m_MatchingTicket); 
			}; // void DequeueItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID DequeueItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 12);
			HRESULT DequeueItemRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofPlayers = 0; MatchingPlayerInformation* pPlayers = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RegisterUID, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RegisterID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPlayers, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pPlayers, pCur, iMsgSize, sizeof(MatchingPlayerInformation)*numberofPlayers ) );
				m_Players.SetLinkedBuffer(numberofPlayers, numberofPlayers, pPlayers);


			Proc_End:

				return hr;

			}; // HRESULT DequeueItemRes::ParseIMsg( MessageData* pIMsg )

			HRESULT DequeueItemRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(MatchingQueueTicket)
					+ sizeof(EntityUID)
					+ sizeof(PlayerID)
					+ sizeof(MatchingPlayerInformation)*InPlayers.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInPlayers = (UINT16)InPlayers.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::DequeueItemRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));
				Protocol::PackParamCopy( pMsgData, &InRegisterUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InRegisterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &numberOfInPlayers, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InPlayers.data(), (INT)(sizeof(MatchingPlayerInformation)*InPlayers.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT DequeueItemRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )

			HRESULT DequeueItemRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT DequeueItemRes::OverrideRouteContextDestination( EntityUID to )

			HRESULT DequeueItemRes::OverrideRouteInformation( EntityUID to, UINT hopCount )
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
				pCur += sizeof(MatchingQueueTicket); iMsgSize -= sizeof(MatchingQueueTicket);
				pCur += sizeof(EntityUID); iMsgSize -= sizeof(EntityUID);
				pCur += sizeof(PlayerID); iMsgSize -= sizeof(PlayerID);
				pCur += sizeof(UINT16); iMsgSize -= sizeof(UINT16);
				pCur += sizeof(MatchingPlayerInformation)*m_Players.GetSize(); iMsgSize -= (INT)(sizeof(MatchingPlayerInformation)*m_Players.GetSize());


			Proc_End:

				return hr;

			}; // HRESULT DequeueItemRes::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void DequeueItemRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:DequeueItemRes:{1}:{2} , RouteContext:{3}, Context:{4}, Result:{5:X8}, MatchingTicket:{6}, RegisterUID:{7}, RegisterID:{8}, Players:{9}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Context, m_Result, m_MatchingTicket, m_RegisterUID, m_RegisterID, m_Players); 
			}; // void DequeueItemRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Item error you should delete it
			const MessageID MatchingItemErrorC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHINGQUEUE, 13);
			HRESULT MatchingItemErrorC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT MatchingItemErrorC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT MatchingItemErrorC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatchingQueue::MatchingItemErrorC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT MatchingItemErrorC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			HRESULT MatchingItemErrorC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT MatchingItemErrorC2SEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT MatchingItemErrorC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT MatchingItemErrorC2SEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			void MatchingItemErrorC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:MatchingItemErrorC2SEvt:{1}:{2} , RouteContext:{3}, RouteHopCount:{4}, MatchingTicket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_MatchingTicket); 
			}; // void MatchingItemErrorC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace PartyMatchingQueue
	}; // namespace Message
}; // namespace BR


