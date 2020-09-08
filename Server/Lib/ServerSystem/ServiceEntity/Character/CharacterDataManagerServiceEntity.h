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
#include "Util/LocalUIDGenerator.h"
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
	//	CharacterDataManagerServiceEntity class
	//

	class CharacterDataManagerServiceEntity : public LoadbalanceClusterServiceEntity
	{
	public:

		typedef LoadbalanceClusterServiceEntity super;

		static constexpr ServerComponentID ComponentID = ServerComponentID_CharacterManagerService;

	private:

		PerformanceCounterRaw<uint64_t> m_CharacterDataCount;

	protected:


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
		virtual Result CreateGameCharacterData( GameID gameID, PlayerUID playerUID, ServerEntity *pServerEntity, CharacterDataUID &CharacterDataUID );

		// Called when a game CharacterData is deleted
		virtual Result FreeGameCharacterData( CharacterDataUID CharacterDataUID );

		// Initialize server component
		Result InitializeComponent() { return ResultCode::SUCCESS; }
		// Terminate server component
		void TerminateComponent() {  }


	};







}; // namespace Svr
}; // namespace SF




