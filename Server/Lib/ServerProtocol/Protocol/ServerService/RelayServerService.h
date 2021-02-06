////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : RelayServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/RelayServerNetPolicy.h"



namespace SF
{
 	class RelayServerService : public ServerServiceBase
	{
 		public:
		// Constructor
		RelayServerService ( ServerServiceInformation* pService );

		// Cmd: Relay Instance
		Result CreateRelayInstanceCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore );
		// Cmd: Add a player to Relay
		Result AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore );
		// Cmd: Remove a player to Relay
		Result RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );

	}; // class RelayServerService : public ServerServiceBase


}; // namespace SF


