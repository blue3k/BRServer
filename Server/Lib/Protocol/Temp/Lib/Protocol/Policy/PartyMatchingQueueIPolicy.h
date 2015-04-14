////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyPartyMatchingQueue : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_PARTYMATCHINGQUEUE };

			// Constructor
			ISvrPolicyPartyMatchingQueue( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Register match by party
			virtual HRESULT RegisterPartyMatchingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: Register match alone
			virtual HRESULT RegisterPlayerMatchingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: update registration information
			virtual HRESULT UpdateMatchingEntityUIDRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// Cmd: calcel registration
			virtual HRESULT UnregisterMatchingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: For some reason, matching is canceled
			virtual HRESULT PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// S2C: For some reason, matching is canceled
			virtual HRESULT PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// S2C: Enqueued item is dequeued
			virtual HRESULT PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// S2C: Enqueued item is dequeued
			virtual HRESULT PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: Reserve a item
			virtual HRESULT ReserveItemRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: Reserve a item
			virtual HRESULT ReserveItemsRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<UINT32>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket ) = 0;
			// Cmd: Cancel reservation
			virtual HRESULT CancelReservationRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// Cmd: Cancel reservation
			virtual HRESULT CancelReservationsRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// Cmd: Dequeue a reserved item
			virtual HRESULT DequeueItemRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers ) = 0;
			// C2S: Item error you should delete it

		}; // class ISvrPolicyPartyMatchingQueue : public Net::INetPolicy


		class IPolicyPartyMatchingQueue : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_PARTYMATCHINGQUEUE };

			// Constructor
			IPolicyPartyMatchingQueue( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Register match by party
			virtual HRESULT RegisterPartyMatchingCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers ) = 0;
			// Cmd: Register match alone
			virtual HRESULT RegisterPlayerMatchingCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID ) = 0;
			// Cmd: update registration information
			virtual HRESULT UpdateMatchingEntityUIDCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID ) = 0;
			// Cmd: calcel registration
			virtual HRESULT UnregisterMatchingCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// S2C: For some reason, matching is canceled
			// S2C: For some reason, matching is canceled
			// S2C: Enqueued item is dequeued
			// S2C: Enqueued item is dequeued
			// Cmd: Reserve a item
			virtual HRESULT ReserveItemCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount ) = 0;
			// Cmd: Reserve a item
			virtual HRESULT ReserveItemsCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve ) = 0;
			// Cmd: Cancel reservation
			virtual HRESULT CancelReservationCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel ) = 0;
			// Cmd: Cancel reservation
			virtual HRESULT CancelReservationsCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel ) = 0;
			// Cmd: Dequeue a reserved item
			virtual HRESULT DequeueItemCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// C2S: Item error you should delete it
			virtual HRESULT MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;

		}; // class IPolicyPartyMatchingQueue : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


