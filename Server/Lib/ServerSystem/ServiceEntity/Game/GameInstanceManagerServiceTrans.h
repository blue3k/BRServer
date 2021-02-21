////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
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


		class GameInstanceTransCreateGameInstance : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameInstanceCmd>
		{
		public:
			using super = ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameInstanceCmd>;

		private:
			GameInsUID	m_GameInsUID;

		public:
			GameInstanceTransCreateGameInstance(IHeap& heap, MessageDataPtr& pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
			virtual ~GameInstanceTransCreateGameInstance() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstanceManager, CreateGameInstanceRes, RouteContext(m_GameInsUID, GetRouteContext().GetFrom()));
		};


		class GameInstanceTransGameInstanceDeleted : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameInstanceDeletedC2SEvt>
		{
		public:
			using super = ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameInstanceDeletedC2SEvt>;

		private:
			GameInsUID	m_GameInsUID;

		public:
			GameInstanceTransGameInstanceDeleted(IHeap& heap, MessageDataPtr& pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
			virtual ~GameInstanceTransGameInstanceDeleted() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


		class GameInstanceTransSearchGameInstance : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::SearchGameInstanceCmd>
		{
		public:
			using super = ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::SearchGameInstanceCmd>;

		private:
			GameInsUID	m_GameInsUID;

		public:
			GameInstanceTransSearchGameInstance(IHeap& heap, MessageDataPtr& pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
			virtual ~GameInstanceTransSearchGameInstance() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstanceManager, CreateGameInstanceRes, RouteContext(m_GameInsUID, GetRouteContext().GetFrom()));
		};

	} // namespace GameServer 
} // namespace SF 

