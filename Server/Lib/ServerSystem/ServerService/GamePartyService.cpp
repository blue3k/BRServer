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
		Result GamePartyService::JoinPartyCmd( const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->JoinPartyCmd( InRouteContext, InTransactionID, InInviterID, InInvitedPlayer ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::JoinPartyCmd( const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		// Cmd: Kick player
		Result GamePartyService::LeavePartyCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->LeavePartyCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::LeavePartyCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player
		Result GamePartyService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Chatting message
		Result GamePartyService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->ChatMessageC2SEvt( InRouteContext, InPlayerID, InChatMessage ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const char* InChatMessage )
		// C2S: Quick Chatting message
		Result GamePartyService::QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const UINT32 &InQuickChatID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->QuickChatMessageC2SEvt( InRouteContext, InPlayerID, InQuickChatID ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::QuickChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InPlayerID, const UINT32 &InQuickChatID )
		// Cmd: Match
		Result GamePartyService::StartGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->StartGameMatchCmd( InRouteContext, InTransactionID, InPlayerID, InMaxGamePlayers ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::StartGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers )
		// Cmd: Match
		Result GamePartyService::CancelGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyGameParty()->CancelGameMatchCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GamePartyService::CancelGameMatchCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )


	}; // namespace Svr
}; // namespace BR


