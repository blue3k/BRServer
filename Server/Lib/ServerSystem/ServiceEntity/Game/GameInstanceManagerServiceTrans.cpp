////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : game instance manager transactions
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "MemoryManager/SFMemoryPool.h"
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
		Result GameInstanceManagerTransCreateGameInstance::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});
			Entity* pEntity = nullptr;
			GameInstanceEntity* pGameInstance = nullptr;

			svrCheck(super::StartTransaction());

			svrCheck(Service::EntityManager->CreateEntity(ClusterID::GameInstance, EntityFaculty::GameInstance, pEntity));

			svrCheckPtr(pGameInstance = static_cast<GameInstanceEntity*>(pEntity));
			svrCheck(pGameInstance->InitializeGameEntity(GetAttributes()));

			svrCheck(GetMyOwner()->OnNewInstance(pGameInstance));

			svrCheck(Service::EntityManager->AddEntity(EntityFaculty::GameInstance, (Entity*)pGameInstance));

			m_GameInsUID = pEntity->GetEntityUID();

			svrTrace(SVR_INFO, "CreateGameInstance:{0}", pGameInstance->GetEntityUID());

			return hr;
		}



	}// namespace Svr 
}// namespace SF 

