***
 
 CopyRight (c) 2021 StormForge
 
 Description : GameServer Message Protocol API Document

***



Game Server Protocol

namespace SF::Message::GameServer


# Protocol interface class NetPolicyGameServer
## RegisterPlayerToJoinGameServer Request
Register player so that the player can connection and join to the game server.

1. Command interface

        Result RegisterPlayerToJoinGameServerCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. PlayerID to join

		- OutInTicket: AuthTicket type. Authentication ticket. represents login session ID

		- OutInFBUserID: FacebookUID type. Facebook user ID

		- OutInShardID: uint32 type. DB shard ID for player partitioning

2. Result interface

C++: Cast message to RegisterPlayerToJoinGameServerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutPublicAddress: String type. Game server address the client can use to connect.

		- OutPublicAddressV6: String type. IPV6 Game server address the client can use to connect.

		- OutPort: uint32 type. Port to connect


## RegisterPlayerToJoinGameServerOnPlayerEntity Request
Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.

1. Command interface

        Result RegisterPlayerToJoinGameServerOnPlayerEntityCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. PlayerID

		- OutInTicket: AuthTicket type. Login session ID

		- OutInFBUserID: FacebookUID type. Facebook user ID

2. Result interface

C++: Cast message to RegisterPlayerToJoinGameServerOnPlayerEntityRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## ChatMessageC2SEvt
Chatting message event.

        Result ChatMessageC2SEvt(const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage)

		- OutInRouteContext: RouteContext type. 

		- OutInSenderID: AccountID type. AccountID is player ID for now

		- OutInRole: PlayerRole type. Role filter

		- OutInSenderName: String type. Sender Name

		- OutInChatMessage: String type. Chatting message event.


## NotifyC2SEvt
Notification event for P2P

        Result NotifyC2SEvt(const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp)

		- OutInRouteContext: RouteContext type. 

		- OutInDestPlayerID: PlayerID type. 

		- OutInNotificationID: uint32 type. 

		- OutInMessageID: NotificationType type. 

		- OutInMessageParam0: uint64 type. 

		- OutInMessageParam1: uint64 type. 

		- OutInMessageText: String type. 

		- OutInTimeStamp: uint64 type. 


## FriendAcceptedC2SEvt
Friend Accepted

        Result FriendAcceptedC2SEvt(const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter)

		- OutInRouteContext: RouteContext type. 

		- OutInDestPlayerID: AccountID type. Destination player who requested friend

		- OutInAccepter: ServerFriendInformation type. My friend information


## FriendRemovedC2SEvt
Friend Remove

        Result FriendRemovedC2SEvt(const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID)

		- OutInRouteContext: RouteContext type. 

		- OutInDestPlayerID: AccountID type. Destination player ID to notify that I removed him.

		- OutInRemoverID: PlayerID type. My player ID


## RequestPlayerStatusUpdateC2SEvt
Request Player Status Update

        Result RequestPlayerStatusUpdateC2SEvt(const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID)

		- OutInRouteContext: RouteContext type. 

		- OutInDestPlayerID: AccountID type. destination player ID to check status

		- OutInRequesterID: PlayerID type. Request sender ID


## NotifyPlayerStatusUpdatedC2SEvt
Notify Player Status Updated

        Result NotifyPlayerStatusUpdatedC2SEvt(const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame)

		- OutInRouteContext: RouteContext type. 

		- OutInDestPlayerID: AccountID type. Destination player ID to send.

		- OutInLatestActiveTime: uint32 type. Latest active time.

		- OutInIsInGame: uint8 type. Is in game?


## NotifyPartyInviteC2SEvt
Party invite Notification

        Result NotifyPartyInviteC2SEvt(const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID)

		- OutInRouteContext: RouteContext type. 

		- OutInDestPlayerID: PlayerID type. Invited player ID

		- OutInInviterID: PlayerID type. Inviter player ID. Usually Party leader.

		- OutInInviterName: String type. Inviter player Name

		- OutInPartyUID: EntityUID type. Party UID








