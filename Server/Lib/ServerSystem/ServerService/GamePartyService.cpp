////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameParty Message debug implementations
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
#include "ServerSystem/ServerService/GamePartyService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		GamePartyService::GamePartyService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyGameParty::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyGameParty::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyGameParty());
		}


		// Cmd: Join Party
		HRESULT GamePartyService::JoinPartyCmd( const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->JoinPartyCmd( InRouteContext, InTransactionID, InInviterID, InInvitedPlayer ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyService::JoinPartyCmd( const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		// Cmd: Kick player
		HRESULT GamePartyService::LeavePartyCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->LeavePartyCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyService::LeavePartyCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player
		HRESULT GamePartyService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Chatting message
		HRESULT GamePartyService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->ChatMessageC2SEvt( InRouteContext, InPlayerID, InChatMessage ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage )
		// C2S: Quick Chatting message
		HRESULT GamePartyService::QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const UINT32 &InQuickChatID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->QuickChatMessageC2SEvt( InRouteContext, InPlayerID, InQuickChatID ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyService::QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const UINT32 &InQuickChatID )
		// Cmd: Match
		HRESULT GamePartyService::StartGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->StartGameMatchCmd( InRouteContext, InTransactionID, InPlayerID, InMaxGamePlayers ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyService::StartGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers )
		// Cmd: Match
		HRESULT GamePartyService::CancelGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->CancelGameMatchCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GamePartyService::CancelGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace BR


