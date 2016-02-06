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
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/Memory.h"
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

	HRESULT GameInstanceServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(ReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		m_CurrentProcessingNumberofMember = 0;

		m_LastRankingFailed = false;

	Proc_End:

		return hr;
	}

	// clear transaction
	HRESULT GameInstanceServiceEntity::ClearEntity()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(ReplicaClusterServiceEntity::ClearEntity() );

	Proc_End:

		return hr;
	}

	HRESULT GameInstanceServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_SYSTEM_OK;

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


