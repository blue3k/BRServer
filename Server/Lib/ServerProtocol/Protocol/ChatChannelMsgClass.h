﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : ChatChannel Message parser definitions
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
 		namespace ChatChannel
		{
 			// Cmd: Request to join chat channel
			class JoinCmd : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				const char* m_Passcode{};
				PlayerInformation m_JoiningPlayer{};
			public:
				JoinCmd()
					{}

				JoinCmd( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const char* GetPasscode() const	{ return m_Passcode; };
				const PlayerInformation& GetJoiningPlayer() const	{ return m_JoiningPlayer; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer );

			}; // class JoinCmd : public MessageBase

			class JoinRes : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				Result m_Result{};
				PlayerID m_ChatChannelLeaderID{};
			public:
				JoinRes()
					{}

				JoinRes( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const PlayerID& GetChatChannelLeaderID() const	{ return m_ChatChannelLeaderID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID );

			}; // class JoinRes : public MessageBase

			// S2C: Server envent to notify joined player information
			class PlayerJoinedS2CEvt : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint64_t GetTransactionID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				PlayerInformation m_JoinedPlayer{};
			public:
				PlayerJoinedS2CEvt()
					{}

				PlayerJoinedS2CEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerInformation& GetJoinedPlayer() const	{ return m_JoinedPlayer; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer );

			}; // class PlayerJoinedS2CEvt : public MessageBase

			// S2C: Chat channel leader changed
			class LeaderChangedS2CEvt : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint64_t GetTransactionID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				PlayerID m_NewLeaderID{};
			public:
				LeaderChangedS2CEvt()
					{}

				LeaderChangedS2CEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetNewLeaderID() const	{ return m_NewLeaderID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InNewLeaderID );

			}; // class LeaderChangedS2CEvt : public MessageBase

			// Cmd: Leave chat channel
			class LeaveCmd : public MessageBase
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
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				PlayerID m_PlayerID{};
			public:
				LeaveCmd()
					{}

				LeaveCmd( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );

			}; // class LeaveCmd : public MessageBase

			class LeaveRes : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				Result m_Result{};
			public:
				LeaveRes()
					{}

				LeaveRes( const MessageDataPtr &pMsg )
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

			}; // class LeaveRes : public MessageBase

			// S2C: Notification event when a player left
			class PlayerLeftS2CEvt : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint64_t GetTransactionID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				PlayerID m_LeftPlayerID{};
			public:
				PlayerLeftS2CEvt()
					{}

				PlayerLeftS2CEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetLeftPlayerID() const	{ return m_LeftPlayerID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );

			}; // class PlayerLeftS2CEvt : public MessageBase

			// Cmd: Kick a player
			class KickPlayerCmd : public MessageBase
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
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				PlayerID m_PlayerID{};
				PlayerID m_PlayerToKick{};
			public:
				KickPlayerCmd()
					{}

				KickPlayerCmd( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const PlayerID& GetPlayerToKick() const	{ return m_PlayerToKick; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );

			}; // class KickPlayerCmd : public MessageBase

			class KickPlayerRes : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				Result m_Result{};
			public:
				KickPlayerRes()
					{}

				KickPlayerRes( const MessageDataPtr &pMsg )
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

			}; // class KickPlayerRes : public MessageBase

			// S2C: Notification event when a player kicked
			class PlayerKickedS2CEvt : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint64_t GetTransactionID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				PlayerID m_KickedPlayerID{};
			public:
				PlayerKickedS2CEvt()
					{}

				PlayerKickedS2CEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetKickedPlayerID() const	{ return m_KickedPlayerID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );

			}; // class PlayerKickedS2CEvt : public MessageBase

			// C2S: Sending a chatting message
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint64_t GetTransactionID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				PlayerID m_SenderID{};
				const char* m_ChatMessage{};
			public:
				ChatMessageC2SEvt()
					{}

				ChatMessageC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetSenderID() const	{ return m_SenderID; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage );

			}; // class ChatMessageC2SEvt : public MessageBase

			// S2C: brocasting event for a chatting message
			class ChatMessageS2CEvt : public MessageBase
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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint64_t GetTransactionID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				PlayerID m_SenderID{};
				const char* m_SenderName{};
				const char* m_ChatMessage{};
			public:
				ChatMessageS2CEvt()
					{}

				ChatMessageS2CEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetSenderID() const	{ return m_SenderID; };
				const char* GetSenderName() const	{ return m_SenderName; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );

			}; // class ChatMessageS2CEvt : public MessageBase





		}; // namespace ChatChannel
	}; // namespace Message
}; // namespace SF


