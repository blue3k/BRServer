////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceTrans.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"




SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GameInstanceTransCreateGame);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GameInstanceTransGameDeleted);


namespace SF {
namespace Svr {




	// Start Transaction
	Result GameInstanceTransCreateGame::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		Entity *pEntity = nullptr;
		GameInstanceEntity* pGameInstance = nullptr;

		svrChk( super::StartTransaction() );

		svrChk(Service::EntityManager->CreateEntity( ClusterID::GameInstanceManager, EntityFaculty::GameInstance, pEntity ) );

		svrChkPtr(pGameInstance = dynamic_cast<GameInstanceEntity*>(pEntity));
		svrChk(pGameInstance->InitializeGameEntity(GetNumberOfBotPlayer(), GetMaxPlayer()));

		svrChk(GetMyOwner()->OnNewInstance(pGameInstance));

		m_GameInsUID = pEntity->GetEntityUID();

		svrTrace(SVR_INFO, "CreateGameInstance:{0}, numBot:{1}, maxPlayer:{2}", pGameInstance->GetEntityUID(), GetNumberOfBotPlayer(), GetMaxPlayer());

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
};// namespace SF 

