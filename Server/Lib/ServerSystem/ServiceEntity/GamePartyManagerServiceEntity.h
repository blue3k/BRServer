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


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrSvrTypes.h"
#include "Common/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"

#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"

namespace BR {
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

		PerformanceCounterRaw<UINT64> m_PartyCount;

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
}; // namespace BR




