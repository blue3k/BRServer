***
 
 CopyRight (c) 2021 StormForge
 
 Description : PartyMatching Message Protocol API Document

***



Game matching Protocol

namespace SF::Message::PartyMatching


# Protocol interface class NetPolicyPartyMatching
## PartyGameMatchedS2CEvt
Event message will be sent from matching srvice when the pary game mathcing has done.

        Result PartyGameMatchedS2CEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 


## PlayerGameMatchedS2CEvt
Event message will be sent when a player game matching has successfully finished.

        Result PlayerGameMatchedS2CEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInDestPlayerID: PlayerID type. Destination player ID

		- OutInGameInsUID: GameInsUID type. Game instance UID

		- OutInRequestedRole: PlayerRole type. Requested role








