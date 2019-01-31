***
 
 CopyRight (c) 2018 StormForge
 
 Description : GameParty Message Protocol API Document

***



Game Party Server Protocol

namespace SF::Message::GameParty


# Protocol interface class NetPolicyGameParty
## JoinParty Request
Join party

1. Command interface

        Result JoinPartyCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInInviterID: PlayerID type. Inviter player ID

		- OutInInvitedPlayer: PlayerInformation type. Player information to join

2. Result interface

C++: Cast message to JoinPartyRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutPartyLeaderID: PlayerID type. Party leader id

		- OutChatHistoryData: uint8 type. [Optional] Party chattaing history


## PlayerJoinedS2CEvt
Player joined event.

        Result PlayerJoinedS2CEvt(const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer)

		- OutInRouteContext: RouteContext type. 

		- OutInJoinedPlayer: PlayerInformation type. Joined player information


## PartyLeaderChangedS2CEvt
Event for party leader is changed

        Result PartyLeaderChangedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InNewLeaderID)

		- OutInRouteContext: RouteContext type. 

		- OutInNewLeaderID: PlayerID type. new leader ID


## LeaveParty Request
Event for player left.

1. Command interface

        Result LeavePartyCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. PlayerID who left.

2. Result interface

C++: Cast message to LeavePartyRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PlayerLeftS2CEvt
Player left

        Result PlayerLeftS2CEvt(const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInLeftPlayerID: PlayerID type. player ID who left.


## KickPlayer Request
Kick player

1. Command interface

        Result KickPlayerCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. player who kicks

		- OutInPlayerToKick: PlayerID type. Player to kick

2. Result interface

C++: Cast message to KickPlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PlayerKickedS2CEvt
Event for Player kicked.

        Result PlayerKickedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInKickedPlayerID: PlayerID type. Kicked player ID


## ChatMessageC2SEvt
Send chat message to server.

        Result ChatMessageC2SEvt(const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage)

		- OutInRouteContext: RouteContext type. 

		- OutInPlayerID: PlayerID type. Sender Player ID

		- OutInChatMessage: String type. Chatting message


## ChatMessageS2CEvt
Brocast chatting message

        Result ChatMessageS2CEvt(const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage)

		- OutInRouteContext: RouteContext type. 

		- OutInSenderID: PlayerID type. Sender player ID

		- OutInSenderName: String type. Sender name

		- OutInChatMessage: String type. Chatting message


## QuickChatMessageC2SEvt
Quick Chatting message

        Result QuickChatMessageC2SEvt(const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID)

		- OutInRouteContext: RouteContext type. 

		- OutInPlayerID: PlayerID type. 

		- OutInQuickChatID: uint32 type. My quick chat message ID


## QuickChatMessageS2CEvt
Quick Chatting message

        Result QuickChatMessageS2CEvt(const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID)

		- OutInRouteContext: RouteContext type. 

		- OutInSenderID: PlayerID type. 

		- OutInQuickChatID: uint32 type. My quick chat message ID


## StartGameMatch Request
Start party game matching

1. Command interface

        Result StartGameMatchCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Player who requests.

		- OutInMaxGamePlayers: uint32 type. Player count.

2. Result interface

C++: Cast message to StartGameMatchRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## QueuedGameMatchingS2CEvt
Event sent when the party is queued for game matching

        Result QueuedGameMatchingS2CEvt(const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInMatchingQueueTicket: MatchingQueueTicket type. Matching queue ticket


## CancelGameMatch Request
Cancel game matching

1. Command interface

        Result CancelGameMatchCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Player who request cancel. have to be party leader.

2. Result interface

C++: Cast message to CancelGameMatchRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## CanceledGameMatchingS2CEvt
Event sent when the game matching is canceled

        Result CanceledGameMatchingS2CEvt(const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInMatchingQueueTicket: MatchingQueueTicket type. Mathcing queue ticket


## MatchingItemDequeuedS2CEvt
Event sent when the the matching has dequeued.

        Result MatchingItemDequeuedS2CEvt(const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInMatchingTicket: MatchingQueueTicket type. Matching queue ticket








