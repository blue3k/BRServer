////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
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
#include "ServerSystem/ServerService/GameInstanceService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		GameInstanceService::GameInstanceService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyGameInstance::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyGameInstance::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyGameInstance());
		}


		// C2S: Game instance deletion
		Result GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->DeleteGameC2SEvt( InRouteContext ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
		// Cmd: Join Game
		Result GameInstanceService::JoinGameCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->JoinGameCmd( InRouteContext, InTransactionID, InPlayer, InTicket, InRequestedRole ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::JoinGameCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole )
		// C2S: Change configue preset
		Result GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const UINT32 &InPresetID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->SetConfigPresetC2SEvt( InRouteContext, InPresetID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const UINT32 &InPresetID )
		// Cmd: Leave Game
		Result GameInstanceService::LeaveGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->LeaveGameCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::LeaveGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player
		Result GameInstanceService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Assign role
		Result GameInstanceService::AssignRoleCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->AssignRoleCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::AssignRoleCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// C2S: Chatting message
		Result GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->ChatMessageC2SEvt( InRouteContext, InPlayerID, InRole, InChatMessage ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage )
		// Cmd: Advance game
		Result GameInstanceService::AdvanceGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->AdvanceGameCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::AdvanceGameCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: *Vote game advance
		Result GameInstanceService::VoteGameAdvanceCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->VoteGameAdvanceCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::VoteGameAdvanceCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Assign role
		Result GameInstanceService::VoteCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->VoteCmd( InRouteContext, InTransactionID, InPlayerID, InVoteTarget, InActionSerial ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::VoteCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		// Cmd: Play again with the current players
		Result GameInstanceService::GamePlayAgainCmd( const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->GamePlayAgainCmd( InRouteContext, InTransactionID, InLeadPlayer, InPartyUID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::GamePlayAgainCmd( const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID )
		// Cmd: Player. revive himself
		Result GameInstanceService::GameRevealPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->GameRevealPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InTargetPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::GameRevealPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Player. revive himself
		Result GameInstanceService::GamePlayerReviveCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->GamePlayerReviveCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameInstanceService::GamePlayerReviveCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace BR


