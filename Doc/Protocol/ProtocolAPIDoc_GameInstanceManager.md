***
 
 CopyRight (c) 2021 StormForge
 
 Description : GameInstanceManager Message Protocol API Document

***



Game Instance manager service Protocol

namespace SF::Message::GameInstanceManager


# Protocol interface class NetPolicyGameInstanceManager
## CreateGameInstance Request
Create a game instance

1. Command interface

        Result CreateGameInstanceCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<uint8_t>& InAttributes)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInAttributes: VariableTable type. Instance attributes

2. Result interface

C++: Cast message to CreateGameInstanceRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## SearchGameInstance Request
Search game instance

1. Command interface

        Result SearchGameInstanceCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InSearchKeyword)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInSearchKeyword: String type. Search keyword

2. Result interface

C++: Cast message to SearchGameInstanceRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutGameInstances: GameInstanceInfo type. Game instance informations


## GameInstanceDeletedC2SEvt
Game instance notification of deletion

        Result GameInstanceDeletedC2SEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 








