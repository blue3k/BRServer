////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2020 The Braves
// 
// Author : KyungKun Ko
//
// Description : CharacterData service class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Util/GlobalUIDGenerator.h"
#include "Variable/SFVariableTable.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"

#include "PerformanceCounter/PerformanceCounter.h"

namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;
	class GameCharacterDataData;

	using CharacterDataUID = EntityUID;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	CharacterData class
	//

	class CharacterData
	{
	public:

		CharacterData(IHeap& heap, GameID gameID, PlayerID playerID, CharacterDataUID characterUID);

	public:

		GameID m_GameID;
		PlayerID m_PlayerID;

		// Character UID;
		CharacterDataUID m_CharacterUID;

		// Modification serial
		uint32_t m_SavedSerial{};
		uint32_t m_UpdateSerial{};

		// Attributes
		VariableTable m_Attributes;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	CharacterDataManagerServiceEntity class
	//

	class CharacterDataManagerServiceEntity : public ShardedClusterServiceEntity
	{
	public:

		using super = ShardedClusterServiceEntity;

		static constexpr ServerComponentID ComponentID = ServerComponentID_CharacterManagerService;

	private:

		static GlobalUIDGenerator stm_CharacterIDGenerator;

		// Cached Character data map
		SortedArray<EntityUID, CharacterData*> m_CharacterDataMap;

		// Perf counter for character data count
		PerformanceCounterRaw<uint64_t> m_CharacterDataCount;

	public:

		CharacterDataManagerServiceEntity(GameID gameID, ClusterMembership initialMembership = ClusterMembership::Slave);
		~CharacterDataManagerServiceEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual Result InitializeEntity(EntityID newEntityID) override;

		virtual Result RegisterServiceMessageHandler(ServerEntity *pServerEntity) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Game CharacterData operations
		//

		// Add new Entity
		virtual Result CreateCharacterData( GameID gameID, PlayerID playerID, CharacterDataUID characterUID, CharacterData*& pCharacterData );

		// Called when a CharacterData is deleted
		virtual Result FreeCharacterData( CharacterDataUID characterUID);



		// Initialize server component
		Result InitializeComponent() { return ResultCode::SUCCESS; }
		// Terminate server component
		void TerminateComponent() {  }


	};







}; // namespace Svr
}; // namespace SF




