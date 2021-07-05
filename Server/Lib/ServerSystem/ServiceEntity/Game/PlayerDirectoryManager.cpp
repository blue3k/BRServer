////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/Game/PlayerDirectoryManager.h"
#include "Protocol/ClusterServerService.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "Service/ServerService.h"
#include "Server/BrServer.h"
#include "Protocol/LoginServerMsgClass.h"





namespace SF {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	constexpr StringCrc64 PlayerDirectoryManager::TypeName;


	PlayerDirectoryManager::PlayerDirectoryManager()
		: m_LocalPlayersByID(GetEngineHeap())
	{
		//BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityCreatedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		//BR_ENTITY_MESSAGE(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) GameServerTransGamePlayerEntityDeletedS2CEvt(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		Service::PlayerManager = this;
	}

	PlayerDirectoryManager::~PlayerDirectoryManager()
	{
		Service::PlayerManager = nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Player operations
	//

	// Initialize entity to proceed new connection
	Result PlayerDirectoryManager::InitializeComponent()
	{
		Result hr;
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		char gamePath[512];

		StrUtil::Format(gamePath, "{0}/{1}", ZKBasePath, Service::ServerConfig->GameClusterID);
		m_BasePath = gamePath;

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "PlayerDirectoryManager: DataCenter hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		if (!zkSession->Exists(ZKBasePath))
		{
			zkSession->ACreate(ZKBasePath, Json::Value(Json::objectValue), nullptr, 0);
		}

		if (!zkSession->Exists(gamePath))
			zkSession->ACreate(gamePath, Json::Value(Json::objectValue), nullptr, 0);


		zkSession->GetWatcher().OnSessionEvent.AddDelegate(uintptr_t(this), [this](int newState)
			{
				if (newState == Zookeeper::STATE_CONNECTED)
				{
					RegisterLocalPlayers();
				}
				else if (newState == Zookeeper::STATE_SESSION_EXPIRED)
				{
					// timed out
				}
			});

		//Start();

		return hr;
	}

	Result PlayerDirectoryManager::DeinitializeComponent()
	{
		//Stop();

		return ResultCode::SUCCESS;
	}

	void PlayerDirectoryManager::RegisterLocalPlayers()
	{
		MutexScopeLock lock(m_PlayerLock);

		m_LocalPlayersByID.for_each([this](PlayerID playerId, EntityUID entityUID)
			{
				RegisterOrUpdatePlayer(Service::ServerConfig->GameClusterID, playerId, entityUID, true);
				return true;
			});
	}

	void PlayerDirectoryManager::RegisterOrUpdatePlayer(GameID gameID, PlayerID playerID, EntityUID entityUID, bool bIsUpdate)
	{
		auto zkSession = Service::ZKSession->GetZookeeperSession();
		Json::Value nodeValue(Json::objectValue);
		char nodePath[256];

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "RegisterOrUpdatePlayer: Zookeeper session server hasn't ready!");
			return;
		}

		nodeValue["EntityUID"] = Json::Value(entityUID.UID);

		StrUtil::Format(nodePath, "{0}/{1}", m_BasePath, playerID);
		if (zkSession->Exists(nodePath)) // synchronize exist check
		{
			if (bIsUpdate)
			{
				// We just need to update node value
				zkSession->ASet(nodePath, nodeValue);
				return;
			}
			else
			{
				// Maybe from other entity?
				// We need to recreate it so that we can take control of it
				svrTrace(SVR_INFO, "RegisterOrUpdatePlayer: Existing node info from another server instance? recreating PlayerID:{0}", playerID);
				zkSession->Delete(nodePath);
			}
		}

		svrTrace(SVR_INFO, "RegisterOrUpdatePlayer: Creating player entity PlayerID:{0}, EntityUID:{1}", playerID, entityUID);
		zkSession->ACreate(nodePath, nodeValue, nullptr, Zookeeper::NODE_FLAG_EPHEMERAL);
	}

	// Create or update player id
	Result PlayerDirectoryManager::CreatePlayer(GameID gameID, PlayerID playerID, EntityUID entityUID)
	{
		Result hr;
		PlayerIDMapIterator itPlayer;
		bool bIsUpdate = false;
		//Json::Value nodeValue(Json::objectValue);
		//auto zkSession = Service::ZKSession->GetZookeeperSession();

		if (playerID == 0 || entityUID.UID == 0)
		{
			svrCheck(ResultCode::INVALID_PLAYERID);
		}

		//if (entityUID.GetServerID() != GetMyServerID())
		//{
		//	svrErr(ResultCode::SVR_INVALID_SERVERID);
		//}

		{
			MutexScopeLock lock(m_PlayerLock);

			if (m_LocalPlayersByID.find(playerID, itPlayer))
			{
				if (itPlayer != entityUID)
				{
					m_LocalPlayersByID.erase(playerID);
					svrCheck(m_LocalPlayersByID.Insert(playerID, entityUID));
				}
				bIsUpdate = true;
			}
			else
			{
				svrCheck(m_LocalPlayersByID.Insert(playerID, entityUID));
			}
		}

		m_NumberOfPlayerOnThisServer.fetch_add(1, std::memory_order_relaxed);
		
		RegisterOrUpdatePlayer(gameID, playerID, entityUID, bIsUpdate);

		return hr;
	}

	Result PlayerDirectoryManager::DeletePlayer(GameID gameID, PlayerID playerID)
	{
		Result hr;
		PlayerIDMapIterator itPlayer;
		char nodePath[256];
		auto zkSession = Service::ZKSession->GetZookeeperSession();

		if (playerID == 0)
		{
			svrCheck(ResultCode::INVALID_PLAYERID);
		}

		{
			MutexScopeLock lock(m_PlayerLock);

			if (!m_LocalPlayersByID.find(playerID, itPlayer))
			{
				return ResultCode::INVALID_ENTITY;
			}

			svrCheck(m_LocalPlayersByID.erase(playerID));
		}

		m_NumberOfPlayerOnThisServer.fetch_sub(1, std::memory_order_relaxed);

		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		StrUtil::Format(nodePath, "{0}/{1}", m_BasePath, playerID);
		zkSession->ADelete(nodePath);

		return hr;
	}

	Result PlayerDirectoryManager::FindPlayer(GameID gameID, PlayerID playerID, EntityUID& entityUID)
	{
		Result hr = ResultCode::SUCCESS;
		PlayerIDMapIterator itPlayer;
		char nodePath[256];
		Json::Value jsonValue(Json::objectValue);

		// We find player form online directly
		// We might need to consider moving player directory to something like MongoDB as Zookeeper isn't good with high traffic operation
		if (m_LocalPlayersByID.find(playerID, itPlayer))
		{
			entityUID = itPlayer;
			return hr;
		}

		auto zkSession = Service::ZKSession->GetZookeeperSession();
		if (zkSession == nullptr || !zkSession->IsConnected())
		{
			svrTrace(Error, "Zookeeper session server hasn't ready!");
			return ResultCode::UNEXPECTED;
		}

		StrUtil::Format(nodePath, "{0}/{1}", m_BasePath, playerID);

		if(!zkSession->Get(nodePath, jsonValue))
			return ResultCode::FAIL;

		entityUID.UID = jsonValue.get("EntityUID", Json::Value(0)).asUInt64();

		return hr;
	}

	// Use server gameID for search
	Result PlayerDirectoryManager::CreatePlayer(PlayerID playerID, EntityUID entityUID)
	{
		return CreatePlayer(Service::ServerConfig->GameClusterID, playerID, entityUID);
	}

	Result PlayerDirectoryManager::DeletePlayer(PlayerID playerID)
	{
		return DeletePlayer(Service::ServerConfig->GameClusterID, playerID);
	}

	Result PlayerDirectoryManager::FindPlayer(PlayerID playerID, EntityUID& entityUID)
	{
		return FindPlayer(Service::ServerConfig->GameClusterID, playerID, entityUID);
	}


} // namespace SF {

