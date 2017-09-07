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
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Memory/SFMemory.h"
#include "Net/NetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceTrans.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Service/ServerService.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "PerformanceCounter/PerformanceCounterClient.h"



namespace SF {
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
		BR_ENTITY_MESSAGE(Message::GameInstanceManager::CreateGameCmd)		{ svrMemReturn(pNewTrans = new(GetMemoryManager()) GameInstanceTransCreateGame(GetMemoryManager(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstanceManager::GameDeletedC2SEvt)	{ svrMemReturn(pNewTrans = new(GetMemoryManager()) GameInstanceTransGameDeleted(GetMemoryManager(), pMsgData)); return ResultCode::SUCCESS; } );
	}

	GameInstanceManagerServiceEntity::~GameInstanceManagerServiceEntity()
	{
	}

	Result GameInstanceManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr = ResultCode::SUCCESS;
		EntityUID entityUID;
		PerformanceCounterInstance* pInstance = nullptr;

		svrChk(LoadbalanceClusterServiceEntity::InitializeEntity(newEntityID));

		//entityUID = EntityUID(GetMyServerID(), Service::EntityTable->GenEntityID(EntityFaculty::Service));
		pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_NumberOfInstance);
		}

	Proc_End:

		return hr;
	}

	Result GameInstanceManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(LoadbalanceClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity ) );

		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstanceManager::CreateGameCmd)				{ svrMemReturn(pNewTrans = new(GetMemoryManager()) GameInstanceTransCreateGame(GetMemoryManager(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstanceManager::GameDeletedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetMemoryManager()) GameInstanceTransGameDeleted(GetMemoryManager(), pMsgData)); return ResultCode::SUCCESS; } );

	Proc_End:

		return hr;
	}

	Result GameInstanceManagerServiceEntity::OnNewInstance(GameInstanceEntity* pGameInstance)
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr(pGameInstance);

		if ((GetServerComponent<EntityManager>()->AddEntity(EntityFaculty::GameInstance, (Entity*)pGameInstance)))
		{
			++m_NumberOfInstance;
			m_LocalWorkload.fetch_add(1, std::memory_order_relaxed);
		}

	Proc_End:

		return hr;
	}

	// Called when a game instance is deleted
	Result GameInstanceManagerServiceEntity::FreeGameInstance(GameInsUID gameUID)
	{
		Result hr = ResultCode::SUCCESS;

		if ((GetServerComponent<EntityManager>()->RemoveEntity(gameUID.GetEntityID())))
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
		case GameID::MyTownHero:
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
}; // namespace SF {



