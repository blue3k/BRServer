////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Ranking Message parser implementations
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
#include "Protocol/Message/RankingMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace Ranking
		{
 			// Cmd: Add a player to ranking
			const MessageID AddPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 0);
			Result AddPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofPlayerInfo = 0; uint8_t* pPlayerInfo = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RankingScore, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPlayerInfo, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pPlayerInfo, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofPlayerInfo ) );
				m_PlayerInfo.SetLinkedBuffer(numberofPlayerInfo, numberofPlayerInfo, pPlayerInfo);


			Proc_End:

				return hr;

			}; // Result AddPlayerCmd::ParseMessage( MessageData* pIMsg )

			Result AddPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(uint64_t)
					+ sizeof(uint8_t)*InPlayerInfo.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInPlayerInfo = (UINT16)InPlayerInfo.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::AddPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRankingScore, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInPlayerInfo, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InPlayerInfo.data(), (INT)(sizeof(uint8_t)*InPlayerInfo.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AddPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )

			Result AddPlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result AddPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			void AddPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AddPlayerCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}, RankingScore:{6}, PlayerInfo:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID, m_RankingScore, m_PlayerInfo); 
			}; // void AddPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AddPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 0);
			Result AddPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ranking, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result AddPlayerRes::ParseMessage( MessageData* pIMsg )

			Result AddPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::AddPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InRanking, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AddPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )

			Result AddPlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result AddPlayerRes::OverrideRouteContextDestination( EntityUID to )


			void AddPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AddPlayerRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, Ranking:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_Ranking); 
			}; // void AddPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID RemovePlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 1);
			Result RemovePlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RemovePlayerCmd::ParseMessage( MessageData* pIMsg )

			Result RemovePlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::RemovePlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RemovePlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result RemovePlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result RemovePlayerCmd::OverrideRouteContextDestination( EntityUID to )


			void RemovePlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RemovePlayerCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID); 
			}; // void RemovePlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RemovePlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 1);
			Result RemovePlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result RemovePlayerRes::ParseMessage( MessageData* pIMsg )

			Result RemovePlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::RemovePlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RemovePlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result RemovePlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result RemovePlayerRes::OverrideRouteContextDestination( EntityUID to )


			void RemovePlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RemovePlayerRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result); 
			}; // void RemovePlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID GetPlayerRankingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 2);
			Result GetPlayerRankingCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GetPlayerRankingCmd::ParseMessage( MessageData* pIMsg )

			Result GetPlayerRankingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::GetPlayerRankingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetPlayerRankingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result GetPlayerRankingCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetPlayerRankingCmd::OverrideRouteContextDestination( EntityUID to )


			void GetPlayerRankingCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetPlayerRankingCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID); 
			}; // void GetPlayerRankingCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetPlayerRankingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 2);
			Result GetPlayerRankingRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ranking, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result GetPlayerRankingRes::ParseMessage( MessageData* pIMsg )

			Result GetPlayerRankingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::GetPlayerRankingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InRanking, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetPlayerRankingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )

			Result GetPlayerRankingRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetPlayerRankingRes::OverrideRouteContextDestination( EntityUID to )


			void GetPlayerRankingRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetPlayerRankingRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, Ranking:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_Ranking); 
			}; // void GetPlayerRankingRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Update a player to ranking
			const MessageID UpdatePlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 3);
			Result UpdatePlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofPlayerInfo = 0; uint8_t* pPlayerInfo = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RankingScore, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPlayerInfo, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pPlayerInfo, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofPlayerInfo ) );
				m_PlayerInfo.SetLinkedBuffer(numberofPlayerInfo, numberofPlayerInfo, pPlayerInfo);


			Proc_End:

				return hr;

			}; // Result UpdatePlayerCmd::ParseMessage( MessageData* pIMsg )

			Result UpdatePlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(uint64_t)
					+ sizeof(uint8_t)*InPlayerInfo.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInPlayerInfo = (UINT16)InPlayerInfo.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::UpdatePlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRankingScore, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInPlayerInfo, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InPlayerInfo.data(), (INT)(sizeof(uint8_t)*InPlayerInfo.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UpdatePlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )

			Result UpdatePlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result UpdatePlayerCmd::OverrideRouteContextDestination( EntityUID to )


			void UpdatePlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UpdatePlayerCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}, RankingScore:{6}, PlayerInfo:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID, m_RankingScore, m_PlayerInfo); 
			}; // void UpdatePlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UpdatePlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 3);
			Result UpdatePlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ranking, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result UpdatePlayerRes::ParseMessage( MessageData* pIMsg )

			Result UpdatePlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::UpdatePlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InRanking, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UpdatePlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )

			Result UpdatePlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result UpdatePlayerRes::OverrideRouteContextDestination( EntityUID to )


			void UpdatePlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UpdatePlayerRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, Ranking:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_Ranking); 
			}; // void UpdatePlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID GetRankingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 4);
			Result GetRankingCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_From, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result GetRankingCmd::ParseMessage( MessageData* pIMsg )

			Result GetRankingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint32_t)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::GetRankingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InFrom, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetRankingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount )

			Result GetRankingCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetRankingCmd::OverrideRouteContextDestination( EntityUID to )


			void GetRankingCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetRankingCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, From:{5}, Count:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_From, m_Count); 
			}; // void GetRankingCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetRankingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 4);
			Result GetRankingRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerRanking, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result GetRankingRes::ParseMessage( MessageData* pIMsg )

			Result GetRankingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InPlayerRanking )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::GetRankingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPlayerRanking, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetRankingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InPlayerRanking )

			Result GetRankingRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				unused(iMsgSize);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetRankingRes::OverrideRouteContextDestination( EntityUID to )


			void GetRankingRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetRankingRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, PlayerRanking:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_PlayerRanking); 
			}; // void GetRankingRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Ranking
	}; // namespace Message
}; // namespace BR


