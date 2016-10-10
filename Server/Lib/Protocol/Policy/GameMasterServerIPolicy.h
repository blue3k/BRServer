////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : GameMasterServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyGameMasterServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_GAMEMASTERSERVER };

			// Constructor
			ISvrPolicyGameMasterServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Player entered
			// C2S: Player leaved

		}; // class ISvrPolicyGameMasterServer : public Net::INetPolicy


		class IPolicyGameMasterServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAMEMASTERSERVER };

			// Constructor
			IPolicyGameMasterServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Player entered
			virtual Result PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName ) = 0;
			// C2S: Player leaved
			virtual Result PlayerLeavedC2SEvt( const AccountID &InPlayerID ) = 0;

		}; // class IPolicyGameMasterServer : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


