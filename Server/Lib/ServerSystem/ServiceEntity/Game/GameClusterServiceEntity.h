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


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrSvrTypes.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"


namespace BR {
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
		class PlayerTableItem : public MemoryPoolObject<PlayerTableItem>
		{
		public:
			// Hash table mapping Item
			typedef OrderedLinkedList<UINT64>::Node TableItemType;
			TableItemType m_TableNode;

		private:

			// Player ID
			BRCLASS_ATTRIBUTE_READONLY(PlayerID,PlayerID);

			// Entity UID
			BRCLASS_ATTRIBUTE_READONLY(EntityUID,EntityUID);

			// Server Up time, Used for checking latest updated server entity
			BRCLASS_ATTRIBUTE_READONLY(TimeStampMS,ServerUpTime);

			// Linked server entity
			BRCLASS_ATTRIBUTE_READONLY(ServerEntity*,ServerEntity);


		public:

			// Constructor with constructor
			PlayerTableItem( PlayerID playerID, EntityUID entityUID, ServerEntity* pServerEntity )
				:m_PlayerID(playerID)
				,m_EntityUID(entityUID)
				,m_ServerUpTime(pServerEntity->GetServerUpTime())
				,m_ServerEntity(pServerEntity)
			{
				memset(&m_TableNode, 0, sizeof(m_TableNode) );
			}

		public:

			void UpdateEntityInfo( EntityUID entityUID, ServerEntity* pServerEntity );
		};


		typedef PlayerTableItem::TableItemType TableItemType;
		typedef Hash::StaticHashTable<	PlayerID, PlayerTableItem,
										//Indexing::ConstMemFunc<PlayerTableItem,PlayerID,&PlayerTableItem::GetPlayerID,UINT64>,
										Indexing::MapItemConverter<PlayerTableItem,TableItemType,&PlayerTableItem::m_TableNode>,
										Hash::UniqueKeyTrait, ThreadSyncTraitNone
										> PlayerIDMap;


	private:

		MemoryAllocator& GetAllocator() { return STDAllocator::GetInstance(); }

		// Player ID map
		PlayerIDMap m_PlayerIDMap;

		// Number of player who exists on this server
		SyncCounter m_NumberOfPlayerOnThisServer;


		Config::PublicNetSocket*	m_PublicNetSocket;

	public:
		// Constructor/Destructor
		GameClusterServiceEntity(Config::PublicNetSocket *publicNetSocket, GameID gameID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher);
		virtual ~GameClusterServiceEntity();

		virtual HRESULT RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;

		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		// Game Cluster ID
		static ClusterID GetGameClusterID( GameID gameID ) { return (ClusterID)((UINT)ClusterID::Game + (UINT)gameID); }


		Config::PublicNetSocket* GetPublicNetConfig() { return m_PublicNetSocket; }

		//////////////////////////////////////////////////////////////////////////
		//
		//	Player operations
		//

		// Create PlayerInfo
		HRESULT CreatePlayer( PlayerID playerID, EntityUID entityUID, ServerEntity* pGameServerEntity );
		HRESULT CreatePlayer( PlayerID playerID, EntityUID entityUID );

		// Create PlayerInfo
		HRESULT DeletePlayer(PlayerID playerID, EntityUID entityUID);

		// Get Player info
		HRESULT FindPlayer( PlayerID playerID, EntityUID &playerUID );

	};




}; // namespace Svr
}; // namespace BR




