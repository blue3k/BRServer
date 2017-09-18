////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Container/HashTable2.h"
#include "Container/Indexing.h"
#include "Container/StaticHashTable.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameClusterServiceEntity class
	//

	class GameClusterServiceEntity : public ShardedClusterServiceEntity, public PlayerManagerService
	{
	public:

		typedef ShardedClusterServiceEntity super;

		const char* ZKBasePath = "/Player";


		// Player table item
		class PlayerTableItem
		{
		private:

			// Player ID
			PlayerID m_PlayerID = 0;

			// Entity UID
			EntityUID m_EntityUID;


		public:

			PlayerID GetPlayerID() const { return m_PlayerID; }
			EntityUID GetEntityUID() const { return m_EntityUID; }

			// Constructor with constructor
			PlayerTableItem( PlayerID playerID, EntityUID entityUID )
				:m_PlayerID(playerID)
				,m_EntityUID(entityUID)
			{
			}

		public:

			void UpdateEntityInfo( EntityUID entityUID);
		};


		typedef HashTable2< PlayerID,
			PlayerTableItem*,
			UniqueKeyTrait, 
			ThreadSyncTraitReadWriteT<PlayerID, PlayerTableItem*>,
			hash < PlayerID >
			> PlayerIDMap;
		typedef PlayerTableItem* PlayerIDMapIterator;

	private:

		// Player ID map. Locally created players
		PlayerIDMap m_PlayerIDMap;

		// Number of player who exists on this server
		SyncCounter m_NumberOfPlayerOnThisServer;


		const ServerConfig::NetPublic*	m_PublicNetSocket;

	public:
		// Constructor/Destructor
		GameClusterServiceEntity(const ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership = ClusterMembership::StatusWatcher);
		virtual ~GameClusterServiceEntity();

		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;

		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		// Game Cluster ID
		const ServerConfig::NetPublic* GetPublicNetConfig() { return m_PublicNetSocket; }

		//////////////////////////////////////////////////////////////////////////
		//
		//	Player operations
		//

		virtual void Clear() override;

		// Create or update player id
		virtual Result CreatePlayer(GameID gameID, PlayerID playerID, EntityUID entityUID) override;
		virtual Result DeletePlayer(GameID gameID, PlayerID playerID) override;
		virtual Result FindPlayer(GameID gameID, PlayerID playerID, EntityUID& entityUID) override;

		// Use server gameID for search
		virtual Result CreatePlayer(PlayerID playerID, EntityUID entityUID) override;
		virtual Result DeletePlayer(PlayerID playerID) override;
		virtual Result FindPlayer(PlayerID playerID, EntityUID& entityUID) override;



	};




}; // namespace Svr
}; // namespace SF




