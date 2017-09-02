////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class PartyMatchingQueueService : public ServerServiceBase
		{
 			public:
			// Constructor
			PartyMatchingQueueService ( ServerServiceInformation* pService );

			// Cmd: Register match by party
			Result RegisterPartyMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers );
			// Cmd: Register match alone
			Result RegisterPlayerMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID );
			// Cmd: update registration information
			Result UpdateMatchingEntityUIDCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID );
			// Cmd: calcel registration
			Result UnregisterMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			Result ReserveItemCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount );
			// Cmd: Reserve a item
			Result ReserveItemsCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve );
			// Cmd: Cancel reservation
			Result CancelReservationCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel );
			// Cmd: Cancel reservation
			Result CancelReservationsCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel );
			// Cmd: Dequeue a reserved item
			Result DequeueItemCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// C2S: Item error you should delete it
			Result MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

		}; // class PartyMatchingQueueService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


