////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/GameInstanceService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		GameInstanceService::GameInstanceService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// C2S: Nitify that a game instance is deleted. Game instance send this message to manager before it destroy itself.
		Result GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).DeleteGameC2SEvt( InRouteContext ) );

			return hr;

		}; // Result GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
		// Cmd: Join to a game instance. You can call multiple times, but it would be waste
		Result GameInstanceService::JoinGameCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).JoinGameCmd( InRouteContext, InTransactionID, InPlayer, InRequestedRole ) );

			return hr;

		}; // Result GameInstanceService::JoinGameCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
		// C2S: For debug purpose, change configue preset. There is a game setting table. you can switch between those setting value.
		Result GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InPresetID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).SetConfigPresetC2SEvt( InRouteContext, InPresetID ) );

			return hr;

		}; // Result GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InPresetID )
		// Cmd: Leave game instance.
		Result GameInstanceService::LeaveGameCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).LeaveGameCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result GameInstanceService::LeaveGameCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player with given ID
		Result GameInstanceService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

			return hr;

		}; // Result GameInstanceService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Assign new roles to all players.
		Result GameInstanceService::AssignRoleCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).AssignRoleCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result GameInstanceService::AssignRoleCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// C2S: in-game chatting message.
		Result GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).ChatMessageC2SEvt( InRouteContext, InPlayerID, InRole, InChatMessage ) );

			return hr;

		}; // Result GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
		// Cmd: Advance game
		Result GameInstanceService::AdvanceGameCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).AdvanceGameCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result GameInstanceService::AdvanceGameCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: *Vote game advance
		Result GameInstanceService::VoteGameAdvanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).VoteGameAdvanceCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result GameInstanceService::VoteGameAdvanceCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Assign role
		Result GameInstanceService::VoteCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).VoteCmd( InRouteContext, InTransactionID, InPlayerID, InVoteTarget, InActionSerial ) );

			return hr;

		}; // Result GameInstanceService::VoteCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
		// Cmd: Play again with the current players
		Result GameInstanceService::GamePlayAgainCmd( const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).GamePlayAgainCmd( InRouteContext, InTransactionID, InLeadPlayer, InPartyUID ) );

			return hr;

		}; // Result GameInstanceService::GamePlayAgainCmd( const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
		// Cmd: Player. revive himself
		Result GameInstanceService::GameRevealPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).GameRevealPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InTargetPlayerID ) );

			return hr;

		}; // Result GameInstanceService::GameRevealPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Player. revive himself
		Result GameInstanceService::GamePlayerReviveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).GamePlayerReviveCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result GameInstanceService::GamePlayerReviveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace SF


