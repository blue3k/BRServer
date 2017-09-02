////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : LoginServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/LoginServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		LoginServerService::LoginServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Notify user joind and see it's valid authticket instance
		Result LoginServerService::PlayerJoinedToGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyLoginServer(GetConnection()).PlayerJoinedToGameServerCmd( InRouteContext, InTransactionID, InPlayerID, InAuthTicket ) );

		Proc_End:

			return hr;

		}; // Result LoginServerService::PlayerJoinedToGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		// Cmd: Kick logged in player
		Result LoginServerService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyLoginServer(GetConnection()).KickPlayerCmd( InRouteContext, InTransactionID, InKickedPlayerID ) );

		Proc_End:

			return hr;

		}; // Result LoginServerService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID )


	}; // namespace Svr
}; // namespace SF


