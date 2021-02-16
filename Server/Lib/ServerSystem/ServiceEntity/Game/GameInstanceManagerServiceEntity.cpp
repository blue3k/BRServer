////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Game instance service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
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

		GameInstanceManagerServiceEntity::GameInstanceManagerServiceEntity(GameID gameID, ServerConfig::ServerModuleGameInstanceManager* config, ClusterID clusterID, ClusterMembership initialMembership)
			: super(gameID, clusterID, initialMembership)
			, m_NumberOfInstance("NumberOfGameInstances")
			, m_GameInstances(GetHeap())
		{
			// Those are fallback handlers.
			// ServerEntity will handle it
			RegisterMessageHandler<GameInstanceTransCreateGameInstance>();
			RegisterMessageHandler<GameInstanceTransGameInstanceDeleted>();
			RegisterMessageHandler<GameInstanceTransSearchGameInstance>();
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

			//entityUID = EntityUID(GetMyServerID(), Service::EntityTable->GenEntityID(EntityFaculty::Service));
			pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
			if (pInstance != nullptr)
			{
				pInstance->AddCounter(&m_NumberOfInstance);
			}

			return hr;
		}

		Result GameInstanceManagerServiceEntity::RegisterServiceMessageHandler()
		{
			ScopeContext hr;

			svrCheck(super::RegisterServiceMessageHandler());

			// Let server entity handle it first
			svrCheck(RegisterMessageHandler<GameInstanceTransCreateGameInstance>());
			svrCheck(RegisterMessageHandler<GameInstanceTransGameInstanceDeleted>());
			svrCheck(RegisterMessageHandler<GameInstanceTransSearchGameInstance>());

			return hr;
		}

		Result GameInstanceManagerServiceEntity::OnNewInstance(GameInstanceEntity* pGameInstance)
		{
			ScopeContext hr;

			svrCheckPtr(pGameInstance);

			if ((Service::EntityManager->AddEntity(EntityFaculty::GameInstance, (Entity*)pGameInstance)))
			{
				++m_NumberOfInstance;
				m_LocalWorkload.fetch_add(1, std::memory_order_relaxed);
			}

			{
				SF::MutexScopeLock scopeLock(m_GameInstanceListLock);
				svrCheck(m_GameInstances.Insert(pGameInstance->GetEntityUID(), pGameInstance));
			}

			return hr;
		}

		// Called when a game instance is deleted
		Result GameInstanceManagerServiceEntity::FreeGameInstance(GameInsUID gameUID)
		{
			ScopeContext hr;
			GameInstanceEntity* pGameInstance = nullptr;

			{
				SF::MutexScopeLock scopeLock(m_GameInstanceListLock);
				svrCheck(m_GameInstances.Remove(pGameInstance->GetEntityUID(), pGameInstance));
			}

			if ((Service::EntityManager->RemoveEntity(gameUID.GetEntityID())))
			{
				--m_NumberOfInstance;
				m_LocalWorkload.fetch_sub(1, std::memory_order_relaxed);
			}

			return hr;
		}

		Result GameInstanceManagerServiceEntity::SearchGameInstance(size_t maxSearch, const char* searchKeyword, Array<GameInstanceInfo>& outList)
		{
			ScopeContext hr;

			int maxOffset = Util::Max(0, (int)m_GameInstances.size() - (int)maxSearch - 1);

			// TODO: search keyword
			m_GameInstances.ForeachOrder(
				Util::Random.Rand(maxOffset),
				static_cast<uint>(maxSearch),
				[this, &outList](GameInsUID key, GameInstanceEntity* value) -> Result
				{
					GameInstanceInfo instanceInfo;
					instanceInfo.GameInstanceUID = key;
					instanceInfo.TypeName = value->GetInstanceType();
					instanceInfo.DataID = value->GetDataID();
					outList.push_back(std::forward<GameInstanceInfo>(instanceInfo));
					return ResultCode::SUCCESS;
				});

			return hr;
		}


	} // namespace Svr {
} // namespace SF {


