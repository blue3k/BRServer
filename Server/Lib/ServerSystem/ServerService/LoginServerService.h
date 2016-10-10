////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : LoginServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/LoginServerIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class LoginServerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyLoginServer* GetPolicyLoginServer() { return GetPolicy<Policy::IPolicyLoginServer>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyLoginServer::ID_POLICY };
			// Constructor
			LoginServerService ( ServerServiceInformation* pService );

			// Cmd: Notify user joind and see it's valid authticket instance
			Result PlayerJoinedToGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );
			// Cmd: Kick logged in player
			Result KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID );

		}; // class LoginServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


