////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game party service class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/PageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/HashTable.h"
#include "Container/Indexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"

#include "PerformanceCounter/PerformanceCounter.h"

namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;
	class GamePartyEntity;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GamePartyManagerServiceEntity class
	//

	class GamePartyManagerServiceEntity : public LoadbalanceClusterServiceEntity
	{
	public:

		typedef LoadbalanceClusterServiceEntity super;

		enum { ComponentID = ServerComponentID_GamePartyManagerService };

	private:

		PerformanceCounterRaw<uint64_t> m_PartyCount;

	protected:


	public:

		GamePartyManagerServiceEntity( ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~GamePartyManagerServiceEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual Result InitializeEntity(EntityID newEntityID) override;

		virtual Result RegisterServiceMessageHandler(ServerEntity *pServerEntity) override;

		//////////////////////////////////////////////////////////////////////////
		//
		//	Game party operations
		//

		// Add new Entity
		virtual Result CreateGameParty( GameID gameID, const PlayerInformation& creator, EntityUID playerUID, ServerEntity *pServerEntity, PartyUID &partyUID );

		// Called when a game party is deleted
		virtual Result FreeGameParty( PartyUID partyUID );

		// Initialize server component
		Result InitializeComponent() { return ResultCode::SUCCESS; }
		// Terminate server component
		void TerminateComponent() {  }


	};







}; // namespace Svr
}; // namespace SF




