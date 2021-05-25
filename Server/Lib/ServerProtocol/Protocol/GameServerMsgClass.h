////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : GameServer Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/SFProtocol.h"
#include "Net/SFMessage.h"
#include "Types/SFEngineTypedefs.h"
#include "Variable/SFVariableTable.h"
#include "Container/SFArray.h"
#include "Actor/Movement/SFActorMovement.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameServer
		{
 			// Cmd: Register player so that the player can connection and join to the game server.
			class RegisterPlayerToJoinGameServerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				PlayerID m_PlayerID{};
				AuthTicket m_Ticket{};
				FacebookUID m_FBUserID{};
				uint32_t m_ShardID{};
			public:
				RegisterPlayerToJoinGameServerCmd()
					{}

				RegisterPlayerToJoinGameServerCmd( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const FacebookUID& GetFBUserID() const	{ return m_FBUserID; };
				const uint32_t& GetShardID() const	{ return m_ShardID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID );

			}; // class RegisterPlayerToJoinGameServerCmd : public MessageBase

			class RegisterPlayerToJoinGameServerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				Result m_Result{};
				const char* m_PublicAddress{};
				const char* m_PublicAddressV6{};
				uint32_t m_Port{};
			public:
				RegisterPlayerToJoinGameServerRes()
					{}

				RegisterPlayerToJoinGameServerRes( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const char* GetPublicAddress() const	{ return m_PublicAddress; };
				const char* GetPublicAddressV6() const	{ return m_PublicAddressV6; };
				const uint32_t& GetPort() const	{ return m_Port; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort );

			}; // class RegisterPlayerToJoinGameServerRes : public MessageBase

			// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
			class RegisterPlayerToJoinGameServerOnPlayerEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				PlayerID m_PlayerID{};
				AuthTicket m_Ticket{};
				FacebookUID m_FBUserID{};
			public:
				RegisterPlayerToJoinGameServerOnPlayerEntityCmd()
					{}

				RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const FacebookUID& GetFBUserID() const	{ return m_FBUserID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID );

			}; // class RegisterPlayerToJoinGameServerOnPlayerEntityCmd : public MessageBase

			class RegisterPlayerToJoinGameServerOnPlayerEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				Result m_Result{};
			public:
				RegisterPlayerToJoinGameServerOnPlayerEntityRes()
					{}

				RegisterPlayerToJoinGameServerOnPlayerEntityRes( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

			}; // class RegisterPlayerToJoinGameServerOnPlayerEntityRes : public MessageBase

			// C2S: Chatting message event.
			class ChatMessageC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				AccountID m_SenderID{};
				uint8_t m_Role{};
				const char* m_SenderName{};
				const char* m_ChatMessage{};
			public:
				ChatMessageC2SEvt()
					{}

				ChatMessageC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetSenderID() const	{ return m_SenderID; };
				const uint8_t& GetRole() const	{ return m_Role; };
				const char* GetSenderName() const	{ return m_SenderName; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage );

			}; // class ChatMessageC2SEvt : public MessageBase

			// C2S: Notification event for P2P
			class NotifyC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				PlayerID m_DestPlayerID{};
				uint32_t m_NotificationID{};
				NotificationType m_MessageID{};
				uint64_t m_MessageParam0{};
				uint64_t m_MessageParam1{};
				const char* m_MessageText{};
				uint64_t m_TimeStamp{};
			public:
				NotifyC2SEvt()
					{}

				NotifyC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const uint32_t& GetNotificationID() const	{ return m_NotificationID; };
				const NotificationType& GetMessageID() const	{ return m_MessageID; };
				const uint64_t& GetMessageParam0() const	{ return m_MessageParam0; };
				const uint64_t& GetMessageParam1() const	{ return m_MessageParam1; };
				const char* GetMessageText() const	{ return m_MessageText; };
				const uint64_t& GetTimeStamp() const	{ return m_TimeStamp; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp );

			}; // class NotifyC2SEvt : public MessageBase

			// C2S: Friend Accepted
			class FriendAcceptedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				AccountID m_DestPlayerID{};
				ServerFriendInformation m_Accepter{};
			public:
				FriendAcceptedC2SEvt()
					{}

				FriendAcceptedC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const ServerFriendInformation& GetAccepter() const	{ return m_Accepter; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter );

			}; // class FriendAcceptedC2SEvt : public MessageBase

			// C2S: Friend Remove
			class FriendRemovedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				AccountID m_DestPlayerID{};
				PlayerID m_RemoverID{};
			public:
				FriendRemovedC2SEvt()
					{}

				FriendRemovedC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const PlayerID& GetRemoverID() const	{ return m_RemoverID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID );

			}; // class FriendRemovedC2SEvt : public MessageBase

			// C2S: Request Player Status Update
			class RequestPlayerStatusUpdateC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				AccountID m_DestPlayerID{};
				PlayerID m_RequesterID{};
			public:
				RequestPlayerStatusUpdateC2SEvt()
					{}

				RequestPlayerStatusUpdateC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const PlayerID& GetRequesterID() const	{ return m_RequesterID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID );

			}; // class RequestPlayerStatusUpdateC2SEvt : public MessageBase

			// C2S: Notify Player Status Updated
			class NotifyPlayerStatusUpdatedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				AccountID m_DestPlayerID{};
				uint32_t m_LatestActiveTime{};
				uint8_t m_IsInGame{};
			public:
				NotifyPlayerStatusUpdatedC2SEvt()
					{}

				NotifyPlayerStatusUpdatedC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const uint32_t& GetLatestActiveTime() const	{ return m_LatestActiveTime; };
				const uint8_t& GetIsInGame() const	{ return m_IsInGame; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame );

			}; // class NotifyPlayerStatusUpdatedC2SEvt : public MessageBase

			// C2S: Party invite Notification
			class NotifyPartyInviteC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				PlayerID m_DestPlayerID{};
				PlayerID m_InviterID{};
				const char* m_InviterName{};
				uint64_t m_PartyUID{};
			public:
				NotifyPartyInviteC2SEvt()
					{}

				NotifyPartyInviteC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const PlayerID& GetInviterID() const	{ return m_InviterID; };
				const char* GetInviterName() const	{ return m_InviterName; };
				const uint64_t& GetPartyUID() const	{ return m_PartyUID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID );

			}; // class NotifyPartyInviteC2SEvt : public MessageBase





		}; // namespace GameServer
	}; // namespace Message
}; // namespace SF


