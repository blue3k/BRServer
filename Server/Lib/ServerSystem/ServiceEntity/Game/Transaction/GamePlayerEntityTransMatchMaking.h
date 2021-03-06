////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2020 The Braves
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
#include "Protocol/GameMsgClass.h"
#include "Protocol/GameNetPolicy.h"
#include "Protocol/PartyMatchingMsgClass.h"
#include "Protocol/PartyMatchingQueueMsgClass.h"

#include "Transaction/MessageRoute.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Net/SFMessage.h"



namespace SF {
	namespace Svr {


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Matchmaking
		//



		class PlayerTransRequestGameMatch : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::RequestGameMatchCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::RequestGameMatchCmd> super;

		private:
			uint64_t m_TotalGem;
			uint64_t m_TotalGameMoney;

		public:
			PlayerTransRequestGameMatch(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransRequestGameMatch() {}

			Result OnRequestPartyMatchingRes(Svr::TransactionResult*& pRes);
			Result OnRequestPlayerMatchingRes(Svr::TransactionResult*& pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, RequestGameMatchRes, m_TotalGem, m_TotalGameMoney);
		};

		class PlayerTransCancelGameMatch : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::CancelGameMatchCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::CancelGameMatchCmd> super;

		public:
			PlayerTransCancelGameMatch(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransCancelGameMatch() {}

			Result OnCancelPartyMatchingRes(Svr::TransactionResult*& pRes);
			Result OnCancelPlayerMatchingRes(Svr::TransactionResult*& pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, CancelGameMatchRes);
		};



		//////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Matching queue
		//
		class PlayerTransPlayerMatchingCanceledS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt>
		{
		public:
			typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt> super;

		public:
			PlayerTransPlayerMatchingCanceledS2CEvt(IHeap& heap, const MessageDataPtr& pIMsg) :UserTransactionS2SEvt(heap, pIMsg) {}
			virtual ~PlayerTransPlayerMatchingCanceledS2CEvt() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};



		class PlayerTransPlayerMatchingItemDequeuedS2CEvt : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt>
		{
		public:
			typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt> super;

		public:
			PlayerTransPlayerMatchingItemDequeuedS2CEvt(IHeap& heap, const MessageDataPtr& pIMsg) :UserTransactionS2SEvt(heap, pIMsg) {}
			virtual ~PlayerTransPlayerMatchingItemDequeuedS2CEvt() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


	} // namespace Svr
} // namespace SF 

