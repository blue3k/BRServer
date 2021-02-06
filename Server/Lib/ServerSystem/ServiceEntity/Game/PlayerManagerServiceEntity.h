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
#include "Container/SFHashTable2.h"
#include "Container/SFIndexing.h"
#include "Container/SFStaticHashTable.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "Service/PlayerManagerService.h"
#include "ServerConfig/SFServerConfig.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	PlayerManagerServiceEntity class
	//

	class PlayerManagerServiceEntity : public ShardedClusterServiceEntity, public PlayerManagerService
	{
	public:

		typedef ShardedClusterServiceEntity super;

		const char* ZKBasePath = "/BRPlayer";



		typedef HashTable2< PlayerID, EntityUID,
			UniqueKeyTrait, 
			ThreadSyncTraitReadWriteT<PlayerID, EntityUID>,
			hash < PlayerID >
			> PlayerIDMap;

		typedef EntityUID PlayerIDMapIterator;


	private:

		// Player ID map. Locally created players
		PlayerIDMap m_PlayerIDMap;

		// Number of player who exists on this server
		SyncCounter m_NumberOfPlayerOnThisServer;


		const ServerConfig::NetPublic*	m_PublicNetSocket;

	public:
		// Constructor/Destructor
		PlayerManagerServiceEntity();
		virtual ~PlayerManagerServiceEntity();

		virtual void Dispose() override;

		virtual Result RegisterServiceMessageHandler() override;


		// Initialize entity to proceed new connection
		virtual Result InitializeEntity(EntityID newEntityID) override;


		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


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




