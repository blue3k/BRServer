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
#include "ServerSystem/Transaction.h"
#include "Container/SFArray.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"




namespace SF {
namespace Svr {
namespace MatchingUtil
{

	PlayerRole GetPlayerRoleFromQueueComponentID(UINT componentID);
	UINT GetPartyMemberCountFromQueueComponentID(UINT componentID);
	Result GetQueueComponentIDMinMax(UINT matchingMemberCount, UINT& minComponentID, UINT& maxComponentID);
	UINT GetQueueCount(UINT matchingMemberCount);

	UINT GetQueueComponentID(UINT matchingMemberCount, UINT partyMemberCount, PlayerRole playerRole);

	UINT GetComponentIDFromClusterID(ClusterID clusterID);

}
} // namespace Svr 
} // namespace SF 

