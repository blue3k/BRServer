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
#include "Common/TimeUtil.h"
#include "Common/BRBasetypes.h"

#include "ServerSystem/BrServer.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"


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


	// add entity to table
	HRESULT EntityManager::AddEntity( EntityFaculty faculty, Svr::Entity* pEntity )
	{
		HRESULT hr = S_OK;

		svrChk( pEntity->InitializeEntity( GetEntityTable().GenEntityID(faculty) ) );

		if( faculty == EntityFaculty::Service )
		{
			ServiceEntity* pServiceEntity = dynamic_cast<ServiceEntity*>(pEntity);
			//AssertRel(pServiceEntity != nullptr);
			if (pServiceEntity != nullptr)
			{
				pServiceEntity->RegisterServiceMessageHandler(GetLoopbackServerEntity());
			}
		}

		svrChk( AddTickTask( pEntity ) );

		svrChk(GetEntityTable().Insert(pEntity->GetEntityID(), pEntity));

		OnEntityAdded(pEntity);

	Proc_End:

		return hr;
	}
	
	// add entity to table
	HRESULT EntityManager::AddEntity( EntityID entityID, Svr::Entity* pEntity )
	{
		HRESULT hr = S_OK;

		svrChk( GetEntityTable().ReserveEntityID( entityID ) );

		svrChk( pEntity->InitializeEntity( entityID ) );

		if( entityID.FacultyID == (UINT)EntityFaculty::Service )
		{
			ClusteredServiceEntity* pClusterService = DEBUG_DYNAMIC_CAST(ClusteredServiceEntity*,pEntity);
			if( pClusterService != nullptr )
				pClusterService->RegisterServiceMessageHandler( GetLoopbackServerEntity() );
		}

		svrChk( AddTickTask( pEntity ) );

		svrChk(GetEntityTable().Insert(pEntity->GetEntityID(), pEntity));

		OnEntityAdded(pEntity);

	Proc_End:

		return hr;
	}

	void EntityManager::OnEntityAdded(Entity* pEntity)
	{
		switch (pEntity->GetEntityID().EntityLID)
		{
		case (UINT)EntityFaculty::Service:
			++m_NumberOfServices;
			break;
		}
		++m_NumberOfTotalEntities;
	}

	HRESULT EntityManager::FindEntity(EntityID entityID, SharedPointerT<Entity> &pEntity)
	{
		return GetEntityTable().Find( entityID, pEntity );
	}

	// add entity to table
	HRESULT EntityManager::RemoveEntity(EntityID entityID)
	{
		HRESULT hr = S_OK;

		SharedPointerT<Entity> pEntity;
		hr = GetEntityTable().Erase(entityID, pEntity);
		if (FAILED(hr))
		{
			hr = S_FALSE;
			goto Proc_End;
		}
		pEntity->SetEntityUID(0);

		switch (pEntity->GetEntityState())
		{
		case EntityState::CLOSING:
		case EntityState::WORKING:
			pEntity->SetEntityState(EntityState::FREE);
			break;
		};

		svrChk(RemoveTickTask((Entity*)pEntity));

		OnEntityRemoved((Entity*)pEntity);

	Proc_End:

		return hr;
	}

	// add entity to table
	HRESULT EntityManager::RemoveEntity( Svr::Entity* pEntity )
	{
		//HRESULT hr = S_OK;

		if (pEntity == nullptr)
			return E_FAIL;

		return RemoveEntity(pEntity->GetEntityID());

		//SharedPointerT<Entity> pRemoved;

		//hr = GetEntityTable().Erase(pEntity->GetEntityID(), pRemoved);
		//if (FAILED(hr))
		//{
		//	hr = S_FALSE;
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
		switch (pEntity->GetEntityID().EntityLID)
		{
		case (UINT)EntityFaculty::Service:
			--m_NumberOfServices;
			break;
		}
		--m_NumberOfTotalEntities;
	}


	// Initialize TaskManager
	HRESULT EntityManager::InitializeManager(UINT uiNumGroup)
	{
		HRESULT hr = S_OK;

		svrChk(__super::InitializeManager(uiNumGroup));

		//m_PerformanceCounterInstance = SharedPointerT < PerformanceCounterInstance >(new PerformanceCounterInstance("EntityManager", entityUID));
		auto counterInstance = PerformanceCounterClient::GetDefaultCounterInstance();
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
	HRESULT EntityManager::TerminateManager()
	{
		HRESULT hr = S_OK;

		//if (m_PerformanceCounterInstance != nullptr)
		//{
		//	//GetEntityTable().FreeEntityID(m_PerformanceCounterInstance->GetInstanceEntityUID());
		//	m_PerformanceCounterInstance = SharedPointerT < PerformanceCounterInstance >();
		//}

		svrChk(__super::TerminateManager());

	Proc_End:

		return hr;
	}


	void EntityManager::TerminateComponent()
	{
		TerminateManager();
	}



}; // namespace GameServer
}; // namespace BR







