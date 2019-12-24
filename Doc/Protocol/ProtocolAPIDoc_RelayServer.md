***
 
 CopyRight (c) 2019 StormForge
 
 Description : RelayServer Message Protocol API Document

***



Game Relay Server Protocol

namespace SF::Message::RelayServer


# Protocol interface class NetPolicyRelayServer
## CreateRelayInstance Request
Relay Instance

1. Command interface

        Result CreateRelayInstanceCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerInfo: PlayerInformation type. PlayerInformation to add

		- OutInRelayScore: uint64 type. Relay score

2. Result interface

C++: Cast message to CreateRelayInstanceRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutRelay: uint32 type. Relay


## AddPlayer Request
Add a player to Relay

1. Command interface

        Result AddPlayerCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerInfo: PlayerInformation type. PlayerInformation to add

		- OutInRelayScore: uint64 type. Relay score

2. Result interface

C++: Cast message to AddPlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutRelay: uint32 type. Relay


## RemovePlayer Request
Remove a player to Relay

1. Command interface

        Result RemovePlayerCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Player ID to remove

2. Result interface

C++: Cast message to RemovePlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 








