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
#include "GameConst.h"

#include "Net/SFNetServerUDP.h"

#include "GameServerClass.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"

#include "ServiceEntity/Game/PlayerDirectoryManager.h"
#include "Transaction/ExternalTransaction.h"
#include "Transaction/ExternalTransactionManager.h"

#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"

#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"

#include "GamePlayerEntityTrans.h"
#include "GameInstance/GamePlayerEntity.h"

#include "Server/BrServer.h"


#include "GameSystem/UserFriendSystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"
#include "GameSystem/UserNotifySystem.h"


#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "GameConspiracyDB.h"
#include "GameConspiracyQuery.h"
#include "DB/RankingDB.h"
#include "DB/RankingDBQuery.h"

#include "conspiracy/GameConfigTbl.h"
#include "conspiracy/OrganicTbl.h"

//#include "openssl/sha.h"
#define 	SHA256_DIGEST_LENGTH   32




	
namespace SF {
namespace GameServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//




}// namespace GameServer 
}// namespace SF 

