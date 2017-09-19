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
#include "Util/Memento.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"



#include "Table/conspiracy/ShopTbl.h"

#include "Transaction/MessageRoute.h"

#include "GameServerClass.h"

#include "GameInstance/GamePlayerEntity.h"
#include "Net/Message.h"

#include "GameSystem/UserGamePlayerInfoSystem.h"


namespace SF {

	namespace DB{
		struct QueryGetPlayerInfoData;
	};

namespace GameServer {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//


	class PlayerTransJoinGameServer : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinGameServerCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::JoinGameServerCmd> super;

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

	public:
		PlayerTransJoinGameServer(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransJoinGameServer() {}

		const char* GetPlayerNick() { return m_PlayerNick; }
		void SetPlayerNick(const char* value) { StrUtil::StringCpy(m_PlayerNick, value); }



		Result OnGameServerJoined( Svr::TransactionResult* &pRes );
		Result OnJoinPartyRes( Svr::TransactionResult* &pRes );
		Result OnCreatePlayerGameDataRes(Svr::TransactionResult* &pRes);
		Result OnGetPlayerGameDataRes(Svr::TransactionResult* &pRes);

		Result SetPlayerGameData(const DB::QueryGetPlayerInfoData &playerData);

		Result RegisterToPlayerManager();

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, JoinGameServerRes,m_PlayerNick, m_GameUID, GetMyOwner()->GetPartyUID(), m_PartyLeaderID, m_MatchingTicket);
	};

	
	class PlayerTransGetUserGamePlayerInfo : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetUserGamePlayerInfoCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetUserGamePlayerInfoCmd> super;

	private:
		struct {
			int16_t Level;
			int64_t Exp;
			int64_t GameMoney;
			int64_t Gem;
			int16_t Stamina;
			uint32_t LastUpdateTime;
			int32_t TotalPlayed;
			int32_t WinPlaySC;
			int32_t WinPlaySM;
			int32_t WinPlaySS;
			int32_t LosePlaySC;
			int32_t LosePlaySM;
			int32_t LosePlaySS;
			int32_t WinPlayNC;
			int32_t WinPlayNM;
			int32_t WinPlayNS;
			int32_t LosePlayNC;
			int32_t LosePlayNM;
			int32_t LosePlayNS;
			int32_t WeeklyWin;
			int32_t WeeklyLose;
		} m_Result;


	public:
		PlayerTransGetUserGamePlayerInfo(IHeap& heap, MessageDataPtr &pIMsg ) : MessageTransaction( heap, pIMsg ) {}
		virtual ~PlayerTransGetUserGamePlayerInfo() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, GetUserGamePlayerInfoRes,
			m_Result.Level,
			m_Result.Exp,
			m_Result.GameMoney,
			m_Result.Gem,
			m_Result.Stamina,
			m_Result.LastUpdateTime,
			m_Result.TotalPlayed,
			m_Result.WinPlaySC,
			m_Result.WinPlaySM,
			m_Result.WinPlaySS,
			m_Result.LosePlaySC,
			m_Result.LosePlaySM,
			m_Result.LosePlaySS,
			m_Result.WinPlayNC,
			m_Result.WinPlayNM,
			m_Result.WinPlayNS,
			m_Result.LosePlayNC,
			m_Result.LosePlayNM,
			m_Result.LosePlayNS,
			m_Result.WeeklyWin,
			m_Result.WeeklyLose
			);
	};
	
	
	
	class PlayerTransGetGamePlayerInfo : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetGamePlayerInfoCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetGamePlayerInfoCmd> super;

	private:
		struct {
			int16_t Level;
			int32_t TotalPlayed;
			int32_t WinPlaySC;
			int32_t WinPlaySM;
			int32_t WinPlaySS;
			int32_t LosePlaySC;
			int32_t LosePlaySM;
			int32_t LosePlaySS;
			int32_t WinPlayNC;
			int32_t WinPlayNM;
			int32_t WinPlayNS;
			int32_t LosePlayNC;
			int32_t LosePlayNM;
			int32_t LosePlayNS;
			int32_t WeeklyWin;
			int32_t WeeklyLose;
		} m_Result;

	public:
		PlayerTransGetGamePlayerInfo(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransGetGamePlayerInfo() {}

		Result OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		Result OnGetGamePlayerInfo( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, GetGamePlayerInfoRes,
					GetPlayerID(),
					m_Result.Level,
					m_Result.TotalPlayed,
					m_Result.WinPlaySC,
					m_Result.WinPlaySM,
					m_Result.WinPlaySS,
					m_Result.LosePlaySC,
					m_Result.LosePlaySM,
					m_Result.LosePlaySS,
					m_Result.WinPlayNC,
					m_Result.WinPlayNM,
					m_Result.WinPlayNS,
					m_Result.LosePlayNC,
					m_Result.LosePlayNM,
					m_Result.LosePlayNS,
					m_Result.WeeklyWin,
					m_Result.WeeklyLose
			);
	};



	class PlayerTransGetComplitionState : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetComplitionStateCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetComplitionStateCmd> super;

	private:

		char m_ComplitionState[GameConst::MAX_COMPLITIONSTATE];

	public:
		PlayerTransGetComplitionState(IHeap& heap, MessageDataPtr &pIMsg);// : MessageTransaction(pIMsg) {}
		virtual ~PlayerTransGetComplitionState() {}

		const char* GetComplitionState() { return m_ComplitionState; }
		void SetComplitionState(const char* value) { StrUtil::StringCpy(m_ComplitionState, value); }


		Result OnGetComplitionState(Svr::TransactionResult* &pRes);


		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, GetComplitionStateRes, m_ComplitionState);
	};



	class PlayerTransSetComplitionState : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetComplitionStateCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetComplitionStateCmd> super;

	public:
		PlayerTransSetComplitionState(IHeap& heap, MessageDataPtr &pIMsg);
		virtual ~PlayerTransSetComplitionState() {}

		Result OnSetComplitionState(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(Policy::NetSvrPolicyGame, SetComplitionStateRes);
	};




	/////////////////////////////////////////////////////////////////////////////
	//
	//  Debug messages
	//

	class PlayerTransSetConfigPreset : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetPresetGameConfigIDCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetPresetGameConfigIDCmd> super;

	private:
	public:
		PlayerTransSetConfigPreset(IHeap& heap, MessageDataPtr &pIMsg )  :MessageTransaction( heap, pIMsg ) {}
		virtual ~PlayerTransSetConfigPreset() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(Policy::NetSvrPolicyGame, SetPresetGameConfigIDRes);
	};
	
	class PlayerTransGainGameResource : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GainGameResourceCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GainGameResourceCmd> super;

	private:
	public:
		PlayerTransGainGameResource(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransGainGameResource() {}

		Result OnSetPlayerInfoRes(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(Policy::NetSvrPolicyGame, GainGameResourceRes);
	};


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
		PlayerTransRegisterGCM(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransRegisterGCM() {}

		Result OnUpdated( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(Policy::NetSvrPolicyGame, RegisterGCMRes);
	};


	class PlayerTransUnregisterGCM : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::UnregisterGCMCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::UnregisterGCMCmd> super;

	private:
	public:
		PlayerTransUnregisterGCM(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransUnregisterGCM() {}

		Result OnUpdated( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(Policy::NetSvrPolicyGame, UnregisterGCMRes);
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
		PlayerTransGetNotificationList(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransGetNotificationList() {}

		Result OnGetList( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(Policy::NetSvrPolicyGame, GetNotificationListRes);
	};
	
	class PlayerTransDeleteNotification : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::DeleteNotificationCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::DeleteNotificationCmd> super;

	private:
	public:
		PlayerTransDeleteNotification(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransDeleteNotification() {}

		Result OnDeletedNotification( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, DeleteNotificationRes,GetNotificationID());
	};


	class PlayerTransSetNotificationRead : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetNotificationReadCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::SetNotificationReadCmd> super;

	private:
	public:
		PlayerTransSetNotificationRead(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransSetNotificationRead() {}

		Result OnSetRead( Svr::TransactionResult* &pRes );
		Result OnUpdateStatus( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, SetNotificationReadRes, GetNotificationID());
	};


	class PlayerTransAcceptNotification : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::AcceptNotificationCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::AcceptNotificationCmd> super;

	private:
	public:
		PlayerTransAcceptNotification(IHeap& heap, MessageDataPtr &pIMsg);
		virtual ~PlayerTransAcceptNotification() {}

		Result OnDeletedNotification(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, AcceptNotificationRes, GetNotificationID());
	};


	class PlayerTransNotifyS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyC2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyC2SEvt> super;

	public:
		PlayerTransNotifyS2S(IHeap& heap, MessageDataPtr &pIMsg ): UserTransactionS2SEvt(heap, pIMsg) {}
		virtual ~PlayerTransNotifyS2S() {}

		// Start Transaction
		virtual Result StartTransaction();
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
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransSetNickName(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransSetNickName() {}

		Result OnNickChanged( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, SetNickNameRes, m_TotalGem, m_TotalGameMoney);
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
		PlayerInformation m_Player;
		int m_PlayerShardID;

	public:
		PlayerTransFindPlayerByEMail(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransFindPlayerByEMail() {}

		Result OnFindPlayer( Svr::TransactionResult* &pRes );
		Result OnGetNickName(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, FindPlayerByEMailRes,m_Player);
	};


	class PlayerTransFindPlayerByPlayerID : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::FindPlayerByPlayerIDCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::FindPlayerByPlayerIDCmd> super;

	private:
		PlayerInformation m_Player;
		int m_PlayerShardID;

	public:
		PlayerTransFindPlayerByPlayerID(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransFindPlayerByPlayerID() {}

		Result OnFindPlayer( Svr::TransactionResult* &pRes );
		Result OnGetNickName(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, FindPlayerByPlayerIDRes,m_Player);
	};


	class PlayerTransRequestPlayerStatusUpdate : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::RequestPlayerStatusUpdateCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::RequestPlayerStatusUpdateCmd> super;

	private:

		uint m_PlayerStatusQueryCount;

	public:
		PlayerTransRequestPlayerStatusUpdate(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransRequestPlayerStatusUpdate() {}

		Result OnPlayerShardIDRes(Svr::TransactionResult* &pRes);
		Result OnPlayerStatusUpdateRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(Policy::NetSvrPolicyGame, RequestPlayerStatusUpdateRes);
	};
	
	class PlayerTransRequestPlayerStatusUpdateS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::RequestPlayerStatusUpdateC2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::RequestPlayerStatusUpdateC2SEvt> super;

	public:
		PlayerTransRequestPlayerStatusUpdateS2S(IHeap& heap, MessageDataPtr &pIMsg ):UserTransactionS2SEvt(heap, pIMsg) {}
		virtual ~PlayerTransRequestPlayerStatusUpdateS2S() {}

		// Start Transaction
		virtual Result StartTransaction();
	};
	
	class PlayerTransNotifyPlayerStatusUpdatedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt> super;

	public:
		PlayerTransNotifyPlayerStatusUpdatedS2S(IHeap& heap, MessageDataPtr &pIMsg ):UserTransactionS2SEvt(heap, pIMsg) {}
		virtual ~PlayerTransNotifyPlayerStatusUpdatedS2S() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	class PlayerTransGetRankingList : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetRankingListCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::GetRankingListCmd> super;

	private:

		StaticArray<TotalRankingPlayerInformation, 100> m_RankingList;

	public:
		PlayerTransGetRankingList(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransGetRankingList() {}

		Result OnGetRankingListRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, GetRankingListRes, m_RankingList);
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
		PlayerTransBuyShopItemPrepare(IHeap& heap, MessageDataPtr &pIMsg);
		virtual ~PlayerTransBuyShopItemPrepare() {}

		Result OnPurchaseIDChecked(Svr::TransactionResult* &pRes);

		Result GenerateSigunatureAndCheck();

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, BuyShopItemPrepareRes, GetShopItemID(), (const char*)m_Signagure.data());
	};



	class PlayerTransBuyShopItem : public Svr::MessageTransaction< GamePlayerEntity, Message::Game::BuyShopItemCmd>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Message::Game::BuyShopItemCmd> super;

	private:
		Memento<UserGamePlayerInfoSystem::MEMENTO_SIZE> m_SavedData;
		conspiracy::ShopTbl::ShopItem *m_pShopItem;

	public:
		PlayerTransBuyShopItem(IHeap& heap, MessageDataPtr &pIMsg );
		virtual ~PlayerTransBuyShopItem() {}

		Result OnPurchaseCheckedAndroid(Svr::TransactionResult* &pRes);
		Result OnPurchaseCheckedIOS(Svr::TransactionResult* &pRes);
		Result OnSavedToDB(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGame, BuyShopItemRes, GetShopItemID());
	};
	


} // namespace GameServer 
} // namespace SF 

