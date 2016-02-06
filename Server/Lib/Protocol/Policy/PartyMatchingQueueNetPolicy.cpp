﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Register match by party
		HRESULT NetPolicyPartyMatchingQueue::RegisterPartyMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::RegisterPartyMatchingCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InPlayers));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::RegisterPartyMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		// Cmd: Register match alone
		HRESULT NetPolicyPartyMatchingQueue::RegisterPlayerMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::RegisterPlayerMatchingCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::RegisterPlayerMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID )
		// Cmd: update registration information
		HRESULT NetPolicyPartyMatchingQueue::UpdateMatchingEntityUIDCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket, InPreviousUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::UpdateMatchingEntityUIDCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID )
		// Cmd: calcel registration
		HRESULT NetPolicyPartyMatchingQueue::UnregisterMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::UnregisterMatchingCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::UnregisterMatchingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		HRESULT NetPolicyPartyMatchingQueue::ReserveItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::ReserveItemCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::ReserveItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount )
		// Cmd: Reserve a item
		HRESULT NetPolicyPartyMatchingQueue::ReserveItemsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::ReserveItemsCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InNumberOfItemsToReserve));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::ReserveItemsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve )
		// Cmd: Cancel reservation
		HRESULT NetPolicyPartyMatchingQueue::CancelReservationCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::CancelReservationCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::CancelReservationCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		// Cmd: Cancel reservation
		HRESULT NetPolicyPartyMatchingQueue::CancelReservationsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::CancelReservationsCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::CancelReservationsCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		// Cmd: Dequeue a reserved item
		HRESULT NetPolicyPartyMatchingQueue::DequeueItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::DequeueItemCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::DequeueItemCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// C2S: Item error you should delete it
		HRESULT NetPolicyPartyMatchingQueue::MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::MatchingItemErrorC2SEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyPartyMatchingQueue::MatchingItemErrorC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )


		// Cmd: Register match by party
		HRESULT NetSvrPolicyPartyMatchingQueue::RegisterPartyMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::RegisterPartyMatchingRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::RegisterPartyMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Register match alone
		HRESULT NetSvrPolicyPartyMatchingQueue::RegisterPlayerMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::RegisterPlayerMatchingRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::RegisterPlayerMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: update registration information
		HRESULT NetSvrPolicyPartyMatchingQueue::UpdateMatchingEntityUIDRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::UpdateMatchingEntityUIDRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::UpdateMatchingEntityUIDRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
		// Cmd: calcel registration
		HRESULT NetSvrPolicyPartyMatchingQueue::UnregisterMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::UnregisterMatchingRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::UnregisterMatchingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
		// S2C: For some reason, matching is canceled
		HRESULT NetSvrPolicyPartyMatchingQueue::PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InDestPartyUID, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::PartyMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
		// S2C: For some reason, matching is canceled
		HRESULT NetSvrPolicyPartyMatchingQueue::PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InDestPlayerID, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::PlayerMatchingCanceledS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
		// S2C: Enqueued item is dequeued
		HRESULT NetSvrPolicyPartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// S2C: Enqueued item is dequeued
		HRESULT NetSvrPolicyPartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRouteHopCount, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		HRESULT NetSvrPolicyPartyMatchingQueue::ReserveItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::ReserveItemRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InNumberOfPlayersInTheItem, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::ReserveItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		HRESULT NetSvrPolicyPartyMatchingQueue::ReserveItemsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const Array<UINT32>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::ReserveItemsRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InNumberOfPlayersInTheItem, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::ReserveItemsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const Array<UINT32>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
		// Cmd: Cancel reservation
		HRESULT NetSvrPolicyPartyMatchingQueue::CancelReservationRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::CancelReservationRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::CancelReservationRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
		// Cmd: Cancel reservation
		HRESULT NetSvrPolicyPartyMatchingQueue::CancelReservationsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::CancelReservationsRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::CancelReservationsRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult )
		// Cmd: Dequeue a reserved item
		HRESULT NetSvrPolicyPartyMatchingQueue::DequeueItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::PartyMatchingQueue::DequeueItemRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InMatchingTicket, InRegisterUID, InRegisterID, InPlayers));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyPartyMatchingQueue::DequeueItemRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )


	}; // namespace Policy
}; // namespace BR


