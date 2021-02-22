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

        Result CreateGameInstanceCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const Array<uint8_t>& InAttributes)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInZoneTableId: uint32 type. Zone table Id

		- OutInAttributes: VariableTable type. creation parameters

2. Result interface

C++: Cast message to CreateGameInstanceRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 








