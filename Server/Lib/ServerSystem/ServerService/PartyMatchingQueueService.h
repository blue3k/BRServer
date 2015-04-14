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
#include "Net/Netdef.h"
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
			HRESULT RegisterPartyMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers );
			// Cmd: Register match alone
			HRESULT RegisterPlayerMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID );
			// Cmd: update registration information
			HRESULT UpdateMatchingEntityUIDCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID );
			// Cmd: calcel registration
			HRESULT UnregisterMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			HRESULT ReserveItemCmd( const Context &InContext, const UINT16 &InRouteHopCount );
			// Cmd: Reserve a item
			HRESULT ReserveItemsCmd( const Context &InContext, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve );
			// Cmd: Cancel reservation
			HRESULT CancelReservationCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel );
			// Cmd: Cancel reservation
			HRESULT CancelReservationsCmd( const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel );
			// Cmd: Dequeue a reserved item
			HRESULT DequeueItemCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// C2S: Item error you should delete it
			HRESULT MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

		}; // class PartyMatchingQueueService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


