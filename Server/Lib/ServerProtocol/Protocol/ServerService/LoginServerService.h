////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : LoginServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"



namespace SF
{
 	class LoginServerService : public ServerServiceBase
	{
 		public:
		// Constructor
		LoginServerService ( ServerServiceInformation* pService );

		// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
		Result PlayerJoinedToGameServerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );
		// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
		Result KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InKickedPlayerID );

	}; // class LoginServerService : public ServerServiceBase


}; // namespace SF


