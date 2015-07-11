////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Game Message parser definitions
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
 		namespace Game
		{
 			// C2S: Client heart bit
			class HeartBitC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				HeartBitC2SEvt()
					{}

				HeartBitC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class HeartBitC2SEvt : public MessageBase

			// Cmd: Player connected from a login server and moved to game server
			class JoinGameServerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				AccountID m_AccID;
				AuthTicket m_Ticket;
				UINT64 m_LoginEntityUID;
			public:
				JoinGameServerCmd()
					{}

				JoinGameServerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const AccountID& GetAccID() const	{ return m_AccID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const UINT64& GetLoginEntityUID() const	{ return m_LoginEntityUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );

			}; // class JoinGameServerCmd : public MessageBase

			class JoinGameServerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				const char* m_NickName;
				GameInsUID m_GameUID;
				PartyUID m_PartyUID;
				PlayerID m_PartyLeaderID;
				MatchingQueueTicket m_MatchingTicket;
			public:
				JoinGameServerRes()
				:m_NickName(nullptr)
					{}

				JoinGameServerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_NickName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const char* GetNickName() const	{ return m_NickName; };
				const GameInsUID& GetGameUID() const	{ return m_GameUID; };
				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetPartyLeaderID() const	{ return m_PartyLeaderID; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket );

			}; // class JoinGameServerRes : public MessageBase

			// Cmd: player complition statues
			class GetComplitionStateCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				GetComplitionStateCmd()
					{}

				GetComplitionStateCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class GetComplitionStateCmd : public MessageBase

			class GetComplitionStateRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				const char* m_ComplitionState;
			public:
				GetComplitionStateRes()
				:m_ComplitionState(nullptr)
					{}

				GetComplitionStateRes( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_ComplitionState(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const char* GetComplitionState() const	{ return m_ComplitionState; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const char* InComplitionState );

			}; // class GetComplitionStateRes : public MessageBase

			// Cmd: Player complition state
			class SetComplitionStateCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				const char* m_ComplitionState;
			public:
				SetComplitionStateCmd()
				:m_ComplitionState(nullptr)
					{}

				SetComplitionStateCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_ComplitionState(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetComplitionState() const	{ return m_ComplitionState; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const char* InComplitionState );

			}; // class SetComplitionStateCmd : public MessageBase

			class SetComplitionStateRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				SetComplitionStateRes()
					{}

				SetComplitionStateRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class SetComplitionStateRes : public MessageBase

			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			class RegisterGCMCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				const char* m_GCMRegisteredID;
			public:
				RegisterGCMCmd()
				:m_GCMRegisteredID(nullptr)
					{}

				RegisterGCMCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_GCMRegisteredID(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetGCMRegisteredID() const	{ return m_GCMRegisteredID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID );

			}; // class RegisterGCMCmd : public MessageBase

			class RegisterGCMRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				RegisterGCMRes()
					{}

				RegisterGCMRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class RegisterGCMRes : public MessageBase

			// Cmd: Unregister Google notification service ID
			class UnregisterGCMCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				const char* m_GCMRegisteredID;
			public:
				UnregisterGCMCmd()
				:m_GCMRegisteredID(nullptr)
					{}

				UnregisterGCMCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_GCMRegisteredID(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetGCMRegisteredID() const	{ return m_GCMRegisteredID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID );

			}; // class UnregisterGCMCmd : public MessageBase

			class UnregisterGCMRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				UnregisterGCMRes()
					{}

				UnregisterGCMRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class UnregisterGCMRes : public MessageBase

			// Cmd: Invite friend
			class InviteFriendCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_FriendID;
			public:
				InviteFriendCmd()
					{}

				InviteFriendCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetFriendID() const	{ return m_FriendID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID );

			}; // class InviteFriendCmd : public MessageBase

			class InviteFriendRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				InviteFriendRes()
					{}

				InviteFriendRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class InviteFriendRes : public MessageBase

			// Cmd: Accept friend request
			class AcceptFriendRequestCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_InviterID;
				FacebookUID m_InviterFacebookUID;
			public:
				AcceptFriendRequestCmd()
					{}

				AcceptFriendRequestCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetInviterID() const	{ return m_InviterID; };
				const FacebookUID& GetInviterFacebookUID() const	{ return m_InviterFacebookUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID );

			}; // class AcceptFriendRequestCmd : public MessageBase

			class AcceptFriendRequestRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				FriendInformation m_NewFriend;
			public:
				AcceptFriendRequestRes()
					{}

				AcceptFriendRequestRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const FriendInformation& GetNewFriend() const	{ return m_NewFriend; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const FriendInformation &InNewFriend );

			}; // class AcceptFriendRequestRes : public MessageBase

			// S2C: Notification for friend request is accepted
			class FriendRequestAcceptedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				FriendInformation m_Accepter;
			public:
				FriendRequestAcceptedS2CEvt()
					{}

				FriendRequestAcceptedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const FriendInformation& GetAccepter() const	{ return m_Accepter; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const FriendInformation &InAccepter );

			}; // class FriendRequestAcceptedS2CEvt : public MessageBase

			// Cmd: Remove friden form the friend list
			class RemoveFriendCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_FriendID;
			public:
				RemoveFriendCmd()
					{}

				RemoveFriendCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetFriendID() const	{ return m_FriendID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID );

			}; // class RemoveFriendCmd : public MessageBase

			class RemoveFriendRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				PlayerID m_FriendID;
			public:
				RemoveFriendRes()
					{}

				RemoveFriendRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const PlayerID& GetFriendID() const	{ return m_FriendID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InFriendID );

			}; // class RemoveFriendRes : public MessageBase

			// S2C: Friend removed
			class FriendRemovedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_FriendID;
			public:
				FriendRemovedS2CEvt()
					{}

				FriendRemovedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetFriendID() const	{ return m_FriendID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID );

			}; // class FriendRemovedS2CEvt : public MessageBase

			// Cmd: Get friend list
			class GetFriendListCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT16 m_StartIndex;
				UINT16 m_Count;
			public:
				GetFriendListCmd()
					{}

				GetFriendListCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT16& GetStartIndex() const	{ return m_StartIndex; };
				const UINT16& GetCount() const	{ return m_Count; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT16 &InStartIndex, const UINT16 &InCount );

			}; // class GetFriendListCmd : public MessageBase

			class GetFriendListRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT16 m_MaxFriendSlot;
				UINT16 m_TotalNumberOfFriends;
				UINT16 m_StartIndex;
				LinkedArray<FriendInformation> m_FriendList;
			public:
				GetFriendListRes()
					{}

				GetFriendListRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT16& GetMaxFriendSlot() const	{ return m_MaxFriendSlot; };
				const UINT16& GetTotalNumberOfFriends() const	{ return m_TotalNumberOfFriends; };
				const UINT16& GetStartIndex() const	{ return m_StartIndex; };
				const Array<FriendInformation>& GetFriendList() const	{ return m_FriendList; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList );

			}; // class GetFriendListRes : public MessageBase

			// Cmd: Query notification list
			class GetNotificationListCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				GetNotificationListCmd()
					{}

				GetNotificationListCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class GetNotificationListCmd : public MessageBase

			class GetNotificationListRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				GetNotificationListRes()
					{}

				GetNotificationListRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class GetNotificationListRes : public MessageBase

			// Cmd: Delete notification
			class DeleteNotificationCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT32 m_NotificationID;
			public:
				DeleteNotificationCmd()
					{}

				DeleteNotificationCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT32& GetNotificationID() const	{ return m_NotificationID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID );

			}; // class DeleteNotificationCmd : public MessageBase

			class DeleteNotificationRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT32 m_NotificationID;
			public:
				DeleteNotificationRes()
					{}

				DeleteNotificationRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT32& GetNotificationID() const	{ return m_NotificationID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID );

			}; // class DeleteNotificationRes : public MessageBase

			// Cmd: Set notification is read
			class SetNotificationReadCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT32 m_NotificationID;
			public:
				SetNotificationReadCmd()
					{}

				SetNotificationReadCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT32& GetNotificationID() const	{ return m_NotificationID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID );

			}; // class SetNotificationReadCmd : public MessageBase

			class SetNotificationReadRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT32 m_NotificationID;
			public:
				SetNotificationReadRes()
					{}

				SetNotificationReadRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT32& GetNotificationID() const	{ return m_NotificationID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID );

			}; // class SetNotificationReadRes : public MessageBase

			// Cmd: Accept notification
			class AcceptNotificationCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT32 m_NotificationID;
			public:
				AcceptNotificationCmd()
					{}

				AcceptNotificationCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT32& GetNotificationID() const	{ return m_NotificationID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID );

			}; // class AcceptNotificationCmd : public MessageBase

			class AcceptNotificationRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT32 m_NotificationID;
			public:
				AcceptNotificationRes()
					{}

				AcceptNotificationRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT32& GetNotificationID() const	{ return m_NotificationID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID );

			}; // class AcceptNotificationRes : public MessageBase

			// S2C: Notify new notification
			class NotifyS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT32 m_NotificationID;
				NotificationType m_MessageID;
				UINT64 m_MessageParam0;
				UINT64 m_MessageParam1;
				const char* m_MessageText;
				BYTE m_IsRead;
				UINT64 m_TimeStamp;
			public:
				NotifyS2CEvt()
				:m_MessageText(nullptr)
					{}

				NotifyS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_MessageText(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT32& GetNotificationID() const	{ return m_NotificationID; };
				const NotificationType& GetMessageID() const	{ return m_MessageID; };
				const UINT64& GetMessageParam0() const	{ return m_MessageParam0; };
				const UINT64& GetMessageParam1() const	{ return m_MessageParam1; };
				const char* GetMessageText() const	{ return m_MessageText; };
				const BYTE& GetIsRead() const	{ return m_IsRead; };
				const UINT64& GetTimeStamp() const	{ return m_TimeStamp; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp );

			}; // class NotifyS2CEvt : public MessageBase

			// Cmd: Query playerID list
			class FindPlayerByEMailCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				const char* m_PlayerEMail;
			public:
				FindPlayerByEMailCmd()
				:m_PlayerEMail(nullptr)
					{}

				FindPlayerByEMailCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_PlayerEMail(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetPlayerEMail() const	{ return m_PlayerEMail; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const char* InPlayerEMail );

			}; // class FindPlayerByEMailCmd : public MessageBase

			class FindPlayerByEMailRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				PlayerInformation m_Player;
			public:
				FindPlayerByEMailRes()
					{}

				FindPlayerByEMailRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const PlayerInformation& GetPlayer() const	{ return m_Player; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerInformation &InPlayer );

			}; // class FindPlayerByEMailRes : public MessageBase

			// Cmd: *Request Player Status Update
			class RequestPlayerStatusUpdateCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				LinkedArray<PlayerID> m_TargetPlayerID;
			public:
				RequestPlayerStatusUpdateCmd()
					{}

				RequestPlayerStatusUpdateCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Array<PlayerID>& GetTargetPlayerID() const	{ return m_TargetPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID );

			}; // class RequestPlayerStatusUpdateCmd : public MessageBase

			class RequestPlayerStatusUpdateRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				RequestPlayerStatusUpdateRes()
					{}

				RequestPlayerStatusUpdateRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class RequestPlayerStatusUpdateRes : public MessageBase

			// S2C: *Notify Player Status Updated
			class NotifyPlayerStatusUpdatedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_PlayerID;
				UINT32 m_LatestActiveTime;
				BYTE m_IsInGame;
			public:
				NotifyPlayerStatusUpdatedS2CEvt()
					{}

				NotifyPlayerStatusUpdatedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const UINT32& GetLatestActiveTime() const	{ return m_LatestActiveTime; };
				const BYTE& GetIsInGame() const	{ return m_IsInGame; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame );

			}; // class NotifyPlayerStatusUpdatedS2CEvt : public MessageBase

			// Cmd: Get Ranking lise
			class GetRankingListCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RankingType m_RankingType;
				BYTE m_BaseRanking;
				BYTE m_Count;
			public:
				GetRankingListCmd()
					{}

				GetRankingListCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RankingType& GetRankingType() const	{ return m_RankingType; };
				const BYTE& GetBaseRanking() const	{ return m_BaseRanking; };
				const BYTE& GetCount() const	{ return m_Count; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount );

			}; // class GetRankingListCmd : public MessageBase

			class GetRankingListRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				LinkedArray<TotalRankingPlayerInformation> m_Ranking;
			public:
				GetRankingListRes()
					{}

				GetRankingListRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const Array<TotalRankingPlayerInformation>& GetRanking() const	{ return m_Ranking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const Array<TotalRankingPlayerInformation>& InRanking );

			}; // class GetRankingListRes : public MessageBase

			// Cmd: Game user game play information
			class GetUserGamePlayerInfoCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				GetUserGamePlayerInfoCmd()
					{}

				GetUserGamePlayerInfoCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class GetUserGamePlayerInfoCmd : public MessageBase

			class GetUserGamePlayerInfoRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				INT16 m_Level;
				INT64 m_Exp;
				INT64 m_GameMoney;
				INT64 m_Gem;
				INT16 m_Stamina;
				INT32 m_TotalPlayed;
				INT32 m_WinPlaySC;
				INT32 m_WinPlaySM;
				INT32 m_WinPlaySS;
				INT32 m_LosePlaySC;
				INT32 m_LosePlaySM;
				INT32 m_LosePlaySS;
				INT32 m_WinPlayNC;
				INT32 m_WinPlayNM;
				INT32 m_WinPlayNS;
				INT32 m_LosePlayNC;
				INT32 m_LosePlayNM;
				INT32 m_LosePlayNS;
				INT32 m_WeeklyWin;
				INT32 m_WeeklyLose;
			public:
				GetUserGamePlayerInfoRes()
					{}

				GetUserGamePlayerInfoRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const INT16& GetLevel() const	{ return m_Level; };
				const INT64& GetExp() const	{ return m_Exp; };
				const INT64& GetGameMoney() const	{ return m_GameMoney; };
				const INT64& GetGem() const	{ return m_Gem; };
				const INT16& GetStamina() const	{ return m_Stamina; };
				const INT32& GetTotalPlayed() const	{ return m_TotalPlayed; };
				const INT32& GetWinPlaySC() const	{ return m_WinPlaySC; };
				const INT32& GetWinPlaySM() const	{ return m_WinPlaySM; };
				const INT32& GetWinPlaySS() const	{ return m_WinPlaySS; };
				const INT32& GetLosePlaySC() const	{ return m_LosePlaySC; };
				const INT32& GetLosePlaySM() const	{ return m_LosePlaySM; };
				const INT32& GetLosePlaySS() const	{ return m_LosePlaySS; };
				const INT32& GetWinPlayNC() const	{ return m_WinPlayNC; };
				const INT32& GetWinPlayNM() const	{ return m_WinPlayNM; };
				const INT32& GetWinPlayNS() const	{ return m_WinPlayNS; };
				const INT32& GetLosePlayNC() const	{ return m_LosePlayNC; };
				const INT32& GetLosePlayNM() const	{ return m_LosePlayNM; };
				const INT32& GetLosePlayNS() const	{ return m_LosePlayNS; };
				const INT32& GetWeeklyWin() const	{ return m_WeeklyWin; };
				const INT32& GetWeeklyLose() const	{ return m_WeeklyLose; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose );

			}; // class GetUserGamePlayerInfoRes : public MessageBase

			// Cmd: Game game play information
			class GetGamePlayerInfoCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_PlayerID;
			public:
				GetGamePlayerInfoCmd()
					{}

				GetGamePlayerInfoCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID );

			}; // class GetGamePlayerInfoCmd : public MessageBase

			class GetGamePlayerInfoRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				PlayerID m_PlayerID;
				INT16 m_Level;
				INT32 m_TotalPlayed;
				INT32 m_WinPlaySC;
				INT32 m_WinPlaySM;
				INT32 m_WinPlaySS;
				INT32 m_LosePlaySC;
				INT32 m_LosePlaySM;
				INT32 m_LosePlaySS;
				INT32 m_WinPlayNC;
				INT32 m_WinPlayNM;
				INT32 m_WinPlayNS;
				INT32 m_LosePlayNC;
				INT32 m_LosePlayNM;
				INT32 m_LosePlayNS;
				INT32 m_WeeklyWin;
				INT32 m_WeeklyLose;
			public:
				GetGamePlayerInfoRes()
					{}

				GetGamePlayerInfoRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const INT16& GetLevel() const	{ return m_Level; };
				const INT32& GetTotalPlayed() const	{ return m_TotalPlayed; };
				const INT32& GetWinPlaySC() const	{ return m_WinPlaySC; };
				const INT32& GetWinPlaySM() const	{ return m_WinPlaySM; };
				const INT32& GetWinPlaySS() const	{ return m_WinPlaySS; };
				const INT32& GetLosePlaySC() const	{ return m_LosePlaySC; };
				const INT32& GetLosePlaySM() const	{ return m_LosePlaySM; };
				const INT32& GetLosePlaySS() const	{ return m_LosePlaySS; };
				const INT32& GetWinPlayNC() const	{ return m_WinPlayNC; };
				const INT32& GetWinPlayNM() const	{ return m_WinPlayNM; };
				const INT32& GetWinPlayNS() const	{ return m_WinPlayNS; };
				const INT32& GetLosePlayNC() const	{ return m_LosePlayNC; };
				const INT32& GetLosePlayNM() const	{ return m_LosePlayNM; };
				const INT32& GetLosePlayNS() const	{ return m_LosePlayNS; };
				const INT32& GetWeeklyWin() const	{ return m_WeeklyWin; };
				const INT32& GetWeeklyLose() const	{ return m_WeeklyLose; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose );

			}; // class GetGamePlayerInfoRes : public MessageBase

			// S2C: Player level up event
			class LevelUpS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT64 m_CurrentTotalExp;
				UINT32 m_CurrentLevel;
			public:
				LevelUpS2CEvt()
					{}

				LevelUpS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT64& GetCurrentTotalExp() const	{ return m_CurrentTotalExp; };
				const UINT32& GetCurrentLevel() const	{ return m_CurrentLevel; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel );

			}; // class LevelUpS2CEvt : public MessageBase

			// Cmd: Change NickName
			class SetNickNameCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				const char* m_NickName;
				BYTE m_IsCostFree;
			public:
				SetNickNameCmd()
				:m_NickName(nullptr)
					{}

				SetNickNameCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_NickName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetNickName() const	{ return m_NickName; };
				const BYTE& GetIsCostFree() const	{ return m_IsCostFree; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const char* InNickName, const BYTE &InIsCostFree );

			}; // class SetNickNameCmd : public MessageBase

			class SetNickNameRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT64 m_TotalGem;
				UINT64 m_TotalGameMoney;
			public:
				SetNickNameRes()
					{}

				SetNickNameRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT64& GetTotalGem() const	{ return m_TotalGem; };
				const UINT64& GetTotalGameMoney() const	{ return m_TotalGameMoney; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );

			}; // class SetNickNameRes : public MessageBase

			// Cmd: Create Party
			class CreatePartyCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				CreatePartyCmd()
					{}

				CreatePartyCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class CreatePartyCmd : public MessageBase

			class CreatePartyRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				PartyUID m_PartyUID;
			public:
				CreatePartyRes()
					{}

				CreatePartyRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PartyUID &InPartyUID );

			}; // class CreatePartyRes : public MessageBase

			// Cmd: Join party
			class JoinPartyCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PartyUID m_PartyUID;
				PlayerID m_InviterID;
			public:
				JoinPartyCmd()
					{}

				JoinPartyCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetInviterID() const	{ return m_InviterID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InInviterID );

			}; // class JoinPartyCmd : public MessageBase

			class JoinPartyRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				PartyUID m_PartyUID;
				PlayerID m_PartyLeaderID;
				LinkedArray<BYTE> m_ChatHistoryData;
			public:
				JoinPartyRes()
					{}

				JoinPartyRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetPartyLeaderID() const	{ return m_PartyLeaderID; };
				const Array<BYTE>& GetChatHistoryData() const	{ return m_ChatHistoryData; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData );

			}; // class JoinPartyRes : public MessageBase

			// S2C: Player Joined event
			class PartyPlayerJoinedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PartyUID m_PartyUID;
				PlayerInformation m_JoinedPlayer;
			public:
				PartyPlayerJoinedS2CEvt()
					{}

				PartyPlayerJoinedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerInformation& GetJoinedPlayer() const	{ return m_JoinedPlayer; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer );

			}; // class PartyPlayerJoinedS2CEvt : public MessageBase

			// S2C: Party leader changed event
			class PartyLeaderChangedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PartyUID m_PartyUID;
				PlayerID m_NewLeaderID;
			public:
				PartyLeaderChangedS2CEvt()
					{}

				PartyLeaderChangedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetNewLeaderID() const	{ return m_NewLeaderID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InNewLeaderID );

			}; // class PartyLeaderChangedS2CEvt : public MessageBase

			// Cmd: Leave party command
			class LeavePartyCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PartyUID m_PartyUID;
				PlayerID m_PlayerID;
			public:
				LeavePartyCmd()
					{}

				LeavePartyCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID );

			}; // class LeavePartyCmd : public MessageBase

			class LeavePartyRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				LeavePartyRes()
					{}

				LeavePartyRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class LeavePartyRes : public MessageBase

			// S2C: Party Player left event
			class PartyPlayerLeftS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PartyUID m_PartyUID;
				PlayerID m_LeftPlayerID;
			public:
				PartyPlayerLeftS2CEvt()
					{}

				PartyPlayerLeftS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetLeftPlayerID() const	{ return m_LeftPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID );

			}; // class PartyPlayerLeftS2CEvt : public MessageBase

			// Cmd: Kick player from the party
			class PartyKickPlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PartyUID m_PartyUID;
				PlayerID m_PlayerID;
				PlayerID m_PlayerToKick;
			public:
				PartyKickPlayerCmd()
					{}

				PartyKickPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const PlayerID& GetPlayerToKick() const	{ return m_PlayerToKick; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );

			}; // class PartyKickPlayerCmd : public MessageBase

			class PartyKickPlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				PartyKickPlayerRes()
					{}

				PartyKickPlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class PartyKickPlayerRes : public MessageBase

			// S2C: Party Player kicked message
			class PartyPlayerKickedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PartyUID m_PartyUID;
				PlayerID m_KickedPlayerID;
			public:
				PartyPlayerKickedS2CEvt()
					{}

				PartyPlayerKickedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetKickedPlayerID() const	{ return m_KickedPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID );

			}; // class PartyPlayerKickedS2CEvt : public MessageBase

			// Cmd: Invite a player to the party
			class PartyInviteCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_InviteTargetID;
			public:
				PartyInviteCmd()
					{}

				PartyInviteCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetInviteTargetID() const	{ return m_InviteTargetID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviteTargetID );

			}; // class PartyInviteCmd : public MessageBase

			class PartyInviteRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				PartyInviteRes()
					{}

				PartyInviteRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class PartyInviteRes : public MessageBase

			// S2C: Party invite requested
			class PartyInviteRequestedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_InviterID;
				const char* m_InviterName;
				PartyUID m_PartyToJoinUID;
			public:
				PartyInviteRequestedS2CEvt()
				:m_InviterName(nullptr)
					{}

				PartyInviteRequestedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_InviterName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetInviterID() const	{ return m_InviterID; };
				const char* GetInviterName() const	{ return m_InviterName; };
				const PartyUID& GetPartyToJoinUID() const	{ return m_PartyToJoinUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID );

			}; // class PartyInviteRequestedS2CEvt : public MessageBase

			// Cmd: Send Party quick chat message
			class PartyQuickChatMessageCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT32 m_QuickChatID;
			public:
				PartyQuickChatMessageCmd()
					{}

				PartyQuickChatMessageCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT32& GetQuickChatID() const	{ return m_QuickChatID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InQuickChatID );

			}; // class PartyQuickChatMessageCmd : public MessageBase

			class PartyQuickChatMessageRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				PartyQuickChatMessageRes()
					{}

				PartyQuickChatMessageRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class PartyQuickChatMessageRes : public MessageBase

			// S2C: Party Chatting message event
			class PartyQuickChatMessageS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_SenderID;
				UINT32 m_QuickChatID;
			public:
				PartyQuickChatMessageS2CEvt()
					{}

				PartyQuickChatMessageS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetSenderID() const	{ return m_SenderID; };
				const UINT32& GetQuickChatID() const	{ return m_QuickChatID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const UINT32 &InQuickChatID );

			}; // class PartyQuickChatMessageS2CEvt : public MessageBase

			// Cmd: Party chatting
			class PartyChatMessageCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				const char* m_ChatMessage;
			public:
				PartyChatMessageCmd()
				:m_ChatMessage(nullptr)
					{}

				PartyChatMessageCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_ChatMessage(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetChatMessage() const	{ return m_ChatMessage; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage );

			}; // class PartyChatMessageCmd : public MessageBase

			class PartyChatMessageRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				PartyChatMessageRes()
					{}

				PartyChatMessageRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class PartyChatMessageRes : public MessageBase

			// S2C: Party Chatting message event
			class PartyChatMessageS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_SenderID;
				const char* m_SenderName;
				const char* m_ChatMessage;
			public:
				PartyChatMessageS2CEvt()
				:m_SenderName(nullptr)
				,m_ChatMessage(nullptr)
					{}

				PartyChatMessageS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_SenderName(nullptr)
				,m_ChatMessage(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetSenderID() const	{ return m_SenderID; };
				const char* GetSenderName() const	{ return m_SenderName; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );

			}; // class PartyChatMessageS2CEvt : public MessageBase

			// Cmd: Join to a game
			class JoinGameCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_PlayerID;
				AuthTicket m_Ticket;
				GameInsUID m_InsUID;
			public:
				JoinGameCmd()
					{}

				JoinGameCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const GameInsUID& GetInsUID() const	{ return m_InsUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID );

			}; // class JoinGameCmd : public MessageBase

			class JoinGameRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				GameInsUID m_InsUID;
				UINT32 m_TimeStamp;
				GameStateID m_GameState;
				UINT8 m_Day;
				UINT8 m_MaxPlayer;
				UINT8 m_PlayerIndex;
				UINT8 m_PlayerCharacter;
				PlayerRole m_Role;
				UINT8 m_Dead;
				LinkedArray<BYTE> m_ChatHistoryData;
				LinkedArray<BYTE> m_GameLogData;
			public:
				JoinGameRes()
					{}

				JoinGameRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const GameInsUID& GetInsUID() const	{ return m_InsUID; };
				const UINT32& GetTimeStamp() const	{ return m_TimeStamp; };
				const GameStateID& GetGameState() const	{ return m_GameState; };
				const UINT8& GetDay() const	{ return m_Day; };
				const UINT8& GetMaxPlayer() const	{ return m_MaxPlayer; };
				const UINT8& GetPlayerIndex() const	{ return m_PlayerIndex; };
				const UINT8& GetPlayerCharacter() const	{ return m_PlayerCharacter; };
				const PlayerRole& GetRole() const	{ return m_Role; };
				const UINT8& GetDead() const	{ return m_Dead; };
				const Array<BYTE>& GetChatHistoryData() const	{ return m_ChatHistoryData; };
				const Array<BYTE>& GetGameLogData() const	{ return m_GameLogData; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData );

			}; // class JoinGameRes : public MessageBase

			// S2C: Player Joined in the game
			class PlayerJoinedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerInformation m_JoinedPlayer;
				PlayerRole m_JoinedPlayerRole;
				UINT8 m_JoinedPlayerDead;
				UINT8 m_JoinedPlayerIndex;
				UINT8 m_JoinedPlayerCharacter;
			public:
				PlayerJoinedS2CEvt()
					{}

				PlayerJoinedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerInformation& GetJoinedPlayer() const	{ return m_JoinedPlayer; };
				const PlayerRole& GetJoinedPlayerRole() const	{ return m_JoinedPlayerRole; };
				const UINT8& GetJoinedPlayerDead() const	{ return m_JoinedPlayerDead; };
				const UINT8& GetJoinedPlayerIndex() const	{ return m_JoinedPlayerIndex; };
				const UINT8& GetJoinedPlayerCharacter() const	{ return m_JoinedPlayerCharacter; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter );

			}; // class PlayerJoinedS2CEvt : public MessageBase

			// Cmd: Leave Game
			class LeaveGameCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_PlayerID;
				AuthTicket m_Ticket;
			public:
				LeaveGameCmd()
					{}

				LeaveGameCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );

			}; // class LeaveGameCmd : public MessageBase

			class LeaveGameRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				LeaveGameRes()
					{}

				LeaveGameRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class LeaveGameRes : public MessageBase

			// S2C: Player left event
			class PlayerLeftS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_LeftPlayerID;
			public:
				PlayerLeftS2CEvt()
					{}

				PlayerLeftS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetLeftPlayerID() const	{ return m_LeftPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID );

			}; // class PlayerLeftS2CEvt : public MessageBase

			// Cmd: Kick player
			class KickPlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_PlayerID;
				PlayerID m_PlayerToKick;
			public:
				KickPlayerCmd()
					{}

				KickPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const PlayerID& GetPlayerToKick() const	{ return m_PlayerToKick; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );

			}; // class KickPlayerCmd : public MessageBase

			class KickPlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				KickPlayerRes()
					{}

				KickPlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class KickPlayerRes : public MessageBase

			// S2C: Player kicked
			class PlayerKickedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_KickedPlayerID;
			public:
				PlayerKickedS2CEvt()
					{}

				PlayerKickedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetKickedPlayerID() const	{ return m_KickedPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID );

			}; // class PlayerKickedS2CEvt : public MessageBase

			// Cmd: Assign role + Game state reset
			class AssignRoleCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_PlayerID;
				AuthTicket m_Ticket;
			public:
				AssignRoleCmd()
					{}

				AssignRoleCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );

			}; // class AssignRoleCmd : public MessageBase

			class AssignRoleRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				AssignRoleRes()
					{}

				AssignRoleRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class AssignRoleRes : public MessageBase

			// S2C: Role assigned event
			class RoleAssignedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_PlayerID;
				PlayerRole m_Role;
			public:
				RoleAssignedS2CEvt()
					{}

				RoleAssignedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const PlayerRole& GetRole() const	{ return m_Role; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole );

			}; // class RoleAssignedS2CEvt : public MessageBase

			// Cmd: Send chatting message to the game
			class ChatMessageCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				const char* m_ChatMessage;
				PlayerRole m_Role;
			public:
				ChatMessageCmd()
				:m_ChatMessage(nullptr)
					{}

				ChatMessageCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_ChatMessage(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetChatMessage() const	{ return m_ChatMessage; };
				const PlayerRole& GetRole() const	{ return m_Role; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage, const PlayerRole &InRole );

			}; // class ChatMessageCmd : public MessageBase

			class ChatMessageRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				ChatMessageRes()
					{}

				ChatMessageRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class ChatMessageRes : public MessageBase

			// S2C: Chatting message event 
			class ChatMessageS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_SenderID;
				PlayerRole m_Role;
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

				const PlayerID& GetSenderID() const	{ return m_SenderID; };
				const PlayerRole& GetRole() const	{ return m_Role; };
				const char* GetSenderName() const	{ return m_SenderName; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage );

			}; // class ChatMessageS2CEvt : public MessageBase

			// Cmd: Advance game
			class AdvanceGameCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_PlayerID;
				AuthTicket m_Ticket;
			public:
				AdvanceGameCmd()
					{}

				AdvanceGameCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );

			}; // class AdvanceGameCmd : public MessageBase

			class AdvanceGameRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				AdvanceGameRes()
					{}

				AdvanceGameRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class AdvanceGameRes : public MessageBase

			// S2C: The game state is advanced
			class GameAdvancedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				UINT32 m_TimeStamp;
				GameStateID m_GameState;
				UINT8 m_Day;
			public:
				GameAdvancedS2CEvt()
					{}

				GameAdvancedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const UINT32& GetTimeStamp() const	{ return m_TimeStamp; };
				const GameStateID& GetGameState() const	{ return m_GameState; };
				const UINT8& GetDay() const	{ return m_Day; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay );

			}; // class GameAdvancedS2CEvt : public MessageBase

			// S2C: Game is ended
			class GameEndedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				GameWinner m_Winner;
				UINT32 m_GainedExp;
				UINT32 m_GainedGameMoney;
			public:
				GameEndedS2CEvt()
					{}

				GameEndedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const GameWinner& GetWinner() const	{ return m_Winner; };
				const UINT32& GetGainedExp() const	{ return m_GainedExp; };
				const UINT32& GetGainedGameMoney() const	{ return m_GainedGameMoney; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney );

			}; // class GameEndedS2CEvt : public MessageBase

			// Cmd: Vote game advance
			class VoteGameAdvanceCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_PlayerID;
				AuthTicket m_Ticket;
			public:
				VoteGameAdvanceCmd()
					{}

				VoteGameAdvanceCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );

			}; // class VoteGameAdvanceCmd : public MessageBase

			class VoteGameAdvanceRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				VoteGameAdvanceRes()
					{}

				VoteGameAdvanceRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class VoteGameAdvanceRes : public MessageBase

			// S2C: GameAdvance is Voted
			class GameAdvanceVotedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_Voter;
			public:
				GameAdvanceVotedS2CEvt()
					{}

				GameAdvanceVotedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetVoter() const	{ return m_Voter; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter );

			}; // class GameAdvanceVotedS2CEvt : public MessageBase

			// Cmd: Vote
			class VoteCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_PlayerID;
				AuthTicket m_Ticket;
				PlayerID m_VoteTarget;
				UINT32 m_ActionSerial;
			public:
				VoteCmd()
					{}

				VoteCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const PlayerID& GetVoteTarget() const	{ return m_VoteTarget; };
				const UINT32& GetActionSerial() const	{ return m_ActionSerial; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial );

			}; // class VoteCmd : public MessageBase

			class VoteRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				VoteRes()
					{}

				VoteRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class VoteRes : public MessageBase

			// S2C: Player Voted
			class VotedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_Voter;
				PlayerID m_VotedTarget;
			public:
				VotedS2CEvt()
					{}

				VotedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetVoter() const	{ return m_Voter; };
				const PlayerID& GetVotedTarget() const	{ return m_VotedTarget; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget );

			}; // class VotedS2CEvt : public MessageBase

			// S2C: Vote is ended
			class VoteEndS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				LinkedArray<PlayerID> m_Voted;
			public:
				VoteEndS2CEvt()
					{}

				VoteEndS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const Array<PlayerID>& GetVoted() const	{ return m_Voted; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted );

			}; // class VoteEndS2CEvt : public MessageBase

			// S2C: Player Killed
			class PlayerKilledS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_KilledPlayer;
				PlayerKilledReason m_Reason;
			public:
				PlayerKilledS2CEvt()
					{}

				PlayerKilledS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetKilledPlayer() const	{ return m_KilledPlayer; };
				const PlayerKilledReason& GetReason() const	{ return m_Reason; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason );

			}; // class PlayerKilledS2CEvt : public MessageBase

			// S2C: Player Voted
			class PlayerRevealedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_GameInsUID;
				PlayerID m_RevealedPlayerID;
				PlayerRole m_Role;
				PlayerRevealedReason m_Reason;
			public:
				PlayerRevealedS2CEvt()
					{}

				PlayerRevealedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerID& GetRevealedPlayerID() const	{ return m_RevealedPlayerID; };
				const PlayerRole& GetRole() const	{ return m_Role; };
				const PlayerRevealedReason& GetReason() const	{ return m_Reason; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason );

			}; // class PlayerRevealedS2CEvt : public MessageBase

			// Cmd: Play again with the current players
			class GamePlayAgainCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				GamePlayAgainCmd()
					{}

				GamePlayAgainCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class GamePlayAgainCmd : public MessageBase

			class GamePlayAgainRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT64 m_TotalGem;
				UINT64 m_TotalGameMoney;
			public:
				GamePlayAgainRes()
					{}

				GamePlayAgainRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT64& GetTotalGem() const	{ return m_TotalGem; };
				const UINT64& GetTotalGameMoney() const	{ return m_TotalGameMoney; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );

			}; // class GamePlayAgainRes : public MessageBase

			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			class GamePlayAgainS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PartyUID m_PartyUID;
				PlayerID m_LeadPlayer;
			public:
				GamePlayAgainS2CEvt()
					{}

				GamePlayAgainS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetLeadPlayer() const	{ return m_LeadPlayer; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer );

			}; // class GamePlayAgainS2CEvt : public MessageBase

			// Cmd: Player. reveal a player
			class GameRevealPlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				LinkedArray<PlayerID> m_TargetPlayerID;
			public:
				GameRevealPlayerCmd()
					{}

				GameRevealPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Array<PlayerID>& GetTargetPlayerID() const	{ return m_TargetPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID );

			}; // class GameRevealPlayerCmd : public MessageBase

			class GameRevealPlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				LinkedArray<PlayerID> m_RevealedPlayerID;
				LinkedArray<PlayerRole> m_RevealedRole;
				UINT64 m_TotalGem;
				UINT64 m_TotalGameMoney;
			public:
				GameRevealPlayerRes()
					{}

				GameRevealPlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const Array<PlayerID>& GetRevealedPlayerID() const	{ return m_RevealedPlayerID; };
				const Array<PlayerRole>& GetRevealedRole() const	{ return m_RevealedRole; };
				const UINT64& GetTotalGem() const	{ return m_TotalGem; };
				const UINT64& GetTotalGameMoney() const	{ return m_TotalGameMoney; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );

			}; // class GameRevealPlayerRes : public MessageBase

			// Cmd: Player. revive himself
			class GamePlayerReviveCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				GamePlayerReviveCmd()
					{}

				GamePlayerReviveCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class GamePlayerReviveCmd : public MessageBase

			class GamePlayerReviveRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT64 m_TotalGem;
				UINT64 m_TotalGameMoney;
			public:
				GamePlayerReviveRes()
					{}

				GamePlayerReviveRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT64& GetTotalGem() const	{ return m_TotalGem; };
				const UINT64& GetTotalGameMoney() const	{ return m_TotalGameMoney; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );

			}; // class GamePlayerReviveRes : public MessageBase

			// S2C: Player is revived
			class GamePlayerRevivedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_RevivedPlayerID;
			public:
				GamePlayerRevivedS2CEvt()
					{}

				GamePlayerRevivedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetRevivedPlayerID() const	{ return m_RevivedPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InRevivedPlayerID );

			}; // class GamePlayerRevivedS2CEvt : public MessageBase

			// Cmd: Player. reset ranking
			class GamePlayerResetRankCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				GamePlayerResetRankCmd()
					{}

				GamePlayerResetRankCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class GamePlayerResetRankCmd : public MessageBase

			class GamePlayerResetRankRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT64 m_TotalGem;
				UINT64 m_TotalGameMoney;
			public:
				GamePlayerResetRankRes()
					{}

				GamePlayerResetRankRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT64& GetTotalGem() const	{ return m_TotalGem; };
				const UINT64& GetTotalGameMoney() const	{ return m_TotalGameMoney; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );

			}; // class GamePlayerResetRankRes : public MessageBase

			// Cmd: Request Game match
			class RequestGameMatchCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				BYTE m_NumPlayer;
				PlayerRole m_RequestRole;
			public:
				RequestGameMatchCmd()
					{}

				RequestGameMatchCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const BYTE& GetNumPlayer() const	{ return m_NumPlayer; };
				const PlayerRole& GetRequestRole() const	{ return m_RequestRole; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const BYTE &InNumPlayer, const PlayerRole &InRequestRole );

			}; // class RequestGameMatchCmd : public MessageBase

			class RequestGameMatchRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT64 m_TotalGem;
				UINT64 m_TotalGameMoney;
			public:
				RequestGameMatchRes()
					{}

				RequestGameMatchRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT64& GetTotalGem() const	{ return m_TotalGem; };
				const UINT64& GetTotalGameMoney() const	{ return m_TotalGameMoney; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );

			}; // class RequestGameMatchRes : public MessageBase

			// S2C: Game matched
			class GameMatchedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				GameInsUID m_InsUID;
				UINT32 m_TimeStamp;
				GameStateID m_GameState;
				UINT8 m_Day;
				UINT8 m_MaxPlayer;
				UINT8 m_PlayerIndex;
				UINT8 m_PlayerCharacter;
				PlayerRole m_Role;
				UINT8 m_Dead;
				LinkedArray<BYTE> m_ChatHistoryData;
				LinkedArray<BYTE> m_GameLogData;
				UINT32 m_Stamina;
				UINT64 m_TotalGem;
				UINT64 m_TotalGameMoney;
			public:
				GameMatchedS2CEvt()
					{}

				GameMatchedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameInsUID& GetInsUID() const	{ return m_InsUID; };
				const UINT32& GetTimeStamp() const	{ return m_TimeStamp; };
				const GameStateID& GetGameState() const	{ return m_GameState; };
				const UINT8& GetDay() const	{ return m_Day; };
				const UINT8& GetMaxPlayer() const	{ return m_MaxPlayer; };
				const UINT8& GetPlayerIndex() const	{ return m_PlayerIndex; };
				const UINT8& GetPlayerCharacter() const	{ return m_PlayerCharacter; };
				const PlayerRole& GetRole() const	{ return m_Role; };
				const UINT8& GetDead() const	{ return m_Dead; };
				const Array<BYTE>& GetChatHistoryData() const	{ return m_ChatHistoryData; };
				const Array<BYTE>& GetGameLogData() const	{ return m_GameLogData; };
				const UINT32& GetStamina() const	{ return m_Stamina; };
				const UINT64& GetTotalGem() const	{ return m_TotalGem; };
				const UINT64& GetTotalGameMoney() const	{ return m_TotalGameMoney; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );

			}; // class GameMatchedS2CEvt : public MessageBase

			// S2C: Game match failed
			class GameMatchFailedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_FailedReason;
			public:
				GameMatchFailedS2CEvt()
					{}

				GameMatchFailedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetFailedReason() const	{ return m_FailedReason; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InFailedReason );

			}; // class GameMatchFailedS2CEvt : public MessageBase

			// S2C: Game matching started
			class GameMatchingStartedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				GameMatchingStartedS2CEvt()
					{}

				GameMatchingStartedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class GameMatchingStartedS2CEvt : public MessageBase

			// Cmd: Cancel Game match
			class CancelGameMatchCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				CancelGameMatchCmd()
					{}

				CancelGameMatchCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class CancelGameMatchCmd : public MessageBase

			class CancelGameMatchRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				CancelGameMatchRes()
					{}

				CancelGameMatchRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class CancelGameMatchRes : public MessageBase

			// S2C: game matching canceled
			class GameMatchingCanceledS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
			public:
				GameMatchingCanceledS2CEvt()
					{}

				GameMatchingCanceledS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }


				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg );

			}; // class GameMatchingCanceledS2CEvt : public MessageBase

			// Cmd: Buy shop item prepare
			class BuyShopItemPrepareCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT32 m_ShopItemID;
			public:
				BuyShopItemPrepareCmd()
					{}

				BuyShopItemPrepareCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT32& GetShopItemID() const	{ return m_ShopItemID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID );

			}; // class BuyShopItemPrepareCmd : public MessageBase

			class BuyShopItemPrepareRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT32 m_ShopItemID;
				const char* m_PurchaseID;
			public:
				BuyShopItemPrepareRes()
				:m_PurchaseID(nullptr)
					{}

				BuyShopItemPrepareRes( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_PurchaseID(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT32& GetShopItemID() const	{ return m_ShopItemID; };
				const char* GetPurchaseID() const	{ return m_PurchaseID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InShopItemID, const char* InPurchaseID );

			}; // class BuyShopItemPrepareRes : public MessageBase

			// Cmd: Buy shop item
			class BuyShopItemCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT32 m_ShopItemID;
				const char* m_Platform;
				const char* m_PackageName;
				const char* m_PurchaseTransactionID;
				LinkedArray<BYTE> m_PurchaseToken;
			public:
				BuyShopItemCmd()
				:m_Platform(nullptr)
				,m_PackageName(nullptr)
				,m_PurchaseTransactionID(nullptr)
					{}

				BuyShopItemCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_Platform(nullptr)
				,m_PackageName(nullptr)
				,m_PurchaseTransactionID(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT32& GetShopItemID() const	{ return m_ShopItemID; };
				const char* GetPlatform() const	{ return m_Platform; };
				const char* GetPackageName() const	{ return m_PackageName; };
				const char* GetPurchaseTransactionID() const	{ return m_PurchaseTransactionID; };
				const Array<BYTE>& GetPurchaseToken() const	{ return m_PurchaseToken; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken );

			}; // class BuyShopItemCmd : public MessageBase

			class BuyShopItemRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				UINT32 m_ShopItemID;
			public:
				BuyShopItemRes()
					{}

				BuyShopItemRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const UINT32& GetShopItemID() const	{ return m_ShopItemID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InShopItemID );

			}; // class BuyShopItemRes : public MessageBase

			// Cmd: Give my stamina to other player
			class GiveStaminaCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				PlayerID m_TargetPlayer;
			public:
				GiveStaminaCmd()
					{}

				GiveStaminaCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const PlayerID& GetTargetPlayer() const	{ return m_TargetPlayer; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InTargetPlayer );

			}; // class GiveStaminaCmd : public MessageBase

			class GiveStaminaRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
				PlayerID m_TargetPlayer;
				UINT64 m_TimeStamp;
			public:
				GiveStaminaRes()
					{}

				GiveStaminaRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const PlayerID& GetTargetPlayer() const	{ return m_TargetPlayer; };
				const UINT64& GetTimeStamp() const	{ return m_TimeStamp; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp );

			}; // class GiveStaminaRes : public MessageBase

			// Cmd: For debug, Change configue preset
			class SetPresetGameConfigIDCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				UINT32 m_PresetID;
			public:
				SetPresetGameConfigIDCmd()
					{}

				SetPresetGameConfigIDCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const UINT32& GetPresetID() const	{ return m_PresetID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InPresetID );

			}; // class SetPresetGameConfigIDCmd : public MessageBase

			class SetPresetGameConfigIDRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				SetPresetGameConfigIDRes()
					{}

				SetPresetGameConfigIDRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class SetPresetGameConfigIDRes : public MessageBase

			// Cmd: For Debug
			class GainGameResourceCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				INT32 m_Resource;
				INT32 m_Value;
			public:
				GainGameResourceCmd()
					{}

				GainGameResourceCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const INT32& GetResource() const	{ return m_Resource; };
				const INT32& GetValue() const	{ return m_Value; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const INT32 &InResource, const INT32 &InValue );

			}; // class GainGameResourceCmd : public MessageBase

			class GainGameResourceRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				HRESULT m_Result;
			public:
				GainGameResourceRes()
					{}

				GainGameResourceRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult );

			}; // class GainGameResourceRes : public MessageBase





		}; // namespace Game
	}; // namespace Message
}; // namespace BR


