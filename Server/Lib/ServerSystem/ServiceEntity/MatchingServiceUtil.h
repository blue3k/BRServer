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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/ArrayUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"




namespace BR {
namespace Svr {
namespace MatchingUtil
{

	PlayerRole GetPlayerRoleFromQueueComponentID(UINT componentID);
	UINT GetPartyMemberCountFromQueueComponentID(UINT componentID);
	HRESULT GetQueueComponentIDMinMax(UINT matchingMemberCount, UINT& minComponentID, UINT& maxComponentID);
	UINT GetQueueCount(UINT matchingMemberCount);

	UINT GetQueueComponentID(UINT matchingMemberCount, UINT partyMemberCount, PlayerRole playerRole);

	UINT GetComponentIDFromClusterID(ClusterID clusterID);

}
} // namespace Svr 
} // namespace BR 

