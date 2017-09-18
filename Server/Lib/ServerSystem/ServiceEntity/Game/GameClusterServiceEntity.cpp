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
#include "ServiceEntity/Game/GameClusterServiceTrans.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GameClusterServiceEntity::PlayerTableItem);


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameClusterServiceEntity::GameClusterServiceEntity(const ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership)
		: super(GetServerGameID(), ClusterID::Game, initialMembership)
		, m_PlayerIDMap(GetMemoryManager())
		, m_PublicNetSocket(publicNetSocket)
	{
		//BR_ENTITY_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerCmd) { svrMemReturn(pNewTrans = new(GetMemoryManager()) GameServerTransRegisterPlayerToJoinGameServer(pMsgData)); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityCreatedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityDeletedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
	}

	GameClusterServiceEntity::~GameClusterServiceEntity()
	{

	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Player operations
	//

	void GameClusterServiceEntity::PlayerTableItem::UpdateEntityInfo( EntityUID entityUID )
	{
		m_EntityUID = entityUID;
	}

	Result GameClusterServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::RegisterServiceMessageHandler( pServerEntity ) );

		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityCreatedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityDeletedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );


	Proc_End:

		return hr;
	}

	Result GameClusterServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::TickUpdate(pAction) );

		if( (SignedCounterType)m_NumberOfPlayerOnThisServer > 0 )
		{
			SetWorkload((uint)m_NumberOfPlayerOnThisServer);
		}

	Proc_End:

		return hr;
	}

	void GameClusterServiceEntity::Clear()
	{

	}

	// Create or update player id
	Result GameClusterServiceEntity::CreatePlayer(GameID gameID, PlayerID playerID, EntityUID entityUID)
	{
		Result hr;
		PlayerTableItem *pNewPlayerInfo = nullptr;
		PlayerIDMapIterator itPlayer;
		char nodePath[256];
		bool bIsUpdate = false;

		if (playerID == 0 || entityUID.UID == 0) svrErr(ResultCode::INVALID_PLAYERID);

		//if (entityUID.GetServerID() != GetMyServerID())
		//{
		//	svrErr(ResultCode::SVR_INVALID_SERVERID);
		//}

		if ((m_PlayerIDMap.find(playerID, itPlayer)))
		{
			Assert(itPlayer->GetPlayerID() == playerID);
			itPlayer->UpdateEntityInfo(entityUID);
			bIsUpdate = true;
			return hr;
		}

		svrMem(pNewPlayerInfo = new(GetMemoryManager()) PlayerTableItem(playerID, entityUID));
		m_NumberOfPlayerOnThisServer.fetch_add(1, std::memory_order_relaxed);


		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		Json::Value nodeValue(Json::objectValue);
		nodeValue["EntityID"] = Json::Value(pNewServiceInfo->GetEntityUID().UID);

		StrUtil::Format(nodePath, "{0}/{1}/{2}", ZKBasePath, Enum<GameID>().GetValueName(gameID), playerID);
		if (zkSession.Exists(nodePath))
		{
			if (bIsUpdate)
			{
				zkSession.ASet(nodePath, , -1);
			}
		}
		zkSession.ACreate(nodePath,);


	Proc_End:

		return hr;
	}

	Result GameClusterServiceEntity::DeletePlayer(GameID gameID, PlayerID playerID)
	{
		Result hr;
		PlayerTableItem *pPlayerInfo = nullptr;
		PlayerIDMapIterator itPlayer;
		char nodePath[256];

		if (!m_PlayerIDMap.find(playerID, itPlayer))
		{
			return ResultCode::INVALID_ENTITY;
		}


		PlayerTableItem* pTblItem = itPlayer;

		if (pTblItem->GetPlayerID() != playerID)
			svrErr(ResultCode::INVALID_ENTITY);

		svrChk(m_PlayerIDMap.erase(playerID));

		m_NumberOfPlayerOnThisServer.fetch_sub(1, std::memory_order_relaxed);

		IHeap::Delete(pTblItem);


		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		StrUtil::Format(nodePath, "{0}/{1}/{2}", ZKBasePath, Enum<GameID>().GetValueName(gameID), playerID);
		zkSession.ADelete(nodePath);


	Proc_End:


		return hr;
	}

	Result GameClusterServiceEntity::FindPlayer(GameID gameID, PlayerID playerID, EntityUID& entityUID)
	{
		Result hr = ResultCode::SUCCESS;
		PlayerIDMapIterator itPlayer;
		char nodePath[256];
		Json::Value jsonValue(Json::objectValue);

		if (m_PlayerIDMap.find(playerID, itPlayer))
		{
			entityUID = itPlayer->GetEntityUID();
			return hr;
		}


		auto& zkSession = Service::ZKSession->GetZooKeeperSession();
		if (!zkSession.IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		StrUtil::Format(nodePath, "{0}/{1}/{2}", ZKBasePath, Enum<GameID>().GetValueName(gameID), playerID);

		zkSession.Get(nodePath, jsonValue);

		entityUID.UID = jsonValue.get("EntityID", Json::Value(0)).asUInt64();

	Proc_End:

		return hr;
	}


	// Use server gameID for search
	Result GameClusterServiceEntity::CreatePlayer(PlayerID playerID, EntityUID entityUID)
	{
		return CreatePlayer(Svr::GetServerGameID(), playerID, entityUID);
	}

	Result GameClusterServiceEntity::DeletePlayer(PlayerID playerID)
	{
		return DeletePlayer(Svr::GetServerGameID(), playerID);
	}

	Result GameClusterServiceEntity::FindPlayer(PlayerID playerID, EntityUID& entityUID)
	{
		return FindPlayer(Svr::GetServerGameID(), playerID, entityUID);
	}





}; // namespace Svr {
}; // namespace SF {



