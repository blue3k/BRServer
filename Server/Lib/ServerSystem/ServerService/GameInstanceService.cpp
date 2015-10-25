﻿////////////////////////////////////////////////////////////////////////////////
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
#include "Common/BRBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
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
			static_assert(Policy::IPolicyGameInstance::ID_POLICY == ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyGameInstance());
		}


		// C2S: Game instance deletion
		HRESULT GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->DeleteGameC2SEvt( InRouteContext ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::DeleteGameC2SEvt( const EntityID &InSenderEntityID )
		// Cmd: Join Game
		HRESULT GameInstanceService::JoinGameCmd( const Context &InContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->JoinGameCmd( InContext, InRouteContext, InPlayer, InTicket, InRequestedRole ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::JoinGameCmd( const Context &InContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole )
		// C2S: Change configue preset
		HRESULT GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const UINT32 &InPresetID )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->SetConfigPresetC2SEvt( InRouteContext, InPresetID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::SetConfigPresetC2SEvt( const EntityID &InSenderEntityID, const UINT32 &InPresetID )
		// Cmd: Leave Game
		HRESULT GameInstanceService::LeaveGameCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->LeaveGameCmd( InContext, InRouteContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::LeaveGameCmd( const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: Kick player
		HRESULT GameInstanceService::KickPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->KickPlayerCmd( InContext, InRouteContext, InPlayerID, InPlayerToKick ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::KickPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Assign role
		HRESULT GameInstanceService::AssignRoleCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->AssignRoleCmd( InContext, InRouteContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::AssignRoleCmd( const Context &InContext, const PlayerID &InPlayerID )
		// C2S: Chatting message
		HRESULT GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage )
		{
 			HRESULT hr = S_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->ChatMessageC2SEvt( InRouteContext, InPlayerID, InRole, InChatMessage ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage )
		// Cmd: Advance game
		HRESULT GameInstanceService::AdvanceGameCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->AdvanceGameCmd( InContext, InRouteContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::AdvanceGameCmd( const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: *Vote game advance
		HRESULT GameInstanceService::VoteGameAdvanceCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->VoteGameAdvanceCmd( InContext, InRouteContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::VoteGameAdvanceCmd( const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: Assign role
		HRESULT GameInstanceService::VoteCmd( const Context &InContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->VoteCmd( InContext, InRouteContext, InPlayerID, InVoteTarget, InActionSerial ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::VoteCmd( const Context &InContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		// Cmd: Play again with the current players
		HRESULT GameInstanceService::GamePlayAgainCmd( const Context &InContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->GamePlayAgainCmd( InContext, InRouteContext, InLeadPlayer, InPartyUID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::GamePlayAgainCmd( const Context &InContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID )
		// Cmd: Player. revive himself
		HRESULT GameInstanceService::GameRevealPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->GameRevealPlayerCmd( InContext, InRouteContext, InPlayerID, InTargetPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::GameRevealPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Player. revive himself
		HRESULT GameInstanceService::GamePlayerReviveCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameInstance()->GamePlayerReviveCmd( InContext, InRouteContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameInstanceService::GamePlayerReviveCmd( const Context &InContext, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace BR

