////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Game instance service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Memory/SFMemory.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceTrans.h"
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

	GameInstanceManagerServiceEntity::GameInstanceManagerServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership)
		: super(gameID, clusterID, initialMembership )
		, m_NumberOfInstance("NumberOfGameInstances")
	{
		BR_ENTITY_MESSAGE(Message::GameInstanceManager::CreateGameCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) GameInstanceTransCreateGame(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::GameInstanceManager::GameDeletedC2SEvt)	{ svrMemReturn(pNewTrans = new(GetHeap()) GameInstanceTransGameDeleted(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
	}

	GameInstanceManagerServiceEntity::~GameInstanceManagerServiceEntity()
	{
	}

	Result GameInstanceManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr = ResultCode::SUCCESS;
		EntityUID entityUID;
		PerformanceCounterInstance* pInstance = nullptr;

		svrChk(super::InitializeEntity(newEntityID));

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

		svrChk(super::RegisterServiceMessageHandler( pServerEntity ) );

		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstanceManager::CreateGameCmd)				{ svrMemReturn(pNewTrans = new(GetHeap()) GameInstanceTransCreateGame(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstanceManager::GameDeletedC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) GameInstanceTransGameDeleted(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

	Proc_End:

		return hr;
	}

	Result GameInstanceManagerServiceEntity::OnNewInstance(GameInstanceEntity* pGameInstance)
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr(pGameInstance);

		if ((Service::EntityManager->AddEntity(EntityFaculty::GameInstance, (Entity*)pGameInstance)))
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

		if ((Service::EntityManager->RemoveEntity(gameUID.GetEntityID())))
		{
			--m_NumberOfInstance;
			m_LocalWorkload.fetch_sub(1, std::memory_order_relaxed);
		}

		//Proc_End:

		return hr;
	}






}; // namespace Svr {
}; // namespace SF {



