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
#include "Memory/SFMemoryPool.h"
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


	class GameInstanceTransCreateGame : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameCmd> super;

	private:
		GameInsUID	m_GameInsUID;

	public:
		GameInstanceTransCreateGame(IHeap& heap, MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
		virtual ~GameInstanceTransCreateGame() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameInstanceManager, CreateGameRes, RouteContext(m_GameInsUID,GetRouteContext().GetFrom()));
	};


	class GameInstanceTransGameDeleted : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameDeletedC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameDeletedC2SEvt> super;

	private:
		GameInsUID	m_GameInsUID;

	public:
		GameInstanceTransGameDeleted(IHeap& heap, MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(heap, pIMsg) {}
		virtual ~GameInstanceTransGameDeleted() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
} // namespace GameServer 
} // namespace SF 

