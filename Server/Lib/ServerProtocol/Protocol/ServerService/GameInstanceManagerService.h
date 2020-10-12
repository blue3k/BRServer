////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
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
			Result CreateGameCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer );
			// Cmd: Search game instance
			Result SearchGameInstanceCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InSearchKeyword );
			// C2S: Game instance notification of deletion
			Result GameDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount );

		}; // class GameInstanceManagerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


