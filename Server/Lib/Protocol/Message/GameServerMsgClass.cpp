////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameServer Message parser implementations
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
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace GameServer
		{
 			// Cmd: Kick
			const MessageID RegisterPlayerToJoinGameServerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 0);
			Result RegisterPlayerToJoinGameServerCmd::ParseMessage( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_FBUserID, pCur, iMsgSize, (int)sizeof(FacebookUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ShardID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerCmd::ParseMessage( MessageData* pIMsg )

			Result RegisterPlayerToJoinGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(FacebookUID)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::RegisterPlayerToJoinGameServerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InFBUserID, sizeof(FacebookUID));
				Protocol::PackParamCopy( pMsgData, &InShardID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )

			Result RegisterPlayerToJoinGameServerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerToJoinGameServerCmd::OverrideRouteContextDestination( EntityUID to )


			void RegisterPlayerToJoinGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterPlayerToJoinGameServerCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}, Ticket:{6}, FBUserID:{7}, ShardID:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID, m_Ticket, m_FBUserID, m_ShardID); 
			}; // void RegisterPlayerToJoinGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterPlayerToJoinGameServerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 0);
			Result RegisterPlayerToJoinGameServerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfPublicAddress = 0;
				UINT16 uiSizeOfPublicAddressV6 = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPublicAddress, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PublicAddress, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPublicAddress ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPublicAddressV6, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PublicAddressV6, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPublicAddressV6 ) );
				protocolChk( Protocol::StreamParamCopy( &m_Port, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerRes::ParseMessage( MessageData* pIMsg )

			Result RegisterPlayerToJoinGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInPublicAddressLength = InPublicAddress ? (UINT16)(strlen(InPublicAddress)+1) : 1;
				UINT16 __uiInPublicAddressV6Length = InPublicAddressV6 ? (UINT16)(strlen(InPublicAddressV6)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInPublicAddressLength + sizeof(UINT16) + __uiInPublicAddressV6Length 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::RegisterPlayerToJoinGameServerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &__uiInPublicAddressLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPublicAddress ? InPublicAddress : "", __uiInPublicAddressLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPublicAddressV6Length, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPublicAddressV6 ? InPublicAddressV6 : "", __uiInPublicAddressV6Length );
				Protocol::PackParamCopy( pMsgData, &InPort, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort )

			Result RegisterPlayerToJoinGameServerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerToJoinGameServerRes::OverrideRouteContextDestination( EntityUID to )


			void RegisterPlayerToJoinGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterPlayerToJoinGameServerRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}, PublicAddress:{6,60}, PublicAddressV6:{7,60}, Port:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result, m_PublicAddress, m_PublicAddressV6, m_Port); 
			}; // void RegisterPlayerToJoinGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick
			const MessageID RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 1);
			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessage( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_FBUserID, pCur, iMsgSize, (int)sizeof(FacebookUID) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessage( MessageData* pIMsg )

			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(FacebookUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InFBUserID, sizeof(FacebookUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )

			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::OverrideRouteContextDestination( EntityUID to )


			void RegisterPlayerToJoinGameServerOnPlayerEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterPlayerToJoinGameServerOnPlayerEntityCmd:{1}:{2} , RouteContext:{3}, TransactionID:{4}, PlayerID:{5}, Ticket:{6}, FBUserID:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_PlayerID, m_Ticket, m_FBUserID); 
			}; // void RegisterPlayerToJoinGameServerOnPlayerEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 1);
			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessage( MessageData* pIMsg )

			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::OverrideRouteContextDestination( EntityUID to )


			void RegisterPlayerToJoinGameServerOnPlayerEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterPlayerToJoinGameServerOnPlayerEntityRes:{1}:{2} , RouteContext:{3}, TransactionID:{4}, Result:{5:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TransactionID, m_Result); 
			}; // void RegisterPlayerToJoinGameServerOnPlayerEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Chatting message
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 2);
			Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfSenderName = 0;
				UINT16 uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfSenderName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )

			Result ChatMessageC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInSenderNameLength = InSenderName ? (UINT16)(strlen(InSenderName)+1) : 1;
				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInSenderNameLength + sizeof(UINT16) + __uiInChatMessageLength 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::ChatMessageC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ChatMessageC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )

			Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			void ChatMessageC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ChatMessageC2SEvt:{1}:{2} , RouteContext:{3}, SenderID:{4}, Role:{5}, SenderName:{6,60}, ChatMessage:{7,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_SenderID, m_Role, m_SenderName, m_ChatMessage); 
			}; // void ChatMessageC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Notification
			const MessageID NotifyC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 3);
			Result NotifyC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfMessageText = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageID, pCur, iMsgSize, (int)sizeof(NotificationType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam0, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam1, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfMessageText, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_MessageText, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfMessageText ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result NotifyC2SEvt::ParseMessage( MessageData* pIMsg )

			Result NotifyC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInMessageTextLength = InMessageText ? (UINT16)(strlen(InMessageText)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInMessageTextLength 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(uint32_t)
					+ sizeof(NotificationType)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::NotifyC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InMessageID, sizeof(NotificationType));
				Protocol::PackParamCopy( pMsgData, &InMessageParam0, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InMessageParam1, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &__uiInMessageTextLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InMessageText ? InMessageText : "", __uiInMessageTextLength );
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result NotifyC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )

			Result NotifyC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result NotifyC2SEvt::OverrideRouteContextDestination( EntityUID to )


			void NotifyC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:NotifyC2SEvt:{1}:{2} , RouteContext:{3}, DestPlayerID:{4}, NotificationID:{5}, MessageID:{6}, MessageParam0:{7}, MessageParam1:{8}, MessageText:{9,60}, TimeStamp:{10}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_DestPlayerID, m_NotificationID, m_MessageID, m_MessageParam0, m_MessageParam1, m_MessageText, m_TimeStamp); 
			}; // void NotifyC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Friend Accept
			const MessageID FriendAcceptedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 4);
			Result FriendAcceptedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Accepter, pCur, iMsgSize, (int)sizeof(ServerFriendInformation) ) );


			Proc_End:

				return hr;

			}; // Result FriendAcceptedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result FriendAcceptedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(ServerFriendInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::FriendAcceptedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InAccepter, sizeof(ServerFriendInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FriendAcceptedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )

			Result FriendAcceptedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result FriendAcceptedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			void FriendAcceptedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FriendAcceptedC2SEvt:{1}:{2} , RouteContext:{3}, DestPlayerID:{4}, Accepter:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_DestPlayerID, m_Accepter); 
			}; // void FriendAcceptedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Friend Remove
			const MessageID FriendRemovedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 5);
			Result FriendRemovedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RemoverID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result FriendRemovedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result FriendRemovedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::FriendRemovedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InRemoverID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FriendRemovedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )

			Result FriendRemovedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result FriendRemovedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			void FriendRemovedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FriendRemovedC2SEvt:{1}:{2} , RouteContext:{3}, DestPlayerID:{4}, RemoverID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_DestPlayerID, m_RemoverID); 
			}; // void FriendRemovedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Request Player Status Update
			const MessageID RequestPlayerStatusUpdateC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 6);
			Result RequestPlayerStatusUpdateC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RequesterID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateC2SEvt::ParseMessage( MessageData* pIMsg )

			Result RequestPlayerStatusUpdateC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::RequestPlayerStatusUpdateC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InRequesterID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )

			Result RequestPlayerStatusUpdateC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RequestPlayerStatusUpdateC2SEvt::OverrideRouteContextDestination( EntityUID to )


			void RequestPlayerStatusUpdateC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestPlayerStatusUpdateC2SEvt:{1}:{2} , RouteContext:{3}, DestPlayerID:{4}, RequesterID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_DestPlayerID, m_RequesterID); 
			}; // void RequestPlayerStatusUpdateC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Notify Player Status Updated
			const MessageID NotifyPlayerStatusUpdatedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 7);
			Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LatestActiveTime, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsInGame, pCur, iMsgSize, (int)sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result NotifyPlayerStatusUpdatedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(uint32_t)
					+ sizeof(uint8_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InLatestActiveTime, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InIsInGame, sizeof(uint8_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )

			Result NotifyPlayerStatusUpdatedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			void NotifyPlayerStatusUpdatedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:NotifyPlayerStatusUpdatedC2SEvt:{1}:{2} , RouteContext:{3}, DestPlayerID:{4}, LatestActiveTime:{5}, IsInGame:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_DestPlayerID, m_LatestActiveTime, m_IsInGame); 
			}; // void NotifyPlayerStatusUpdatedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Notification
			const MessageID NotifyPartyInviteC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMESERVER, 8);
			Result NotifyPartyInviteC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfInviterName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, (int)sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfInviterName, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_InviterName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfInviterName ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result NotifyPartyInviteC2SEvt::ParseMessage( MessageData* pIMsg )

			Result NotifyPartyInviteC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInInviterNameLength = InInviterName ? (UINT16)(strlen(InInviterName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInInviterNameLength 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameServer::NotifyPartyInviteC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInInviterNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InInviterName ? InInviterName : "", __uiInInviterNameLength );
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result NotifyPartyInviteC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID )

			Result NotifyPartyInviteC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result NotifyPartyInviteC2SEvt::OverrideRouteContextDestination( EntityUID to )


			void NotifyPartyInviteC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:NotifyPartyInviteC2SEvt:{1}:{2} , RouteContext:{3}, DestPlayerID:{4}, InviterID:{5}, InviterName:{6,60}, PartyUID:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_DestPlayerID, m_InviterID, m_InviterName, m_PartyUID); 
			}; // void NotifyPartyInviteC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace GameServer
	}; // namespace Message
}; // namespace BR


