////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "GameServer.h"
#include "Net/NetServerUDP.h"
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Transaction/MessageRoute.h"
#include "GameServerClass.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"



#include "GameServerEntityTrans.h"
#include "GameInstance/GamePlayerEntity.h"
#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"
#include "Protocol/Message/GameServerMsgClass.h"


SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::GameServerTransRegisterPlayerToJoinGameServer<SF::GameServer::PlayerManagerServiceEntity>);
SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::GameServerTransRegisterPlayerToJoinGameServer<SF::GameServer::GamePlayerEntity>);


namespace SF {
namespace GameServer {

};// namespace GameServer 
};// namespace SF 

