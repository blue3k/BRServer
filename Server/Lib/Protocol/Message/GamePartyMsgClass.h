////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameParty Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/PolicyID.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"



namespace BR
{
 	namespace Message
	{
 		namespace GameParty
		{
 			// Cmd: Join Party
			class JoinPartyCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_InviterID;
				PlayerInformation m_InvitedPlayer;
			public:
				JoinPartyCmd()
					{}

				JoinPartyCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetInviterID() const	{ return m_InviterID; };
				const PlayerInformation& GetInvitedPlayer() const	{ return m_InvitedPlayer; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class JoinPartyCmd : public MessageBase

			class JoinPartyRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				PlayerID m_PartyLeaderID;
				LinkedArray<BYTE> m_ChatHistoryData;
			public:
				JoinPartyRes()
					{}

				JoinPartyRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPartyLeaderID() const	{ return m_PartyLeaderID; };
				const Array<BYTE>& GetChatHistoryData() const	{ return m_ChatHistoryData; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class JoinPartyRes : public MessageBase

			// S2C: Player Joined
			class PlayerJoinedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerInformation m_JoinedPlayer;
			public:
				PlayerJoinedS2CEvt()
					{}

				PlayerJoinedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerInformation& GetJoinedPlayer() const	{ return m_JoinedPlayer; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerJoinedS2CEvt : public MessageBase

			// S2C: Player Joined
			class PartyLeaderChangedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_NewLeaderID;
			public:
				PartyLeaderChangedS2CEvt()
					{}

				PartyLeaderChangedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetNewLeaderID() const	{ return m_NewLeaderID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InNewLeaderID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PartyLeaderChangedS2CEvt : public MessageBase

			// Cmd: Kick player
			class LeavePartyCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				LeavePartyCmd()
					{}

				LeavePartyCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class LeavePartyCmd : public MessageBase

			class LeavePartyRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				LeavePartyRes()
					{}

				LeavePartyRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class LeavePartyRes : public MessageBase

			// S2C: Player left
			class PlayerLeftS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_LeftPlayerID;
			public:
				PlayerLeftS2CEvt()
					{}

				PlayerLeftS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetLeftPlayerID() const	{ return m_LeftPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerLeftS2CEvt : public MessageBase

			// Cmd: Kick player
			class KickPlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				PlayerID m_PlayerToKick;
			public:
				KickPlayerCmd()
					{}

				KickPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const PlayerID& GetPlayerToKick() const	{ return m_PlayerToKick; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class KickPlayerCmd : public MessageBase

			class KickPlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				KickPlayerRes()
					{}

				KickPlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class KickPlayerRes : public MessageBase

			// S2C: Player kicked
			class PlayerKickedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_KickedPlayerID;
			public:
				PlayerKickedS2CEvt()
					{}

				PlayerKickedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetKickedPlayerID() const	{ return m_KickedPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerKickedS2CEvt : public MessageBase

			// C2S: Chatting message
			class ChatMessageC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				const char* m_ChatMessage;
			public:
				ChatMessageC2SEvt()
				:m_ChatMessage(nullptr)
					{}

				ChatMessageC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_ChatMessage(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class ChatMessageC2SEvt : public MessageBase

			// S2C: Chatting message
			class ChatMessageS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_SenderID;
				const char* m_SenderName;
				const char* m_ChatMessage;
			public:
				ChatMessageS2CEvt()
				:m_SenderName(nullptr)
				,m_ChatMessage(nullptr)
					{}

				ChatMessageS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_SenderName(nullptr)
				,m_ChatMessage(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetSenderID() const	{ return m_SenderID; };
				const char* GetSenderName() const	{ return m_SenderName; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class ChatMessageS2CEvt : public MessageBase

			// C2S: Quick Chatting message
			class QuickChatMessageC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				UINT32 m_QuickChatID;
			public:
				QuickChatMessageC2SEvt()
					{}

				QuickChatMessageC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const UINT32& GetQuickChatID() const	{ return m_QuickChatID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InQuickChatID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class QuickChatMessageC2SEvt : public MessageBase

			// S2C: Quick Chatting message
			class QuickChatMessageS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_SenderID;
				UINT32 m_QuickChatID;
			public:
				QuickChatMessageS2CEvt()
					{}

				QuickChatMessageS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetSenderID() const	{ return m_SenderID; };
				const UINT32& GetQuickChatID() const	{ return m_QuickChatID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InSenderID, const UINT32 &InQuickChatID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class QuickChatMessageS2CEvt : public MessageBase

			// Cmd: Match
			class StartGameMatchCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				UINT32 m_MaxGamePlayers;
			public:
				StartGameMatchCmd()
					{}

				StartGameMatchCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const UINT32& GetMaxGamePlayers() const	{ return m_MaxGamePlayers; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class StartGameMatchCmd : public MessageBase

			class StartGameMatchRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				StartGameMatchRes()
					{}

				StartGameMatchRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class StartGameMatchRes : public MessageBase

			// S2C: Enqueued at a game matching
			class QueuedGameMatchingS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				MatchingQueueTicket m_MatchingQueueTicket;
			public:
				QueuedGameMatchingS2CEvt()
					{}

				QueuedGameMatchingS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const MatchingQueueTicket& GetMatchingQueueTicket() const	{ return m_MatchingQueueTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class QueuedGameMatchingS2CEvt : public MessageBase

			// Cmd: Match
			class CancelGameMatchCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				CancelGameMatchCmd()
					{}

				CancelGameMatchCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class CancelGameMatchCmd : public MessageBase

			class CancelGameMatchRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				CancelGameMatchRes()
					{}

				CancelGameMatchRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class CancelGameMatchRes : public MessageBase

			// S2C: Canceled at a game matching
			class CanceledGameMatchingS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				MatchingQueueTicket m_MatchingQueueTicket;
			public:
				CanceledGameMatchingS2CEvt()
					{}

				CanceledGameMatchingS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const MatchingQueueTicket& GetMatchingQueueTicket() const	{ return m_MatchingQueueTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class CanceledGameMatchingS2CEvt : public MessageBase

			// S2C: Enqueued item is dequeued
			class MatchingItemDequeuedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				MatchingQueueTicket m_MatchingTicket;
			public:
				MatchingItemDequeuedS2CEvt()
					{}

				MatchingItemDequeuedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class MatchingItemDequeuedS2CEvt : public MessageBase





		}; // namespace GameParty
	}; // namespace Message
}; // namespace BR


