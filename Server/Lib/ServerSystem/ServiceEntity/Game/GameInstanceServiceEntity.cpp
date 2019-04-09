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



namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameInstanceServiceEntity::GameInstanceServiceEntity(GameID gameID, ClusterMembership initialMembership)
		: super(gameID, ClusterID::GameInstanceManager, initialMembership)
	{
	}

	GameInstanceServiceEntity::~GameInstanceServiceEntity()
	{
	}

	Result GameInstanceServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::InitializeEntity(newEntityID) );

		m_CurrentProcessingNumberofMember = 0;

		m_LastRankingFailed = false;

	Proc_End:

		return hr;
	}

	// clear transaction
	Result GameInstanceServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::ClearEntity() );

	Proc_End:

		return hr;
	}

	Result GameInstanceServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::TickUpdate(pAction) );

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
}; // namespace SF {



