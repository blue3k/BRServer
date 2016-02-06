////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Game instance service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/Memory.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceTrans.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"



namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameInstanceManagerServiceEntity::GameInstanceManagerServiceEntity(ClusterID clusterID, ClusterMembership initialMembership)
		: LoadbalanceClusterServiceEntity(clusterID, initialMembership )
		, IServerComponent(ComponentID)
		, m_NumberOfInstance("NumberOfGameInstances")
	{
		BR_ENTITY_MESSAGE(Message::GameInstanceManager::CreateGameCmd) { svrMemReturn(pNewTrans = new GameInstanceTransCreateGame(pMsgData)); return S_SYSTEM_OK; } );
		BR_ENTITY_MESSAGE(Message::GameInstanceManager::GameDeletedC2SEvt) { svrMemReturn(pNewTrans = new GameInstanceTransGameDeleted(pMsgData)); return S_SYSTEM_OK; } );
	}

	GameInstanceManagerServiceEntity::~GameInstanceManagerServiceEntity()
	{
	}

	HRESULT GameInstanceManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		HRESULT hr = S_SYSTEM_OK;
		EntityUID entityUID;
		PerformanceCounterInstance* pInstance = nullptr;

		svrChk(LoadbalanceClusterServiceEntity::InitializeEntity(newEntityID));

		//entityUID = EntityUID(GetMyServerID(), GetEntityTable().GenEntityID(EntityFaculty::Service));
		pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_NumberOfInstance);
		}

	Proc_End:

		return hr;
	}

	HRESULT GameInstanceManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(LoadbalanceClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity ) );

		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstanceManager::CreateGameCmd)				{ svrMemReturn(pNewTrans = new GameInstanceTransCreateGame(pMsgData)); return S_SYSTEM_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstanceManager::GameDeletedC2SEvt)			{ svrMemReturn(pNewTrans = new GameInstanceTransGameDeleted(pMsgData)); return S_SYSTEM_OK; } );

	Proc_End:

		return hr;
	}

	HRESULT GameInstanceManagerServiceEntity::OnNewInstance(GameInstanceEntity* pGameInstance)
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChkPtr(pGameInstance);

		if (SUCCEEDED(GetServerComponent<EntityManager>()->AddEntity(EntityFaculty::GameInstance, (Entity*)pGameInstance)))
		{
			++m_NumberOfInstance;
			m_LocalWorkload.fetch_add(1, std::memory_order_relaxed);
		}

	Proc_End:

		return hr;
	}

	// Called when a game instance is deleted
	HRESULT GameInstanceManagerServiceEntity::FreeGameInstance(GameInsUID gameUID)
	{
		HRESULT hr = S_SYSTEM_OK;

		if (SUCCEEDED(GetServerComponent<EntityManager>()->RemoveEntity(gameUID.GetEntityID())))
		{
			--m_NumberOfInstance;
			m_LocalWorkload.fetch_sub(1, std::memory_order_relaxed);
		}

		//Proc_End:

		return hr;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameInstanceManagerWatcherServiceEntity class
	//


	GameInstanceManagerWatcherServiceEntity::GameInstanceManagerWatcherServiceEntity( GameID gameID )
		:LoadbalanceClusterServiceEntity(ClusterIDFromGameID(gameID), ClusterMembership::StatusWatcher)
		,IServerComponent(ComponentID)
	{
	}

	ClusterID GameInstanceManagerWatcherServiceEntity::ClusterIDFromGameID( GameID gameID )
	{
		switch(gameID)
		{
		case GameID::Game:
		case GameID::Conspiracy:
			return ClusterID::GameInstanceManager;
			break;
		default:
			// invalid game id
			AssertRel(0);
			return ClusterID::GameInstanceManager;
		}
	}

	GameInstanceManagerWatcherServiceEntity::~GameInstanceManagerWatcherServiceEntity()
	{
	}

	





}; // namespace Svr {
}; // namespace BR {



