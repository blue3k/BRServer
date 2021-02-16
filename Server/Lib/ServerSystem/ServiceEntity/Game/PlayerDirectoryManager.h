////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "MemoryManager/SFMemory.h"
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

	class Entity;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	PlayerDirectoryManager class
	//

	class PlayerDirectoryManager : public PlayerManagerService, public SharedObject
	{
	public:

		static constexpr StringCrc64 TypeName = "PlayerDirectoryManager";

		const char* ZKBasePath = "/BRPlayer";

		typedef HashTable2< PlayerID, EntityUID,
			UniqueKeyTrait, 
			ThreadSyncTraitReadWriteT<PlayerID, EntityUID>,
			hash < PlayerID >
			> PlayerIDMap;

		typedef EntityUID PlayerIDMapIterator;


	private:

		CriticalSection m_PlayerLock;

		// Player ID map. Locally created players
		PlayerIDMap m_LocalPlayersByID;

		// Number of player who exists on this server
		Atomic<int32_t> m_NumberOfPlayerOnThisServer;

		// directory base path
		String m_BasePath;

		const ServerConfig::NetPublic*	m_PublicNetSocket;

	private:

		//virtual bool Tick() override;
		void RegisterLocalPlayers();
		void RegisterOrUpdatePlayer(GameID gameID, PlayerID playerID, EntityUID entityUID, bool bIsUpdate);



	public:
		// Constructor/Destructor
		PlayerDirectoryManager();
		virtual ~PlayerDirectoryManager();


		// Initialize entity to proceed new connection
		Result InitializeComponent();

		Result DeinitializeComponent();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Player operations
		//

		// Create or update player id
		virtual Result CreatePlayer(GameID gameID, PlayerID playerID, EntityUID entityUID) override;
		virtual Result DeletePlayer(GameID gameID, PlayerID playerID) override;
		virtual Result FindPlayer(GameID gameID, PlayerID playerID, EntityUID& entityUID) override;

		// Use server gameID for search
		virtual Result CreatePlayer(PlayerID playerID, EntityUID entityUID) override;
		virtual Result DeletePlayer(PlayerID playerID) override;
		virtual Result FindPlayer(PlayerID playerID, EntityUID& entityUID) override;

	};

} // namespace SF




