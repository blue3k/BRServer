﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyPartyMatchingQueue 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyPartyMatchingQueue ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Register match by party
			Result RegisterPartyMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers );
			// Cmd: Register match alone
			Result RegisterPlayerMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID );
			// Cmd: update registration information
			Result UpdateMatchingEntityUIDCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID );
			// Cmd: calcel registration
			Result UnregisterMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			Result ReserveItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount );
			// Cmd: Reserve a item
			Result ReserveItemsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve );
			// Cmd: Cancel reservation
			Result CancelReservationCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel );
			// Cmd: Cancel reservation
			Result CancelReservationsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel );
			// Cmd: Dequeue a reserved item
			Result DequeueItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// C2S: Item error you should delete it
			Result MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

		}; // class NetPolicyPartyMatchingQueue 


		class NetSvrPolicyPartyMatchingQueue
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyPartyMatchingQueue ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Register match by party
			Result RegisterPartyMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Register match alone
			Result RegisterPlayerMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: update registration information
			Result UpdateMatchingEntityUIDRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: calcel registration
			Result UnregisterMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: For some reason, matching is canceled
			Result PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket );
			// S2C: For some reason, matching is canceled
			Result PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket );
			// S2C: Enqueued item is dequeued
			Result PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// S2C: Enqueued item is dequeued
			Result PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			Result ReserveItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: Reserve a item
			Result ReserveItemsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket );
			// Cmd: Cancel reservation
			Result CancelReservationRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: Cancel reservation
			Result CancelReservationsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: Dequeue a reserved item
			Result DequeueItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers );

		}; // class NetSvrPolicyPartyMatchingQueue


	}; // namespace Policy
}; // namespace SF

