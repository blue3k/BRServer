////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/ResultCode/BRResultCodeLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BrSvrTypes.h"

#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceTrans.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceEntity.h"




BR_MEMORYPOOL_IMPLEMENT(Svr::GameInstanceTransCreateGame);
BR_MEMORYPOOL_IMPLEMENT(Svr::GameInstanceTransGameDeleted);


namespace BR {
namespace Svr {




	// Start Transaction
	Result GameInstanceTransCreateGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Entity *pEntity = nullptr;
		GameInstanceEntity* pGameInstance = nullptr;

		svrChk( super::StartTransaction() );

		svrChk( GetServerComponent<EntityManager>()->CreateEntity( ClusterID::GameInstanceManager, EntityFaculty::GameInstance, pEntity ) );

		svrChkPtr(pGameInstance = dynamic_cast<GameInstanceEntity*>(pEntity));
		svrChk(pGameInstance->InitializeGameEntity(GetNumberOfBotPlayer(), GetMaxPlayer()));

		svrChk(GetMyOwner()->OnNewInstance(pGameInstance));

		m_GameInsUID = pEntity->GetEntityUID();

		svrTrace(TRC_INFO, "CreateGameInstance:{0}, numBot:{1}, maxPlayer:{2}", pGameInstance->GetEntityUID(), GetNumberOfBotPlayer(), GetMaxPlayer());

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result GameInstanceTransGameDeleted::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		svrChk(GetMyOwner()->FreeGameInstance(GetRouteContext().GetFrom()));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


};// namespace Svr 
};// namespace BR 

