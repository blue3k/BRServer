***
 
 CopyRight (c) 2020 StormForge
 
 Description : ChatChannel Message Protocol API Document

***



Chatting channel service

namespace SF::Message::ChatChannel


# Protocol interface class NetPolicyChatChannel
## Join Request
Request to join chat channel

1. Command interface

        Result JoinCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPasscode: String type. Channel pass code

		- OutInJoiningPlayer: PlayerInformation type. Player information structure to join

2. Result interface

C++: Cast message to JoinRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutChatChannelLeaderID: PlayerID type. Chat channel leader ID


## PlayerJoinedS2CEvt
Server envent to notify joined player information

        Result PlayerJoinedS2CEvt(const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer)

		- OutInRouteContext: RouteContext type. 

		- OutInJoinedPlayer: PlayerInformation type. Joined player information


## LeaderChangedS2CEvt
Chat channel leader changed

        Result LeaderChangedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InNewLeaderID)

		- OutInRouteContext: RouteContext type. 

		- OutInNewLeaderID: PlayerID type. New leader player id


## Leave Request
Leave chat channel

1. Command interface

        Result LeaveCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. PlayerID to leave

2. Result interface

C++: Cast message to LeaveRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PlayerLeftS2CEvt
Notification event when a player left

        Result PlayerLeftS2CEvt(const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInLeftPlayerID: PlayerID type. Player ID who left


## KickPlayer Request
Kick a player

1. Command interface

        Result KickPlayerCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Player ID who does kick

		- OutInPlayerToKick: PlayerID type. Kicked player ID

2. Result interface

C++: Cast message to KickPlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PlayerKickedS2CEvt
Notification event when a player kicked

        Result PlayerKickedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInKickedPlayerID: PlayerID type. Kicked playerID


## ChatMessageC2SEvt
Sending a chatting message

        Result ChatMessageC2SEvt(const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage)

		- OutInRouteContext: RouteContext type. 

		- OutInSenderID: PlayerID type. Sender ID

		- OutInChatMessage: String type. Chatting message


## ChatMessageS2CEvt
brocasting event for a chatting message

        Result ChatMessageS2CEvt(const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage)

		- OutInRouteContext: RouteContext type. 

		- OutInSenderID: PlayerID type. Sender ID

		- OutInSenderName: String type. Sender name, came from sender

		- OutInChatMessage: String type. Chatting message








