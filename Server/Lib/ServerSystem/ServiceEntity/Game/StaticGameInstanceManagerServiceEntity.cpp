////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Game instance service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "MemoryManager/SFMemory.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Game/StaticGameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceTrans.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "Component/SFLibraryComponentAdapter.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Service/ServerService.h"
#include "Util/SFRandom.h"

#include "Protocol/GameInstanceManagerMsgClass.h"
#include "PerformanceCounter/PerformanceCounterClient.h"
#include "Protocol/GameInstanceManagerService.h"


namespace SF {
	namespace Svr {


		//////////////////////////////////////////////////////////////////////////
		//
		//	StaticGameInstanceManagerServiceEntity class
		//

		StaticGameInstanceManagerServiceEntity::StaticGameInstanceManagerServiceEntity(GameID gameID, ServerConfig::ServerModuleStaticGameInstanceManager* config, ClusterID clusterID, const EndpointAddress& endpoint)
			: super(gameID, clusterID, endpoint)
			, m_NumberOfInstance("NumberOfGameInstances")
			, m_GameInstances(GetHeap())
		{
		}

		StaticGameInstanceManagerServiceEntity::~StaticGameInstanceManagerServiceEntity()
		{
		}

		Result StaticGameInstanceManagerServiceEntity::InitializeEntity(EntityID newEntityID)
		{
			ScopeContext hr;
			EntityUID entityUID;
			PerformanceCounterInstance* pInstance = nullptr;

			svrCheck(super::InitializeEntity(newEntityID));

			svrCheck(Service::DataTableManager->LoadDataTable("ZoneTable", "ZoneId", "gtbl_ZoneTable"));

			auto pZoneTable = Service::DataTableManager->GetDataTable("ZoneTable");
			svrCheckPtr(pZoneTable);

			pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
			if (pInstance != nullptr)
			{
				pInstance->AddCounter(&m_NumberOfInstance);
			}

			auto pEngine = Engine::GetInstance();
			if (pEngine)
				pEngine->AddComponent(new(GetEngineHeap()) LibraryComponentAdapter<StaticGameInstanceManagerServiceEntity>(GetEngineHeap(), this));

			svrCheck(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::GameInstance));
			svrCheck(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::GameInstanceManager));

			// first update delay
			m_TimeToUpdate.SetTimer(DurationMS(3000));


			return hr;
		}

		Result StaticGameInstanceManagerServiceEntity::TickUpdate(TimerAction* pAction)
		{
			Result hr;

			svrCheck(super::TickUpdate(pAction));

			auto pServer = Svr::BrServer::GetInstance();
			svrCheckPtr(pServer);
			if (pServer->GetServerState() != ServerState::RUNNING)
				return hr;

			if (m_TimeToUpdate.CheckTimer())
			{
				UpdateStaticZone();

				m_TimeToUpdate.SetTimer(DurationMS(10000));
			}


			return hr;
		}

		Result StaticGameInstanceManagerServiceEntity::UpdateStaticZone()
		{
			Result hr;
			auto pZoneTable = Service::DataTableManager->GetDataTable("ZoneTable");
			svrCheckPtr(pZoneTable);

			DynamicArray<SharedPointerT<EntityInformation>> services(GetHeap());
			DynamicArray<int32_t> zoneCountCounters(GetHeap());
			HashTable2<uint32_t, int32_t*> zoneCount(GetHeap());

			VariableTable attributes;
			attributes.SetValue<String>("Custom.Type", "Static");

			// Counting active static zones
			svrCheck(Service::ServiceDirectory->FindObjects(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, attributes, services));

			int32_t usedCounter = 0;
			svrCheck(zoneCountCounters.resize(services.size()));
			memset(zoneCountCounters.data(), 0, sizeof(int32_t) * zoneCountCounters.size());

			for (auto itZoneService : services)
			{
				auto zoneTableID = itZoneService->GetCustomAttributes().GetValue<uint32_t>("ZoneTableID");
				int32_t* count = 0;
				if (!zoneCount.Find(zoneTableID, count))
				{
					count = &zoneCountCounters[usedCounter]; usedCounter++;
					zoneCount.insert(zoneTableID, count);
				}

				(*count) = (*count) + 1;
			}

			for (auto itZone : *pZoneTable)
			{
				auto staticZoneCount = itZone.second->GetValue<int>("BaseStaticZoneCount");
				if (staticZoneCount <= 0)
					continue;

				auto zoneTableId = itZone.second->GetValue<uint32_t>("ZoneId");

				int32_t curCount{};
				int32_t* pCount{};
				if (zoneCount.Find(zoneTableId, pCount))
				{
					curCount = *pCount;
				}

				if (staticZoneCount <= curCount)
					continue;

				staticZoneCount -= curCount;

				VariableTable attributes(GetHeap());

				attributes.SetValue("MaxPlayer", itZone.second->GetValue<uint32_t>("MaxPlayer"));
				attributes.SetValue("ZoneTableID", zoneTableId);
				attributes.SetValue("Type", StringCrc32("Static"));

				// create missing zones
				for (int32_t iZone = 0; iZone < staticZoneCount; iZone++)
				{
					// Query new service instance per loop for better load balance
					SharedPointerT<ServerServiceInformation> pInstanceMangerService{};
					svrCheck(Service::ServiceDirectory->GetRandomService(GetGameID(), ClusterID::GameInstanceManager, pInstanceMangerService));
					svrCheck(pInstanceMangerService->GetService<GameInstanceManagerService>()->CreateGameInstanceCmd(0, 0, zoneTableId, attributes));
				}
			}

			return hr;
		}


	} // namespace Svr {
} // namespace SF {
