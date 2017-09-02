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
#include "Util/TimeUtil.h"
#include "Types/BrBaseTypes.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"


#include "GameInstanceEntity.h"
#include "GameEntityManager.h"

#include "ConspiracyGameInstanceServerClass.h"



namespace SF {
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


	Result GameEntityManager::CreateEntity(ClusterID clusterID, EntityFaculty faculty, Svr::Entity* &pEntity)
	{
		switch (clusterID)
		{
		case ClusterID::GameInstanceManager:
		{
			assert(faculty == EntityFaculty::GameInstance);
			pEntity = new GameInstanceEntity;
			return pEntity != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
		}
		default:
			return super::CreateEntity(clusterID, faculty, pEntity);
		}

		return pEntity != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
	}


	void GameEntityManager::OnEntityAdded(Svr::Entity* pEntity)
	{
		Svr::EntityManager::OnEntityAdded(pEntity);

		switch (pEntity->GetEntityID().GetFacultyID())
		{
		case (uint)EntityFaculty::User:
			++m_NumberOfPlayers;
			break;
		}
	}

	void GameEntityManager::OnEntityRemoved(Svr::Entity* pEntity)
	{
		Svr::EntityManager::OnEntityRemoved(pEntity);

		switch (pEntity->GetEntityID().GetFacultyID())
		{
		case (uint)EntityFaculty::User:
			--m_NumberOfPlayers;
			break;
		}
	}

	void GameEntityManager::RegisterCounter()
	{
		//Result hr = ResultCode::SUCCESS;

		auto pCounterInstance = Svr::PerformanceCounterClient::GetDefaultCounterInstance();
		if (pCounterInstance != nullptr)
		{
			pCounterInstance->AddCounter(&m_NumberOfPlayers);
		}

	//Proc_End:

		return;
	}

	// Initialize TaskManager
	Result GameEntityManager::InitializeManager( uint uiNumGroup )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::EntityManager::InitializeManager( uiNumGroup ) );

	Proc_End:

		return hr;
	}


	// Terminate TaskManager
	Result GameEntityManager::TerminateManager()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::EntityManager::TerminateManager() );

	Proc_End:

		return hr;
	}




}; // namespace GameServer
}; // namespace SF







