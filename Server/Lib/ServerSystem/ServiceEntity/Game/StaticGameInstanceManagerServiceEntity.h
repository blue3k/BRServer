////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Game instance service class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "MemoryManager/SFMemory.h"
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
		//	StaticGameInstanceManagerServiceEntity class
		//

		class StaticGameInstanceManagerServiceEntity : public ServiceEntity
		{
		public:

			using super = ServiceEntity;

			static constexpr StringCrc64 TypeName = "StaticGameInstanceManagerServiceEntity"_crc64;

		protected:

			Svr::PerformanceCounterRaw < uint64_t > m_NumberOfInstance;

		private:
			// 
			SF::Mutex m_GameInstanceListLock;
			// NOTE: We might need to use shared ptr
			SortedMap<GameInsUID, GameInstanceEntity*> m_GameInstances;

		public:

			StaticGameInstanceManagerServiceEntity(GameID gameID, ServerConfig::ServerModuleStaticGameInstanceManager* config, ClusterID clusterID, const EndpointAddress& endpoint);
			~StaticGameInstanceManagerServiceEntity();


			//////////////////////////////////////////////////////////////////////////
			//
			//	Entity operations
			//

			virtual Result InitializeEntity(EntityID newEntityID) override;

			virtual Result RegisterServiceMessageHandler() override;


			// Initialize server component
			Result InitializeComponent() { return ResultCode::SUCCESS; }
			// Terminate server component
			void DeinitializeComponent() {  }

		};


	} // namespace Svr
} // namespace SF

