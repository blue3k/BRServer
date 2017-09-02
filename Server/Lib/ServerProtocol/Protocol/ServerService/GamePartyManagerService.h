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
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GamePartyManagerIPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class GamePartyManagerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyGamePartyManager* GetPolicyGamePartyManager() { return GetPolicy<Policy::IPolicyGamePartyManager>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyGamePartyManager::ID_POLICY };
			// Constructor
			GamePartyManagerService ( ServerServiceInformation* pService );

			// Cmd: Create a party instance
			Result CreatePartyCmd( const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerInformation &InCreator );
			// C2S: Party instance notify of deletion
			Result PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const uint16_t &InRouteHopCount );

		}; // class GamePartyManagerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


