////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameParty Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
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


		// Cmd: Join Party
		Result GamePartyService::JoinPartyCmd( const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameParty(GetConnection()).JoinPartyCmd( InRouteContext, InTransactionID, InInviterID, InInvitedPlayer ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::JoinPartyCmd( const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		// Cmd: Kick player
		Result GamePartyService::LeavePartyCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameParty(GetConnection()).LeavePartyCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::LeavePartyCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player
		Result GamePartyService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameParty(GetConnection()).KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Chatting message
		Result GamePartyService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameParty(GetConnection()).ChatMessageC2SEvt( InRouteContext, InPlayerID, InChatMessage ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage )
		// C2S: Quick Chatting message
		Result GamePartyService::QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameParty(GetConnection()).QuickChatMessageC2SEvt( InRouteContext, InPlayerID, InQuickChatID ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
		// Cmd: Match
		Result GamePartyService::StartGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameParty(GetConnection()).StartGameMatchCmd( InRouteContext, InTransactionID, InPlayerID, InMaxGamePlayers ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::StartGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
		// Cmd: Match
		Result GamePartyService::CancelGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameParty(GetConnection()).CancelGameMatchCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::CancelGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace SF


