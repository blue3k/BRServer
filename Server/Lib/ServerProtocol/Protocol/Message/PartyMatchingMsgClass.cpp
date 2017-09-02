////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : PartyMatching Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace PartyMatching
		{
 			// S2C: 
			const MessageID PartyGameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHING, 0);
			Result PartyGameMatchedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );


			Proc_End:

				return hr;

			}; // Result PartyGameMatchedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PartyGameMatchedS2CEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				PartyGameMatchedS2CEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());


			Proc_End:

				return hr;

			}; // Result PartyGameMatchedS2CEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result PartyGameMatchedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PartyGameMatchedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PartyGameMatchedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PartyGameMatchedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatching::PartyGameMatchedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PartyGameMatchedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )

			Result PartyGameMatchedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result PartyGameMatchedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			Result PartyGameMatchedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result PartyGameMatchedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result PartyGameMatchedS2CEvt::TraceOut(MessageData* pMsg)
			{
 				PartyGameMatchedS2CEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "PartyGameMatched:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount()); 
				return ResultCode::SUCCESS;
			}; // Result PartyGameMatchedS2CEvt::TraceOut(MessageData* pMsg)

			// S2C: 
			const MessageID PlayerGameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHING, 1);
			Result PlayerGameMatchedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RequestedRole, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // Result PlayerGameMatchedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerGameMatchedS2CEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				PlayerGameMatchedS2CEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("DestPlayerID", parser.GetDestPlayerID());
				variableBuilder.SetVariable("GameInsUID", parser.GetGameInsUID());
				variableBuilder.SetVariable("RequestedRole", (int)parser.GetRequestedRole());


			Proc_End:

				return hr;

			}; // Result PlayerGameMatchedS2CEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result PlayerGameMatchedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerGameMatchedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerGameMatchedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerGameMatchedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(PlayerID)
					+ sizeof(GameInsUID)
					+ sizeof(PlayerRole));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatching::PlayerGameMatchedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InRequestedRole, sizeof(PlayerRole));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerGameMatchedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole )

			Result PlayerGameMatchedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result PlayerGameMatchedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			Result PlayerGameMatchedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result PlayerGameMatchedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result PlayerGameMatchedS2CEvt::TraceOut(MessageData* pMsg)
			{
 				PlayerGameMatchedS2CEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "PlayerGameMatched:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, DestPlayerID:{4}, GameInsUID:{5}, RequestedRole:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetDestPlayerID(), parser.GetGameInsUID(), (int)parser.GetRequestedRole()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerGameMatchedS2CEvt::TraceOut(MessageData* pMsg)



		}; // namespace PartyMatching
	}; // namespace Message
}; // namespace SF


