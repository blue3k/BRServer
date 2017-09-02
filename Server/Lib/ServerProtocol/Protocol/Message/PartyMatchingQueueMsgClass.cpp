////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace PartyMatchingQueue
		{
 			// Cmd: Register match by party
			const MessageID RegisterPartyMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 0);
			Result RegisterPartyMatchingCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofPlayers = 0; MatchingPlayerInformation* pPlayers = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPlayers, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pPlayers, pCur, iMsgSize, sizeof(MatchingPlayerInformation)*numberofPlayers ) );
				m_Players.SetLinkedBuffer(numberofPlayers, numberofPlayers, pPlayers);


			Proc_End:

				return hr;

			}; // Result RegisterPartyMatchingCmd::ParseMessage( MessageData* pIMsg )


			Result RegisterPartyMatchingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterPartyMatchingCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterPartyMatchingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPartyMatchingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(MatchingPlayerInformation)*InPlayers.size() + sizeof(uint16_t));


				uint16_t numberOfInPlayers = (uint16_t)InPlayers.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::RegisterPartyMatchingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInPlayers, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InPlayers.data(), (INT)(sizeof(MatchingPlayerInformation)*InPlayers.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterPartyMatchingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )

			Result RegisterPartyMatchingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPartyMatchingCmd::OverrideRouteContextDestination( EntityUID to )

			Result RegisterPartyMatchingCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result RegisterPartyMatchingCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RegisterPartyMatchingCmd::TraceOut(MessageData* pMsg)
			{
 				RegisterPartyMatchingCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RegisterPartyMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, Players:{5,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetPlayers()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPartyMatchingCmd::TraceOut(MessageData* pMsg)

			const MessageID RegisterPartyMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 0);
			Result RegisterPartyMatchingRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPartyMatchingRes::ParseMessage( MessageData* pIMsg )


			Result RegisterPartyMatchingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterPartyMatchingRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterPartyMatchingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPartyMatchingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::RegisterPartyMatchingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterPartyMatchingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )

			Result RegisterPartyMatchingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPartyMatchingRes::OverrideRouteContextDestination( EntityUID to )

			Result RegisterPartyMatchingRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(MatchingQueueTicket); iMsgSize -= sizeof(MatchingQueueTicket);


			Proc_End:

				return hr;

			}; // Result RegisterPartyMatchingRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RegisterPartyMatchingRes::TraceOut(MessageData* pMsg)
			{
 				RegisterPartyMatchingRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RegisterPartyMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MatchingTicket:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPartyMatchingRes::TraceOut(MessageData* pMsg)

			// Cmd: Register match alone
			const MessageID RegisterPlayerMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 1);
			Result RegisterPlayerMatchingCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPlayerMatchingCmd::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerMatchingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterPlayerMatchingCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterPlayerMatchingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerMatchingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::RegisterPlayerMatchingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterPlayerMatchingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )

			Result RegisterPlayerMatchingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerMatchingCmd::OverrideRouteContextDestination( EntityUID to )

			Result RegisterPlayerMatchingCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result RegisterPlayerMatchingCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RegisterPlayerMatchingCmd::TraceOut(MessageData* pMsg)
			{
 				RegisterPlayerMatchingCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RegisterPlayerMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, PlayerID:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerMatchingCmd::TraceOut(MessageData* pMsg)

			const MessageID RegisterPlayerMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 1);
			Result RegisterPlayerMatchingRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPlayerMatchingRes::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerMatchingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterPlayerMatchingRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterPlayerMatchingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerMatchingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::RegisterPlayerMatchingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterPlayerMatchingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )

			Result RegisterPlayerMatchingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerMatchingRes::OverrideRouteContextDestination( EntityUID to )

			Result RegisterPlayerMatchingRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(MatchingQueueTicket); iMsgSize -= sizeof(MatchingQueueTicket);


			Proc_End:

				return hr;

			}; // Result RegisterPlayerMatchingRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RegisterPlayerMatchingRes::TraceOut(MessageData* pMsg)
			{
 				RegisterPlayerMatchingRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RegisterPlayerMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MatchingTicket:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerMatchingRes::TraceOut(MessageData* pMsg)

			// Cmd: update registration information
			const MessageID UpdateMatchingEntityUIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 2);
			Result UpdateMatchingEntityUIDCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PreviousUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result UpdateMatchingEntityUIDCmd::ParseMessage( MessageData* pIMsg )


			Result UpdateMatchingEntityUIDCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) UpdateMatchingEntityUIDCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result UpdateMatchingEntityUIDCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* UpdateMatchingEntityUIDCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::UpdateMatchingEntityUIDCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));
				Protocol::PackParamCopy( pMsgData, &InPreviousUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* UpdateMatchingEntityUIDCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )

			Result UpdateMatchingEntityUIDCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result UpdateMatchingEntityUIDCmd::OverrideRouteContextDestination( EntityUID to )

			Result UpdateMatchingEntityUIDCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result UpdateMatchingEntityUIDCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result UpdateMatchingEntityUIDCmd::TraceOut(MessageData* pMsg)
			{
 				UpdateMatchingEntityUIDCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "UpdateMatchingEntityUID:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, MatchingTicket:{5}, PreviousUID:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetMatchingTicket(), parser.GetPreviousUID()); 
				return ResultCode::SUCCESS;
			}; // Result UpdateMatchingEntityUIDCmd::TraceOut(MessageData* pMsg)

			const MessageID UpdateMatchingEntityUIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 2);
			Result UpdateMatchingEntityUIDRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result UpdateMatchingEntityUIDRes::ParseMessage( MessageData* pIMsg )


			Result UpdateMatchingEntityUIDRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) UpdateMatchingEntityUIDRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result UpdateMatchingEntityUIDRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* UpdateMatchingEntityUIDRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::UpdateMatchingEntityUIDRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* UpdateMatchingEntityUIDRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result UpdateMatchingEntityUIDRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result UpdateMatchingEntityUIDRes::OverrideRouteContextDestination( EntityUID to )

			Result UpdateMatchingEntityUIDRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);


			Proc_End:

				return hr;

			}; // Result UpdateMatchingEntityUIDRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result UpdateMatchingEntityUIDRes::TraceOut(MessageData* pMsg)
			{
 				UpdateMatchingEntityUIDRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "UpdateMatchingEntityUID:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result UpdateMatchingEntityUIDRes::TraceOut(MessageData* pMsg)

			// Cmd: calcel registration
			const MessageID UnregisterMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 3);
			Result UnregisterMatchingCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result UnregisterMatchingCmd::ParseMessage( MessageData* pIMsg )


			Result UnregisterMatchingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) UnregisterMatchingCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result UnregisterMatchingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* UnregisterMatchingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::UnregisterMatchingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* UnregisterMatchingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result UnregisterMatchingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result UnregisterMatchingCmd::OverrideRouteContextDestination( EntityUID to )

			Result UnregisterMatchingCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result UnregisterMatchingCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result UnregisterMatchingCmd::TraceOut(MessageData* pMsg)
			{
 				UnregisterMatchingCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "UnregisterMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, MatchingTicket:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result UnregisterMatchingCmd::TraceOut(MessageData* pMsg)

			const MessageID UnregisterMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 3);
			Result UnregisterMatchingRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result UnregisterMatchingRes::ParseMessage( MessageData* pIMsg )


			Result UnregisterMatchingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) UnregisterMatchingRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result UnregisterMatchingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* UnregisterMatchingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::UnregisterMatchingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* UnregisterMatchingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result UnregisterMatchingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result UnregisterMatchingRes::OverrideRouteContextDestination( EntityUID to )

			Result UnregisterMatchingRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);


			Proc_End:

				return hr;

			}; // Result UnregisterMatchingRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result UnregisterMatchingRes::TraceOut(MessageData* pMsg)
			{
 				UnregisterMatchingRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "UnregisterMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result UnregisterMatchingRes::TraceOut(MessageData* pMsg)

			// S2C: For some reason, matching is canceled
			const MessageID PartyMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 4);
			Result PartyMatchingCanceledS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPartyUID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result PartyMatchingCanceledS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PartyMatchingCanceledS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PartyMatchingCanceledS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PartyMatchingCanceledS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PartyMatchingCanceledS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(uint64_t)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::PartyMatchingCanceledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InDestPartyUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PartyMatchingCanceledS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )

			Result PartyMatchingCanceledS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PartyMatchingCanceledS2CEvt::OverrideRouteContextDestination( EntityUID to )

			Result PartyMatchingCanceledS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result PartyMatchingCanceledS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result PartyMatchingCanceledS2CEvt::TraceOut(MessageData* pMsg)
			{
 				PartyMatchingCanceledS2CEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "PartyMatchingCanceled:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, DestPartyUID:{4}, MatchingTicket:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetDestPartyUID(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result PartyMatchingCanceledS2CEvt::TraceOut(MessageData* pMsg)

			// S2C: For some reason, matching is canceled
			const MessageID PlayerMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 5);
			Result PlayerMatchingCanceledS2CEvt::ParseMessage( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result PlayerMatchingCanceledS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerMatchingCanceledS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerMatchingCanceledS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerMatchingCanceledS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerMatchingCanceledS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(PlayerID)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerMatchingCanceledS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )

			Result PlayerMatchingCanceledS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PlayerMatchingCanceledS2CEvt::OverrideRouteContextDestination( EntityUID to )

			Result PlayerMatchingCanceledS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result PlayerMatchingCanceledS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result PlayerMatchingCanceledS2CEvt::TraceOut(MessageData* pMsg)
			{
 				PlayerMatchingCanceledS2CEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "PlayerMatchingCanceled:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, DestPlayerID:{4}, MatchingTicket:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetDestPlayerID(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerMatchingCanceledS2CEvt::TraceOut(MessageData* pMsg)

			// S2C: Enqueued item is dequeued
			const MessageID PartyMatchingItemDequeuedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 6);
			Result PartyMatchingItemDequeuedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result PartyMatchingItemDequeuedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PartyMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PartyMatchingItemDequeuedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PartyMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PartyMatchingItemDequeuedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PartyMatchingItemDequeuedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result PartyMatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PartyMatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			Result PartyMatchingItemDequeuedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result PartyMatchingItemDequeuedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result PartyMatchingItemDequeuedS2CEvt::TraceOut(MessageData* pMsg)
			{
 				PartyMatchingItemDequeuedS2CEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "PartyMatchingItemDequeued:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, MatchingTicket:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result PartyMatchingItemDequeuedS2CEvt::TraceOut(MessageData* pMsg)

			// S2C: Enqueued item is dequeued
			const MessageID PlayerMatchingItemDequeuedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 7);
			Result PlayerMatchingItemDequeuedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result PlayerMatchingItemDequeuedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerMatchingItemDequeuedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerMatchingItemDequeuedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerMatchingItemDequeuedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result PlayerMatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PlayerMatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			Result PlayerMatchingItemDequeuedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result PlayerMatchingItemDequeuedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result PlayerMatchingItemDequeuedS2CEvt::TraceOut(MessageData* pMsg)
			{
 				PlayerMatchingItemDequeuedS2CEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "PlayerMatchingItemDequeued:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, MatchingTicket:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerMatchingItemDequeuedS2CEvt::TraceOut(MessageData* pMsg)

			// Cmd: Reserve a item
			const MessageID ReserveItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 8);
			Result ReserveItemCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );


			Proc_End:

				return hr;

			}; // Result ReserveItemCmd::ParseMessage( MessageData* pIMsg )


			Result ReserveItemCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ReserveItemCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ReserveItemCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ReserveItemCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::ReserveItemCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ReserveItemCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount )

			Result ReserveItemCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ReserveItemCmd::OverrideRouteContextDestination( EntityUID to )

			Result ReserveItemCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result ReserveItemCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ReserveItemCmd::TraceOut(MessageData* pMsg)
			{
 				ReserveItemCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "ReserveItem:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount()); 
				return ResultCode::SUCCESS;
			}; // Result ReserveItemCmd::TraceOut(MessageData* pMsg)

			const MessageID ReserveItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 8);
			Result ReserveItemRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NumberOfPlayersInTheItem, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result ReserveItemRes::ParseMessage( MessageData* pIMsg )


			Result ReserveItemRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ReserveItemRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ReserveItemRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ReserveItemRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::ReserveItemRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNumberOfPlayersInTheItem, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ReserveItemRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )

			Result ReserveItemRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ReserveItemRes::OverrideRouteContextDestination( EntityUID to )

			Result ReserveItemRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(uint32_t); iMsgSize -= sizeof(uint32_t);
				pCur += sizeof(MatchingQueueTicket); iMsgSize -= sizeof(MatchingQueueTicket);


			Proc_End:

				return hr;

			}; // Result ReserveItemRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ReserveItemRes::TraceOut(MessageData* pMsg)
			{
 				ReserveItemRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "ReserveItem:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, NumberOfPlayersInTheItem:{5}, MatchingTicket:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetNumberOfPlayersInTheItem(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result ReserveItemRes::TraceOut(MessageData* pMsg)

			// Cmd: Reserve a item
			const MessageID ReserveItemsCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 9);
			Result ReserveItemsCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NumberOfItemsToReserve, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result ReserveItemsCmd::ParseMessage( MessageData* pIMsg )


			Result ReserveItemsCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ReserveItemsCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ReserveItemsCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ReserveItemsCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::ReserveItemsCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InNumberOfItemsToReserve, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ReserveItemsCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )

			Result ReserveItemsCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ReserveItemsCmd::OverrideRouteContextDestination( EntityUID to )

			Result ReserveItemsCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result ReserveItemsCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ReserveItemsCmd::TraceOut(MessageData* pMsg)
			{
 				ReserveItemsCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "ReserveItems:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, NumberOfItemsToReserve:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetNumberOfItemsToReserve()); 
				return ResultCode::SUCCESS;
			}; // Result ReserveItemsCmd::TraceOut(MessageData* pMsg)

			const MessageID ReserveItemsRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 9);
			Result ReserveItemsRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofNumberOfPlayersInTheItem = 0; uint32_t* pNumberOfPlayersInTheItem = nullptr;
				uint16_t numberofMatchingTicket = 0; MatchingQueueTicket* pMatchingTicket = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofNumberOfPlayersInTheItem, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pNumberOfPlayersInTheItem, pCur, iMsgSize, sizeof(uint32_t)*numberofNumberOfPlayersInTheItem ) );
				m_NumberOfPlayersInTheItem.SetLinkedBuffer(numberofNumberOfPlayersInTheItem, numberofNumberOfPlayersInTheItem, pNumberOfPlayersInTheItem);
				protocolChk( Protocol::StreamParamCopy( &numberofMatchingTicket, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pMatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket)*numberofMatchingTicket ) );
				m_MatchingTicket.SetLinkedBuffer(numberofMatchingTicket, numberofMatchingTicket, pMatchingTicket);


			Proc_End:

				return hr;

			}; // Result ReserveItemsRes::ParseMessage( MessageData* pIMsg )


			Result ReserveItemsRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ReserveItemsRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ReserveItemsRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ReserveItemsRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t)*InNumberOfPlayersInTheItem.size() + sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket)*InMatchingTicket.size() + sizeof(uint16_t));


				uint16_t numberOfInNumberOfPlayersInTheItem = (uint16_t)InNumberOfPlayersInTheItem.size(); 
				uint16_t numberOfInMatchingTicket = (uint16_t)InMatchingTicket.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::ReserveItemsRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInNumberOfPlayersInTheItem, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InNumberOfPlayersInTheItem.data(), (INT)(sizeof(uint32_t)*InNumberOfPlayersInTheItem.size())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInMatchingTicket, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InMatchingTicket.data(), (INT)(sizeof(MatchingQueueTicket)*InMatchingTicket.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ReserveItemsRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )

			Result ReserveItemsRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ReserveItemsRes::OverrideRouteContextDestination( EntityUID to )

			Result ReserveItemsRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);
				pCur += sizeof(uint32_t)*m_NumberOfPlayersInTheItem.size(); iMsgSize -= (INT)(sizeof(uint32_t)*m_NumberOfPlayersInTheItem.size());
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);
				pCur += sizeof(MatchingQueueTicket)*m_MatchingTicket.size(); iMsgSize -= (INT)(sizeof(MatchingQueueTicket)*m_MatchingTicket.size());


			Proc_End:

				return hr;

			}; // Result ReserveItemsRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ReserveItemsRes::TraceOut(MessageData* pMsg)
			{
 				ReserveItemsRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "ReserveItems:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, NumberOfPlayersInTheItem:{5,30}, MatchingTicket:{6,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetNumberOfPlayersInTheItem(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result ReserveItemsRes::TraceOut(MessageData* pMsg)

			// Cmd: Cancel reservation
			const MessageID CancelReservationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 10);
			Result CancelReservationCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TicketToCancel, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result CancelReservationCmd::ParseMessage( MessageData* pIMsg )


			Result CancelReservationCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) CancelReservationCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CancelReservationCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* CancelReservationCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::CancelReservationCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InTicketToCancel, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CancelReservationCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )

			Result CancelReservationCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CancelReservationCmd::OverrideRouteContextDestination( EntityUID to )

			Result CancelReservationCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result CancelReservationCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CancelReservationCmd::TraceOut(MessageData* pMsg)
			{
 				CancelReservationCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "CancelReservation:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, TicketToCancel:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetTicketToCancel()); 
				return ResultCode::SUCCESS;
			}; // Result CancelReservationCmd::TraceOut(MessageData* pMsg)

			const MessageID CancelReservationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 10);
			Result CancelReservationRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result CancelReservationRes::ParseMessage( MessageData* pIMsg )


			Result CancelReservationRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) CancelReservationRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CancelReservationRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* CancelReservationRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::CancelReservationRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CancelReservationRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result CancelReservationRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CancelReservationRes::OverrideRouteContextDestination( EntityUID to )

			Result CancelReservationRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);


			Proc_End:

				return hr;

			}; // Result CancelReservationRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CancelReservationRes::TraceOut(MessageData* pMsg)
			{
 				CancelReservationRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "CancelReservation:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result CancelReservationRes::TraceOut(MessageData* pMsg)

			// Cmd: Cancel reservation
			const MessageID CancelReservationsCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 11);
			Result CancelReservationsCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofTicketToCancel = 0; MatchingQueueTicket* pTicketToCancel = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofTicketToCancel, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pTicketToCancel, pCur, iMsgSize, sizeof(MatchingQueueTicket)*numberofTicketToCancel ) );
				m_TicketToCancel.SetLinkedBuffer(numberofTicketToCancel, numberofTicketToCancel, pTicketToCancel);


			Proc_End:

				return hr;

			}; // Result CancelReservationsCmd::ParseMessage( MessageData* pIMsg )


			Result CancelReservationsCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) CancelReservationsCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CancelReservationsCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* CancelReservationsCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket)*InTicketToCancel.size() + sizeof(uint16_t));


				uint16_t numberOfInTicketToCancel = (uint16_t)InTicketToCancel.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::CancelReservationsCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInTicketToCancel, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InTicketToCancel.data(), (INT)(sizeof(MatchingQueueTicket)*InTicketToCancel.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CancelReservationsCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )

			Result CancelReservationsCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CancelReservationsCmd::OverrideRouteContextDestination( EntityUID to )

			Result CancelReservationsCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result CancelReservationsCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CancelReservationsCmd::TraceOut(MessageData* pMsg)
			{
 				CancelReservationsCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "CancelReservations:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, TicketToCancel:{5,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetTicketToCancel()); 
				return ResultCode::SUCCESS;
			}; // Result CancelReservationsCmd::TraceOut(MessageData* pMsg)

			const MessageID CancelReservationsRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 11);
			Result CancelReservationsRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result CancelReservationsRes::ParseMessage( MessageData* pIMsg )


			Result CancelReservationsRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) CancelReservationsRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CancelReservationsRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* CancelReservationsRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::CancelReservationsRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CancelReservationsRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result CancelReservationsRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CancelReservationsRes::OverrideRouteContextDestination( EntityUID to )

			Result CancelReservationsRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);


			Proc_End:

				return hr;

			}; // Result CancelReservationsRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CancelReservationsRes::TraceOut(MessageData* pMsg)
			{
 				CancelReservationsRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "CancelReservations:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result CancelReservationsRes::TraceOut(MessageData* pMsg)

			// Cmd: Dequeue a reserved item
			const MessageID DequeueItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 12);
			Result DequeueItemCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result DequeueItemCmd::ParseMessage( MessageData* pIMsg )


			Result DequeueItemCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) DequeueItemCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result DequeueItemCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* DequeueItemCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::DequeueItemCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* DequeueItemCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result DequeueItemCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result DequeueItemCmd::OverrideRouteContextDestination( EntityUID to )

			Result DequeueItemCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result DequeueItemCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result DequeueItemCmd::TraceOut(MessageData* pMsg)
			{
 				DequeueItemCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "DequeueItem:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, MatchingTicket:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result DequeueItemCmd::TraceOut(MessageData* pMsg)

			const MessageID DequeueItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 12);
			Result DequeueItemRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofPlayers = 0; MatchingPlayerInformation* pPlayers = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RegisterUID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RegisterID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPlayers, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pPlayers, pCur, iMsgSize, sizeof(MatchingPlayerInformation)*numberofPlayers ) );
				m_Players.SetLinkedBuffer(numberofPlayers, numberofPlayers, pPlayers);


			Proc_End:

				return hr;

			}; // Result DequeueItemRes::ParseMessage( MessageData* pIMsg )


			Result DequeueItemRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) DequeueItemRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result DequeueItemRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* DequeueItemRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(MatchingQueueTicket)
					+ sizeof(uint64_t)
					+ sizeof(PlayerID)
					+ sizeof(MatchingPlayerInformation)*InPlayers.size() + sizeof(uint16_t));


				uint16_t numberOfInPlayers = (uint16_t)InPlayers.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::DequeueItemRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));
				Protocol::PackParamCopy( pMsgData, &InRegisterUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InRegisterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &numberOfInPlayers, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InPlayers.data(), (INT)(sizeof(MatchingPlayerInformation)*InPlayers.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* DequeueItemRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )

			Result DequeueItemRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result DequeueItemRes::OverrideRouteContextDestination( EntityUID to )

			Result DequeueItemRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(MatchingQueueTicket); iMsgSize -= sizeof(MatchingQueueTicket);
				pCur += sizeof(uint64_t); iMsgSize -= sizeof(uint64_t);
				pCur += sizeof(PlayerID); iMsgSize -= sizeof(PlayerID);
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);
				pCur += sizeof(MatchingPlayerInformation)*m_Players.size(); iMsgSize -= (INT)(sizeof(MatchingPlayerInformation)*m_Players.size());


			Proc_End:

				return hr;

			}; // Result DequeueItemRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result DequeueItemRes::TraceOut(MessageData* pMsg)
			{
 				DequeueItemRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "DequeueItem:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MatchingTicket:{5}, RegisterUID:{6}, RegisterID:{7}, Players:{8,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMatchingTicket(), parser.GetRegisterUID(), parser.GetRegisterID(), parser.GetPlayers()); 
				return ResultCode::SUCCESS;
			}; // Result DequeueItemRes::TraceOut(MessageData* pMsg)

			// C2S: Item error you should delete it
			const MessageID MatchingItemErrorC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 13);
			Result MatchingItemErrorC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result MatchingItemErrorC2SEvt::ParseMessage( MessageData* pIMsg )


			Result MatchingItemErrorC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) MatchingItemErrorC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result MatchingItemErrorC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* MatchingItemErrorC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, PartyMatchingQueue::MatchingItemErrorC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* MatchingItemErrorC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result MatchingItemErrorC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result MatchingItemErrorC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result MatchingItemErrorC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result MatchingItemErrorC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result MatchingItemErrorC2SEvt::TraceOut(MessageData* pMsg)
			{
 				MatchingItemErrorC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "MatchingItemError:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, MatchingTicket:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result MatchingItemErrorC2SEvt::TraceOut(MessageData* pMsg)



		}; // namespace PartyMatchingQueue
	}; // namespace Message
}; // namespace SF


