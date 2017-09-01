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
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "Util/TimeUtil.h"
#include "Types/BrBaseTypes.h"

#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"
#include "ServerSystem/ServiceEntity/Login/LoginPlayerEntity.h"
#include "ServerSystem/ServiceEntity/Game/GamePlayerEntity.h"


namespace BR {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game User entity manager
	//


	// Constructor/Destructor
	EntityManager::EntityManager()
		: IServerComponent(ComponentID)
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
			pEntity = new LoginPlayerEntity;
			break;
		case ClusterID::Game:
			pEntity = new GamePlayerEntity;
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
		//if( entityID.GetFacultyID() == (UINT)EntityFaculty::Service )
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
		case (UINT)EntityFaculty::Service:
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
		case (UINT)EntityFaculty::Service:
			--m_NumberOfServices;
			break;
		}
		--m_NumberOfTotalEntities;
	}


	// Initialize TaskManager
	Result EntityManager::InitializeManager(UINT uiNumGroup)
	{
		Result hr = ResultCode::SUCCESS;
		PerformanceCounterInstance* counterInstance = nullptr;

		svrChk(TaskManager::InitializeManager(uiNumGroup));

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


	// Terminate TaskManager
	Result EntityManager::TerminateManager()
	{
		Result hr = ResultCode::SUCCESS;

		//if (m_PerformanceCounterInstance != nullptr)
		//{
		//	//GetEntityTable().FreeEntityID(m_PerformanceCounterInstance->GetInstanceEntityUID());
		//	m_PerformanceCounterInstance = SharedPointerT < PerformanceCounterInstance >();
		//}

		svrChk(TaskManager::TerminateManager());

	Proc_End:

		return hr;
	}


	void EntityManager::TerminateComponent()
	{
		TerminateManager();
	}



}; // namespace GameServer
}; // namespace BR







