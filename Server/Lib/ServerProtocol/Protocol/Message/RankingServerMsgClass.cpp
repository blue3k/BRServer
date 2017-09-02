////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : RankingServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/RankingServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace RankingServer
		{
 			// Cmd: Add a player to ranking
			const MessageID AddPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 0);
			Result AddPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerInfo, pCur, iMsgSize, sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RankingScore, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result AddPlayerCmd::ParseMessage( MessageData* pIMsg )


			Result AddPlayerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) AddPlayerCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result AddPlayerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* AddPlayerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerInformation)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::AddPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerInfo, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InRankingScore, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* AddPlayerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )

			Result AddPlayerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AddPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			Result AddPlayerCmd::TraceOut(MessageData* pMsg)
			{
 				AddPlayerCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "AddPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerInfo:{4}, RankingScore:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerInfo(), parser.GetRankingScore()); 
				return ResultCode::SUCCESS;
			}; // Result AddPlayerCmd::TraceOut(MessageData* pMsg)

			const MessageID AddPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 0);
			Result AddPlayerRes::ParseMessage( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_Ranking, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result AddPlayerRes::ParseMessage( MessageData* pIMsg )


			Result AddPlayerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) AddPlayerRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result AddPlayerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* AddPlayerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::AddPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InRanking, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* AddPlayerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )

			Result AddPlayerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AddPlayerRes::OverrideRouteContextDestination( EntityUID to )


			Result AddPlayerRes::TraceOut(MessageData* pMsg)
			{
 				AddPlayerRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "AddPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, Ranking:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetRanking()); 
				return ResultCode::SUCCESS;
			}; // Result AddPlayerRes::TraceOut(MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID RemovePlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 1);
			Result RemovePlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RemovePlayerCmd::ParseMessage( MessageData* pIMsg )


			Result RemovePlayerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RemovePlayerCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RemovePlayerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RemovePlayerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::RemovePlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RemovePlayerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result RemovePlayerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RemovePlayerCmd::OverrideRouteContextDestination( EntityUID to )


			Result RemovePlayerCmd::TraceOut(MessageData* pMsg)
			{
 				RemovePlayerCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RemovePlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result RemovePlayerCmd::TraceOut(MessageData* pMsg)

			const MessageID RemovePlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 1);
			Result RemovePlayerRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result RemovePlayerRes::ParseMessage( MessageData* pIMsg )


			Result RemovePlayerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RemovePlayerRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RemovePlayerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RemovePlayerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::RemovePlayerRes::MID, __uiMessageSize ) );

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

			}; // MessageData* RemovePlayerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result RemovePlayerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RemovePlayerRes::OverrideRouteContextDestination( EntityUID to )


			Result RemovePlayerRes::TraceOut(MessageData* pMsg)
			{
 				RemovePlayerRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RemovePlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result RemovePlayerRes::TraceOut(MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID GetPlayerRankingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 2);
			Result GetPlayerRankingCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GetPlayerRankingCmd::ParseMessage( MessageData* pIMsg )


			Result GetPlayerRankingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetPlayerRankingCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetPlayerRankingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetPlayerRankingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::GetPlayerRankingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetPlayerRankingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result GetPlayerRankingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GetPlayerRankingCmd::OverrideRouteContextDestination( EntityUID to )


			Result GetPlayerRankingCmd::TraceOut(MessageData* pMsg)
			{
 				GetPlayerRankingCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GetPlayerRanking:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result GetPlayerRankingCmd::TraceOut(MessageData* pMsg)

			const MessageID GetPlayerRankingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 2);
			Result GetPlayerRankingRes::ParseMessage( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_Ranking, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result GetPlayerRankingRes::ParseMessage( MessageData* pIMsg )


			Result GetPlayerRankingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetPlayerRankingRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetPlayerRankingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetPlayerRankingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::GetPlayerRankingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InRanking, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetPlayerRankingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )

			Result GetPlayerRankingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GetPlayerRankingRes::OverrideRouteContextDestination( EntityUID to )


			Result GetPlayerRankingRes::TraceOut(MessageData* pMsg)
			{
 				GetPlayerRankingRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GetPlayerRanking:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, Ranking:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetRanking()); 
				return ResultCode::SUCCESS;
			}; // Result GetPlayerRankingRes::TraceOut(MessageData* pMsg)

			// Cmd: Update a player to ranking
			const MessageID UpdatePlayerScoreCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 3);
			Result UpdatePlayerScoreCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RankingScore, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerInfo, pCur, iMsgSize, sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, sizeof(uint16_t) ) );


			Proc_End:

				return hr;

			}; // Result UpdatePlayerScoreCmd::ParseMessage( MessageData* pIMsg )


			Result UpdatePlayerScoreCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) UpdatePlayerScoreCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result UpdatePlayerScoreCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* UpdatePlayerScoreCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint64_t)
					+ sizeof(PlayerInformation)
					+ sizeof(uint16_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::UpdatePlayerScoreCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRankingScore, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerInfo, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(uint16_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* UpdatePlayerScoreCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )

			Result UpdatePlayerScoreCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result UpdatePlayerScoreCmd::OverrideRouteContextDestination( EntityUID to )


			Result UpdatePlayerScoreCmd::TraceOut(MessageData* pMsg)
			{
 				UpdatePlayerScoreCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "UpdatePlayerScore:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RankingScore:{4}, PlayerInfo:{5}, Count:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRankingScore(), parser.GetPlayerInfo(), parser.GetCount()); 
				return ResultCode::SUCCESS;
			}; // Result UpdatePlayerScoreCmd::TraceOut(MessageData* pMsg)

			const MessageID UpdatePlayerScoreRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 3);
			Result UpdatePlayerScoreRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofRanking = 0; TotalRankingPlayerInformation* pRanking = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRanking, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pRanking, pCur, iMsgSize, sizeof(TotalRankingPlayerInformation)*numberofRanking ) );
				m_Ranking.SetLinkedBuffer(numberofRanking, numberofRanking, pRanking);


			Proc_End:

				return hr;

			}; // Result UpdatePlayerScoreRes::ParseMessage( MessageData* pIMsg )


			Result UpdatePlayerScoreRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) UpdatePlayerScoreRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result UpdatePlayerScoreRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* UpdatePlayerScoreRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(TotalRankingPlayerInformation)*InRanking.size() + sizeof(uint16_t));


				uint16_t numberOfInRanking = (uint16_t)InRanking.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::UpdatePlayerScoreRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRanking, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InRanking.data(), (INT)(sizeof(TotalRankingPlayerInformation)*InRanking.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* UpdatePlayerScoreRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )

			Result UpdatePlayerScoreRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result UpdatePlayerScoreRes::OverrideRouteContextDestination( EntityUID to )


			Result UpdatePlayerScoreRes::TraceOut(MessageData* pMsg)
			{
 				UpdatePlayerScoreRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "UpdatePlayerScore:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, Ranking:{5,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetRanking()); 
				return ResultCode::SUCCESS;
			}; // Result UpdatePlayerScoreRes::TraceOut(MessageData* pMsg)

			// Cmd: Get ranking list
			const MessageID GetRankingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 4);
			Result GetRankingCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RankingType, pCur, iMsgSize, sizeof(RankingType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_BaseRanking, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, sizeof(uint16_t) ) );


			Proc_End:

				return hr;

			}; // Result GetRankingCmd::ParseMessage( MessageData* pIMsg )


			Result GetRankingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetRankingCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetRankingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetRankingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(RankingType)
					+ sizeof(uint16_t)
					+ sizeof(uint16_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::GetRankingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRankingType, sizeof(RankingType));
				Protocol::PackParamCopy( pMsgData, &InBaseRanking, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(uint16_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetRankingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )

			Result GetRankingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GetRankingCmd::OverrideRouteContextDestination( EntityUID to )


			Result GetRankingCmd::TraceOut(MessageData* pMsg)
			{
 				GetRankingCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GetRanking:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RankingType:{4}, BaseRanking:{5}, Count:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), (int)parser.GetRankingType(), parser.GetBaseRanking(), parser.GetCount()); 
				return ResultCode::SUCCESS;
			}; // Result GetRankingCmd::TraceOut(MessageData* pMsg)

			const MessageID GetRankingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 4);
			Result GetRankingRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofRanking = 0; TotalRankingPlayerInformation* pRanking = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRanking, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pRanking, pCur, iMsgSize, sizeof(TotalRankingPlayerInformation)*numberofRanking ) );
				m_Ranking.SetLinkedBuffer(numberofRanking, numberofRanking, pRanking);


			Proc_End:

				return hr;

			}; // Result GetRankingRes::ParseMessage( MessageData* pIMsg )


			Result GetRankingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetRankingRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetRankingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetRankingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(TotalRankingPlayerInformation)*InRanking.size() + sizeof(uint16_t));


				uint16_t numberOfInRanking = (uint16_t)InRanking.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::GetRankingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRanking, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InRanking.data(), (INT)(sizeof(TotalRankingPlayerInformation)*InRanking.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetRankingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )

			Result GetRankingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GetRankingRes::OverrideRouteContextDestination( EntityUID to )


			Result GetRankingRes::TraceOut(MessageData* pMsg)
			{
 				GetRankingRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GetRanking:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, Ranking:{5,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetRanking()); 
				return ResultCode::SUCCESS;
			}; // Result GetRankingRes::TraceOut(MessageData* pMsg)

			// Cmd: Debug test
			const MessageID DebugPrintALLRankingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 5);
			Result DebugPrintALLRankingCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfFileName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfFileName, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_FileName, pCur, iMsgSize, sizeof(char)*uiSizeOfFileName ) );


			Proc_End:

				return hr;

			}; // Result DebugPrintALLRankingCmd::ParseMessage( MessageData* pIMsg )


			Result DebugPrintALLRankingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) DebugPrintALLRankingCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result DebugPrintALLRankingCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* DebugPrintALLRankingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const char* InFileName )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInFileNameLength = InFileName ? (uint16_t)(strlen(InFileName)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInFileNameLength 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::DebugPrintALLRankingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &__uiInFileNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InFileName ? InFileName : "", __uiInFileNameLength );


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* DebugPrintALLRankingCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const char* InFileName )

			Result DebugPrintALLRankingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result DebugPrintALLRankingCmd::OverrideRouteContextDestination( EntityUID to )


			Result DebugPrintALLRankingCmd::TraceOut(MessageData* pMsg)
			{
 				DebugPrintALLRankingCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "DebugPrintALLRanking:{0}:{1} , RouteContext:{2}, TransactionID:{3}, FileName:{4,60}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetFileName()); 
				return ResultCode::SUCCESS;
			}; // Result DebugPrintALLRankingCmd::TraceOut(MessageData* pMsg)

			const MessageID DebugPrintALLRankingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_RANKINGSERVER, 5);
			Result DebugPrintALLRankingRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result DebugPrintALLRankingRes::ParseMessage( MessageData* pIMsg )


			Result DebugPrintALLRankingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) DebugPrintALLRankingRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result DebugPrintALLRankingRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* DebugPrintALLRankingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, RankingServer::DebugPrintALLRankingRes::MID, __uiMessageSize ) );

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

			}; // MessageData* DebugPrintALLRankingRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result DebugPrintALLRankingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result DebugPrintALLRankingRes::OverrideRouteContextDestination( EntityUID to )


			Result DebugPrintALLRankingRes::TraceOut(MessageData* pMsg)
			{
 				DebugPrintALLRankingRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "DebugPrintALLRanking:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result DebugPrintALLRankingRes::TraceOut(MessageData* pMsg)



		}; // namespace RankingServer
	}; // namespace Message
}; // namespace SF


