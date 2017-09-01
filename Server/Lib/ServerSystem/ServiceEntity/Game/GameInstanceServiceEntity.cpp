////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Memory/SFMemory.h"
#include "Common/GameConst.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceServiceEntity.h"
#include "ServerSystem/ServiceEntity/RankingServiceTrans.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"



namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameInstanceServiceEntity::GameInstanceServiceEntity(ClusterMembership initialMembership)
		:ReplicaClusterServiceEntity(ClusterID::GameInstanceManager, initialMembership)
	{
	}

	GameInstanceServiceEntity::~GameInstanceServiceEntity()
	{
	}

	Result GameInstanceServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		m_CurrentProcessingNumberofMember = 0;

		m_LastRankingFailed = false;

	Proc_End:

		return hr;
	}

	// clear transaction
	Result GameInstanceServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ReplicaClusterServiceEntity::ClearEntity() );

	Proc_End:

		return hr;
	}

	Result GameInstanceServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ReplicaClusterServiceEntity::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			goto Proc_End;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			goto Proc_End;


	Proc_End:

		return hr;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	
	//







}; // namespace Svr {
}; // namespace BR {



