////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/ServerService/GameInstanceService.h"
#include "SvrTrace.h"



namespace SF
{
 	GameInstanceService::GameInstanceService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// C2S: Nitify that a game instance is deleted. Game instance send this message to manager before it destroy itself.
	Result GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).DeleteGameC2SEvt( InRouteContext ) );

		return hr;

	}; // Result GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
	// Cmd: Join to a game instance. You can call multiple times, but it would be waste
	Result GameInstanceService::JoinGameInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).JoinGameInstanceCmd( InRouteContext, InTransactionID, InPlayer ) );

		return hr;

	}; // Result GameInstanceService::JoinGameInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer )
	// C2S: For debug purpose, change configue preset. There is a game setting table. you can switch between those setting value.
	Result GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InPresetID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).SetConfigPresetC2SEvt( InRouteContext, InPresetID ) );

		return hr;

	}; // Result GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InPresetID )
	// Cmd: Leave game instance.
	Result GameInstanceService::LeaveGameInstanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).LeaveGameInstanceCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result GameInstanceService::LeaveGameInstanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// C2S: Player Movement
	Result GameInstanceService::PlayerMovementC2SEvt( const EntityID &InSenderEntityID, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).PlayerMovementC2SEvt( InRouteContext, InGameInsUID, InPlayerID, InAttributes ) );

		return hr;

	}; // Result GameInstanceService::PlayerMovementC2SEvt( const EntityID &InSenderEntityID, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes )
	// Cmd: Kick player with given ID
	Result GameInstanceService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

		return hr;

	}; // Result GameInstanceService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	// Cmd: Join to a game instance. You can call multiple times, but it would be waste
	Result GameInstanceService::JoinGameCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).JoinGameCmd( InRouteContext, InTransactionID, InPlayer, InRequestedRole ) );

		return hr;

	}; // Result GameInstanceService::JoinGameCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
	// Cmd: Assign new roles to all players.
	Result GameInstanceService::AssignRoleCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).AssignRoleCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result GameInstanceService::AssignRoleCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// C2S: in-game chatting message.
	Result GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).ChatMessageC2SEvt( InRouteContext, InPlayerID, InRole, InChatMessage ) );

		return hr;

	}; // Result GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
	// Cmd: Advance game
	Result GameInstanceService::AdvanceGameCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).AdvanceGameCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result GameInstanceService::AdvanceGameCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: *Vote game advance
	Result GameInstanceService::VoteGameAdvanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).VoteGameAdvanceCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result GameInstanceService::VoteGameAdvanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Assign role
	Result GameInstanceService::VoteCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).VoteCmd( InRouteContext, InTransactionID, InPlayerID, InVoteTarget, InActionSerial ) );

		return hr;

	}; // Result GameInstanceService::VoteCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
	// Cmd: Play again with the current players
	Result GameInstanceService::GamePlayAgainCmd( const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).GamePlayAgainCmd( InRouteContext, InTransactionID, InLeadPlayer, InPartyUID ) );

		return hr;

	}; // Result GameInstanceService::GamePlayAgainCmd( const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
	// Cmd: Player. revive himself
	Result GameInstanceService::GameRevealPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).GameRevealPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InTargetPlayerID ) );

		return hr;

	}; // Result GameInstanceService::GameRevealPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
	// Cmd: Player. revive himself
	Result GameInstanceService::GamePlayerReviveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyGameInstance(GetTargetEndpoint()).GamePlayerReviveCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result GameInstanceService::GamePlayerReviveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )


}; // namespace SF


