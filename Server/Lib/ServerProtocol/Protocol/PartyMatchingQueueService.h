////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/PartyMatchingQueueNetPolicy.h"



namespace SF
{
 	class PartyMatchingQueueService : public ServerServiceBase
	{
 		public:
		// Constructor
		PartyMatchingQueueService ( ServerServiceInformation* pService );

		// Cmd: Register a party for matching
		Result RegisterPartyMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers );
		// Cmd: Register a player for matching
		Result RegisterPlayerMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID );
		// Cmd: [Deprecated] update registration information
		Result UpdateMatchingEntityUIDCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID );
		// Cmd: calcel registration
		Result UnregisterMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
		// Cmd: Reserve a item
		Result ReserveItemCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount );
		// Cmd: Reserve a item
		Result ReserveItemsCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve );
		// Cmd: Cancel reservation
		Result CancelReservationCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel );
		// Cmd: Cancel reservation
		Result CancelReservationsCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel );
		// Cmd: Dequeue a reserved item
		Result DequeueItemCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
		// C2S: Item error you should delete it
		Result MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

	}; // class PartyMatchingQueueService : public ServerServiceBase


}; // namespace SF


