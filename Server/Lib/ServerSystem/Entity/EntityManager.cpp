////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game entity manager
//	
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"

#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityTable.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "PerformanceCounter/PerformanceCounterClient.h"
#include "ServiceEntity/Login/LoginPlayerEntity.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "Service/ServerService.h"


namespace SF {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game entity manager
	//

	constexpr StringCrc64 EntityManager::TypeName;


	// Constructor/Destructor
	EntityManager::EntityManager(uint numTaskGroup)
		: m_Heap("EntityManager", GetSystemHeap())
		, m_SharedObjectManager(m_Heap)
		, m_NumTaskGroup(numTaskGroup)
		, m_NumberOfServices("NumberOfService")
		, m_NumberOfTotalEntities("TotalEntities")
		, m_EntityCreators(GetHeap())
	{

		RegisterEntityCreator([this](ClusterID clusterID, EntityFaculty faculty) -> Entity*
			{
				if (clusterID == ClusterID::Login && faculty == EntityFaculty::User)
					return new(GetHeap()) LoginPlayerEntity;
				return nullptr;
			});

		RegisterEntityCreator([this](ClusterID clusterID, EntityFaculty faculty) -> Entity*
			{
				if (clusterID == ClusterID::Game && faculty == EntityFaculty::User)
					return new(GetHeap()) Svr::GamePlayerEntity;
				return nullptr;
			});

		RegisterEntityCreator([this](ClusterID clusterID, EntityFaculty faculty) -> Entity*
			{
				if (faculty == EntityFaculty::GameInstance)
					return new(GetHeap()) Svr::GameInstanceEntity;
				return nullptr;
			});

	}

	EntityManager::~EntityManager()
	{
	}

	void EntityManager::RegisterEntityCreator(const EntityCreator& creator)
	{
		m_EntityCreators.push_back(creator);
	}

	Result EntityManager::CreateEntity(ClusterID clusterID, EntityFaculty faculty, Entity* &pEntity)
	{
		// registered laster gets higher priority
		for (int iItem = m_EntityCreators.size() - 1; iItem >= 0; --iItem)
		{
			pEntity = m_EntityCreators[iItem](clusterID, faculty);
			if (pEntity != nullptr)
				return ResultCode::SUCCESS;
		}

		return pEntity != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
	}

	// add entity to table
	Result EntityManager::AddEntity( EntityFaculty faculty, Svr::Entity* pEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrCheck(pEntity->InitializeEntity(Service::EntityTable->GenEntityID(faculty)));

		// Disable ServiceMessage handler
		//if( faculty == EntityFaculty::Service )
		//{
		//	ServiceEntity* pServiceEntity = dynamic_cast<ServiceEntity*>(pEntity);
		//	//AssertRel(pServiceEntity != nullptr);
		//	if (pServiceEntity != nullptr)
		//	{
		//		pServiceEntity->RegisterServiceMessageHandler();
		//	}
		//}

		m_SharedObjectManager.RegisterSharedObject(pEntity);

		svrCheck( AddTickTask( pEntity ) );

		svrCheck(Service::EntityTable->insert(pEntity));

		OnEntityAdded(pEntity);

		return hr;
	}
	
	// add entity to table
	Result EntityManager::AddEntity( EntityID entityID, Svr::Entity* pEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrCheck(Service::EntityTable->ReserveEntityID( entityID ));

		svrCheck(pEntity->InitializeEntity( entityID ));

		// Disable ServiceMessage handler
		//if(entityID.GetFacultyID() == (uint)EntityFaculty::Service)
		//{
		//	auto* pClusterService = dynamic_cast<ClusteredServiceEntity*>(pEntity);
		//	if( pClusterService != nullptr )
		//		pClusterService->RegisterServiceMessageHandler();
		//}

		svrCheck( AddTickTask( pEntity ) );

		svrCheck(Service::EntityTable->insert(pEntity));

		OnEntityAdded(pEntity);

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
		if (!hr)
		{
			return hr = ResultCode::SUCCESS_FALSE;
		}
		pEntity->ClearEntityUID();

		switch (pEntity->GetEntityState())
		{
		case EntityState::CLOSING:
		case EntityState::WORKING:
			pEntity->SetEntityState(EntityState::FREE);
			break;
		default:
			break;
		};

		svrCheck(RemoveTickTask((Entity*)pEntity));

		OnEntityRemoved((Entity*)pEntity);

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







