***
 
 CopyRight (c) 2020 StormForge
 
 Description : GameInstanceManager Message Protocol API Document

***



Game Instance manager service Protocol

namespace SF::Message::GameInstanceManager


# Protocol interface class NetPolicyGameInstanceManager
## CreateGame Request
Create a game instance

1. Command interface

        Result CreateGameCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInNumberOfBotPlayer: uint16 type. Number of bots.

		- OutInMaxPlayer: uint16 type. Total player count.

2. Result interface

C++: Cast message to CreateGameRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## GameDeletedC2SEvt
Game instance notification of deletion

        Result GameDeletedC2SEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 








