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
#include "Types/SFEngineTypedefs.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "PerformanceCounter/PerformanceCounterClient.h"


#include "GameSvrConst.h"
#include "GamePlayerEntity.h"

#include "GameEntityManager.h"

#include "GameServerClass.h"



namespace SF {
namespace GameServer {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game User entity manager
	//


	// Constructor/Destructor
	GameEntityManager::GameEntityManager(uint NumTaskGroup)
		: EntityManager(NumTaskGroup)
		, m_NumberOfPlayers("NumberOfPlayers")
	{
	}

	GameEntityManager::~GameEntityManager()
	{
	}


	Result GameEntityManager::CreateEntity(ClusterID clusterID, EntityFaculty faculty, Svr::Entity* &pEntity)
	{
		switch (clusterID)
		{
		case ClusterID::Game:
		case ClusterID::Game_Conspiracy:
		{
			assert(faculty == EntityFaculty::User);
			pEntity = new(GetMemoryManager()) GamePlayerEntity;
			return pEntity != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
		}
		default:
			return super::CreateEntity(clusterID, faculty, pEntity);
		}

		return pEntity != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
	}

	//// Create new game instance
	//Result GameEntityManager::CreateGamePlayer(PlayerID playerID, GamePlayerEntity* &pGamePlayer)
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	svrChkPtr( pGamePlayer = new(GetMemoryManager()) GamePlayerEntity );

	//	svrChk(AddEntity(EntityFaculty::User, pGamePlayer));

	//	pGamePlayer->SetAccountID(playerID);

	//Proc_End:

	//	return hr;
	//}

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




}; // namespace GameServer
}; // namespace SF







