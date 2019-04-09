////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GamePartyManager Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/GamePartyManagerNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class GamePartyManagerService : public ServerServiceBase
		{
 			public:
			// Constructor
			GamePartyManagerService ( ServerServiceInformation* pService );

			// Cmd: Create a party instance
			Result CreatePartyCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator );
			// C2S: Party instance notify of deletion. Sent by party instance
			Result PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount );

		}; // class GamePartyManagerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


