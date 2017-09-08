////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/Game/GameClusterServiceEntity.h"
#include "Protocol/ServerService/ClusterServerService.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"

#include "Protocol/Message/LoginServerMsgClass.h"
#include "GameClusterServiceEntity.h"

#include "GameClusterServiceEntity.h"
#include "Transaction/GameServerEntityTrans.h"



namespace SF {
namespace GameServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameClusterServiceEntity::GameClusterServiceEntity(GameID gameID, const ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership)
		: Svr::GameClusterServiceEntity(gameID, publicNetSocket, initialMembership)
	{
		BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerCmd) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransRegisterPlayerToJoinGameServer<GameClusterServiceEntity>(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
	}

	GameClusterServiceEntity::~GameClusterServiceEntity()
	{
	}





}; // namespace Svr {
}; // namespace SF {



