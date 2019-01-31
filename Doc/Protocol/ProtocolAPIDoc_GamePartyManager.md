***
 
 CopyRight (c) 2018 StormForge
 
 Description : GamePartyManager Message Protocol API Document

***



Game Party manager service Protocol

namespace SF::Message::GamePartyManager


# Protocol interface class NetPolicyGamePartyManager
## CreateParty Request
Create a party instance

1. Command interface

        Result CreatePartyCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInGameID: uint32 type. Game ID

		- OutInCreator: PlayerInformation type. Player Information who is requesting.

2. Result interface

C++: Cast message to CreatePartyRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PartyDeletedC2SEvt
Party instance notify of deletion. Sent by party instance

        Result PartyDeletedC2SEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 








