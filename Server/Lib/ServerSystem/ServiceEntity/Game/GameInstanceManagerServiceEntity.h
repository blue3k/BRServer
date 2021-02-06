////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game instance service class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"
#include "Container/SFSortedMap.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "ServerConfig/SFServerConfig.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "Variable/SFVariableTable.h"


namespace SF {
	namespace Svr {

		class Entity;
		class ServerEntity;
		class GameInstanceEntity;


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	GameInstanceManagerServiceEntity class
		//

		class GameInstanceManagerServiceEntity : public LoadbalanceClusterServiceEntity
		{
		public:

			typedef LoadbalanceClusterServiceEntity super;

			enum { ComponentID = ServerComponentID_GameInstanceManagerService };

		protected:

			Svr::PerformanceCounterRaw < uint64_t > m_NumberOfInstance;

		private:
			SF::Mutex m_GameInstanceListLock;
			// NOTE: We might need to use shared ptr
			SortedMap<GameInsUID, GameInstanceEntity*> m_GameInstances;

		public:

			GameInstanceManagerServiceEntity(GameID gameID, ServerConfig::ServerModuleGameInstanceManager* config, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave);
			~GameInstanceManagerServiceEntity();


			//////////////////////////////////////////////////////////////////////////
			//
			//	Entity operations
			//

			virtual Result InitializeEntity(EntityID newEntityID) override;

			virtual Result RegisterServiceMessageHandler() override;

			//////////////////////////////////////////////////////////////////////////
			//
			//	Game Instance operations
			//

			// Added new game instance
			virtual Result OnNewInstance(GameInstanceEntity* pGameInstance);

			// Called when a game instance is deleted
			virtual Result FreeGameInstance(GameInsUID gameUID);

			// Search game instance
			virtual Result SearchGameInstance(size_t maxSearch, const char* searchKeyword, Array<GameInstanceInfo>& outList);


			// Initialize server component
			Result InitializeComponent() { return ResultCode::SUCCESS; }
			// Terminate server component
			void TerminateComponent() {  }


		};


	} // namespace Svr
} // namespace SF

