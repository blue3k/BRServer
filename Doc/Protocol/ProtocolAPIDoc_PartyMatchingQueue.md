***
 
 CopyRight (c) 2018 StormForge
 
 Description : PartyMatchingQueue Message Protocol API Document

***



Party matching queue Protocol

namespace SF::Message::PartyMatchingQueue


# Protocol interface class NetPolicyPartyMatchingQueue
## RegisterPartyMatching Request
Register a party for matching

1. Command interface

        Result RegisterPartyMatchingCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInPlayers: MatchingPlayerInformation type. requster(Party) information

2. Result interface

C++: Cast message to RegisterPartyMatchingRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutMatchingTicket: MatchingQueueTicket type. Ticket used for cancel and any other notification


## RegisterPlayerMatching Request
Register a player for matching

1. Command interface

        Result RegisterPlayerMatchingCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInPlayerID: PlayerID type. Register ID

2. Result interface

C++: Cast message to RegisterPlayerMatchingRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutMatchingTicket: MatchingQueueTicket type. Ticket used for cancel and any other notification


## UpdateMatchingEntityUID Request
[Deprecated] update registration information

1. Command interface

        Result UpdateMatchingEntityUIDCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInMatchingTicket: MatchingQueueTicket type. 

		- OutInPreviousUID: EntityUID type. 

2. Result interface

C++: Cast message to UpdateMatchingEntityUIDRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## UnregisterMatching Request
calcel registration

1. Command interface

        Result UnregisterMatchingCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInMatchingTicket: MatchingQueueTicket type. Ticket to cancel

2. Result interface

C++: Cast message to UnregisterMatchingRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## PartyMatchingCanceledS2CEvt
Event when the matching is canceled, for party

        Result PartyMatchingCanceledS2CEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInDestPartyUID: PartyUID type. Party UID to cancel

		- OutInMatchingTicket: MatchingQueueTicket type. Matching ticket to cancel


## PlayerMatchingCanceledS2CEvt
Event whan the matching is canceled, for a player

        Result PlayerMatchingCanceledS2CEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInDestPlayerID: PlayerID type. Player ID

		- OutInMatchingTicket: MatchingQueueTicket type. Matching ticket to cancel


## PartyMatchingItemDequeuedS2CEvt
Matching item has been dequeued for matching

        Result PartyMatchingItemDequeuedS2CEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInMatchingTicket: MatchingQueueTicket type. Matching Ticket


## PlayerMatchingItemDequeuedS2CEvt
Matching item has been dequeued for matching

        Result PlayerMatchingItemDequeuedS2CEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInMatchingTicket: MatchingQueueTicket type. Matching ticket


## ReserveItem Request
Reserve a item

1. Command interface

        Result ReserveItemCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

2. Result interface

C++: Cast message to ReserveItemRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutNumberOfPlayersInTheItem: uint32 type. 

		- OutMatchingTicket: MatchingQueueTicket type. 


## ReserveItems Request
Reserve a item

1. Command interface

        Result ReserveItemsCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInNumberOfItemsToReserve: uint32 type. 

2. Result interface

C++: Cast message to ReserveItemsRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutNumberOfPlayersInTheItem: uint32 type. 

		- OutMatchingTicket: MatchingQueueTicket type. 


## CancelReservation Request
Cancel reservation

1. Command interface

        Result CancelReservationCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInTicketToCancel: MatchingQueueTicket type. 

2. Result interface

C++: Cast message to CancelReservationRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## CancelReservations Request
Cancel reservation

1. Command interface

        Result CancelReservationsCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInTicketToCancel: MatchingQueueTicket type. 

2. Result interface

C++: Cast message to CancelReservationsRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## DequeueItem Request
Dequeue a reserved item

1. Command interface

        Result DequeueItemCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInMatchingTicket: MatchingQueueTicket type. 

2. Result interface

C++: Cast message to DequeueItemRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutMatchingTicket: MatchingQueueTicket type. 

		- OutRegisterUID: EntityUID type. Reserver UID

		- OutRegisterID: PlayerID type. Reserver PlayerID

		- OutPlayers: MatchingPlayerInformation type. 


## MatchingItemErrorC2SEvt
Item error you should delete it

        Result MatchingItemErrorC2SEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInMatchingTicket: MatchingQueueTicket type. 








