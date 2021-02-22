////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : game instance manager transactions
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/SFMessage.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Policy/GameInstanceManagerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
	namespace Svr {


		class GameInstanceManagerTransCreateGameInstance : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameInstanceCmd>
		{
		public:
			using super = ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameInstanceCmd>;

		private:
			GameInsUID	m_GameInsUID;

		public:
			GameInstanceManagerTransCreateGameInstance(IHeap& heap, MessageDataPtr& pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
			virtual ~GameInstanceManagerTransCreateGameInstance() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstanceManager, CreateGameInstanceRes, RouteContext(m_GameInsUID, GetRouteContext().GetFrom()));
		};


	} // namespace Svr 
} // namespace SF 

