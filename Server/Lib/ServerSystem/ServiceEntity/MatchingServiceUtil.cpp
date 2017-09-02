////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game matching service utility
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Memory/MemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "ServerSystem/ServiceEntity/MatchingServiceUtil.h"
#include "Component/ServerComponent.h"



namespace SF {
namespace Svr {
namespace MatchingUtil {

	PlayerRole GetPlayerRoleFromQueueComponentID(uint componentID)
	{
		switch (componentID)
		{
		case ServerComponentID_MatchingQueueWatcherService_4x1:
		case ServerComponentID_MatchingQueueWatcherService_4x2:
		case ServerComponentID_MatchingQueueWatcherService_4x3:		return PlayerRole::None;
		case ServerComponentID_MatchingQueueWatcherService_4x1S:	return PlayerRole::Seer;
		case ServerComponentID_MatchingQueueWatcherService_4x1W:	return PlayerRole::Werewolf;
		case ServerComponentID_MatchingQueueWatcherService_8x1:
		case ServerComponentID_MatchingQueueWatcherService_8x2:
		case ServerComponentID_MatchingQueueWatcherService_8x3:
		case ServerComponentID_MatchingQueueWatcherService_8x4:
		case ServerComponentID_MatchingQueueWatcherService_8x5:
		case ServerComponentID_MatchingQueueWatcherService_8x6:
		case ServerComponentID_MatchingQueueWatcherService_8x7:		return PlayerRole::None;
		case ServerComponentID_MatchingQueueWatcherService_8x1S:	return PlayerRole::Seer;
		case ServerComponentID_MatchingQueueWatcherService_8x1W:	return PlayerRole::Werewolf;
		default:
			Assert(false);
			return PlayerRole::None;;
		}
	}


	uint GetPartyMemberCountFromQueueComponentID(uint componentID)
	{
		switch (componentID)
		{
		case ServerComponentID_MatchingQueueWatcherService_4x1:		return 1;
		case ServerComponentID_MatchingQueueWatcherService_4x2:		return 2;
		case ServerComponentID_MatchingQueueWatcherService_4x3:		return 3;
		case ServerComponentID_MatchingQueueWatcherService_4x1S:	return 1;
		case ServerComponentID_MatchingQueueWatcherService_4x1W:	return 1;
		case ServerComponentID_MatchingQueueWatcherService_8x1:		return 1;
		case ServerComponentID_MatchingQueueWatcherService_8x2:		return 2;
		case ServerComponentID_MatchingQueueWatcherService_8x3:		return 3;
		case ServerComponentID_MatchingQueueWatcherService_8x4:		return 4;
		case ServerComponentID_MatchingQueueWatcherService_8x5:		return 5;
		case ServerComponentID_MatchingQueueWatcherService_8x6:		return 6;
		case ServerComponentID_MatchingQueueWatcherService_8x7:		return 7;
		case ServerComponentID_MatchingQueueWatcherService_8x1S:	return 1;
		case ServerComponentID_MatchingQueueWatcherService_8x1W:	return 1;
		default:
			Assert(false);
			return 1;
		}
	}
	Result GetQueueComponentIDMinMax(uint matchingMemberCount, uint& minComponentID, uint& maxComponentID)
	{
		switch (matchingMemberCount)
		{
		case 4:
			minComponentID = ServerComponentID_MatchingQueueWatcherService_4x1;
			maxComponentID = ServerComponentID_MatchingQueueWatcherService_4x1W;
			break;

		case 8:
			minComponentID = ServerComponentID_MatchingQueueWatcherService_8x1;
			maxComponentID = ServerComponentID_MatchingQueueWatcherService_8x1W;
			break;

		default:
			minComponentID = maxComponentID = 0;
			return ResultCode::UNEXPECTED;
		};

		return ResultCode::SUCCESS;
	}

	uint GetQueueCount(uint matchingMemberCount)
	{
		uint minComponentID, maxComponentID;

		if (!(GetQueueComponentIDMinMax(matchingMemberCount, minComponentID, maxComponentID)))
		{
			Assert(false);
			return (uint)0;
		}

		return maxComponentID - minComponentID + 1;
	}


	uint GetQueueComponentID(uint matchingMemberCount, uint partyMemberCount, PlayerRole playerRole)
	{
		uint minComponentID, maxComponentID;

		if (!(GetQueueComponentIDMinMax(matchingMemberCount, minComponentID, maxComponentID)))
		{
			Assert(false);
			return (uint)-1;
		}

		uint componentID = minComponentID;


		switch (playerRole)
		{
		case PlayerRole::Seer:      componentID = maxComponentID - 1;  break;
		case PlayerRole::Werewolf:  componentID = maxComponentID;  break;
		default:                    componentID = minComponentID + partyMemberCount - 1;  break;
		}


		if (componentID < minComponentID)
		{
			Assert(false);
			componentID = minComponentID;
		}

		if (componentID > maxComponentID)
		{
			Assert(false);
			componentID = maxComponentID;
		}

		return componentID;
	}


	uint GetComponentIDFromClusterID(ClusterID clusterID)
	{
		switch (clusterID)
		{
		case ClusterID::MatchingQueue_Game_4x1:		return ServerComponentID_MatchingQueueWatcherService_4x1;
		case ClusterID::MatchingQueue_Game_4x2:		return ServerComponentID_MatchingQueueWatcherService_4x2;
		case ClusterID::MatchingQueue_Game_4x3:		return ServerComponentID_MatchingQueueWatcherService_4x3;
		case ClusterID::MatchingQueue_Game_4x1S:	return ServerComponentID_MatchingQueueWatcherService_4x1S;
		case ClusterID::MatchingQueue_Game_4x1W:	return ServerComponentID_MatchingQueueWatcherService_4x1W;
		case ClusterID::MatchingQueue_Game_8x1:		return ServerComponentID_MatchingQueueWatcherService_8x1;
		case ClusterID::MatchingQueue_Game_8x2:		return ServerComponentID_MatchingQueueWatcherService_8x2;
		case ClusterID::MatchingQueue_Game_8x3:		return ServerComponentID_MatchingQueueWatcherService_8x3;
		case ClusterID::MatchingQueue_Game_8x4:		return ServerComponentID_MatchingQueueWatcherService_8x4;
		case ClusterID::MatchingQueue_Game_8x5:		return ServerComponentID_MatchingQueueWatcherService_8x5;
		case ClusterID::MatchingQueue_Game_8x6:		return ServerComponentID_MatchingQueueWatcherService_8x6;
		case ClusterID::MatchingQueue_Game_8x7:		return ServerComponentID_MatchingQueueWatcherService_8x7;
		case ClusterID::MatchingQueue_Game_8x1S:	return ServerComponentID_MatchingQueueWatcherService_8x1S;
		case ClusterID::MatchingQueue_Game_8x1W:	return ServerComponentID_MatchingQueueWatcherService_8x1W;
		default:
			Assert(false);
			svrTrace(Trace::TRC_ERROR, "Invalid cluster id for compoent %0%", clusterID);
			return ServerComponentID_MatchingQueueWatcherService_8x1;
		}
	}


};// namespace 
};// namespace Svr 
};// namespace SF 

