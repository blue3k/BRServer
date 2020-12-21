﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Register a party for matching
		Result NetPolicyPartyMatchingQueue::RegisterPartyMatchingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::RegisterPartyMatchingCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InPlayers);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::RegisterPartyMatchingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		// Cmd: Register a player for matching
		Result NetPolicyPartyMatchingQueue::RegisterPlayerMatchingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::RegisterPlayerMatchingCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InPlayerID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::RegisterPlayerMatchingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
		// Cmd: [Deprecated] update registration information
		Result NetPolicyPartyMatchingQueue::UpdateMatchingEntityUIDCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket, InPreviousUID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::UpdateMatchingEntityUIDCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
		// Cmd: calcel registration
		Result NetPolicyPartyMatchingQueue::UnregisterMatchingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::UnregisterMatchingCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::UnregisterMatchingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		Result NetPolicyPartyMatchingQueue::ReserveItemCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::ReserveItemCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::ReserveItemCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount )
		// Cmd: Reserve a item
		Result NetPolicyPartyMatchingQueue::ReserveItemsCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::ReserveItemsCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InNumberOfItemsToReserve);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::ReserveItemsCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
		// Cmd: Cancel reservation
		Result NetPolicyPartyMatchingQueue::CancelReservationCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::CancelReservationCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::CancelReservationCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		// Cmd: Cancel reservation
		Result NetPolicyPartyMatchingQueue::CancelReservationsCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::CancelReservationsCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::CancelReservationsCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		// Cmd: Dequeue a reserved item
		Result NetPolicyPartyMatchingQueue::DequeueItemCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::DequeueItemCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::DequeueItemCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// C2S: Item error you should delete it
		Result NetPolicyPartyMatchingQueue::MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::MatchingItemErrorC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )


		// Cmd: Register a party for matching
		Result NetSvrPolicyPartyMatchingQueue::RegisterPartyMatchingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::RegisterPartyMatchingRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::RegisterPartyMatchingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Register a player for matching
		Result NetSvrPolicyPartyMatchingQueue::RegisterPlayerMatchingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::RegisterPlayerMatchingRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::RegisterPlayerMatchingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: [Deprecated] update registration information
		Result NetSvrPolicyPartyMatchingQueue::UpdateMatchingEntityUIDRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::UpdateMatchingEntityUIDRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::UpdateMatchingEntityUIDRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: calcel registration
		Result NetSvrPolicyPartyMatchingQueue::UnregisterMatchingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::UnregisterMatchingRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::UnregisterMatchingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// S2C: Event when the matching is canceled, for party
		Result NetSvrPolicyPartyMatchingQueue::PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount, InDestPartyUID, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
		// S2C: Event whan the matching is canceled, for a player
		Result NetSvrPolicyPartyMatchingQueue::PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount, InDestPlayerID, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
		// S2C: Matching item has been dequeued for matching
		Result NetSvrPolicyPartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// S2C: Matching item has been dequeued for matching
		Result NetSvrPolicyPartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		Result NetSvrPolicyPartyMatchingQueue::ReserveItemRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::ReserveItemRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InNumberOfPlayersInTheItem, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::ReserveItemRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		Result NetSvrPolicyPartyMatchingQueue::ReserveItemsRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::ReserveItemsRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InNumberOfPlayersInTheItem, InMatchingTicket);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::ReserveItemsRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
		// Cmd: Cancel reservation
		Result NetSvrPolicyPartyMatchingQueue::CancelReservationRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::CancelReservationRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::CancelReservationRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Cancel reservation
		Result NetSvrPolicyPartyMatchingQueue::CancelReservationsRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::CancelReservationsRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::CancelReservationsRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Dequeue a reserved item
		Result NetSvrPolicyPartyMatchingQueue::DequeueItemRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::DequeueItemRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InMatchingTicket, InRegisterUID, InRegisterID, InPlayers);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::DequeueItemRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )


	}; // namespace Policy
}; // namespace SF


