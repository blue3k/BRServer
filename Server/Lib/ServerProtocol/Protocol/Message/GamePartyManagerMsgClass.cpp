////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GamePartyManager Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GamePartyManager
		{
 			// Cmd: Create a party instance
			const MessageID CreatePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTYMANAGER, 0);
			Result CreatePartyCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameID, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Creator, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result CreatePartyCmd::ParseMessage( MessageData* pIMsg )


			Result CreatePartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) CreatePartyCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CreatePartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* CreatePartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint64_t)
					+ sizeof(uint16_t)
					+ sizeof(uint32_t)
					+ sizeof(PlayerInformation));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GamePartyManager::CreatePartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InGameID, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InCreator, sizeof(PlayerInformation));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CreatePartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator )

			Result CreatePartyCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result CreatePartyCmd::OverrideRouteContextDestination( EntityUID to )

			Result CreatePartyCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(uint64_t); iMsgSize -= sizeof(uint64_t);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result CreatePartyCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CreatePartyCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				CreatePartyCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CreateParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, GameID:{5}, Creator:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetGameID(), parser.GetCreator()); 
				return ResultCode::SUCCESS;
			}; // Result CreatePartyCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID CreatePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTYMANAGER, 0);
			Result CreatePartyRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result CreatePartyRes::ParseMessage( MessageData* pIMsg )


			Result CreatePartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) CreatePartyRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CreatePartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* CreatePartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint64_t)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GamePartyManager::CreatePartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CreatePartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result CreatePartyRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result CreatePartyRes::OverrideRouteContextDestination( EntityUID to )

			Result CreatePartyRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(uint64_t); iMsgSize -= sizeof(uint64_t);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);


			Proc_End:

				return hr;

			}; // Result CreatePartyRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CreatePartyRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				CreatePartyRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CreateParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result CreatePartyRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Party instance notify of deletion. Sent by party instance
			const MessageID PartyDeletedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTYMANAGER, 1);
			Result PartyDeletedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );


			Proc_End:

				return hr;

			}; // Result PartyDeletedC2SEvt::ParseMessage( MessageData* pIMsg )


			Result PartyDeletedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PartyDeletedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PartyDeletedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PartyDeletedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GamePartyManager::PartyDeletedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PartyDeletedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )

			Result PartyDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result PartyDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result PartyDeletedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result PartyDeletedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result PartyDeletedC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PartyDeletedC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PartyDeleted:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount()); 
				return ResultCode::SUCCESS;
			}; // Result PartyDeletedC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)



		}; // namespace GamePartyManager
	}; // namespace Message
}; // namespace SF


