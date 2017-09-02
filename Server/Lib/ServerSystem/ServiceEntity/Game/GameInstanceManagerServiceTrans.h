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
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Policy/GameInstanceManagerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace SF {
namespace Svr {


	class GameInstanceTransCreateGame : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameCmd, GameInstanceTransCreateGame, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameCmd, GameInstanceTransCreateGame, 1> super;

	private:
		GameInsUID	m_GameInsUID;

	public:
		GameInstanceTransCreateGame(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~GameInstanceTransCreateGame() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyGameInstanceManager* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyGameInstanceManager>(); }

		BR_SVR_MSGTRANS_CLOSE(CreateGameRes, RouteContext(m_GameInsUID,GetRouteContext().GetFrom()));
	};


	class GameInstanceTransGameDeleted : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameDeletedC2SEvt, GameInstanceTransGameDeleted, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameDeletedC2SEvt, GameInstanceTransGameDeleted, 1> super;

	private:
		GameInsUID	m_GameInsUID;

	public:
		GameInstanceTransGameDeleted(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~GameInstanceTransGameDeleted() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
} // namespace GameServer 
} // namespace SF 

