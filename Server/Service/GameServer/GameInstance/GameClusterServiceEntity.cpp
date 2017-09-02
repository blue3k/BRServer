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
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServerSystem/ServerService/ClusterServerService.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"

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

	GameClusterServiceEntity::GameClusterServiceEntity(GameID gameID, Svr::Config::PublicNetSocket *publicNetSocket, ClusterMembership initialMembership)
		: Svr::GameClusterServiceEntity(gameID, publicNetSocket, initialMembership)
	{
		BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerCmd) { svrMemReturn(pNewTrans = new GameServerTransRegisterPlayerToJoinGameServer<GameClusterServiceEntity>(pMsgData)); return ResultCode::SUCCESS; } );
	}

	GameClusterServiceEntity::~GameClusterServiceEntity()
	{
	}





}; // namespace Svr {
}; // namespace SF {



