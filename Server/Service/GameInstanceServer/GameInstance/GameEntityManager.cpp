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
#include "Common/TimeUtil.h"
#include "Common/BrBaseTypes.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"


#include "GameInstanceEntity.h"
#include "GameEntityManager.h"

#include "ConspiracyGameInstanceServerClass.h"



namespace BR {
namespace ConspiracyGameInstanceServer {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game User entity manager
	//


	// Constructor/Destructor
	GameEntityManager::GameEntityManager()
		: m_NumberOfPlayers("NumberOfPlayers")
	{
	}

	GameEntityManager::~GameEntityManager()
	{
	}


	HRESULT GameEntityManager::CreateEntity(ClusterID clusterID, EntityFaculty faculty, Svr::Entity* &pEntity)
	{
		switch (clusterID)
		{
		case ClusterID::GameInstanceManager:
		{
			assert(faculty == EntityFaculty::GameInstance);
			pEntity = new GameInstanceEntity;
			return pEntity != nullptr ? S_SYSTEM_OK : E_SYSTEM_OUTOFMEMORY;
		}
		default:
			return super::CreateEntity(clusterID, faculty, pEntity);
		}

		return pEntity != nullptr ? S_SYSTEM_OK : E_SYSTEM_OUTOFMEMORY;
	}


	void GameEntityManager::OnEntityAdded(Svr::Entity* pEntity)
	{
		Svr::EntityManager::OnEntityAdded(pEntity);

		switch (pEntity->GetEntityID().GetFacultyID())
		{
		case (UINT)EntityFaculty::User:
			++m_NumberOfPlayers;
			break;
		}
	}

	void GameEntityManager::OnEntityRemoved(Svr::Entity* pEntity)
	{
		Svr::EntityManager::OnEntityRemoved(pEntity);

		switch (pEntity->GetEntityID().GetFacultyID())
		{
		case (UINT)EntityFaculty::User:
			--m_NumberOfPlayers;
			break;
		}
	}

	void GameEntityManager::RegisterCounter()
	{
		//HRESULT hr = S_SYSTEM_OK;

		auto pCounterInstance = Svr::PerformanceCounterClient::GetDefaultCounterInstance();
		if (pCounterInstance != nullptr)
		{
			pCounterInstance->AddCounter(&m_NumberOfPlayers);
		}

	//Proc_End:

		return;
	}

	// Initialize TaskManager
	HRESULT GameEntityManager::InitializeManager( UINT uiNumGroup )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(Svr::EntityManager::InitializeManager( uiNumGroup ) );

	Proc_End:

		return hr;
	}


	// Terminate TaskManager
	HRESULT GameEntityManager::TerminateManager()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(Svr::EntityManager::TerminateManager() );

	Proc_End:

		return hr;
	}




}; // namespace GameServer
}; // namespace BR







