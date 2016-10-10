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
#include "Common/BrBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/PartyMatchingQueueService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		PartyMatchingQueueService::PartyMatchingQueueService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyPartyMatchingQueue::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyPartyMatchingQueue::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyPartyMatchingQueue());
		}


		// Cmd: Register match by party
		Result PartyMatchingQueueService::RegisterPartyMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->RegisterPartyMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayers ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::RegisterPartyMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		// Cmd: Register match alone
		Result PartyMatchingQueueService::RegisterPlayerMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->RegisterPlayerMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::RegisterPlayerMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID )
		// Cmd: update registration information
		Result PartyMatchingQueueService::UpdateMatchingEntityUIDCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->UpdateMatchingEntityUIDCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket, InPreviousUID ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::UpdateMatchingEntityUIDCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID )
		// Cmd: calcel registration
		Result PartyMatchingQueueService::UnregisterMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->UnregisterMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::UnregisterMatchingCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		Result PartyMatchingQueueService::ReserveItemCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->ReserveItemCmd( InRouteContext, InTransactionID, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::ReserveItemCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount )
		// Cmd: Reserve a item
		Result PartyMatchingQueueService::ReserveItemsCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->ReserveItemsCmd( InRouteContext, InTransactionID, InRouteHopCount, InNumberOfItemsToReserve ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::ReserveItemsCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve )
		// Cmd: Cancel reservation
		Result PartyMatchingQueueService::CancelReservationCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->CancelReservationCmd( InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::CancelReservationCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		// Cmd: Cancel reservation
		Result PartyMatchingQueueService::CancelReservationsCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->CancelReservationsCmd( InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::CancelReservationsCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		// Cmd: Dequeue a reserved item
		Result PartyMatchingQueueService::DequeueItemCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->DequeueItemCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::DequeueItemCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// C2S: Item error you should delete it
		Result PartyMatchingQueueService::MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->MatchingItemErrorC2SEvt( InRouteContext, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )


	}; // namespace Svr
}; // namespace BR


