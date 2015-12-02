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
#include "Common/BRBasetypes.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"


#include "GameSvrConst.h"
#include "GamePlayerEntity.h"

#include "GameEntityManager.h"

#include "GameServerClass.h"



namespace BR {
namespace GameServer {



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


	// Create new game instance
	HRESULT GameEntityManager::CreateGamePlayer(PlayerID playerID, GamePlayerEntity* &pGamePlayer)
	{
		HRESULT hr = S_OK;

		svrChkPtr( pGamePlayer = new GamePlayerEntity );

		svrChk(AddEntity(EntityFaculty::User, pGamePlayer));

		pGamePlayer->SetAccountID(playerID);

	Proc_End:

		return hr;
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
		HRESULT hr = S_OK;

		auto pCounterInstance = Svr::PerformanceCounterClient::GetDefaultCounterInstance();
		if (pCounterInstance != nullptr)
		{
			pCounterInstance->AddCounter(&m_NumberOfPlayers);
		}

	Proc_End:

		return;
	}

	// Initialize TaskManager
	HRESULT GameEntityManager::InitializeManager( UINT uiNumGroup )
	{
		HRESULT hr = S_OK;

		svrChk(Svr::EntityManager::InitializeManager( uiNumGroup ) );

	Proc_End:

		return hr;
	}


	// Terminate TaskManager
	HRESULT GameEntityManager::TerminateManager()
	{
		HRESULT hr = S_OK;

		svrChk(Svr::EntityManager::TerminateManager() );

	Proc_End:

		return hr;
	}




}; // namespace GameServer
}; // namespace BR







