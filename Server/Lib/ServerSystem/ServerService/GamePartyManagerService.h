////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GamePartyManagerIPolicy.h"



namespace BR
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
			HRESULT CreatePartyCmd( const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator );
			// C2S: Party instance notify of deletion
			HRESULT PartyDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount );

		}; // class GamePartyManagerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


