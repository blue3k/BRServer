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
#include "Object/SFLibraryComponentAdapter.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Service/ServerService.h"
#include "Util/SFRandom.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "PerformanceCounter/PerformanceCounterClient.h"



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
				pEngine->AddComponent(new(GetEngineHeap) LibraryComponentAdapter<StaticGameInstanceManagerServiceEntity>(GetEngineHeap(), this));

			svrCheck(Service::ServiceDirectory->WatchForService(Service::ServerConfig->GameClusterID, ClusterID::GameInstance));

			// first update delay
			m_TimeToUpdate.SetTimer(DurationMS(2000));


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

				m_TimeToUpdate.SetTimer(DurationMS(1000));
			}


			return hr;
		}

		Result StaticGameInstanceManagerServiceEntity::UpdateStaticZone()
		{
			Result hr;
			auto pZoneTable = Service::DataTableManager->GetDataTable("ZoneTable");
			svrCheckPtr(pZoneTable);

			Array<ServerServiceInformation*> services;

			svrCheck(Service::ServiceDirectory->GetServiceList(Service::ServerConfig->GameClusterID, ClusterID::GameInstance, services));

			for (auto itZone : *pZoneTable)
			{
				auto staticZoneCount = itZone.second->GetValue<int>("BaseStaticZoneCount");
				if (staticZoneCount <= 0)
					continue;

			}

			return hr;
		}


	} // namespace Svr {
} // namespace SF {
