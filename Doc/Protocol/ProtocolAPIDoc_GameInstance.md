***
 
 CopyRight (c) 2019 StormForge
 
 Description : GameInstance Message Protocol API Document

***



Game instance service Protocol

namespace SF::Message::GameInstance


# Protocol interface class NetPolicyGameInstance
## DeleteGameC2SEvt
Nitify that a game instance is deleted. Game instance send this message to manager before it destroy itself.

        Result DeleteGameC2SEvt(const RouteContext &InRouteContext)

		- OutInRouteContext: RouteContext type. 


## JoinGame Request
Join to a game instance. You can call multiple times, but it would be waste

1. Command interface

        Result JoinGameCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayer: PlayerInformation type. Player information who join

		- OutInRequestedRole: PlayerRole type. Requested player role[For specific game. otherwise ignore it]

2. Result interface

C++: Cast message to JoinGameRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutGameInsSvr: NetAddress type. Game instance server address. Client will use this address to connect game server.

		- OutTimeStamp: uint32 type. Time stamp of game state

		- OutGameState: GameStateID type. GameState ID

		- OutDay: uint8 type. Game State information

		- OutMaxPlayer: uint8 type. Maximum player count

		- OutPlayerIndex: uint8 type. Player index in the game instance.

		- OutPlayerCharacter: uint8 type. Player character.

		- OutRole: PlayerRole type. Player role assigned by game instance.

		- OutDead: uint8 type. Player statues.

		- OutIsNewJoin: uint8 type. true if the player joined at the first time.

		- OutChatHistoryData: uint8 type. [optional].

		- OutGameLogData: uint8 type. [Optional] you can send game status through this parameter.


## PlayerJoinedS2CEvt
Player joined event. This event is brocasted when a player joined

        Result PlayerJoinedS2CEvt(const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const uint8_t &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter)

		- OutInRouteContext: RouteContext type. 

		- OutInJoinedPlayer: PlayerInformation type. Joined player information

		- OutInJoinedPlayerRole: PlayerRole type. Joined Player Role

		- OutInJoinedPlayerDead: uint8 type. Joined player state

		- OutInJoinedPlayerIndex: uint8 type. [optional]Joined player index

		- OutInJoinedPlayerCharacter: uint8 type. Joined player character, assigned by game instance.


## SetConfigPresetC2SEvt
For debug purpose, change configue preset. There is a game setting table. you can switch between those setting value.

        Result SetConfigPresetC2SEvt(const RouteContext &InRouteContext, const uint32_t &InPresetID)

		- OutInRouteContext: RouteContext type. 

		- OutInPresetID: uint32 type. Preset ID


## LeaveGame Request
Leave game instance.

1. Command interface

        Result LeaveGameCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. PlayID to leave

2. Result interface

C++: Cast message to LeaveGameRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PlayerLeftS2CEvt
Player left event.

        Result PlayerLeftS2CEvt(const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInLeftPlayerID: PlayerID type. Player ID who left.


## KickPlayer Request
Kick player with given ID

1. Command interface

        Result KickPlayerCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Player who kick.

		- OutInPlayerToKick: PlayerID type. Player who kicked.

2. Result interface

C++: Cast message to KickPlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PlayerKickedS2CEvt
Player kicked event. this event will be brocasted when a player kicked.

        Result PlayerKickedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInKickedPlayerID: PlayerID type. Kicked player ID


## AssignRole Request
Assign new roles to all players.

1. Command interface

        Result AssignRoleCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. [Optional] requester playerID

2. Result interface

C++: Cast message to AssignRoleRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## RoleAssignedS2CEvt
Event for role assigned

        Result RoleAssignedS2CEvt(const RouteContext &InRouteContext, const uint8_t &InRole)

		- OutInRouteContext: RouteContext type. 

		- OutInRole: PlayerRole type. Assigned new role.


## ChatMessageC2SEvt
in-game chatting message.

        Result ChatMessageC2SEvt(const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage)

		- OutInRouteContext: RouteContext type. 

		- OutInPlayerID: PlayerID type. Sender playerID

		- OutInRole: PlayerRole type. player Role for recipent role filtering

		- OutInChatMessage: String type. Chatting message


## AdvanceGame Request
Advance game

1. Command interface

        Result AdvanceGameCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Advance game state

2. Result interface

C++: Cast message to AdvanceGameRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## GameAdvancedS2CEvt
The game state is advanced

        Result GameAdvancedS2CEvt(const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay)

		- OutInRouteContext: RouteContext type. 

		- OutInTimeStamp: uint32 type. 

		- OutInGameState: GameStateID type. 

		- OutInDay: uint8 type. 


## GameEndedS2CEvt
Game is ended

        Result GameEndedS2CEvt(const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon)

		- OutInRouteContext: RouteContext type. 

		- OutInWinner: GameWinner type. 

		- OutInGainedExp: uint32 type. 

		- OutInGainedGameMoney: uint32 type. 

		- OutInPlayedRole: PlayerRole type. 

		- OutInIsWon: uint8 type. 


## VoteGameAdvance Request
*Vote game advance

1. Command interface

        Result VoteGameAdvanceCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. 

2. Result interface

C++: Cast message to VoteGameAdvanceRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## GameAdvanceVotedS2CEvt
*GameAdvance is Voted

        Result GameAdvanceVotedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InVoter)

		- OutInRouteContext: RouteContext type. 

		- OutInVoter: PlayerID type. 


## Vote Request
Assign role

1. Command interface

        Result VoteCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. 

		- OutInVoteTarget: PlayerID type. 

		- OutInActionSerial: uint32 type. 

2. Result interface

C++: Cast message to VoteRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## VotedS2CEvt
Player Voted

        Result VotedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget)

		- OutInRouteContext: RouteContext type. 

		- OutInVoter: PlayerID type. 

		- OutInVotedTarget: PlayerID type. 


## VoteEndS2CEvt
Player Voted

        Result VoteEndS2CEvt(const RouteContext &InRouteContext, const Array<PlayerID>& InVoted)

		- OutInRouteContext: RouteContext type. 

		- OutInVoted: PlayerID type. 


## PlayerRevealedS2CEvt
Player Voted

        Result PlayerRevealedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason)

		- OutInRouteContext: RouteContext type. 

		- OutInRevealedPlayerID: PlayerID type. 

		- OutInRole: PlayerRole type. 

		- OutInReason: PlayerRevealedReason type. 


## PlayerKilledS2CEvt
Player Killed

        Result PlayerKilledS2CEvt(const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason)

		- OutInRouteContext: RouteContext type. 

		- OutInKilledPlayer: PlayerID type. 

		- OutInReason: PlayerKilledReason type. 


## GamePlayAgain Request
Play again with the current players

1. Command interface

        Result GamePlayAgainCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInLeadPlayer: PlayerID type. Player who pressed replay

		- OutInPartyUID: PartyUID type. Party ID

2. Result interface

C++: Cast message to GamePlayAgainRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutReplayMemberCount: uint32 type. Member count


## GamePlayAgainS2CEvt
Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value

        Result GamePlayAgainS2CEvt(const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer)

		- OutInRouteContext: RouteContext type. 

		- OutInTargetPlayer: PlayerID type. Event target player

		- OutInPartyUID: PartyUID type. Party ID

		- OutInLeadPlayer: PlayerID type. Player who called play again


## GameRevealPlayer Request
Player. revive himself

1. Command interface

        Result GameRevealPlayerCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. 

		- OutInTargetPlayerID: PlayerID type. 

2. Result interface

C++: Cast message to GameRevealPlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutRevealedPlayerID: PlayerID type. 

		- OutRevealedRole: PlayerRole type. 


## GamePlayerRevive Request
Player. revive himself

1. Command interface

        Result GamePlayerReviveCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. 

2. Result interface

C++: Cast message to GamePlayerReviveRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## GamePlayerRevivedS2CEvt
Player is revived

        Result GamePlayerRevivedS2CEvt(const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInRevivedPlayerID: PlayerID type. Player who is revived








