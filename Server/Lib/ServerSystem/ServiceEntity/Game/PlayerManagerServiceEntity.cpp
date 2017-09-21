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
#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"
#include "Protocol/ServerService/ClusterServerService.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"

#include "Protocol/Message/LoginServerMsgClass.h"



SF_MEMORYPOOL_IMPLEMENT(SF::Svr::PlayerManagerServiceEntity::PlayerTableItem);


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	PlayerManagerServiceEntity::PlayerManagerServiceEntity()
		: super(GetServerGameID(), ClusterID::PlayerManager, ClusterMembership::Slave)
		, m_PlayerIDMap(GetMemoryManager())
	{
		//BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityCreatedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityDeletedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		Service::PlayerManager = this;
	}

	PlayerManagerServiceEntity::~PlayerManagerServiceEntity()
	{
		Service::PlayerManager = nullptr;
	}

	void PlayerManagerServiceEntity::Dispose()
	{
		super::Dispose();
		Service::PlayerManager = nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Player operations
	//

	Result PlayerManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::RegisterServiceMessageHandler( pServerEntity ) );

		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityCreatedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//pServerEntity->BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityDeletedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );


	Proc_End:

		return hr;
	}

	// Initialize entity to proceed new connection
	Result PlayerManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr;
		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		char gamePath[512];

		svrChk(super::InitializeEntity(newEntityID));

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		if (!zkSession->Exists(ZKBasePath))
		{
			zkSession->ACreate(ZKBasePath, Json::Value(Json::objectValue), nullptr, 0);
		}

		// TODO: iterate all game type
		StrUtil::Format(gamePath, "{0}/{1}", ZKBasePath, Enum<GameID>().GetValueName(GameID::Conspiracy));
		if (!zkSession->Exists(gamePath))
			zkSession->ACreate(gamePath, Json::Value(Json::objectValue), nullptr, 0);

		StrUtil::Format(gamePath, "{0}/{1}", ZKBasePath, Enum<GameID>().GetValueName(GameID::MyTownHero));
		if (!zkSession->Exists(gamePath))
			zkSession->ACreate(gamePath, Json::Value(Json::objectValue), nullptr, 0);

	Proc_End:

		return hr;
	}


	Result PlayerManagerServiceEntity::TickUpdate(TimerAction *pAction)
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

	void PlayerManagerServiceEntity::Clear()
	{

	}

	// Create or update player id
	Result PlayerManagerServiceEntity::CreatePlayer(GameID gameID, PlayerID playerID, EntityUID entityUID)
	{
		Result hr;
		PlayerIDMapIterator itPlayer;
		char nodePath[256];
		bool bIsUpdate = false;
		Json::Value nodeValue(Json::objectValue);

		if (playerID == 0 || entityUID.UID == 0) svrErr(ResultCode::INVALID_PLAYERID);

		//if (entityUID.GetServerID() != GetMyServerID())
		//{
		//	svrErr(ResultCode::SVR_INVALID_SERVERID);
		//}

		if ((m_PlayerIDMap.find(playerID, itPlayer)))
		{
			if (itPlayer != entityUID)
			{
				m_PlayerIDMap.erase(playerID);
				svrChk(m_PlayerIDMap.Insert(playerID, entityUID));
			}
			bIsUpdate = true;
		}
		else
		{
			svrChk(m_PlayerIDMap.Insert(playerID, entityUID));
		}

		m_NumberOfPlayerOnThisServer.fetch_add(1, std::memory_order_relaxed);
		

		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		nodeValue["EntityUID"] = Json::Value(entityUID.UID);

		StrUtil::Format(nodePath, "{0}/{1}/{2}", ZKBasePath, Enum<GameID>().GetValueName(gameID), playerID);
		if (zkSession->Exists(nodePath)) // synchronize exist check
		{
			if (bIsUpdate)
			{
				// We just need to update node value
				zkSession->ASet(nodePath, nodeValue);
				goto Proc_End;
			}
			else
			{
				// Maybe from other entity?
				// We need to recreate it so that we can take control of it
				svrTrace(SVR_INFO, "Existing node info from another server instance? recreating PlayerID:{0}", playerID);
				zkSession->Delete(nodePath);
			}
		}

		svrTrace(SVR_INFO, "Creating player entity PlayerID:{0}, EntityUID:{1}", playerID, entityUID);
		zkSession->ACreate(nodePath, nodeValue, nullptr, ZooKeeper::NODE_FLAG_EPHEMERAL);


	Proc_End:

		return hr;
	}

	Result PlayerManagerServiceEntity::DeletePlayer(GameID gameID, PlayerID playerID)
	{
		Result hr;
		PlayerIDMapIterator itPlayer;
		char nodePath[256];

		if (!m_PlayerIDMap.find(playerID, itPlayer))
		{
			return ResultCode::INVALID_ENTITY;
		}

		svrChk(m_PlayerIDMap.erase(playerID));

		m_NumberOfPlayerOnThisServer.fetch_sub(1, std::memory_order_relaxed);


		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		StrUtil::Format(nodePath, "{0}/{1}/{2}", ZKBasePath, Enum<GameID>().GetValueName(gameID), playerID);
		zkSession->ADelete(nodePath);


	Proc_End:


		return hr;
	}

	Result PlayerManagerServiceEntity::FindPlayer(GameID gameID, PlayerID playerID, EntityUID& entityUID)
	{
		Result hr = ResultCode::SUCCESS;
		PlayerIDMapIterator itPlayer;
		char nodePath[256];
		Json::Value jsonValue(Json::objectValue);

		if (m_PlayerIDMap.find(playerID, itPlayer))
		{
			entityUID = itPlayer;
			return hr;
		}


		auto zkSession = Service::ZKSession->GetZooKeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		StrUtil::Format(nodePath, "{0}/{1}/{2}", ZKBasePath, Enum<GameID>().GetValueName(gameID), playerID);

		if(!zkSession->Get(nodePath, jsonValue))
			return ResultCode::FAIL;

		entityUID.UID = jsonValue.get("EntityUID", Json::Value(0)).asUInt64();

	Proc_End:

		return hr;
	}


	// Use server gameID for search
	Result PlayerManagerServiceEntity::CreatePlayer(PlayerID playerID, EntityUID entityUID)
	{
		return CreatePlayer(Svr::GetServerGameID(), playerID, entityUID);
	}

	Result PlayerManagerServiceEntity::DeletePlayer(PlayerID playerID)
	{
		return DeletePlayer(Svr::GetServerGameID(), playerID);
	}

	Result PlayerManagerServiceEntity::FindPlayer(PlayerID playerID, EntityUID& entityUID)
	{
		return FindPlayer(Svr::GetServerGameID(), playerID, entityUID);
	}





}; // namespace Svr {
}; // namespace SF {



