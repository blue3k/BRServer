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
			HRESULT AddPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofPlayerInfo = 0; BYTE* pPlayerInfo = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RankingScore, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPlayerInfo, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pPlayerInfo, pCur, iMsgSize, sizeof(BYTE)*numberofPlayerInfo ) );
				m_PlayerInfo.SetLinkedBuffer(numberofPlayerInfo, numberofPlayerInfo, pPlayerInfo);


			Proc_End:

				return hr;

			}; // HRESULT AddPlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT AddPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(UINT64)
					+ sizeof(BYTE)*InPlayerInfo.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInPlayerInfo = (UINT16)InPlayerInfo.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::AddPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRankingScore, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &numberOfInPlayerInfo, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InPlayerInfo.data(), (INT)(sizeof(BYTE)*InPlayerInfo.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AddPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )

			HRESULT AddPlayerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT AddPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			void AddPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AddPlayerCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}, RankingScore:{6}, PlayerInfo:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID, m_RankingScore, m_PlayerInfo); 
			}; // void AddPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AddPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 0);
			HRESULT AddPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ranking, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT AddPlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT AddPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InRanking )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::AddPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRanking, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AddPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InRanking )

			HRESULT AddPlayerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT AddPlayerRes::OverrideRouteContextDestination( EntityUID to )


			void AddPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AddPlayerRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, Ranking:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_Ranking); 
			}; // void AddPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID RemovePlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 1);
			HRESULT RemovePlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT RemovePlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RemovePlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_SYSTEM_OK;

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

			}; // HRESULT RemovePlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			HRESULT RemovePlayerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT RemovePlayerCmd::OverrideRouteContextDestination( EntityUID to )


			void RemovePlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RemovePlayerCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID); 
			}; // void RemovePlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RemovePlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 1);
			HRESULT RemovePlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT RemovePlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RemovePlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::RemovePlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RemovePlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )

			HRESULT RemovePlayerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT RemovePlayerRes::OverrideRouteContextDestination( EntityUID to )


			void RemovePlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RemovePlayerRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result); 
			}; // void RemovePlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID GetPlayerRankingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 2);
			HRESULT GetPlayerRankingCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetPlayerRankingCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetPlayerRankingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_SYSTEM_OK;

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

			}; // HRESULT GetPlayerRankingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			HRESULT GetPlayerRankingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT GetPlayerRankingCmd::OverrideRouteContextDestination( EntityUID to )


			void GetPlayerRankingCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetPlayerRankingCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID); 
			}; // void GetPlayerRankingCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetPlayerRankingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 2);
			HRESULT GetPlayerRankingRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ranking, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetPlayerRankingRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetPlayerRankingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InRanking )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::GetPlayerRankingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRanking, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetPlayerRankingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InRanking )

			HRESULT GetPlayerRankingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT GetPlayerRankingRes::OverrideRouteContextDestination( EntityUID to )


			void GetPlayerRankingRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetPlayerRankingRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, Ranking:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_Ranking); 
			}; // void GetPlayerRankingRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Update a player to ranking
			const MessageID UpdatePlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 3);
			HRESULT UpdatePlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofPlayerInfo = 0; BYTE* pPlayerInfo = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RankingScore, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPlayerInfo, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pPlayerInfo, pCur, iMsgSize, sizeof(BYTE)*numberofPlayerInfo ) );
				m_PlayerInfo.SetLinkedBuffer(numberofPlayerInfo, numberofPlayerInfo, pPlayerInfo);


			Proc_End:

				return hr;

			}; // HRESULT UpdatePlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT UpdatePlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(UINT64)
					+ sizeof(BYTE)*InPlayerInfo.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInPlayerInfo = (UINT16)InPlayerInfo.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::UpdatePlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRankingScore, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &numberOfInPlayerInfo, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InPlayerInfo.data(), (INT)(sizeof(BYTE)*InPlayerInfo.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UpdatePlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )

			HRESULT UpdatePlayerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT UpdatePlayerCmd::OverrideRouteContextDestination( EntityUID to )


			void UpdatePlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UpdatePlayerCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}, RankingScore:{6}, PlayerInfo:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID, m_RankingScore, m_PlayerInfo); 
			}; // void UpdatePlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UpdatePlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 3);
			HRESULT UpdatePlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ranking, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT UpdatePlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT UpdatePlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InRanking )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::UpdatePlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRanking, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UpdatePlayerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InRanking )

			HRESULT UpdatePlayerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT UpdatePlayerRes::OverrideRouteContextDestination( EntityUID to )


			void UpdatePlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UpdatePlayerRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, Ranking:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_Ranking); 
			}; // void UpdatePlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID GetRankingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 4);
			HRESULT GetRankingCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_From, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetRankingCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetRankingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT32 &InFrom, const UINT32 &InCount )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(UINT32)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::GetRankingCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InFrom, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetRankingCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT32 &InFrom, const UINT32 &InCount )

			HRESULT GetRankingCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT GetRankingCmd::OverrideRouteContextDestination( EntityUID to )


			void GetRankingCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetRankingCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, From:{5}, Count:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_From, m_Count); 
			}; // void GetRankingCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetRankingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_RANKING, 4);
			HRESULT GetRankingRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_SYSTEM_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerRanking, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetRankingRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetRankingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InPlayerRanking )
			{
 				HRESULT hr = S_SYSTEM_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Ranking::GetRankingRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InPlayerRanking, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetRankingRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InPlayerRanking )

			HRESULT GetRankingRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // HRESULT GetRankingRes::OverrideRouteContextDestination( EntityUID to )


			void GetRankingRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetRankingRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, PlayerRanking:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_PlayerRanking); 
			}; // void GetRankingRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Ranking
	}; // namespace Message
}; // namespace BR


