////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameParty Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameParty
		{
 			// Cmd: Join party
			const MessageID JoinPartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 0);
			Result JoinPartyCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InvitedPlayer, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result JoinPartyCmd::ParseMessage( MessageData* pIMsg )


			Result JoinPartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) JoinPartyCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result JoinPartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* JoinPartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerInformation));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::JoinPartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InInvitedPlayer, sizeof(PlayerInformation));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* JoinPartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )

			Result JoinPartyCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result JoinPartyCmd::OverrideRouteContextDestination( EntityUID to )


			Result JoinPartyCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				JoinPartyCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "JoinParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, InviterID:{4}, InvitedPlayer:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetInviterID(), parser.GetInvitedPlayer()); 
				return ResultCode::SUCCESS;
			}; // Result JoinPartyCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID JoinPartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 0);
			Result JoinPartyRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofChatHistoryData = 0; uint8_t* pChatHistoryData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyLeaderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(uint8_t)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);


			Proc_End:

				return hr;

			}; // Result JoinPartyRes::ParseMessage( MessageData* pIMsg )


			Result JoinPartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) JoinPartyRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result JoinPartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* JoinPartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(PlayerID)
					+ sizeof(uint8_t)*InChatHistoryData.size() + sizeof(uint16_t));


				uint16_t numberOfInChatHistoryData = (uint16_t)InChatHistoryData.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::JoinPartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPartyLeaderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(uint8_t)*InChatHistoryData.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* JoinPartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )

			Result JoinPartyRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result JoinPartyRes::OverrideRouteContextDestination( EntityUID to )


			Result JoinPartyRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				JoinPartyRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "JoinParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, PartyLeaderID:{5}, ChatHistoryData:{6,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetPartyLeaderID(), parser.GetChatHistoryData()); 
				return ResultCode::SUCCESS;
			}; // Result JoinPartyRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player joined event.
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 1);
			Result PlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PlayerJoinedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerJoinedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerInformation));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerJoinedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )

			Result PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerJoinedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PlayerJoinedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerJoined:{0}:{1} , RouteContext:{2}, JoinedPlayer:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetJoinedPlayer()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerJoinedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Event for party leader is changed
			const MessageID PartyLeaderChangedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 2);
			Result PartyLeaderChangedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NewLeaderID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyLeaderChangedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PartyLeaderChangedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PartyLeaderChangedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PartyLeaderChangedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PartyLeaderChangedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::PartyLeaderChangedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InNewLeaderID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PartyLeaderChangedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )

			Result PartyLeaderChangedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PartyLeaderChangedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PartyLeaderChangedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PartyLeaderChangedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PartyLeaderChanged:{0}:{1} , RouteContext:{2}, NewLeaderID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetNewLeaderID()); 
				return ResultCode::SUCCESS;
			}; // Result PartyLeaderChangedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Event for player left.
			const MessageID LeavePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 3);
			Result LeavePartyCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result LeavePartyCmd::ParseMessage( MessageData* pIMsg )


			Result LeavePartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) LeavePartyCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result LeavePartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* LeavePartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::LeavePartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* LeavePartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result LeavePartyCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result LeavePartyCmd::OverrideRouteContextDestination( EntityUID to )


			Result LeavePartyCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				LeavePartyCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaveParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result LeavePartyCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID LeavePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 3);
			Result LeavePartyRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result LeavePartyRes::ParseMessage( MessageData* pIMsg )


			Result LeavePartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) LeavePartyRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result LeavePartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* LeavePartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::LeavePartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* LeavePartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result LeavePartyRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result LeavePartyRes::OverrideRouteContextDestination( EntityUID to )


			Result LeavePartyRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				LeavePartyRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaveParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result LeavePartyRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player left
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 4);
			Result PlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PlayerLeftS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerLeftS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::PlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerLeftS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )

			Result PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerLeftS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PlayerLeftS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerLeft:{0}:{1} , RouteContext:{2}, LeftPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetLeftPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerLeftS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Kick player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 5);
			Result KickPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result KickPlayerCmd::ParseMessage( MessageData* pIMsg )


			Result KickPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) KickPlayerCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result KickPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* KickPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::KickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* KickPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )

			Result KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			Result KickPlayerCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				KickPlayerCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "KickPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, PlayerToKick:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetPlayerToKick()); 
				return ResultCode::SUCCESS;
			}; // Result KickPlayerCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 5);
			Result KickPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result KickPlayerRes::ParseMessage( MessageData* pIMsg )


			Result KickPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) KickPlayerRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result KickPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* KickPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::KickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* KickPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result KickPlayerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result KickPlayerRes::OverrideRouteContextDestination( EntityUID to )


			Result KickPlayerRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				KickPlayerRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "KickPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result KickPlayerRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Event for Player kicked.
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 6);
			Result PlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PlayerKickedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerKickedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::PlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerKickedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )

			Result PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerKickedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PlayerKickedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerKicked:{0}:{1} , RouteContext:{2}, KickedPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetKickedPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerKickedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Send chat message to server.
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 7);
			Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )


			Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) ChatMessageC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInChatMessageLength = InChatMessage ? (uint16_t)(strlen(InChatMessage)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInChatMessageLength 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::ChatMessageC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )

			Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result ChatMessageC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				ChatMessageC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, PlayerID:{3}, ChatMessage:{4,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetPlayerID(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Brocast chatting message
			const MessageID ChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 8);
			Result ChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfSenderName = 0;
				uint16_t uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, sizeof(char)*uiSizeOfSenderName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )


			Result ChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) ChatMessageS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* ChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInSenderNameLength = InSenderName ? (uint16_t)(strlen(InSenderName)+1) : 1;
				uint16_t __uiInChatMessageLength = InChatMessage ? (uint16_t)(strlen(InChatMessage)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInSenderNameLength + sizeof(uint16_t) + __uiInChatMessageLength 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::ChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )

			Result ChatMessageS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ChatMessageS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result ChatMessageS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				ChatMessageS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, SenderID:{3}, SenderName:{4,60}, ChatMessage:{5,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetSenderID(), parser.GetSenderName(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Quick Chatting message
			const MessageID QuickChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 9);
			Result QuickChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_QuickChatID, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result QuickChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )


			Result QuickChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) QuickChatMessageC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result QuickChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* QuickChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::QuickChatMessageC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InQuickChatID, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* QuickChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )

			Result QuickChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result QuickChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result QuickChatMessageC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				QuickChatMessageC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "QuickChatMessage:{0}:{1} , RouteContext:{2}, PlayerID:{3}, QuickChatID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetPlayerID(), parser.GetQuickChatID()); 
				return ResultCode::SUCCESS;
			}; // Result QuickChatMessageC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Quick Chatting message
			const MessageID QuickChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 10);
			Result QuickChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_QuickChatID, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result QuickChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )


			Result QuickChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) QuickChatMessageS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result QuickChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* QuickChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::QuickChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InQuickChatID, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* QuickChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )

			Result QuickChatMessageS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result QuickChatMessageS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result QuickChatMessageS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				QuickChatMessageS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "QuickChatMessage:{0}:{1} , RouteContext:{2}, SenderID:{3}, QuickChatID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetSenderID(), parser.GetQuickChatID()); 
				return ResultCode::SUCCESS;
			}; // Result QuickChatMessageS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Start party game matching
			const MessageID StartGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 11);
			Result StartGameMatchCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxGamePlayers, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result StartGameMatchCmd::ParseMessage( MessageData* pIMsg )


			Result StartGameMatchCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) StartGameMatchCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result StartGameMatchCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* StartGameMatchCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::StartGameMatchCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InMaxGamePlayers, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* StartGameMatchCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )

			Result StartGameMatchCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result StartGameMatchCmd::OverrideRouteContextDestination( EntityUID to )


			Result StartGameMatchCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				StartGameMatchCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "StartGameMatch:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, MaxGamePlayers:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetMaxGamePlayers()); 
				return ResultCode::SUCCESS;
			}; // Result StartGameMatchCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID StartGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 11);
			Result StartGameMatchRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result StartGameMatchRes::ParseMessage( MessageData* pIMsg )


			Result StartGameMatchRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) StartGameMatchRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result StartGameMatchRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* StartGameMatchRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::StartGameMatchRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* StartGameMatchRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result StartGameMatchRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result StartGameMatchRes::OverrideRouteContextDestination( EntityUID to )


			Result StartGameMatchRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				StartGameMatchRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "StartGameMatch:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result StartGameMatchRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Event sent when the party is queued for game matching
			const MessageID QueuedGameMatchingS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 12);
			Result QueuedGameMatchingS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingQueueTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result QueuedGameMatchingS2CEvt::ParseMessage( MessageData* pIMsg )


			Result QueuedGameMatchingS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) QueuedGameMatchingS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result QueuedGameMatchingS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* QueuedGameMatchingS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::QueuedGameMatchingS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InMatchingQueueTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* QueuedGameMatchingS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )

			Result QueuedGameMatchingS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result QueuedGameMatchingS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result QueuedGameMatchingS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				QueuedGameMatchingS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "QueuedGameMatching:{0}:{1} , RouteContext:{2}, MatchingQueueTicket:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetMatchingQueueTicket()); 
				return ResultCode::SUCCESS;
			}; // Result QueuedGameMatchingS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Cancel game matching
			const MessageID CancelGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 13);
			Result CancelGameMatchCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result CancelGameMatchCmd::ParseMessage( MessageData* pIMsg )


			Result CancelGameMatchCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) CancelGameMatchCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CancelGameMatchCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* CancelGameMatchCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::CancelGameMatchCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CancelGameMatchCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result CancelGameMatchCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CancelGameMatchCmd::OverrideRouteContextDestination( EntityUID to )


			Result CancelGameMatchCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				CancelGameMatchCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CancelGameMatch:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result CancelGameMatchCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID CancelGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 13);
			Result CancelGameMatchRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result CancelGameMatchRes::ParseMessage( MessageData* pIMsg )


			Result CancelGameMatchRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) CancelGameMatchRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CancelGameMatchRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* CancelGameMatchRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::CancelGameMatchRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CancelGameMatchRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result CancelGameMatchRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CancelGameMatchRes::OverrideRouteContextDestination( EntityUID to )


			Result CancelGameMatchRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				CancelGameMatchRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CancelGameMatch:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result CancelGameMatchRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Event sent when the game matching is canceled
			const MessageID CanceledGameMatchingS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 14);
			Result CanceledGameMatchingS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingQueueTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result CanceledGameMatchingS2CEvt::ParseMessage( MessageData* pIMsg )


			Result CanceledGameMatchingS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) CanceledGameMatchingS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result CanceledGameMatchingS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* CanceledGameMatchingS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::CanceledGameMatchingS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InMatchingQueueTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* CanceledGameMatchingS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )

			Result CanceledGameMatchingS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CanceledGameMatchingS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result CanceledGameMatchingS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				CanceledGameMatchingS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CanceledGameMatching:{0}:{1} , RouteContext:{2}, MatchingQueueTicket:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetMatchingQueueTicket()); 
				return ResultCode::SUCCESS;
			}; // Result CanceledGameMatchingS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Event sent when the the matching has dequeued.
			const MessageID MatchingItemDequeuedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 15);
			Result MatchingItemDequeuedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result MatchingItemDequeuedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result MatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) MatchingItemDequeuedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result MatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* MatchingItemDequeuedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(MatchingQueueTicket));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::MatchingItemDequeuedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* MatchingItemDequeuedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )

			Result MatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result MatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result MatchingItemDequeuedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				MatchingItemDequeuedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "MatchingItemDequeued:{0}:{1} , RouteContext:{2}, MatchingTicket:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result MatchingItemDequeuedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)



		}; // namespace GameParty
	}; // namespace Message
}; // namespace SF


