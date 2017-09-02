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
#include "Common/ClassUtil.h"
#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Common/HashTable.h"
#include "Container/Indexing.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
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

	class GamePartyManagerServiceEntity : public LoadbalanceClusterServiceEntity, public IServerComponent
	{
	public:

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
		virtual Result CreateGameParty( const PlayerInformation& creator, EntityUID playerUID, ServerEntity *pServerEntity, PartyUID &partyUID );

		// Called when a game party is deleted
		virtual Result FreeGameParty( PartyUID partyUID );

	};





	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MatchingQueueWatcherServiceEntity class
	//

	class GamePartyManagerWatcherServiceEntity : public LoadbalanceClusterServiceEntity, public IServerComponent
	{
	public:

		enum { ComponentID = ServerComponentID_GamePartyManagerService };

	private:

	public:

		GamePartyManagerWatcherServiceEntity();
		~GamePartyManagerWatcherServiceEntity();

	};



}; // namespace Svr
}; // namespace SF




