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
		FunctionContext hr;

		svrCheck(super::InitializeEntity(newEntityID) );

		m_CurrentProcessingNumberofMember = 0;

		m_LastRankingFailed = false;

		svrCheck(GetComponentManager().InitializeComponents());

		return hr;
	}

	// clear transaction
	Result GameInstanceServiceEntity::ClearEntity()
	{
		FunctionContext hr;

		svrCheck(super::ClearEntity() );

		return hr;
	}

	Result GameInstanceServiceEntity::TickUpdate(TimerAction *pAction)
	{
		FunctionContext hr;

		svrCheck(super::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			return hr;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			return hr;


		m_ComponentManger.TickUpdate();

		return hr;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	
	//







}; // namespace Svr {
}; // namespace SF {



