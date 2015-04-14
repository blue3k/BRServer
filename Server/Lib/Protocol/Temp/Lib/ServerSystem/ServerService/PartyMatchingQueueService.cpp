////////////////////////////////////////////////////////////////////////////////
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
#include "Common/BRBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
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
			static_assert(Policy::IPolicyPartyMatchingQueue::ID_POLICY == ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyPartyMatchingQueue());
		}


		// Cmd: Register match by party
		HRESULT PartyMatchingQueueService::RegisterPartyMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->RegisterPartyMatchingCmd( InContext, InRouteContext, InRouteHopCount, InPlayers ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::RegisterPartyMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		// Cmd: Register match alone
		HRESULT PartyMatchingQueueService::RegisterPlayerMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->RegisterPlayerMatchingCmd( InContext, InRouteContext, InRouteHopCount, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::RegisterPlayerMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID )
		// Cmd: update registration information
		HRESULT PartyMatchingQueueService::UpdateMatchingEntityUIDCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->UpdateMatchingEntityUIDCmd( InContext, InRouteContext, InRouteHopCount, InMatchingTicket, InPreviousUID ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::UpdateMatchingEntityUIDCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID )
		// Cmd: calcel registration
		HRESULT PartyMatchingQueueService::UnregisterMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->UnregisterMatchingCmd( InContext, InRouteContext, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::UnregisterMatchingCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		HRESULT PartyMatchingQueueService::ReserveItemCmd( const Context &InContext, const UINT16 &InRouteHopCount )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->ReserveItemCmd( InContext, InRouteContext, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::ReserveItemCmd( const Context &InContext, const UINT16 &InRouteHopCount )
		// Cmd: Reserve a item
		HRESULT PartyMatchingQueueService::ReserveItemsCmd( const Context &InContext, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->ReserveItemsCmd( InContext, InRouteContext, InRouteHopCount, InNumberOfItemsToReserve ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::ReserveItemsCmd( const Context &InContext, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve )
		// Cmd: Cancel reservation
		HRESULT PartyMatchingQueueService::CancelReservationCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->CancelReservationCmd( InContext, InRouteContext, InRouteHopCount, InTicketToCancel ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::CancelReservationCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		// Cmd: Cancel reservation
		HRESULT PartyMatchingQueueService::CancelReservationsCmd( const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->CancelReservationsCmd( InContext, InRouteContext, InRouteHopCount, InTicketToCancel ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::CancelReservationsCmd( const Context &InContext, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		// Cmd: Dequeue a reserved item
		HRESULT PartyMatchingQueueService::DequeueItemCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->DequeueItemCmd( InContext, InRouteContext, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::DequeueItemCmd( const Context &InContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// C2S: Item error you should delete it
		HRESULT PartyMatchingQueueService::MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyPartyMatchingQueue()->MatchingItemErrorC2SEvt( InRouteContext, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // HRESULT PartyMatchingQueueService::MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )


	}; // namespace Svr
}; // namespace BR


