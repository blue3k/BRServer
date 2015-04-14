////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/Netdef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/PartyMatchingIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class PartyMatchingService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyPartyMatching* GetPolicyPartyMatching() { return GetPolicy<Policy::IPolicyPartyMatching>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyPartyMatching::ID_POLICY };
			// Constructor
			PartyMatchingService ( ServerServiceInformation* pService );


		}; // class PartyMatchingService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


