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


namespace SF {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game User entity manager
	//


	// Constructor/Destructor
	EntityManager::EntityManager()
		: IServerComponent(ComponentID)
		, m_MemoryManager("EntityManager", GetSystemMemoryManager())
		, m_NumberOfServices("NumberOfService")
		, m_NumberOfTotalEntities("TotalEntities")
	{
	}


	EntityManager::~EntityManager()
	{
	}

	Result EntityManager::CreateEntity(ClusterID clusterID, EntityFaculty faculty, Entity* &pEntity)
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

		return pEntity != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
	}

	// add entity to table
	Result EntityManager::AddEntity( EntityFaculty faculty, Svr::Entity* pEntity )
	{
		Result hr = ResultCode::SUCCESS;
		auto& entityTable = GetEntityTable();
		svrChk( pEntity->InitializeEntity(entityTable.GenEntityID(faculty) ) );

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

		svrChk( AddTickTask( pEntity ) );

		svrChk(entityTable.Insert(pEntity));

		OnEntityAdded(pEntity);

	Proc_End:

		return hr;
	}
	
	// add entity to table
	Result EntityManager::AddEntity( EntityID entityID, Svr::Entity* pEntity )
	{
		Result hr = ResultCode::SUCCESS;
		auto& entityTable = GetEntityTable();

		svrChk(entityTable.ReserveEntityID( entityID ) );

		svrChk( pEntity->InitializeEntity( entityID ) );

		// Disable ServiceMessage handler
		//if( entityID.GetFacultyID() == (uint)EntityFaculty::Service )
		//{
		//	ClusteredServiceEntity* pClusterService = dynamic_cast<ClusteredServiceEntity*>(pEntity);
		//	if( pClusterService != nullptr )
		//		pClusterService->RegisterServiceMessageHandler( GetLoopbackServerEntity() );
		//}

		svrChk( AddTickTask( pEntity ) );

		svrChk(entityTable.Insert(pEntity));

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
		auto& entityTable = GetEntityTable();
		return entityTable.Find( entityID, pEntity );
	}

	// add entity to table
	Result EntityManager::RemoveEntity(EntityID entityID)
	{
		Result hr = ResultCode::SUCCESS;
		auto& entityTable = GetEntityTable();

		SharedPointerT<Entity> pEntity;
		hr = entityTable.Erase(entityID, pEntity);
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

		//hr = GetEntityTable().Erase(pEntity->GetEntityID(), pRemoved);
		//if (!(hr))
		//{
		//	hr = ResultCode::SUCCESS_FALSE;
		//	goto Proc_End;
		//}
		////svrChk(GetEntityTable().Erase(pEntity->GetEntityID(), pRemoved));

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


	// Initialize TickTaskManager
	Result EntityManager::InitializeManager(uint uiNumGroup)
	{
		Result hr = ResultCode::SUCCESS;
		PerformanceCounterInstance* counterInstance = nullptr;

		svrChk(TickTaskManager::InitializeManager(uiNumGroup));

		//m_PerformanceCounterInstance = SharedPointerT < PerformanceCounterInstance >(new PerformanceCounterInstance("EntityManager", entityUID));
		counterInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (counterInstance != nullptr)
		{
			counterInstance->AddCounter(&m_NumberOfServices);
			counterInstance->AddCounter(&m_NumberOfTotalEntities);
			RegisterCounter();
		}
		//m_PerformanceCounterInstance->RegisterToClient();

	Proc_End:

		return hr;
	}


	// Terminate TickTaskManager
	Result EntityManager::TerminateManager()
	{
		Result hr = ResultCode::SUCCESS;

		//if (m_PerformanceCounterInstance != nullptr)
		//{
		//	//GetEntityTable().FreeEntityID(m_PerformanceCounterInstance->GetInstanceEntityUID());
		//	m_PerformanceCounterInstance = SharedPointerT < PerformanceCounterInstance >();
		//}

		svrChk(TickTaskManager::TerminateManager());

	Proc_End:

		return hr;
	}


	void EntityManager::TerminateComponent()
	{
		TerminateManager();
	}



}; // namespace GameServer
}; // namespace SF







