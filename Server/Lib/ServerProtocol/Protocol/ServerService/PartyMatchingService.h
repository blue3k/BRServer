////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : PartyMatching Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/PartyMatchingIPolicy.h"



namespace SF
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
}; // namespace SF


