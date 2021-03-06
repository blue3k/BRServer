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
#include "Util/Memento.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Protocol/GameMsgClass.h"
#include "Protocol/GameNetPolicy.h"
#include "Protocol/GameServerMsgClass.h"
#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameInstanceMsgClass.h"
#include "Protocol/GameInstanceNetPolicy.h"



#include "Transaction/MessageRoute.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Net/SFMessage.h"

//#include "GameSystem/UserGamePlayerInfoSystem.h"


namespace SF {

	namespace DB{
		struct QueryGetPlayerInfoData;
	};

	namespace Svr {



		/////////////////////////////////////////////////////////////////////////////
		//
		//	Game command transaction
		//


		class PlayerTransJoinGameServer : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinGameServerCmd>
		{
		public:
			using super = Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinGameServerCmd>;

		private:

			enum class Step
			{
				Begin,
				GetPlayerGameData,
				UpdatePartyMatching,
				JoinParty,
				Done,
			} m_Step;


			char m_PlayerNick[GameConst::MAX_NAME];
			PlayerID m_PartyLeaderID;
			GameInsUID m_GameUID;
			MatchingQueueTicket m_MatchingTicket;

			bool m_HaveValidPlayerData = false;

		public:
			PlayerTransJoinGameServer(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransJoinGameServer() {}

			const char* GetPlayerNick() { return m_PlayerNick; }
			void SetPlayerNick(const char* value) { StrUtil::StringCopy(m_PlayerNick, value); }


			//Result OnGameServerJoined(Svr::TransactionResult* pRes);
			Result OnJoinPartyRes(Svr::TransactionResult* pRes);
			Result OnCreatePlayerGameDataRes(Svr::TransactionResult* pRes);
			Result OnGetPlayerGameDataRes(Svr::TransactionResult* pRes);

			Result SetPlayerGameData(const VariableTable& playerData);

			// Sub actions
			Result ConnectedToGameServerDB();
			Result OnConnectedToGameServerDB(Svr::TransactionResult* pRes);

			//Result NotifyLoginServer();
			Result RequestPlayerInfoFromDB();
			Result RequestPlayerInfoCreateDB();
			Result RequestJoinPartyIfExist();
			Result FinalizeSuccess() override;

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, JoinGameServerRes, m_PlayerNick, m_GameUID, GetMyOwner()->GetPartyUID(), m_PartyLeaderID, m_MatchingTicket);
		};


		class PlayerTransGetUserGamePlayerInfo : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetUserGamePlayerInfoCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetUserGamePlayerInfoCmd> super;

		private:

			VariableTable m_Result;


		public:
			PlayerTransGetUserGamePlayerInfo(IHeap& heap, const MessageDataPtr& pIMsg) : MessageTransaction(heap, pIMsg) {}
			virtual ~PlayerTransGetUserGamePlayerInfo() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GetUserGamePlayerInfoRes, m_Result);
		};



		class PlayerTransGetGamePlayerInfo : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetGamePlayerInfoCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetGamePlayerInfoCmd> super;

		private:
			int32_t m_ShardId = 0;
			VariableTable m_Result;

		public:
			PlayerTransGetGamePlayerInfo(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransGetGamePlayerInfo() {}

			Result RequestPlayerShardID();
			Result OnGetPlayerShardID(Svr::TransactionResult* pRes);

			Result RequestGamePlayerInfo();
			Result OnGetGamePlayerInfo(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GetGamePlayerInfoRes, GetPlayerID(), m_Result);
		};



		class PlayerTransGetComplitionState : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetComplitionStateCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetComplitionStateCmd> super;

		private:

			char m_ComplitionState[GameConst::MAX_COMPLITIONSTATE];

		public:
			PlayerTransGetComplitionState(IHeap& heap, const MessageDataPtr& pIMsg);// : MessageTransaction(pIMsg) {}
			virtual ~PlayerTransGetComplitionState() {}

			const char* GetComplitionState() { return m_ComplitionState; }
			void SetComplitionState(const char* value) { StrUtil::StringCopy(m_ComplitionState, value); }


			Result OnGetComplitionState(Svr::TransactionResult* pRes);


			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GetComplitionStateRes, m_ComplitionState);
		};



		class PlayerTransSetComplitionState : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetComplitionStateCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetComplitionStateCmd> super;

		public:
			PlayerTransSetComplitionState(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransSetComplitionState() {}

			Result OnSetComplitionState(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, SetComplitionStateRes);
		};




		/////////////////////////////////////////////////////////////////////////////
		//
		//  Debug messages
		//

		/////////////////////////////////////////////////////////////////////////////
		//
		//	GCM register/unregister transaction
		//

		class PlayerTransRegisterGCM : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::RegisterGCMCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::RegisterGCMCmd> super;

		private:
		public:
			PlayerTransRegisterGCM(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransRegisterGCM() {}

			Result OnUpdated(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, RegisterGCMRes);
		};


		class PlayerTransUnregisterGCM : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::UnregisterGCMCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::UnregisterGCMCmd> super;

		private:
		public:
			PlayerTransUnregisterGCM(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransUnregisterGCM() {}

			Result OnUpdated(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, UnregisterGCMRes);
		};




		/////////////////////////////////////////////////////////////////////////////
		//
		//	Notifications transaction
		//

		class PlayerTransGetNotificationList : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetNotificationListCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetNotificationListCmd> super;

		private:
		public:
			PlayerTransGetNotificationList(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransGetNotificationList() {}

			Result OnGetList(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, GetNotificationListRes);
		};

		class PlayerTransDeleteNotification : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::DeleteNotificationCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::DeleteNotificationCmd> super;

		private:
		public:
			PlayerTransDeleteNotification(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransDeleteNotification() {}

			Result OnDeletedNotification(Svr::TransactionResult*& pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, DeleteNotificationRes, GetNotificationID());
		};


		class PlayerTransSetNotificationRead : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetNotificationReadCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetNotificationReadCmd> super;

		private:
		public:
			PlayerTransSetNotificationRead(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransSetNotificationRead() {}

			Result OnSetRead(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, SetNotificationReadRes, GetNotificationID());
		};


		class PlayerTransAcceptNotification : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::AcceptNotificationCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::AcceptNotificationCmd> super;

		private:
		public:
			PlayerTransAcceptNotification(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransAcceptNotification() {}

			Result OnDeletedNotification(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, AcceptNotificationRes, GetNotificationID());
		};


		class PlayerTransNotifyS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyC2SEvt>
		{
		public:
			typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyC2SEvt> super;

		public:
			PlayerTransNotifyS2S(IHeap& heap, const MessageDataPtr& pIMsg) : UserTransactionS2SEvt(heap, pIMsg) {}
			virtual ~PlayerTransNotifyS2S() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


		/////////////////////////////////////////////////////////////////////////////
		//
		//	NickName transaction
		//

		class PlayerTransSetNickName : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetNickNameCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetNickNameCmd> super;

		private:
			uint64_t m_TotalGem{};
			uint64_t m_TotalGameMoney{};

		public:
			PlayerTransSetNickName(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransSetNickName() {}

			Result OnNickChanged(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, SetNickNameRes, m_TotalGem, m_TotalGameMoney);
		};


		/////////////////////////////////////////////////////////////////////////////
		//
		//	Find player transaction
		//

		class PlayerTransFindPlayerByEMail : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::FindPlayerByEMailCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::FindPlayerByEMailCmd> super;

		private:
			PlayerInformation m_Player{};
			int m_PlayerShardID{};

		public:
			PlayerTransFindPlayerByEMail(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransFindPlayerByEMail() {}

			Result OnFindPlayer(Svr::TransactionResult* pRes);
			Result OnGetNickName(Svr::TransactionResult* pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, FindPlayerByEMailRes, m_Player);
		};


		class PlayerTransFindPlayerByPlayerID : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::FindPlayerByPlayerIDCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::FindPlayerByPlayerIDCmd> super;

		private:
			PlayerInformation m_Player;
			int m_PlayerShardID{};

		public:
			PlayerTransFindPlayerByPlayerID(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransFindPlayerByPlayerID() {}

			Result OnFindPlayer(Svr::TransactionResult*& pRes);
			Result OnGetNickName(Svr::TransactionResult*& pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, FindPlayerByPlayerIDRes, m_Player);
		};


		class PlayerTransRequestPlayerStatusUpdate : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::RequestPlayerStatusUpdateCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::RequestPlayerStatusUpdateCmd> super;

		private:

			uint m_PlayerStatusQueryCount{};

		public:
			PlayerTransRequestPlayerStatusUpdate(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransRequestPlayerStatusUpdate() {}

			Result OnPlayerShardIDRes(Svr::TransactionResult*& pRes);
			Result OnPlayerStatusUpdateRes(Svr::TransactionResult*& pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyGame, RequestPlayerStatusUpdateRes);
		};

		class PlayerTransRequestPlayerStatusUpdateC2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::RequestPlayerStatusUpdateC2SEvt>
		{
		public:
			typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::RequestPlayerStatusUpdateC2SEvt> super;

		public:
			PlayerTransRequestPlayerStatusUpdateC2S(IHeap& heap, const MessageDataPtr& pIMsg) :UserTransactionS2SEvt(heap, pIMsg) {}
			virtual ~PlayerTransRequestPlayerStatusUpdateC2S() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};

		class PlayerTransNotifyPlayerStatusUpdatedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt>
		{
		public:
			typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt> super;

		public:
			PlayerTransNotifyPlayerStatusUpdatedS2S(IHeap& heap, const MessageDataPtr& pIMsg) :UserTransactionS2SEvt(heap, pIMsg) {}
			virtual ~PlayerTransNotifyPlayerStatusUpdatedS2S() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};



		class PlayerTransGetRankingList : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetRankingListCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetRankingListCmd> super;

		private:

			StaticArray<TotalRankingPlayerInformation, 100> m_RankingList;

		public:
			PlayerTransGetRankingList(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransGetRankingList() {}

			Result OnGetRankingListRes(Svr::TransactionResult*& pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, GetRankingListRes, m_RankingList);
		};



		class PlayerTransBuyShopItemPrepare : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::BuyShopItemPrepareCmd>
		{
		public:
			typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::BuyShopItemPrepareCmd> super;

		private:
			const uint MAX_RETRY = 3;

			StaticArray<uint8_t, 1024> m_Signagure;
			uint m_RetryCount;

		public:
			PlayerTransBuyShopItemPrepare(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransBuyShopItemPrepare() {}

			Result OnPurchaseIDChecked(Svr::TransactionResult* pRes);

			Result GenerateSigunatureAndCheck();

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, BuyShopItemPrepareRes, GetShopItemID(), (const char*)m_Signagure.data());
		};



		class PlayerTransBuyShopItem : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::BuyShopItemCmd>
		{
		public:

			using super = Svr::MessageTransaction< GamePlayerEntity, Message::Game::BuyShopItemCmd>;

			static constexpr size_t MEMENTO_SIZE = 2048;

		private:
			Memento<MEMENTO_SIZE> m_SavedData;
			// TODO: FIXME
			void /*conspiracy::ShopTbl::ShopItem*/* m_pShopItem{};

		public:
			PlayerTransBuyShopItem(IHeap& heap, const MessageDataPtr& pIMsg);
			virtual ~PlayerTransBuyShopItem() {}

			Result OnPurchaseCheckedAndroid(Svr::TransactionResult*& pRes);
			Result OnPurchaseCheckedIOS(Svr::TransactionResult*& pRes);
			Result OnSavedToDB(Svr::TransactionResult*& pRes);

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyGame, BuyShopItemRes, GetShopItemID());
		};




	} // namespace GameServer 
} // namespace SF 

