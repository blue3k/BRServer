﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : PartyMatching Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/PartyMatchingService.h"
#include "ServerSystem/SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		PartyMatchingService::PartyMatchingService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyPartyMatching::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyPartyMatching::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyPartyMatching());
		}




	}; // namespace Svr
}; // namespace SF


