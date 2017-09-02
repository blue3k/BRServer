////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameServer
		{
 			// Cmd: Kick
			const MessageID RegisterPlayerToJoinGameServerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 0);
			Result RegisterPlayerToJoinGameServerCmd::ParseMessage( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_FBUserID, pCur, iMsgSize, sizeof(FacebookUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ShardID, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerCmd::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerToJoinGameServerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterPlayerToJoinGameServerCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterPlayerToJoinGameServerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerToJoinGameServerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(FacebookUID)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::RegisterPlayerToJoinGameServerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InFBUserID, sizeof(FacebookUID));
				Protocol::PackParamCopy( pMsgData, &InShardID, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterPlayerToJoinGameServerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )

			Result RegisterPlayerToJoinGameServerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerToJoinGameServerCmd::OverrideRouteContextDestination( EntityUID to )


			Result RegisterPlayerToJoinGameServerCmd::TraceOut(MessageData* pMsg)
			{
 				RegisterPlayerToJoinGameServerCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RegisterPlayerToJoinGameServer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, Ticket:{5}, FBUserID:{6}, ShardID:{7}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetTicket(), parser.GetFBUserID(), parser.GetShardID()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerToJoinGameServerCmd::TraceOut(MessageData* pMsg)

			const MessageID RegisterPlayerToJoinGameServerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 0);
			Result RegisterPlayerToJoinGameServerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfPublicAddress = 0;
				uint16_t uiSizeOfPublicAddressV6 = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPublicAddress, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_PublicAddress, pCur, iMsgSize, sizeof(char)*uiSizeOfPublicAddress ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPublicAddressV6, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_PublicAddressV6, pCur, iMsgSize, sizeof(char)*uiSizeOfPublicAddressV6 ) );
				protocolChk( Protocol::StreamParamCopy( &m_Port, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerRes::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerToJoinGameServerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterPlayerToJoinGameServerRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterPlayerToJoinGameServerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerToJoinGameServerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInPublicAddressLength = InPublicAddress ? (uint16_t)(strlen(InPublicAddress)+1) : 1;
				uint16_t __uiInPublicAddressV6Length = InPublicAddressV6 ? (uint16_t)(strlen(InPublicAddressV6)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInPublicAddressLength + sizeof(uint16_t) + __uiInPublicAddressV6Length 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::RegisterPlayerToJoinGameServerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &__uiInPublicAddressLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPublicAddress ? InPublicAddress : "", __uiInPublicAddressLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPublicAddressV6Length, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPublicAddressV6 ? InPublicAddressV6 : "", __uiInPublicAddressV6Length );
				Protocol::PackParamCopy( pMsgData, &InPort, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterPlayerToJoinGameServerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort )

			Result RegisterPlayerToJoinGameServerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerToJoinGameServerRes::OverrideRouteContextDestination( EntityUID to )


			Result RegisterPlayerToJoinGameServerRes::TraceOut(MessageData* pMsg)
			{
 				RegisterPlayerToJoinGameServerRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RegisterPlayerToJoinGameServer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, PublicAddress:{5,60}, PublicAddressV6:{6,60}, Port:{7}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetPublicAddress(), parser.GetPublicAddressV6(), parser.GetPort()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerToJoinGameServerRes::TraceOut(MessageData* pMsg)

			// Cmd: Kick
			const MessageID RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 1);
			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessage( MessageData* pIMsg )
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
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_FBUserID, pCur, iMsgSize, sizeof(FacebookUID) ) );


			Proc_End:

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterPlayerToJoinGameServerOnPlayerEntityCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerToJoinGameServerOnPlayerEntityCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(FacebookUID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InFBUserID, sizeof(FacebookUID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterPlayerToJoinGameServerOnPlayerEntityCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )

			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::OverrideRouteContextDestination( EntityUID to )


			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::TraceOut(MessageData* pMsg)
			{
 				RegisterPlayerToJoinGameServerOnPlayerEntityCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RegisterPlayerToJoinGameServerOnPlayerEntity:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, Ticket:{5}, FBUserID:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetTicket(), parser.GetFBUserID()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::TraceOut(MessageData* pMsg)

			const MessageID RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 1);
			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterPlayerToJoinGameServerOnPlayerEntityRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerToJoinGameServerOnPlayerEntityRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID, __uiMessageSize ) );

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

			}; // MessageData* RegisterPlayerToJoinGameServerOnPlayerEntityRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::OverrideRouteContextDestination( EntityUID to )


			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::TraceOut(MessageData* pMsg)
			{
 				RegisterPlayerToJoinGameServerOnPlayerEntityRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RegisterPlayerToJoinGameServerOnPlayerEntity:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::TraceOut(MessageData* pMsg)

			// C2S: Chatting message
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 2);
			Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfSenderName = 0;
				uint16_t uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, sizeof(char)*uiSizeOfSenderName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )


			Result ChatMessageC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ChatMessageC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ChatMessageC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInSenderNameLength = InSenderName ? (uint16_t)(strlen(InSenderName)+1) : 1;
				uint16_t __uiInChatMessageLength = InChatMessage ? (uint16_t)(strlen(InChatMessage)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInSenderNameLength + sizeof(uint16_t) + __uiInChatMessageLength 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::ChatMessageC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ChatMessageC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )

			Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result ChatMessageC2SEvt::TraceOut(MessageData* pMsg)
			{
 				ChatMessageC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, SenderID:{3}, Role:{4}, SenderName:{5,60}, ChatMessage:{6,60}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetSenderID(), (int)parser.GetRole(), parser.GetSenderName(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Notification
			const MessageID NotifyC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 3);
			Result NotifyC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfMessageText = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageID, pCur, iMsgSize, sizeof(NotificationType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam0, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam1, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfMessageText, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_MessageText, pCur, iMsgSize, sizeof(char)*uiSizeOfMessageText ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result NotifyC2SEvt::ParseMessage( MessageData* pIMsg )


			Result NotifyC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) NotifyC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result NotifyC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* NotifyC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInMessageTextLength = InMessageText ? (uint16_t)(strlen(InMessageText)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInMessageTextLength 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(uint32_t)
					+ sizeof(NotificationType)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::NotifyC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InMessageID, sizeof(NotificationType));
				Protocol::PackParamCopy( pMsgData, &InMessageParam0, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InMessageParam1, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &__uiInMessageTextLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InMessageText ? InMessageText : "", __uiInMessageTextLength );
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* NotifyC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )

			Result NotifyC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result NotifyC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result NotifyC2SEvt::TraceOut(MessageData* pMsg)
			{
 				NotifyC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "Notify:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, NotificationID:{4}, MessageID:{5}, MessageParam0:{6}, MessageParam1:{7}, MessageText:{8,60}, TimeStamp:{9}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetNotificationID(), parser.GetMessageID(), parser.GetMessageParam0(), parser.GetMessageParam1(), parser.GetMessageText(), parser.GetTimeStamp()); 
				return ResultCode::SUCCESS;
			}; // Result NotifyC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Friend Accept
			const MessageID FriendAcceptedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 4);
			Result FriendAcceptedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Accepter, pCur, iMsgSize, sizeof(ServerFriendInformation) ) );


			Proc_End:

				return hr;

			}; // Result FriendAcceptedC2SEvt::ParseMessage( MessageData* pIMsg )


			Result FriendAcceptedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) FriendAcceptedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result FriendAcceptedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* FriendAcceptedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(ServerFriendInformation));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::FriendAcceptedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InAccepter, sizeof(ServerFriendInformation));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* FriendAcceptedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )

			Result FriendAcceptedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result FriendAcceptedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result FriendAcceptedC2SEvt::TraceOut(MessageData* pMsg)
			{
 				FriendAcceptedC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "FriendAccepted:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, Accepter:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetAccepter()); 
				return ResultCode::SUCCESS;
			}; // Result FriendAcceptedC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Friend Remove
			const MessageID FriendRemovedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 5);
			Result FriendRemovedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RemoverID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result FriendRemovedC2SEvt::ParseMessage( MessageData* pIMsg )


			Result FriendRemovedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) FriendRemovedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result FriendRemovedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* FriendRemovedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::FriendRemovedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InRemoverID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* FriendRemovedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )

			Result FriendRemovedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result FriendRemovedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result FriendRemovedC2SEvt::TraceOut(MessageData* pMsg)
			{
 				FriendRemovedC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "FriendRemoved:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, RemoverID:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetRemoverID()); 
				return ResultCode::SUCCESS;
			}; // Result FriendRemovedC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Request Player Status Update
			const MessageID RequestPlayerStatusUpdateC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 6);
			Result RequestPlayerStatusUpdateC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RequesterID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateC2SEvt::ParseMessage( MessageData* pIMsg )


			Result RequestPlayerStatusUpdateC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RequestPlayerStatusUpdateC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestPlayerStatusUpdateC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestPlayerStatusUpdateC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::RequestPlayerStatusUpdateC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InRequesterID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestPlayerStatusUpdateC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )

			Result RequestPlayerStatusUpdateC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RequestPlayerStatusUpdateC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result RequestPlayerStatusUpdateC2SEvt::TraceOut(MessageData* pMsg)
			{
 				RequestPlayerStatusUpdateC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RequestPlayerStatusUpdate:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, RequesterID:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetRequesterID()); 
				return ResultCode::SUCCESS;
			}; // Result RequestPlayerStatusUpdateC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Notify Player Status Updated
			const MessageID NotifyPlayerStatusUpdatedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 7);
			Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LatestActiveTime, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsInGame, pCur, iMsgSize, sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessage( MessageData* pIMsg )


			Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) NotifyPlayerStatusUpdatedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* NotifyPlayerStatusUpdatedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(AccountID)
					+ sizeof(uint32_t)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InLatestActiveTime, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InIsInGame, sizeof(uint8_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* NotifyPlayerStatusUpdatedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )

			Result NotifyPlayerStatusUpdatedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result NotifyPlayerStatusUpdatedC2SEvt::TraceOut(MessageData* pMsg)
			{
 				NotifyPlayerStatusUpdatedC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "NotifyPlayerStatusUpdated:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, LatestActiveTime:{4}, IsInGame:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetLatestActiveTime(), parser.GetIsInGame()); 
				return ResultCode::SUCCESS;
			}; // Result NotifyPlayerStatusUpdatedC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Notification
			const MessageID NotifyPartyInviteC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 8);
			Result NotifyPartyInviteC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfInviterName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_DestPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfInviterName, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_InviterName, pCur, iMsgSize, sizeof(char)*uiSizeOfInviterName ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result NotifyPartyInviteC2SEvt::ParseMessage( MessageData* pIMsg )


			Result NotifyPartyInviteC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) NotifyPartyInviteC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result NotifyPartyInviteC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* NotifyPartyInviteC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInInviterNameLength = InInviterName ? (uint16_t)(strlen(InInviterName)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInInviterNameLength 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameServer::NotifyPartyInviteC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InDestPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInInviterNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InInviterName ? InInviterName : "", __uiInInviterNameLength );
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* NotifyPartyInviteC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )

			Result NotifyPartyInviteC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result NotifyPartyInviteC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result NotifyPartyInviteC2SEvt::TraceOut(MessageData* pMsg)
			{
 				NotifyPartyInviteC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "NotifyPartyInvite:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, InviterID:{4}, InviterName:{5,60}, PartyUID:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetInviterID(), parser.GetInviterName(), parser.GetPartyUID()); 
				return ResultCode::SUCCESS;
			}; // Result NotifyPartyInviteC2SEvt::TraceOut(MessageData* pMsg)



		}; // namespace GameServer
	}; // namespace Message
}; // namespace SF


