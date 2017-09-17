////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GamePartyManager Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
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
			Result CreatePartyCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator );
			// C2S: Party instance notify of deletion
			Result PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount );

		}; // class GamePartyManagerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


