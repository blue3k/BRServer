﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameServer
		{
 			// Cmd: Register player so that the player can connection and join to the game server.
			const MessageID RegisterPlayerToJoinGameServerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 0);
			Result RegisterPlayerToJoinGameServerCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));
				protocolCheck(input->Read(m_Ticket));
				protocolCheck(input->Read(m_FBUserID));
				protocolCheck(input->Read(m_ShardID));

				return hr;

			}; // Result RegisterPlayerToJoinGameServerCmd::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerToJoinGameServerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RegisterPlayerToJoinGameServerCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RegisterPlayerToJoinGameServerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerToJoinGameServerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InTicket)
					, SerializedSizeOf(InFBUserID)
					, SerializedSizeOf(InShardID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::RegisterPlayerToJoinGameServerCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InTicket));
				protocolCheck(output->Write(InFBUserID));
				protocolCheck(output->Write(InShardID));

				return hr;
			}; // MessageData* RegisterPlayerToJoinGameServerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )

			Result RegisterPlayerToJoinGameServerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result RegisterPlayerToJoinGameServerCmd::OverrideRouteContextDestination( EntityUID to )


			Result RegisterPlayerToJoinGameServerCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RegisterPlayerToJoinGameServerCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RegisterPlayerToJoinGameServer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, Ticket:{5}, FBUserID:{6}, ShardID:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetTicket(), parser.GetFBUserID(), parser.GetShardID()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerToJoinGameServerCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID RegisterPlayerToJoinGameServerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 0);
			Result RegisterPlayerToJoinGameServerRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_PublicAddress, ArrayLen));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_PublicAddressV6, ArrayLen));
				protocolCheck(input->Read(m_Port));

				return hr;

			}; // Result RegisterPlayerToJoinGameServerRes::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerToJoinGameServerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RegisterPlayerToJoinGameServerRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RegisterPlayerToJoinGameServerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerToJoinGameServerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
					, SerializedSizeOf(InPublicAddress)
					, SerializedSizeOf(InPublicAddressV6)
					, SerializedSizeOf(InPort)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::RegisterPlayerToJoinGameServerRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));
				protocolCheck(output->Write(InPublicAddress));
				protocolCheck(output->Write(InPublicAddressV6));
				protocolCheck(output->Write(InPort));

				return hr;
			}; // MessageData* RegisterPlayerToJoinGameServerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort )

			Result RegisterPlayerToJoinGameServerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result RegisterPlayerToJoinGameServerRes::OverrideRouteContextDestination( EntityUID to )


			Result RegisterPlayerToJoinGameServerRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RegisterPlayerToJoinGameServerRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RegisterPlayerToJoinGameServer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, PublicAddress:{5,60}, PublicAddressV6:{6,60}, Port:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetPublicAddress(), parser.GetPublicAddressV6(), parser.GetPort()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerToJoinGameServerRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
			const MessageID RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 1);
			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));
				protocolCheck(input->Read(m_Ticket));
				protocolCheck(input->Read(m_FBUserID));

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RegisterPlayerToJoinGameServerOnPlayerEntityCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerToJoinGameServerOnPlayerEntityCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InTicket)
					, SerializedSizeOf(InFBUserID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InTicket));
				protocolCheck(output->Write(InFBUserID));

				return hr;
			}; // MessageData* RegisterPlayerToJoinGameServerOnPlayerEntityCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )

			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::OverrideRouteContextDestination( EntityUID to )


			Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RegisterPlayerToJoinGameServerOnPlayerEntityCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RegisterPlayerToJoinGameServerOnPlayerEntity:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, Ticket:{5}, FBUserID:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetTicket(), parser.GetFBUserID()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 1);
			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessage( MessageData* pIMsg )


			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RegisterPlayerToJoinGameServerOnPlayerEntityRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterPlayerToJoinGameServerOnPlayerEntityRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* RegisterPlayerToJoinGameServerOnPlayerEntityRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::OverrideRouteContextDestination( EntityUID to )


			Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RegisterPlayerToJoinGameServerOnPlayerEntityRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RegisterPlayerToJoinGameServerOnPlayerEntity:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerToJoinGameServerOnPlayerEntityRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Chatting message event.
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 2);
			Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_SenderID));
				protocolCheck(input->Read(m_Role));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_SenderName, ArrayLen));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_ChatMessage, ArrayLen));

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )


			Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ChatMessageC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InSenderID)
					, SerializedSizeOf(InRole)
					, SerializedSizeOf(InSenderName)
					, SerializedSizeOf(InChatMessage)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::ChatMessageC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InSenderID));
				protocolCheck(output->Write(InRole));
				protocolCheck(output->Write(InSenderName));
				protocolCheck(output->Write(InChatMessage));

				return hr;
			}; // MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )

			Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result ChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ChatMessageC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, SenderID:{3}, Role:{4}, SenderName:{5,60}, ChatMessage:{6,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetSenderID(), (int)parser.GetRole(), parser.GetSenderName(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Notification event for P2P
			const MessageID NotifyC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 3);
			Result NotifyC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_DestPlayerID));
				protocolCheck(input->Read(m_NotificationID));
				protocolCheck(input->Read(m_MessageID));
				protocolCheck(input->Read(m_MessageParam0));
				protocolCheck(input->Read(m_MessageParam1));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_MessageText, ArrayLen));
				protocolCheck(input->Read(m_TimeStamp));

				return hr;

			}; // Result NotifyC2SEvt::ParseMessage( MessageData* pIMsg )


			Result NotifyC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) NotifyC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result NotifyC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* NotifyC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InDestPlayerID)
					, SerializedSizeOf(InNotificationID)
					, SerializedSizeOf(InMessageID)
					, SerializedSizeOf(InMessageParam0)
					, SerializedSizeOf(InMessageParam1)
					, SerializedSizeOf(InMessageText)
					, SerializedSizeOf(InTimeStamp)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::NotifyC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InDestPlayerID));
				protocolCheck(output->Write(InNotificationID));
				protocolCheck(output->Write(InMessageID));
				protocolCheck(output->Write(InMessageParam0));
				protocolCheck(output->Write(InMessageParam1));
				protocolCheck(output->Write(InMessageText));
				protocolCheck(output->Write(InTimeStamp));

				return hr;
			}; // MessageData* NotifyC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )

			Result NotifyC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result NotifyC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result NotifyC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				NotifyC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Notify:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, NotificationID:{4}, MessageID:{5}, MessageParam0:{6}, MessageParam1:{7}, MessageText:{8,60}, TimeStamp:{9}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetNotificationID(), parser.GetMessageID(), parser.GetMessageParam0(), parser.GetMessageParam1(), parser.GetMessageText(), parser.GetTimeStamp()); 
				return ResultCode::SUCCESS;
			}; // Result NotifyC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Friend Accepted
			const MessageID FriendAcceptedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 4);
			Result FriendAcceptedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_DestPlayerID));
				protocolCheck(input->Read(m_Accepter));

				return hr;

			}; // Result FriendAcceptedC2SEvt::ParseMessage( MessageData* pIMsg )


			Result FriendAcceptedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) FriendAcceptedC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result FriendAcceptedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* FriendAcceptedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InDestPlayerID)
					, SerializedSizeOf(InAccepter)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::FriendAcceptedC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InDestPlayerID));
				protocolCheck(output->Write(InAccepter));

				return hr;
			}; // MessageData* FriendAcceptedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )

			Result FriendAcceptedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result FriendAcceptedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result FriendAcceptedC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				FriendAcceptedC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "FriendAccepted:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, Accepter:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetAccepter()); 
				return ResultCode::SUCCESS;
			}; // Result FriendAcceptedC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Friend Remove
			const MessageID FriendRemovedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 5);
			Result FriendRemovedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_DestPlayerID));
				protocolCheck(input->Read(m_RemoverID));

				return hr;

			}; // Result FriendRemovedC2SEvt::ParseMessage( MessageData* pIMsg )


			Result FriendRemovedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) FriendRemovedC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result FriendRemovedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* FriendRemovedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InDestPlayerID)
					, SerializedSizeOf(InRemoverID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::FriendRemovedC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InDestPlayerID));
				protocolCheck(output->Write(InRemoverID));

				return hr;
			}; // MessageData* FriendRemovedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )

			Result FriendRemovedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result FriendRemovedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result FriendRemovedC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				FriendRemovedC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "FriendRemoved:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, RemoverID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetRemoverID()); 
				return ResultCode::SUCCESS;
			}; // Result FriendRemovedC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Request Player Status Update
			const MessageID RequestPlayerStatusUpdateC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 6);
			Result RequestPlayerStatusUpdateC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_DestPlayerID));
				protocolCheck(input->Read(m_RequesterID));

				return hr;

			}; // Result RequestPlayerStatusUpdateC2SEvt::ParseMessage( MessageData* pIMsg )


			Result RequestPlayerStatusUpdateC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RequestPlayerStatusUpdateC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RequestPlayerStatusUpdateC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestPlayerStatusUpdateC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InDestPlayerID)
					, SerializedSizeOf(InRequesterID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::RequestPlayerStatusUpdateC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InDestPlayerID));
				protocolCheck(output->Write(InRequesterID));

				return hr;
			}; // MessageData* RequestPlayerStatusUpdateC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )

			Result RequestPlayerStatusUpdateC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result RequestPlayerStatusUpdateC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result RequestPlayerStatusUpdateC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RequestPlayerStatusUpdateC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RequestPlayerStatusUpdate:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, RequesterID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetRequesterID()); 
				return ResultCode::SUCCESS;
			}; // Result RequestPlayerStatusUpdateC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Notify Player Status Updated
			const MessageID NotifyPlayerStatusUpdatedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 7);
			Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_DestPlayerID));
				protocolCheck(input->Read(m_LatestActiveTime));
				protocolCheck(input->Read(m_IsInGame));

				return hr;

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessage( MessageData* pIMsg )


			Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) NotifyPlayerStatusUpdatedC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* NotifyPlayerStatusUpdatedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InDestPlayerID)
					, SerializedSizeOf(InLatestActiveTime)
					, SerializedSizeOf(InIsInGame)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InDestPlayerID));
				protocolCheck(output->Write(InLatestActiveTime));
				protocolCheck(output->Write(InIsInGame));

				return hr;
			}; // MessageData* NotifyPlayerStatusUpdatedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )

			Result NotifyPlayerStatusUpdatedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result NotifyPlayerStatusUpdatedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result NotifyPlayerStatusUpdatedC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				NotifyPlayerStatusUpdatedC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "NotifyPlayerStatusUpdated:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, LatestActiveTime:{4}, IsInGame:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetLatestActiveTime(), parser.GetIsInGame()); 
				return ResultCode::SUCCESS;
			}; // Result NotifyPlayerStatusUpdatedC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Party invite Notification
			const MessageID NotifyPartyInviteC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMESERVER, 8);
			Result NotifyPartyInviteC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_DestPlayerID));
				protocolCheck(input->Read(m_InviterID));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_InviterName, ArrayLen));
				protocolCheck(input->Read(m_PartyUID));

				return hr;

			}; // Result NotifyPartyInviteC2SEvt::ParseMessage( MessageData* pIMsg )


			Result NotifyPartyInviteC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) NotifyPartyInviteC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result NotifyPartyInviteC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* NotifyPartyInviteC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InDestPlayerID)
					, SerializedSizeOf(InInviterID)
					, SerializedSizeOf(InInviterName)
					, SerializedSizeOf(InPartyUID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameServer::NotifyPartyInviteC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InDestPlayerID));
				protocolCheck(output->Write(InInviterID));
				protocolCheck(output->Write(InInviterName));
				protocolCheck(output->Write(InPartyUID));

				return hr;
			}; // MessageData* NotifyPartyInviteC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )

			Result NotifyPartyInviteC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result NotifyPartyInviteC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result NotifyPartyInviteC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				NotifyPartyInviteC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "NotifyPartyInvite:{0}:{1} , RouteContext:{2}, DestPlayerID:{3}, InviterID:{4}, InviterName:{5,60}, PartyUID:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetDestPlayerID(), parser.GetInviterID(), parser.GetInviterName(), parser.GetPartyUID()); 
				return ResultCode::SUCCESS;
			}; // Result NotifyPartyInviteC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace GameServer
	}; // namespace Message
}; // namespace SF


