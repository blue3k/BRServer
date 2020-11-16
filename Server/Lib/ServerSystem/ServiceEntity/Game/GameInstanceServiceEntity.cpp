////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Memory/SFMemory.h"
#include "GameConst.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/Game/GameInstanceServiceEntity.h"
#include "ServiceEntity/RankingServiceTrans.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Server/BrServer.h"


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameInstanceServiceEntity::GameInstanceServiceEntity(GameID gameID, ClusterMembership initialMembership)
		: super(gameID, ClusterID::GameInstanceManager, initialMembership)
		, m_ComponentManger(GetHeap())
	{
	}

	GameInstanceServiceEntity::~GameInstanceServiceEntity()
	{
	}

	Result GameInstanceServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		ScopeContext hr;

		svrCheck(super::InitializeEntity(newEntityID) );

		// TODO: Add components
		

		svrCheck(m_ComponentManger.InitializeComponents());

		return hr;
	}

	// clear transaction
	Result GameInstanceServiceEntity::ClearEntity()
	{
		ScopeContext hr;

		svrCheck(GetComponentManager().TerminateComponents());

		svrCheck(super::ClearEntity() );

		return hr;
	}

	Result GameInstanceServiceEntity::TickUpdate(TimerAction *pAction)
	{
		ScopeContext hr;

		svrCheck(super::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			return hr;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			return hr;

		m_ComponentManger.TickUpdate();

		return hr;
	}


} // namespace Svr {
} // namespace SF {

