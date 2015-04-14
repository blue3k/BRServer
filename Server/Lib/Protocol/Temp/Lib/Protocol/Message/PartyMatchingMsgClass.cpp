////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message parser implementations
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
#include "Protocol/Message/PartyMatchingMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace PartyMatching
		{
 			// S2C: 
			const MessageID PartyGameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHING, 0);
			HRESULT PartyGameMatchedS2CEvt::ParseIMsg( MessageData* pIMsg )
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

			}; // HRESULT PartyGameMatchedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyGameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatching::PartyGameMatchedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyGameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount )

			HRESULT PartyGameMatchedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT PartyGameMatchedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT PartyGameMatchedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT PartyGameMatchedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			VOID PartyGameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyGameMatchedS2CEvt:%1%:%2% , RouteContext:%3%, RouteHopCount:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount); 
			}; // VOID PartyGameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: 
			const MessageID PlayerGameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_PARTYMATCHING, 1);
			HRESULT PlayerGameMatchedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerGameMatchedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerGameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT16)
					+ sizeof(PlayerID)
					+ sizeof(GameInsUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( PartyMatching::PlayerGameMatchedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerGameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID )

			HRESULT PlayerGameMatchedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT PlayerGameMatchedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			HRESULT PlayerGameMatchedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )
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

			}; // HRESULT PlayerGameMatchedS2CEvt::OverrideRouteInformation( EntityUID to, UINT hopCount )

			VOID PlayerGameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerGameMatchedS2CEvt:%1%:%2% , RouteContext:%3%, RouteHopCount:%4%, DestPlayerID:%5%, GameInsUID:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RouteHopCount, m_DestPlayerID, m_GameInsUID); 
			}; // VOID PlayerGameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace PartyMatching
	}; // namespace Message
}; // namespace BR


