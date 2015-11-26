////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameInstanceManagerIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class GameInstanceManagerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyGameInstanceManager* GetPolicyGameInstanceManager() { return GetPolicy<Policy::IPolicyGameInstanceManager>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyGameInstanceManager::ID_POLICY };
			// Constructor
			GameInstanceManagerService ( ServerServiceInformation* pService );

			// Cmd: Create a game instance
			HRESULT CreateGameCmd( const Context &InContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer );
			// C2S: Game instance notify of deletion
			HRESULT GameDeletedC2SEvt( const EntityID &InSenderEntityID, const UINT16 &InRouteHopCount );

		}; // class GameInstanceManagerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


