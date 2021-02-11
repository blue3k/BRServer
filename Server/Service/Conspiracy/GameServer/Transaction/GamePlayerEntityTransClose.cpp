////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "GameServerPCH.h"
#include "GameServer.h"
#include "GameServerClass.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"

#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "Protocol/ServerService/PartyMatchingQueueService.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/Game/PlayerDirectoryManager.h"

#include "Protocol/Message/PartyMatchingQueueMsgClass.h"

#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "GamePlayerEntityTransClose.h"
#include "GameInstance/GamePlayerEntity.h"

//#include "PlayerInfoManager.h"
//#include "GameInstance/GamePlayer.h"
#include "Server/BrServer.h"


#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"


#include "GameConspiracyDB.h"
#include "GameConspiracyQuery.h"
#include "DB/LoginSessionDB.h"
#include "DB/LoginSessionQuery.h"




	
namespace SF {
namespace GameServer {


	PlayerTransCloseInstance::PlayerTransCloseInstance(IHeap& heap)
		: super(heap)
	{
		SetExclusive(true);

		BR_TRANS_MESSAGE(conspiracy::QueryUpdateTickStatusCmd, { return UpdateDBRes(pRes); });
	}





};// namespace GameServer 
};// namespace SF 

