////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstanceManager Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/GameInstanceManagerNetPolicy.h"



namespace SF
{
 	class GameInstanceManagerService : public ServerServiceBase
	{
 		public:
		// Constructor
		GameInstanceManagerService ( ServerServiceInformation* pService );

		// Cmd: Create a game instance
		Result CreateGameInstanceCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const VariableTable &InAttributes );

	}; // class GameInstanceManagerService : public ServerServiceBase


}; // namespace SF


