////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMasterServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class GameMasterServerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyGameMasterServer* GetPolicyGameMasterServer() { return GetPolicy<Policy::IPolicyGameMasterServer>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyGameMasterServer::ID_POLICY };
			// Constructor
			GameMasterServerService ( ServerServiceInformation* pService );

			// C2S: Player entered
			Result PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName );
			// C2S: Player leaved
			Result PlayerLeavedC2SEvt( const AccountID &InPlayerID );

		}; // class GameMasterServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


