////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/ServerService/PartyMatchingQueueService.h"
#include "SvrTrace.h"



namespace SF
{
 	PartyMatchingQueueService::PartyMatchingQueueService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Register a party for matching
	Result PartyMatchingQueueService::RegisterPartyMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).RegisterPartyMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayers ) );

		return hr;

	}; // Result PartyMatchingQueueService::RegisterPartyMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
	// Cmd: Register a player for matching
	Result PartyMatchingQueueService::RegisterPlayerMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).RegisterPlayerMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InPlayerID ) );

		return hr;

	}; // Result PartyMatchingQueueService::RegisterPlayerMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
	// Cmd: [Deprecated] update registration information
	Result PartyMatchingQueueService::UpdateMatchingEntityUIDCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).UpdateMatchingEntityUIDCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket, InPreviousUID ) );

		return hr;

	}; // Result PartyMatchingQueueService::UpdateMatchingEntityUIDCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
	// Cmd: calcel registration
	Result PartyMatchingQueueService::UnregisterMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).UnregisterMatchingCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket ) );

		return hr;

	}; // Result PartyMatchingQueueService::UnregisterMatchingCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
	// Cmd: Reserve a item
	Result PartyMatchingQueueService::ReserveItemCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).ReserveItemCmd( InRouteContext, InTransactionID, InRouteHopCount ) );

		return hr;

	}; // Result PartyMatchingQueueService::ReserveItemCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount )
	// Cmd: Reserve a item
	Result PartyMatchingQueueService::ReserveItemsCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).ReserveItemsCmd( InRouteContext, InTransactionID, InRouteHopCount, InNumberOfItemsToReserve ) );

		return hr;

	}; // Result PartyMatchingQueueService::ReserveItemsCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
	// Cmd: Cancel reservation
	Result PartyMatchingQueueService::CancelReservationCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).CancelReservationCmd( InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel ) );

		return hr;

	}; // Result PartyMatchingQueueService::CancelReservationCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
	// Cmd: Cancel reservation
	Result PartyMatchingQueueService::CancelReservationsCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).CancelReservationsCmd( InRouteContext, InTransactionID, InRouteHopCount, InTicketToCancel ) );

		return hr;

	}; // Result PartyMatchingQueueService::CancelReservationsCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
	// Cmd: Dequeue a reserved item
	Result PartyMatchingQueueService::DequeueItemCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).DequeueItemCmd( InRouteContext, InTransactionID, InRouteHopCount, InMatchingTicket ) );

		return hr;

	}; // Result PartyMatchingQueueService::DequeueItemCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
	// C2S: Item error you should delete it
	Result PartyMatchingQueueService::MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyPartyMatchingQueue(GetTargetEndpoint()).MatchingItemErrorC2SEvt( InRouteContext, InRouteHopCount, InMatchingTicket ) );

		return hr;

	}; // Result PartyMatchingQueueService::MatchingItemErrorC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )


}; // namespace SF


