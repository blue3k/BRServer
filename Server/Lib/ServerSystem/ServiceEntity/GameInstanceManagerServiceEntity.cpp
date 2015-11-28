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
#include "ServerSystem/ServerServicebase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceTrans.h"
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
	}

	GameInstanceManagerServiceEntity::~GameInstanceManagerServiceEntity()
	{
	}

	HRESULT GameInstanceManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		HRESULT hr = S_OK;
		EntityUID entityUID;

		svrChk(LoadbalanceClusterServiceEntity::InitializeEntity(newEntityID));

		//entityUID = EntityUID(GetMyServerID(), GetEntityTable().GenEntityID(EntityFaculty::Service));
		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_NumberOfInstance);
		}

	Proc_End:

		return hr;
	}

	HRESULT GameInstanceManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		HRESULT hr = S_OK;

		svrChk(LoadbalanceClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity ) );

		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstanceManager::CreateGameCmd)				{ svrMemReturn(pNewTrans = new GameInstanceTransCreateGame(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::GameInstanceManager::GameDeletedC2SEvt)			{ svrMemReturn(pNewTrans = new GameInstanceTransGameDeleted(pMsgData)); return S_OK; } );

	Proc_End:

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



