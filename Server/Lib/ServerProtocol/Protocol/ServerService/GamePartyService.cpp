////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameParty Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/GamePartyService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		GamePartyService::GamePartyService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Join party
		Result GamePartyService::JoinPartyCmd( const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameParty(GetConnection()).JoinPartyCmd( InRouteContext, InTransactionID, InInviterID, InInvitedPlayer ) );

			return hr;

		}; // Result GamePartyService::JoinPartyCmd( const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		// Cmd: Event for player left.
		Result GamePartyService::LeavePartyCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameParty(GetConnection()).LeavePartyCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result GamePartyService::LeavePartyCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player
		Result GamePartyService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameParty(GetConnection()).KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

			return hr;

		}; // Result GamePartyService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Send chat message to server.
		Result GamePartyService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameParty(GetConnection()).ChatMessageC2SEvt( InRouteContext, InPlayerID, InChatMessage ) );

			return hr;

		}; // Result GamePartyService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage )
		// C2S: Quick Chatting message
		Result GamePartyService::QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameParty(GetConnection()).QuickChatMessageC2SEvt( InRouteContext, InPlayerID, InQuickChatID ) );

			return hr;

		}; // Result GamePartyService::QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
		// Cmd: Start party game matching
		Result GamePartyService::StartGameMatchCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameParty(GetConnection()).StartGameMatchCmd( InRouteContext, InTransactionID, InPlayerID, InMaxGamePlayers ) );

			return hr;

		}; // Result GamePartyService::StartGameMatchCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
		// Cmd: Cancel game matching
		Result GamePartyService::CancelGameMatchCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			ScopeContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyGameParty(GetConnection()).CancelGameMatchCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result GamePartyService::CancelGameMatchCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace SF


