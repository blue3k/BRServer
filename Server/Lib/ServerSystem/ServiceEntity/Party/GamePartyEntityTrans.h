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

#include "Protocol/GameMsgClass.h"
#include "Protocol/GameNetPolicy.h"
#include "Protocol/GameServerMsgClass.h"
#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GamePartyMsgClass.h"
#include "Protocol/GamePartyNetPolicy.h"

#include "Protocol/PartyMatchingQueueMsgClass.h"
#include "Protocol/PartyMatchingMsgClass.h"


#include "Transaction/MessageRoute.h"

#include "ServiceEntity/Party/GamePartyEntity.h"
#include "Net/SFMessage.h"
#include "ServiceEntity/Party/GamePartyTransaction.h"


namespace SF {
	namespace Svr {

		class ServerEntity;



		// Close zone instance
		class PartyTransCloseInstance : public Transaction
		{
		private:
			GameID m_GameID;

		public:
			PartyTransCloseInstance(IHeap& memMgr, GameID gameID)
				: Svr::Transaction(memMgr, TransactionID())
				, m_GameID(gameID)
			{
				SetExclusive(true);
			}
			virtual ~PartyTransCloseInstance() {}

			// Start Transaction
			virtual Result StartTransaction();
		};



		/////////////////////////////////////////////////////////////////////////////
		//
		//	Party transaction
		//



		class PartyTransJoinParty : public GamePartyMessageTransaction< Message::GameParty::JoinPartyCmd>
		{
		public:
			typedef GamePartyMessageTransaction< Message::GameParty::JoinPartyCmd> super;

		private:
			PlayerID m_LeaderID;
			StaticOutputMemoryStream<GameConst::MAX_CHATLOG_BUFFER> m_MessageBuffer;

		public:
			PartyTransJoinParty(IHeap& memMgr, const MessageDataPtr& pIMsg)
				: GamePartyMessageTransaction(memMgr, pIMsg)
				, m_MessageBuffer(memMgr)
			{}
			virtual ~PartyTransJoinParty() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyGameParty, JoinPartyRes, GetRouteContext().GetSwaped(), m_LeaderID, m_MessageBuffer.GetBuffer());
		};


		class PartyTransLeaveParty : public GamePartyMessageTransaction< Message::GameParty::LeavePartyCmd>
		{
		public:
			typedef GamePartyMessageTransaction< Message::GameParty::LeavePartyCmd> super;

		private:

		public:
			PartyTransLeaveParty(IHeap& memHeap, const MessageDataPtr& pIMsg) : GamePartyMessageTransaction(memHeap, pIMsg) {}
			virtual ~PartyTransLeaveParty() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameParty, LeavePartyRes, GetRouteContext().GetSwaped());
		};



		class PartyTransKickPlayer : public GamePartyMessageTransaction< Message::GameParty::KickPlayerCmd>
		{
		public:
			typedef GamePartyMessageTransaction<Message::GameParty::KickPlayerCmd> super;

		private:

		public:
			PartyTransKickPlayer(IHeap& memHeap, const MessageDataPtr& pIMsg) : GamePartyMessageTransaction(memHeap, pIMsg) {}
			virtual ~PartyTransKickPlayer() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameParty, KickPlayerRes, GetRouteContext().GetSwaped());
		};



		class PartyTransChatMessage : public GamePartyMessageTransaction< Message::GameParty::ChatMessageC2SEvt>
		{
		public:
			typedef GamePartyMessageTransaction< Message::GameParty::ChatMessageC2SEvt> super;

		private:

		public:
			PartyTransChatMessage(IHeap& memHeap, const MessageDataPtr& pIMsg) : GamePartyMessageTransaction(memHeap, pIMsg) {}
			virtual ~PartyTransChatMessage() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


		class PartyTransQuickChatMessage : public GamePartyMessageTransaction< Message::GameParty::QuickChatMessageC2SEvt>
		{
		public:
			typedef GamePartyMessageTransaction< Message::GameParty::QuickChatMessageC2SEvt> super;

		private:

		public:
			PartyTransQuickChatMessage(IHeap& memHeap, const MessageDataPtr& pIMsg) : GamePartyMessageTransaction(memHeap, pIMsg) {}
			virtual ~PartyTransQuickChatMessage() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


		// Player voted event
		class PartyTransStartGameMatchCmd : public Svr::GamePartyMessageTransaction< Message::GameParty::StartGameMatchCmd>
		{
		public:
			typedef Svr::GamePartyMessageTransaction< Message::GameParty::StartGameMatchCmd> super;

		public:
			PartyTransStartGameMatchCmd(IHeap& memHeap, const MessageDataPtr& pIMsg);//  :GamePartyMessageTransaction( pIMsg ) {}
			virtual ~PartyTransStartGameMatchCmd() {}

			Result OnPartyMatchingQueued(Svr::TransactionResult*& pRes);
			Result OnCreateGame(TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameParty, StartGameMatchRes, GetRouteContext().GetSwaped());
		};


		// Player voted event
		class PartyTransCancelGameMatchCmd : public Svr::GamePartyMessageTransaction< Message::GameParty::CancelGameMatchCmd>
		{
		public:
			typedef Svr::GamePartyMessageTransaction< Message::GameParty::CancelGameMatchCmd> super;

		public:
			PartyTransCancelGameMatchCmd(IHeap& memHeap, const MessageDataPtr& pIMsg);//  :GamePartyMessageTransaction( pIMsg ) {}
			virtual ~PartyTransCancelGameMatchCmd() {}

			Result OnPartyMatchingCanceled(Svr::TransactionResult*& pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameParty, CancelGameMatchRes, GetRouteContext().GetSwaped());
		};


		class PartyTransPartyMatchingCanceled : public GamePartyMessageTransaction< Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt>
		{
		public:
			typedef GamePartyMessageTransaction< Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt> super;

		private:

		public:
			PartyTransPartyMatchingCanceled(IHeap& memHeap, const MessageDataPtr& pIMsg) : GamePartyMessageTransaction(memHeap, pIMsg) {}
			virtual ~PartyTransPartyMatchingCanceled() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


		class PartyTransMatchingItemDequeued : public GamePartyMessageTransaction< Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt>
		{
		public:
			typedef GamePartyMessageTransaction< Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt> super;

		private:

		public:
			PartyTransMatchingItemDequeued(IHeap& memHeap, const MessageDataPtr& pIMsg) : GamePartyMessageTransaction(memHeap, pIMsg) {}
			virtual ~PartyTransMatchingItemDequeued() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


		class PartyTransPartyGameMatchedS2CEvt : public GamePartyMessageTransaction< Message::PartyMatching::PartyGameMatchedS2CEvt>
		{
		public:
			typedef GamePartyMessageTransaction< Message::PartyMatching::PartyGameMatchedS2CEvt> super;

		private:

		public:
			PartyTransPartyGameMatchedS2CEvt(IHeap& memHeap, const MessageDataPtr& pIMsg) : GamePartyMessageTransaction(memHeap, pIMsg) {}
			virtual ~PartyTransPartyGameMatchedS2CEvt() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};




	} // namespace Svr 
} // namespace SF 

