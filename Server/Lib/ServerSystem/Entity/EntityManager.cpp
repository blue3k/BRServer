////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game entity manager
//	
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Util/TimeUtil.h"
#include "Types/SFEngineTypedefs.h"

#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityTable.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "PerformanceCounter/PerformanceCounterClient.h"
#include "ServiceEntity/Login/LoginPlayerEntity.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Service/ServerService.h"


namespace SF {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game entity manager
	//

	constexpr FixedString EntityManager::TypeName;


	// Constructor/Destructor
	EntityManager::EntityManager(uint numTaskGroup)
		: m_MemoryManager("EntityManager", GetSystemMemoryManager())
		, m_SharedObjectManager(m_MemoryManager)
		, m_NumTaskGroup(numTaskGroup)
		, m_NumberOfServices("NumberOfService")
		, m_NumberOfTotalEntities("TotalEntities")
		, m_EntityCretors(GetHeap())
	{
	}

	EntityManager::~EntityManager()
	{
	}

	void EntityManager::RegisterEntityCreator(const EntityCreator& creator)
	{
		m_EntityCretors.push_back(creator);
	}

	Result EntityManager::CreateEntity(ClusterID clusterID, EntityFaculty faculty, Entity* &pEntity)
	{
		for (auto itCreator : m_EntityCretors)
		{
			pEntity = itCreator(clusterID, faculty);
			if (pEntity != nullptr)
				return ResultCode::SUCCESS;
		}

		if (faculty == EntityFaculty::User)
		{
			switch (clusterID)
			{
			case ClusterID::Login:
				pEntity = new(GetMemoryManager()) LoginPlayerEntity;
				break;
			case ClusterID::Game:
				pEntity = new(GetMemoryManager()) GamePlayerEntity;
				break;
			default:
				return ResultCode::INVALID_ARG;
			}
		}

		return pEntity != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
	}

	// add entity to table
	Result EntityManager::AddEntity( EntityFaculty faculty, Svr::Entity* pEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( pEntity->InitializeEntity(Service::EntityTable->GenEntityID(faculty) ) );

		// Disable ServiceMessage handler
		//if( faculty == EntityFaculty::Service )
		//{
		//	ServiceEntity* pServiceEntity = dynamic_cast<ServiceEntity*>(pEntity);
		//	//AssertRel(pServiceEntity != nullptr);
		//	if (pServiceEntity != nullptr)
		//	{
		//		pServiceEntity->RegisterServiceMessageHandler(GetLoopbackServerEntity());
		//	}
		//}

		m_SharedObjectManager.RegisterSharedObject(pEntity);

		svrChk( AddTickTask( pEntity ) );

		svrChk(Service::EntityTable->insert(pEntity));

		OnEntityAdded(pEntity);

	Proc_End:

		return hr;
	}
	
	// add entity to table
	Result EntityManager::AddEntity( EntityID entityID, Svr::Entity* pEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Service::EntityTable->ReserveEntityID( entityID ) );

		svrChk( pEntity->InitializeEntity( entityID ) );

		// Disable ServiceMessage handler
		//if( entityID.GetFacultyID() == (uint)EntityFaculty::Service )
		//{
		//	ClusteredServiceEntity* pClusterService = dynamic_cast<ClusteredServiceEntity*>(pEntity);
		//	if( pClusterService != nullptr )
		//		pClusterService->RegisterServiceMessageHandler( GetLoopbackServerEntity() );
		//}

		svrChk( AddTickTask( pEntity ) );

		svrChk(Service::EntityTable->insert(pEntity));

		OnEntityAdded(pEntity);

	Proc_End:

		return hr;
	}

	void EntityManager::OnEntityAdded(Entity* pEntity)
	{
		switch (pEntity->GetEntityID().GetEntityLID())
		{
		case (uint)EntityFaculty::Service:
			++m_NumberOfServices;
			break;
		}
		++m_NumberOfTotalEntities;
	}

	Result EntityManager::FindEntity(EntityID entityID, SharedPointerT<Entity> &pEntity)
	{
		return Service::EntityTable->find( entityID, pEntity );
	}

	// add entity to table
	Result EntityManager::RemoveEntity(EntityID entityID)
	{
		Result hr = ResultCode::SUCCESS;

		SharedPointerT<Entity> pEntity;
		hr = Service::EntityTable->erase(entityID, pEntity);
		if (!(hr))
		{
			hr = ResultCode::SUCCESS_FALSE;
			goto Proc_End;
		}
		pEntity->SetEntityUID(0);

		switch (pEntity->GetEntityState())
		{
		case EntityState::CLOSING:
		case EntityState::WORKING:
			pEntity->SetEntityState(EntityState::FREE);
			break;
		default:
			break;
		};

		svrChk(RemoveTickTask((Entity*)pEntity));

		OnEntityRemoved((Entity*)pEntity);

	Proc_End:

		return hr;
	}

	// add entity to table
	Result EntityManager::RemoveEntity( Svr::Entity* pEntity )
	{
		//Result hr = ResultCode::SUCCESS;

		if (pEntity == nullptr)
			return ResultCode::FAIL;

		return RemoveEntity(pEntity->GetEntityID());

		//SharedPointerT<Entity> pRemoved;

		//hr = Service::EntityTable->Erase(pEntity->GetEntityID(), pRemoved);
		//if (!(hr))
		//{
		//	hr = ResultCode::SUCCESS_FALSE;
		//	goto Proc_End;
		//}
		////svrChk(Service::EntityTable->Erase(pEntity->GetEntityID(), pRemoved));

		//pEntity->SetEntityState( EntityState::CLOSING );

		//OnEntityRemoved(pEntity);

		//svrChk( RemoveTickTask( pEntity ) );

	//Proc_End:

	//	return hr;
	}

	void EntityManager::OnEntityRemoved(Entity* pEntity)
	{
		switch (pEntity->GetEntityID().GetEntityLID())
		{
		case (uint)EntityFaculty::Service:
			--m_NumberOfServices;
			break;
		}
		--m_NumberOfTotalEntities;
	}

	// Initialize component
	Result EntityManager::InitializeComponent()
	{
		Result hr;
		PerformanceCounterInstance* counterInstance = nullptr;

		svrChk(TickTaskManager::InitializeManager(m_NumTaskGroup));

		//m_PerformanceCounterInstance = SharedPointerT < PerformanceCounterInstance >(new PerformanceCounterInstance("EntityManager", entityUID));
		counterInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (counterInstance != nullptr)
		{
			counterInstance->AddCounter(&m_NumberOfServices);
			counterInstance->AddCounter(&m_NumberOfTotalEntities);
			RegisterCounter();
		}
		//m_PerformanceCounterInstance->RegisterToClient();

		Service::EntityManager = this;

	Proc_End:

		return hr;
	}

	void EntityManager::Clear()
	{
		DynamicArray<SharedPointerT<Svr::Entity>> entityList(GetHeap());
		entityList.SetIncreaseSize(2048);
		Service::EntityTable->for_each([&](const SharedPointerT<Svr::Entity>& item) -> bool
		{
			entityList.push_back(item);
			return true;
		});

		for (auto& itEntity : entityList)
		{
			RemoveEntity(*itEntity);
		}

		FlushDeletedEntity();
	}

	void EntityManager::FlushDeletedEntity()
	{
		m_SharedObjectManager.ClearFreedObjects();
	}

	void EntityManager::Update()
	{
		m_SharedObjectManager.Update();
	}

	// Terminate component
	void EntityManager::DeinitializeComponent()
	{
		Result hr = ResultCode::SUCCESS;

		//if (m_PerformanceCounterInstance != nullptr)
		//{
		//	//Service::EntityTable->FreeEntityID(m_PerformanceCounterInstance->GetInstanceEntityUID());
		//	m_PerformanceCounterInstance = SharedPointerT < PerformanceCounterInstance >();
		//}

		svrChk(TickTaskManager::TerminateManager());

		Service::EntityManager = nullptr;

	Proc_End:

		return;
	}




}; // namespace GameServer
}; // namespace SF







