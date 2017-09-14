////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Register match by party
		Result NetPolicyPartyMatchingQueue::RegisterPartyMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::RegisterPartyMatchingCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InPlayers);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::RegisterPartyMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		// Cmd: Register match alone
		Result NetPolicyPartyMatchingQueue::RegisterPlayerMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::RegisterPlayerMatchingCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::RegisterPlayerMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
		// Cmd: update registration information
		Result NetPolicyPartyMatchingQueue::UpdateMatchingEntityUIDCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket, InPreviousUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::UpdateMatchingEntityUIDCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
		// Cmd: calcel registration
		Result NetPolicyPartyMatchingQueue::UnregisterMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::UnregisterMatchingCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::UnregisterMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		Result NetPolicyPartyMatchingQueue::ReserveItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::ReserveItemCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::ReserveItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount )
		// Cmd: Reserve a item
		Result NetPolicyPartyMatchingQueue::ReserveItemsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::ReserveItemsCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InNumberOfItemsToReserve);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::ReserveItemsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
		// Cmd: Cancel reservation
		Result NetPolicyPartyMatchingQueue::CancelReservationCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::CancelReservationCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::CancelReservationCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		// Cmd: Cancel reservation
		Result NetPolicyPartyMatchingQueue::CancelReservationsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::CancelReservationsCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::CancelReservationsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		// Cmd: Dequeue a reserved item
		Result NetPolicyPartyMatchingQueue::DequeueItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::DequeueItemCmd::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::DequeueItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// C2S: Item error you should delete it
		Result NetPolicyPartyMatchingQueue::MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::MatchingItemErrorC2SEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyPartyMatchingQueue::MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )


		// Cmd: Register match by party
		Result NetSvrPolicyPartyMatchingQueue::RegisterPartyMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::RegisterPartyMatchingRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::RegisterPartyMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Register match alone
		Result NetSvrPolicyPartyMatchingQueue::RegisterPlayerMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::RegisterPlayerMatchingRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::RegisterPlayerMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: update registration information
		Result NetSvrPolicyPartyMatchingQueue::UpdateMatchingEntityUIDRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::UpdateMatchingEntityUIDRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::UpdateMatchingEntityUIDRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: calcel registration
		Result NetSvrPolicyPartyMatchingQueue::UnregisterMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::UnregisterMatchingRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::UnregisterMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: For some reason, matching is canceled
		Result NetSvrPolicyPartyMatchingQueue::PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InDestPartyUID, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
		// S2C: For some reason, matching is canceled
		Result NetSvrPolicyPartyMatchingQueue::PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InDestPlayerID, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
		// S2C: Enqueued item is dequeued
		Result NetSvrPolicyPartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// S2C: Enqueued item is dequeued
		Result NetSvrPolicyPartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InRouteHopCount, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		Result NetSvrPolicyPartyMatchingQueue::ReserveItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::ReserveItemRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult, InNumberOfPlayersInTheItem, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::ReserveItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		Result NetSvrPolicyPartyMatchingQueue::ReserveItemsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::ReserveItemsRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult, InNumberOfPlayersInTheItem, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::ReserveItemsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
		// Cmd: Cancel reservation
		Result NetSvrPolicyPartyMatchingQueue::CancelReservationRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::CancelReservationRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::CancelReservationRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: Cancel reservation
		Result NetSvrPolicyPartyMatchingQueue::CancelReservationsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::CancelReservationsRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::CancelReservationsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: Dequeue a reserved item
		Result NetSvrPolicyPartyMatchingQueue::DequeueItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::PartyMatchingQueue::DequeueItemRes::Create(m_pConnection->GetNetIOHandler().GetIOHeap(), InRouteContext, InTransactionID, InResult, InMatchingTicket, InRegisterUID, InRegisterID, InPlayers);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyPartyMatchingQueue::DequeueItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )


	}; // namespace Policy
}; // namespace SF


