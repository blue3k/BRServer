////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameInstanceManager Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameInstanceManager
		{
 			// Cmd: Create a game instance
			const MessageID CreateGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCEMANAGER, 0);
			Result CreateGameCmd::ParseMessage( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_NumberOfBotPlayer, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(uint16_t) ) );


			Proc_End:

				return hr;

			}; // Result CreateGameCmd::ParseMessage( MessageData* pIMsg )

			Result CreateGameCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				CreateGameCmd parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("NumberOfBotPlayer", parser.GetNumberOfBotPlayer());
				variableBuilder.SetVariable("MaxPlayer", parser.GetMaxPlayer());


			Proc_End:

				return hr;

			}; // Result CreateGameCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result CreateGameCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) CreateGameCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CreateGameCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* CreateGameCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(uint16_t)
					+ sizeof(uint16_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstanceManager::CreateGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InNumberOfBotPlayer, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(uint16_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CreateGameCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer )

			Result CreateGameCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CreateGameCmd::OverrideRouteContextDestination( EntityUID to )

			Result CreateGameCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result CreateGameCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CreateGameCmd::TraceOut(MessageData* pMsg)
			{
 				CreateGameCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "CreateGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, NumberOfBotPlayer:{5}, MaxPlayer:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetNumberOfBotPlayer(), parser.GetMaxPlayer()); 
				return ResultCode::SUCCESS;
			}; // Result CreateGameCmd::TraceOut(MessageData* pMsg)

			const MessageID CreateGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCEMANAGER, 0);
			Result CreateGameRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result CreateGameRes::ParseMessage( MessageData* pIMsg )

			Result CreateGameRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				CreateGameRes parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("Result", parser.GetResult());


			Proc_End:

				return hr;

			}; // Result CreateGameRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result CreateGameRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) CreateGameRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CreateGameRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* CreateGameRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstanceManager::CreateGameRes::MID, __uiMessageSize ) );

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

			}; // MessageData* CreateGameRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result CreateGameRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CreateGameRes::OverrideRouteContextDestination( EntityUID to )

			Result CreateGameRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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

			}; // Result CreateGameRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CreateGameRes::TraceOut(MessageData* pMsg)
			{
 				CreateGameRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "CreateGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result CreateGameRes::TraceOut(MessageData* pMsg)

			// C2S: Game instance notify of deletion
			const MessageID GameDeletedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCEMANAGER, 1);
			Result GameDeletedC2SEvt::ParseMessage( MessageData* pIMsg )
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

			}; // Result GameDeletedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result GameDeletedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GameDeletedC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());


			Proc_End:

				return hr;

			}; // Result GameDeletedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GameDeletedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GameDeletedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GameDeletedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GameDeletedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstanceManager::GameDeletedC2SEvt::MID, __uiMessageSize ) );

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

			}; // MessageData* GameDeletedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )

			Result GameDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GameDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result GameDeletedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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

			}; // Result GameDeletedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GameDeletedC2SEvt::TraceOut(MessageData* pMsg)
			{
 				GameDeletedC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GameDeleted:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount()); 
				return ResultCode::SUCCESS;
			}; // Result GameDeletedC2SEvt::TraceOut(MessageData* pMsg)



		}; // namespace GameInstanceManager
	}; // namespace Message
}; // namespace SF


