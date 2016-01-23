////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : LoginServer Message debug implementations
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
#include "ServerSystem/ServerService/LoginServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		LoginServerService::LoginServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyLoginServer::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyLoginServer::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyLoginServer());
		}


		// Cmd: Notify user joind and see it's valid authticket instance
		HRESULT LoginServerService::PlayerJoinedToGameServerCmd( const Context &InContext, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyLoginServer()->PlayerJoinedToGameServerCmd( InContext, InRouteContext, InPlayerID, InAuthTicket ) );

		Proc_End:

			return hr;

		}; // HRESULT LoginServerService::PlayerJoinedToGameServerCmd( const Context &InContext, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		// Cmd: Kick logged in player
		HRESULT LoginServerService::KickPlayerCmd( const Context &InContext, const PlayerID &InKickedPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyLoginServer()->KickPlayerCmd( InContext, InRouteContext, InKickedPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT LoginServerService::KickPlayerCmd( const Context &InContext, const PlayerID &InKickedPlayerID )


	}; // namespace Svr
}; // namespace BR


