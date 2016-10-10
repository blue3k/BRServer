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
#include "Net/NetDef.h"
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
			virtual Result RegisterPartyMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: Register match alone
			virtual Result RegisterPlayerMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: update registration information
			virtual Result UpdateMatchingEntityUIDRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// Cmd: calcel registration
			virtual Result UnregisterMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: For some reason, matching is canceled
			virtual Result PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// S2C: For some reason, matching is canceled
			virtual Result PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// S2C: Enqueued item is dequeued
			virtual Result PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// S2C: Enqueued item is dequeued
			virtual Result PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: Reserve a item
			virtual Result ReserveItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const UINT32 &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: Reserve a item
			virtual Result ReserveItemsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<UINT32>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket ) = 0;
			// Cmd: Cancel reservation
			virtual Result CancelReservationRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// Cmd: Cancel reservation
			virtual Result CancelReservationsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// Cmd: Dequeue a reserved item
			virtual Result DequeueItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers ) = 0;
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
			virtual Result RegisterPartyMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers ) = 0;
			// Cmd: Register match alone
			virtual Result RegisterPlayerMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID ) = 0;
			// Cmd: update registration information
			virtual Result UpdateMatchingEntityUIDCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID ) = 0;
			// Cmd: calcel registration
			virtual Result UnregisterMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// S2C: For some reason, matching is canceled
			// S2C: For some reason, matching is canceled
			// S2C: Enqueued item is dequeued
			// S2C: Enqueued item is dequeued
			// Cmd: Reserve a item
			virtual Result ReserveItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount ) = 0;
			// Cmd: Reserve a item
			virtual Result ReserveItemsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve ) = 0;
			// Cmd: Cancel reservation
			virtual Result CancelReservationCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel ) = 0;
			// Cmd: Cancel reservation
			virtual Result CancelReservationsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel ) = 0;
			// Cmd: Dequeue a reserved item
			virtual Result DequeueItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// C2S: Item error you should delete it
			virtual Result MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket ) = 0;

		}; // class IPolicyPartyMatchingQueue : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


