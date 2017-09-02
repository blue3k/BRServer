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

	class GameClusterServiceEntity : public FreeReplicaClusterServiceEntity, public IServerComponent
	{
	public:

		enum { ComponentID = ServerComponentID_GameClusterService };


		// Player table item
		class PlayerTableItem
		{
		private:

			// Player ID
			PlayerID m_PlayerID = 0;

			// Entity UID
			EntityUID m_EntityUID;

			// Server Up time, Used for checking latest updated server entity
			TimeStampMS m_ServerUpTime;

			// Linked server entity
			ServerEntity* m_ServerEntity;


		public:

			// Constructor with constructor
			PlayerTableItem( PlayerID playerID, EntityUID entityUID, ServerEntity* pServerEntity )
				:m_PlayerID(playerID)
				,m_EntityUID(entityUID)
				,m_ServerUpTime(pServerEntity->GetServerUpTime())
				,m_ServerEntity(pServerEntity)
			{
			}

		public:

			void UpdateEntityInfo( EntityUID entityUID, ServerEntity* pServerEntity );
		};


		typedef HashTable2< PlayerID,
			PlayerTableItem*,
			UniqueKeyTrait, 
			ThreadSyncTraitReadWriteT<PlayerID, PlayerTableItem*>,
			hash < PlayerID >
			> PlayerIDMap;
		typedef PlayerTableItem* PlayerIDMapIterator;

	private:

		IMemoryManager& GetAllocator() { return GetSystemMemoryManager(); }

		// Player ID map
		PlayerIDMap m_PlayerIDMap;

		// Number of player who exists on this server
		SyncCounter m_NumberOfPlayerOnThisServer;


		Config::PublicNetSocket*	m_PublicNetSocket;

	public:
		// Constructor/Destructor
		GameClusterServiceEntity(GameID gameID, Config::PublicNetSocket *publicNetSocket, ClusterMembership initialMembership = ClusterMembership::StatusWatcher);
		virtual ~GameClusterServiceEntity();

		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;

		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		// Game Cluster ID
		static ClusterID GetGameClusterID( GameID gameID ) { return (ClusterID)((uint)ClusterID::Game + (uint)gameID); }


		Config::PublicNetSocket* GetPublicNetConfig() { return m_PublicNetSocket; }

		//////////////////////////////////////////////////////////////////////////
		//
		//	Player operations
		//

		// Create PlayerInfo
		Result CreatePlayer( PlayerID playerID, EntityUID entityUID, ServerEntity* pGameServerEntity );
		Result CreatePlayer( PlayerID playerID, EntityUID entityUID );

		// Create PlayerInfo
		Result DeletePlayer(PlayerID playerID, EntityUID entityUID);

		// Get Player info
		Result FindPlayer( PlayerID playerID, EntityUID &playerUID );

	};




}; // namespace Svr
}; // namespace SF




