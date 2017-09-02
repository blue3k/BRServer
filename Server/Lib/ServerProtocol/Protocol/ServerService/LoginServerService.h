////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : LoginServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class LoginServerService : public ServerServiceBase
		{
 			public:
			// Constructor
			LoginServerService ( ServerServiceInformation* pService );

			// Cmd: Notify user joind and see it's valid authticket instance
			Result PlayerJoinedToGameServerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );
			// Cmd: Kick logged in player
			Result KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID );

		}; // class LoginServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


