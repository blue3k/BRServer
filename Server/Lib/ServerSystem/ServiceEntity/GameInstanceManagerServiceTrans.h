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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Policy/GameInstanceManagerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {


	class GameInstanceTransCreateGame : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameCmd, GameInstanceTransCreateGame, 1>
	{
	private:
		GameInsUID	m_GameInsUID;

	public:
		GameInstanceTransCreateGame(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~GameInstanceTransCreateGame() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyGameInstanceManager* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyGameInstanceManager>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(CreateGameRes, RouteContext(m_GameInsUID,GetRouteContext().From));
	};


	class GameInstanceTransGameDeleted : public ClusterEntityMessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::GameDeletedC2SEvt, GameInstanceTransGameDeleted, 1>
	{
	private:
		GameInsUID	m_GameInsUID;

	public:
		GameInstanceTransGameDeleted(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) {}
		virtual ~GameInstanceTransGameDeleted() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
} // namespace GameServer 
} // namespace BR 

