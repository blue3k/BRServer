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
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
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
		//	Entity informations
		//

		GameInstanceManagerServiceEntity::GameInstanceManagerServiceEntity(GameID gameID, ServerConfig::ServerModuleGameInstanceManager* config, ClusterID clusterID, const EndpointAddress& endpoint)
			: super(gameID, clusterID, endpoint)
			, m_NumberOfInstance("NumberOfGameInstances")
			, m_GameInstances(GetHeap())
		{
			RegisterMessageHandler<GameInstanceManagerTransCreateGameInstance>();
		}

		GameInstanceManagerServiceEntity::~GameInstanceManagerServiceEntity()
		{
		}

		Result GameInstanceManagerServiceEntity::InitializeEntity(EntityID newEntityID)
		{
			ScopeContext hr;
			EntityUID entityUID;
			PerformanceCounterInstance* pInstance = nullptr;

			svrCheck(super::InitializeEntity(newEntityID));

			svrCheck(Service::DataTableManager->LoadDataTable("ZoneTable", "ZoneId", "gtbl_ZoneTable"));

			pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
			if (pInstance != nullptr)
			{
				pInstance->AddCounter(&m_NumberOfInstance);
			}

			auto pEngine = Engine::GetInstance();
			if (pEngine)
				pEngine->AddComponent(new(GetEngineHeap()) LibraryComponentAdapter<GameInstanceManagerServiceEntity>(GetEngineHeap(), this));

			return hr;
		}

		Result GameInstanceManagerServiceEntity::OnNewInstance(GameInstanceEntity* pGameInstance)
		{
			ScopeContext hr;

			svrCheckPtr(pGameInstance);

			++m_NumberOfInstance;
			//m_LocalWorkload.fetch_add(1, std::memory_order_relaxed);

			{
				SF::MutexScopeLock scopeLock(m_GameInstanceListLock);
				svrCheck(m_GameInstances.Insert(pGameInstance->GetEntityUID(), pGameInstance));
			}

			return hr;
		}

		// Called when a game instance is deleted
		Result GameInstanceManagerServiceEntity::FreeGameInstance(GameInsUID gameUID)
		{
			GameInstanceEntity* pGameInstance = nullptr;
			ScopeContext hr;

			{
				SF::MutexScopeLock scopeLock(m_GameInstanceListLock);
				svrCheck(m_GameInstances.Remove(gameUID, pGameInstance));
			}

			--m_NumberOfInstance;

			return hr;
		}


	} // namespace Svr {
} // namespace SF {
