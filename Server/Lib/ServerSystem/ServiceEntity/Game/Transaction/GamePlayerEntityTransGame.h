////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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

#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"

#include "Transaction/MessageRoute.h"


namespace SF {
	namespace Svr {


		/////////////////////////////////////////////////////////////////////////////
		//
		//	Game command transaction
		//

		class PlayerTransSearchGameInstance : public Svr::MessageTransaction<GamePlayerEntity, Message::Game::SearchGameInstanceCmd>
		{
		public:
			using super = Svr::MessageTransaction<GamePlayerEntity, Message::Game::SearchGameInstanceCmd>;

		private:
			DynamicArray<GameInstanceInfo> m_GameInstances;

		public:
			PlayerTransSearchGameInstance(IHeap& heap, MessageDataPtr& pIMsg);
			virtual ~PlayerTransSearchGameInstance() {}

			//Result OnSearchGameInstanceRes(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, SearchGameInstanceRes, m_GameInstances);
		};


		class PlayerTransJoinGameInstance : public Svr::MessageTransaction<GamePlayerEntity, Message::Game::JoinGameInstanceCmd>
		{
		public:
			using super = Svr::MessageTransaction<GamePlayerEntity, Message::Game::JoinGameInstanceCmd>;

		private:
			GameInsUID m_GameInsID;
			NetAddress m_GameInsAddress;
			NetAddress m_GameInsAddress4;

		public:
			PlayerTransJoinGameInstance(IHeap& heap, MessageDataPtr& pIMsg);
			virtual ~PlayerTransJoinGameInstance() {}

			Result OnJoinGameInstanceRes(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			virtual Result CloseTransaction(Result hrRes) override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, JoinGameInstanceRes, m_GameInsID, m_GameInsAddress, m_GameInsAddress4);
		};



		class PlayerTransLeaveGameInstance : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::LeaveGameInstanceCmd>
		{
		public:
			using super =  Svr::MessageTransaction< GamePlayerEntity, Message::Game::LeaveGameInstanceCmd>;

		private:

		public:
			PlayerTransLeaveGameInstance(IHeap& heap, MessageDataPtr& pIMsg);//  :MessageTransaction(heap,  pIMsg ) {}
			virtual ~PlayerTransLeaveGameInstance() {}

			Result OnLeaveGameInstanceRes(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			virtual Result CloseTransaction(Result hrRes) override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, LeaveGameRes);
		};


	} // namespace Svr 
} // namespace SF 

