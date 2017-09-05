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
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Policy/GameInstanceManagerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {


	class GameInstanceTransCreateGame : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameCmd, GameInstanceTransCreateGame, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameCmd, GameInstanceTransCreateGame, 1> super;

	private:
		GameInsUID	m_GameInsUID;

	public:
		GameInstanceTransCreateGame(MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~GameInstanceTransCreateGame() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyGameInstanceManager* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyGameInstanceManager>(); }

		BR_SVR_MSGTRANS_CLOSE(CreateGameRes, RouteContext(m_GameInsUID,GetRouteContext().GetFrom()));
	};


	class GameInstanceTransGameDeleted : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameDeletedC2SEvt, GameInstanceTransGameDeleted, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameDeletedC2SEvt, GameInstanceTransGameDeleted, 1> super;

	private:
		GameInsUID	m_GameInsUID;

	public:
		GameInstanceTransGameDeleted(MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~GameInstanceTransGameDeleted() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
} // namespace GameServer 
} // namespace SF 

