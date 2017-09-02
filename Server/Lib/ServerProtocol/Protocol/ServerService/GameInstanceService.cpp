////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
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


		// C2S: Game instance deletion
		Result GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).DeleteGameC2SEvt( InRouteContext ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
		// Cmd: Join Game
		Result GameInstanceService::JoinGameCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const uint8_t &InRequestedRole )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).JoinGameCmd( InRouteContext, InTransactionID, InPlayer, InTicket, InRequestedRole ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::JoinGameCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const uint8_t &InRequestedRole )
		// C2S: Change configue preset
		Result GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InPresetID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).SetConfigPresetC2SEvt( InRouteContext, InPresetID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const uint32_t &InPresetID )
		// Cmd: Leave Game
		Result GameInstanceService::LeaveGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).LeaveGameCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::LeaveGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player
		Result GameInstanceService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Assign role
		Result GameInstanceService::AssignRoleCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).AssignRoleCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::AssignRoleCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// C2S: Chatting message
		Result GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).ChatMessageC2SEvt( InRouteContext, InPlayerID, InRole, InChatMessage ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
		// Cmd: Advance game
		Result GameInstanceService::AdvanceGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).AdvanceGameCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::AdvanceGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: *Vote game advance
		Result GameInstanceService::VoteGameAdvanceCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).VoteGameAdvanceCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::VoteGameAdvanceCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Assign role
		Result GameInstanceService::VoteCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).VoteCmd( InRouteContext, InTransactionID, InPlayerID, InVoteTarget, InActionSerial ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::VoteCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
		// Cmd: Play again with the current players
		Result GameInstanceService::GamePlayAgainCmd( const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).GamePlayAgainCmd( InRouteContext, InTransactionID, InLeadPlayer, InPartyUID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::GamePlayAgainCmd( const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
		// Cmd: Player. revive himself
		Result GameInstanceService::GameRevealPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).GameRevealPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InTargetPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::GameRevealPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Player. revive himself
		Result GameInstanceService::GamePlayerReviveCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyGameInstance(GetConnection()).GamePlayerReviveCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::GamePlayerReviveCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace SF


