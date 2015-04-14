﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BRBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/PartyMatchingService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		PartyMatchingService::PartyMatchingService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyPartyMatching::ID_POLICY)
		{
			static_assert(Policy::IPolicyPartyMatching::ID_POLICY == ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyPartyMatching());
		}




	}; // namespace Svr
}; // namespace BR


