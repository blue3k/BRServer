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
#include "Protocol/Message/PlayInstanceMsgClass.h"
#include "Protocol/Policy/PlayInstanceNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
	namespace Svr {


		class GameInstanceManagerTransCreateGameInstance : public MessageTransaction< GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameInstanceCmd>
		{
		public:
			using super = MessageTransaction<GameInstanceManagerServiceEntity, Message::GameInstanceManager::CreateGameInstanceCmd>;

		private:
			GameInsUID	m_GameInsUID;

		public:
			GameInstanceManagerTransCreateGameInstance(IHeap& heap, MessageDataPtr& pIMsg) : super(heap, Forward<MessageDataPtr>(pIMsg)) {}
			virtual ~GameInstanceManagerTransCreateGameInstance() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			//BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstanceManager, CreateGameInstanceRes, RouteContext(m_GameInsUID, GetRouteContext().GetFrom()));
			virtual Result OnCloseTransaction(Result hrRes) override
			{
				Result hr = ResultCode::SUCCESS; 
				NetSvrPolicyGameInstanceManager _netPolicy(super::GetRemoteEndpoint());
				svrCheck(_netPolicy.CreateGameInstanceRes(RouteContext(m_GameInsUID, GetRouteContext().GetFrom()), GetTransactionID(), hrRes));
				super::OnCloseTransaction(hrRes); 
				return hr; 
			}

		};

		//class GameInstanceManagerTransJoinGameInstance : public MessageTransaction<GameInstanceManagerServiceEntity, Message::PlayInstance::JoinGameInstanceCmd>
		//{
		//public:
		//	using super = MessageTransaction<GameInstanceManagerServiceEntity, Message::PlayInstance::JoinGameInstanceCmd>;

		//private:
		//	GameInsUID	m_GameInsUID;

		//public:
		//	GameInstanceManagerTransJoinGameInstance(IHeap& heap, MessageDataPtr& pIMsg) : super(heap, Forward<MessageDataPtr>(pIMsg)) {}
		//	virtual ~GameInstanceManagerTransJoinGameInstance() {}

		//	// Start Transaction
		//	virtual Result StartTransaction() override;

		//	BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyPlayInstance, JoinGameInstanceRes, GetPlayInstanceID(), GetPlayerID());

		//};


	} // namespace Svr 
} // namespace SF 

