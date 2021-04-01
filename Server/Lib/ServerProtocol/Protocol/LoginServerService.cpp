////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : LoginServer Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/LoginServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	LoginServerService::LoginServerService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
	Result LoginServerService::PlayerJoinedToGameServerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyLoginServer(GetTargetEndpoint()).PlayerJoinedToGameServerCmd( InRouteContext, InTransactionID, InPlayerID, InAuthTicket ) );

		return hr;

	}; // Result LoginServerService::PlayerJoinedToGameServerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
	// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
	Result LoginServerService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InKickedPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyLoginServer(GetTargetEndpoint()).KickPlayerCmd( InRouteContext, InTransactionID, InKickedPlayerID ) );

		return hr;

	}; // Result LoginServerService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InKickedPlayerID )


}; // namespace SF


