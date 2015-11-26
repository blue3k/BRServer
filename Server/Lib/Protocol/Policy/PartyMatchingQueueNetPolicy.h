////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyPartyMatchingQueue : public IPolicyPartyMatchingQueue
		{
 			public:
			// Constructor
			NetPolicyPartyMatchingQueue ( Net::IConnection* pConn ) : IPolicyPartyMatchingQueue(pConn)
			{}

			// Cmd: Register match by party
			HRESULT RegisterPartyMatchingCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers );
			// Cmd: Register match alone
			HRESULT RegisterPlayerMatchingCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID );
			// Cmd: update registration information
			HRESULT UpdateMatchingEntityUIDCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID );
			// Cmd: calcel registration
			HRESULT UnregisterMatchingCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			HRESULT ReserveItemCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount );
			// Cmd: Reserve a item
			HRESULT ReserveItemsCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve );
			// Cmd: Cancel reservation
			HRESULT CancelReservationCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel );
			// Cmd: Cancel reservation
			HRESULT CancelReservationsCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel );
			// Cmd: Dequeue a reserved item
			HRESULT DequeueItemCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// C2S: Item error you should delete it
			HRESULT MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

		}; // class NetPolicyPartyMatchingQueue : public IPolicyPartyMatchingQueue


		class NetSvrPolicyPartyMatchingQueue : public ISvrPolicyPartyMatchingQueue
		{
 			public:
			// Constructor
			NetSvrPolicyPartyMatchingQueue ( Net::IConnection* pConn ) : ISvrPolicyPartyMatchingQueue(pConn)
			{}

			// Cmd: Register match by party
			HRESULT RegisterPartyMatchingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Register match alone
			HRESULT RegisterPlayerMatchingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: update registration information
			HRESULT UpdateMatchingEntityUIDRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );
			// Cmd: calcel registration
			HRESULT UnregisterMatchingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );
			// S2C: For some reason, matching is canceled
			HRESULT PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket );
			// S2C: For some reason, matching is canceled
			HRESULT PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket );
			// S2C: Enqueued item is dequeued
			HRESULT PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// S2C: Enqueued item is dequeued
			HRESULT PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			HRESULT ReserveItemRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			HRESULT ReserveItemsRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<UINT32>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket );
			// Cmd: Cancel reservation
			HRESULT CancelReservationRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );
			// Cmd: Cancel reservation
			HRESULT CancelReservationsRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );
			// Cmd: Dequeue a reserved item
			HRESULT DequeueItemRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers );

		}; // class NetSvrPolicyPartyMatchingQueue : public ISvrPolicyPartyMatchingQueue


	}; // namespace Policy
}; // namespace BR


