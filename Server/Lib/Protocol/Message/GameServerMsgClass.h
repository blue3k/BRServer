////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameServer Message parser definitions
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
 		namespace GameServer
		{
 			// Cmd: Kick
			class RegisterPlayerToJoinGameServerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				AuthTicket m_Ticket;
				FacebookUID m_FBUserID;
				UINT32 m_ShardID;
			public:
				RegisterPlayerToJoinGameServerCmd()
					{}

				RegisterPlayerToJoinGameServerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const FacebookUID& GetFBUserID() const	{ return m_FBUserID; };
				const UINT32& GetShardID() const	{ return m_ShardID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const UINT32 &InShardID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class RegisterPlayerToJoinGameServerCmd : public MessageBase

			class RegisterPlayerToJoinGameServerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				NetAddress m_PublicAddress;
			public:
				RegisterPlayerToJoinGameServerRes()
					{}

				RegisterPlayerToJoinGameServerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const NetAddress& GetPublicAddress() const	{ return m_PublicAddress; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InPublicAddress );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class RegisterPlayerToJoinGameServerRes : public MessageBase

			// Cmd: Kick
			class RegisterPlayerToJoinGameServerOnPlayerEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				AuthTicket m_Ticket;
				FacebookUID m_FBUserID;
			public:
				RegisterPlayerToJoinGameServerOnPlayerEntityCmd()
					{}

				RegisterPlayerToJoinGameServerOnPlayerEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const FacebookUID& GetFBUserID() const	{ return m_FBUserID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class RegisterPlayerToJoinGameServerOnPlayerEntityCmd : public MessageBase

			class RegisterPlayerToJoinGameServerOnPlayerEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				RegisterPlayerToJoinGameServerOnPlayerEntityRes()
					{}

				RegisterPlayerToJoinGameServerOnPlayerEntityRes( MessageData* &pMsg )
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

			}; // class RegisterPlayerToJoinGameServerOnPlayerEntityRes : public MessageBase

			// C2S: Chatting message
			class ChatMessageC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				AccountID m_SenderID;
				PlayerRole m_Role;
				const char* m_SenderName;
				const char* m_ChatMessage;
			public:
				ChatMessageC2SEvt()
				:m_SenderName(nullptr)
				,m_ChatMessage(nullptr)
					{}

				ChatMessageC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_SenderName(nullptr)
				,m_ChatMessage(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetSenderID() const	{ return m_SenderID; };
				const PlayerRole& GetRole() const	{ return m_Role; };
				const char* GetSenderName() const	{ return m_SenderName; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class ChatMessageC2SEvt : public MessageBase

			// C2S: Notification
			class NotifyC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				PlayerID m_DestPlayerID;
				UINT32 m_NotificationID;
				NotificationType m_MessageID;
				UINT64 m_MessageParam0;
				UINT64 m_MessageParam1;
				const char* m_MessageText;
				UINT64 m_TimeStamp;
			public:
				NotifyC2SEvt()
				:m_MessageText(nullptr)
					{}

				NotifyC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_MessageText(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const UINT32& GetNotificationID() const	{ return m_NotificationID; };
				const NotificationType& GetMessageID() const	{ return m_MessageID; };
				const UINT64& GetMessageParam0() const	{ return m_MessageParam0; };
				const UINT64& GetMessageParam1() const	{ return m_MessageParam1; };
				const char* GetMessageText() const	{ return m_MessageText; };
				const UINT64& GetTimeStamp() const	{ return m_TimeStamp; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const UINT64 &InTimeStamp );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class NotifyC2SEvt : public MessageBase

			// C2S: Friend Accept
			class FriendAcceptedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				AccountID m_DestPlayerID;
				ServerFriendInformation m_Accepter;
			public:
				FriendAcceptedC2SEvt()
					{}

				FriendAcceptedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const ServerFriendInformation& GetAccepter() const	{ return m_Accepter; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter );

				HRESULT OverrideRouteContextDestination( EntityUID to );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				AccountID m_DestPlayerID;
				PlayerID m_RemoverID;
			public:
				FriendRemovedC2SEvt()
					{}

				FriendRemovedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const PlayerID& GetRemoverID() const	{ return m_RemoverID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				AccountID m_DestPlayerID;
				PlayerID m_RequesterID;
			public:
				RequestPlayerStatusUpdateC2SEvt()
					{}

				RequestPlayerStatusUpdateC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const PlayerID& GetRequesterID() const	{ return m_RequesterID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				AccountID m_DestPlayerID;
				UINT32 m_LatestActiveTime;
				BYTE m_IsInGame;
			public:
				NotifyPlayerStatusUpdatedC2SEvt()
					{}

				NotifyPlayerStatusUpdatedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const AccountID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const UINT32& GetLatestActiveTime() const	{ return m_LatestActiveTime; };
				const BYTE& GetIsInGame() const	{ return m_IsInGame; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class NotifyPlayerStatusUpdatedC2SEvt : public MessageBase

			// C2S: Notification
			class NotifyPartyInviteC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				PlayerID m_DestPlayerID;
				PlayerID m_InviterID;
				const char* m_InviterName;
				EntityUID m_PartyUID;
			public:
				NotifyPartyInviteC2SEvt()
				:m_InviterName(nullptr)
					{}

				NotifyPartyInviteC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_InviterName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const PlayerID& GetInviterID() const	{ return m_InviterID; };
				const char* GetInviterName() const	{ return m_InviterName; };
				const EntityUID& GetPartyUID() const	{ return m_PartyUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class NotifyPartyInviteC2SEvt : public MessageBase





		}; // namespace GameServer
	}; // namespace Message
}; // namespace BR


