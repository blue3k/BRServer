////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class PartyMatchingQueueService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyPartyMatchingQueue* GetPolicyPartyMatchingQueue() { return GetPolicy<Policy::IPolicyPartyMatchingQueue>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyPartyMatchingQueue::ID_POLICY };
			// Constructor
			PartyMatchingQueueService ( ServerServiceInformation* pService );

			// Cmd: Register match by party
			Result RegisterPartyMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers );
			// Cmd: Register match alone
			Result RegisterPlayerMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID );
			// Cmd: update registration information
			Result UpdateMatchingEntityUIDCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID );
			// Cmd: calcel registration
			Result UnregisterMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			Result ReserveItemCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount );
			// Cmd: Reserve a item
			Result ReserveItemsCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve );
			// Cmd: Cancel reservation
			Result CancelReservationCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel );
			// Cmd: Cancel reservation
			Result CancelReservationsCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel );
			// Cmd: Dequeue a reserved item
			Result DequeueItemCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// C2S: Item error you should delete it
			Result MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

		}; // class PartyMatchingQueueService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


