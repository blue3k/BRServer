////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/PartyMatchingQueueService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		PartyMatchingQueueService::PartyMatchingQueueService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Register a party for matching
		Result PartyMatchingQueueService::RegisterPartyMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).RegisterPartyMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayers ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::RegisterPartyMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
		// Cmd: Register a player for matching
		Result PartyMatchingQueueService::RegisterPlayerMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).RegisterPlayerMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::RegisterPlayerMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
		// Cmd: [Deprecated] update registration information
		Result PartyMatchingQueueService::UpdateMatchingEntityUIDCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).UpdateMatchingEntityUIDCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket, InPreviousUID ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::UpdateMatchingEntityUIDCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
		// Cmd: calcel registration
		Result PartyMatchingQueueService::UnregisterMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).UnregisterMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::UnregisterMatchingCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: Reserve a item
		Result PartyMatchingQueueService::ReserveItemCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).ReserveItemCmd( InRouteContext, InTransactionID, InRouteHopCount ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::ReserveItemCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount )
		// Cmd: Reserve a item
		Result PartyMatchingQueueService::ReserveItemsCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).ReserveItemsCmd( InRouteContext, InTransactionID, InRouteHopCount, InNumberOfItemsToReserve ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::ReserveItemsCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
		// Cmd: Cancel reservation
		Result PartyMatchingQueueService::CancelReservationCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).CancelReservationCmd( InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::CancelReservationCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
		// Cmd: Cancel reservation
		Result PartyMatchingQueueService::CancelReservationsCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).CancelReservationsCmd( InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::CancelReservationsCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
		// Cmd: Dequeue a reserved item
		Result PartyMatchingQueueService::DequeueItemCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).DequeueItemCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::DequeueItemCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		// C2S: Item error you should delete it
		Result PartyMatchingQueueService::MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyPartyMatchingQueue(GetConnection()).MatchingItemErrorC2SEvt( InRouteContext, InRouteHopCount, InMatchingTicket ) );

		Proc_End:

			return hr;

		}; // Result PartyMatchingQueueService::MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )


	}; // namespace Svr
}; // namespace SF


