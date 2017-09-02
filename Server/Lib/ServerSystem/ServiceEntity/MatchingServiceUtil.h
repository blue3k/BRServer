////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : game matching service utility
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"




namespace SF {
namespace Svr {
namespace MatchingUtil
{

	PlayerRole GetPlayerRoleFromQueueComponentID(uint componentID);
	uint GetPartyMemberCountFromQueueComponentID(uint componentID);
	Result GetQueueComponentIDMinMax(uint matchingMemberCount, uint& minComponentID, uint& maxComponentID);
	uint GetQueueCount(uint matchingMemberCount);

	uint GetQueueComponentID(uint matchingMemberCount, uint partyMemberCount, PlayerRole playerRole);

	uint GetComponentIDFromClusterID(ClusterID clusterID);

}
} // namespace Svr 
} // namespace SF 

