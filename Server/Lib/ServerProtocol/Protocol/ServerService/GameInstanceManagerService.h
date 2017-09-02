////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameInstanceManager Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Protocol/Policy/GameInstanceManagerNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class GameInstanceManagerService : public ServerServiceBase
		{
 			public:
			// Constructor
			GameInstanceManagerService ( ServerServiceInformation* pService );

			// Cmd: Create a game instance
			Result CreateGameCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer );
			// C2S: Game instance notify of deletion
			Result GameDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount );

		}; // class GameInstanceManagerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


