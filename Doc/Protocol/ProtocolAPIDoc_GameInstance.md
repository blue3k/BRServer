***
 
 CopyRight (c) 2021 StormForge
 
 Description : GameInstance Message Protocol API Document

***



Game instance service Protocol

namespace SF::Message::GameInstance


# Protocol interface class NetPolicyGameInstance
## JoinGameInstance Request
Join to a game instance. You can call multiple times, but it would be a waste

1. Command interface

        Result JoinGameInstanceCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const Array<uint8_t>& InCharacterVisual, const Array<uint8_t>& InCharacterAttribute)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayer: PlayerInformation type. Player information who join

		- OutInCharacterVisual: VariableTable type. Character information who join

		- OutInCharacterAttribute: VariableTable type. Character information who join

2. Result interface

C++: Cast message to JoinGameInstanceRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutGameInsSvr4: NetAddress type. Game instance server address. Client will use this address to connect game server.

		- OutGameInsSvr6: NetAddress type. Game instance server address. Client will use this address to connect game server.

		- OutTimeStamp: uint32 type. Time stamp of game


## LeaveGameInstance Request
Leave game instance.

1. Command interface

        Result LeaveGameInstanceCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. PlayID to leave

2. Result interface

C++: Cast message to LeaveGameInstanceRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PlayerLeftS2CEvt
Player left event.

        Result PlayerLeftS2CEvt(const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInLeftPlayerID: PlayerID type. Player ID who left.


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








