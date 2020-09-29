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






namespace SF {
namespace Svr {




	// Start Transaction
	Result GameInstanceTransCreateGame::StartTransaction()
	{
		FunctionContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});
		Entity *pEntity = nullptr;
		GameInstanceEntity* pGameInstance = nullptr;

		svrCheck( super::StartTransaction() );

		svrCheck(Service::EntityManager->CreateEntity( ClusterID::GameInstanceManager, EntityFaculty::GameInstance, pEntity ) );

		svrCheckPtr(pGameInstance = dynamic_cast<GameInstanceEntity*>(pEntity));
		svrCheck(pGameInstance->InitializeGameEntity(GetNumberOfBotPlayer(), GetMaxPlayer()));

		svrCheck(GetMyOwner()->OnNewInstance(pGameInstance));

		m_GameInsUID = pEntity->GetEntityUID();

		svrTrace(SVR_INFO, "CreateGameInstance:{0}, numBot:{1}, maxPlayer:{2}", pGameInstance->GetEntityUID(), GetNumberOfBotPlayer(), GetMaxPlayer());

		return hr;
	}


	// Start Transaction
	Result GameInstanceTransGameDeleted::StartTransaction()
	{
		FunctionContext hr([this](Result hr)
			{
				CloseTransaction(hr);
			});

		svrCheck(super::StartTransaction());

		svrCheck(GetMyOwner()->FreeGameInstance(GetRouteContext().GetFrom()));

		return hr;
	}


}// namespace Svr 
}// namespace SF 

