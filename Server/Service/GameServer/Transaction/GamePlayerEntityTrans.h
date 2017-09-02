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
#include "Common/Memento.h"
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

	class PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity : public Svr::UserTransactionS2SCmd< GamePlayerEntity, Policy::NetSvrPolicyGameServer, Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd, PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity>
	{
	public:
		typedef Svr::UserTransactionS2SCmd< GamePlayerEntity, Policy::NetSvrPolicyGameServer, Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd, PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity> super;

	public:
		PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity(Message::MessageData* &pIMsg) : UserTransactionS2SCmd(pIMsg) {}
		virtual ~PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_SVR_MSGTRANS_CLOSE(RegisterPlayerToJoinGameServerOnPlayerEntityRes, GetRouteContext().GetSwaped());
	};


	class PlayerTransJoinGameServer : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::JoinGameServerCmd, PlayerTransJoinGameServer, sizeof(Svr::TransactionMessageHandlerType)*6>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::JoinGameServerCmd, PlayerTransJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 6> super;

	private:
		
		enum class Step
		{
			Begin,
			GetPlayerGameData,
			UpdatePartyMatching,
			JoinParty,
			Done,
		} m_Step;
		

		BRCLASS_ATTRIBUTE_STRING(PlayerNick,GameConst::MAX_NAME);
		PlayerID m_PartyLeaderID;
		GameInsUID m_GameUID;
		MatchingQueueTicket m_MatchingTicket;

	public:
		PlayerTransJoinGameServer( Message::MessageData* &pIMsg );
		virtual ~PlayerTransJoinGameServer() {}

		Result OnGameServerJoined( Svr::TransactionResult* &pRes );
		Result OnJoinPartyRes( Svr::TransactionResult* &pRes );
		Result OnCreatePlayerGameDataRes(Svr::TransactionResult* &pRes);
		Result OnGetPlayerGameDataRes(Svr::TransactionResult* &pRes);

		Result SetPlayerGameData(const DB::QueryGetPlayerInfoData &playerData);

		Result RegisterToPlayerManager();

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(JoinGameServerRes,m_PlayerNick, m_GameUID, GetMyOwner()->GetPartyUID(), m_PartyLeaderID, m_MatchingTicket);
	};

	
	class PlayerTransGetUserGamePlayerInfo : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetUserGamePlayerInfoCmd, PlayerTransGetUserGamePlayerInfo>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetUserGamePlayerInfoCmd, PlayerTransGetUserGamePlayerInfo> super;

	private:
		struct {
			SHORT Level;
			int64_t Exp;
			int64_t GameMoney;
			int64_t Gem;
			SHORT Stamina;
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
		PlayerTransGetUserGamePlayerInfo( Message::MessageData* &pIMsg )  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetUserGamePlayerInfo() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GetUserGamePlayerInfoRes,
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
	
	
	
	class PlayerTransGetGamePlayerInfo : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetGamePlayerInfoCmd, PlayerTransGetGamePlayerInfo>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetGamePlayerInfoCmd, PlayerTransGetGamePlayerInfo> super;

	private:
		struct {
			SHORT Level;
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
		PlayerTransGetGamePlayerInfo( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetGamePlayerInfo() {}

		Result OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		Result OnGetGamePlayerInfo( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GetGamePlayerInfoRes,
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



	class PlayerTransGetComplitionState : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetComplitionStateCmd, PlayerTransGetComplitionState>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetComplitionStateCmd, PlayerTransGetComplitionState> super;

	private:

		BRCLASS_ATTRIBUTE_STRING(ComplitionState, GameConst::MAX_COMPLITIONSTATE);

	public:
		PlayerTransGetComplitionState(Message::MessageData* &pIMsg);// : MessageTransaction(pIMsg) {}
		virtual ~PlayerTransGetComplitionState() {}

		Result OnGetComplitionState(Svr::TransactionResult* &pRes);


		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GetComplitionStateRes, m_ComplitionState);
	};



	class PlayerTransSetComplitionState : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::SetComplitionStateCmd, PlayerTransSetComplitionState>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::SetComplitionStateCmd, PlayerTransSetComplitionState> super;

	public:
		PlayerTransSetComplitionState(Message::MessageData* &pIMsg);// : MessageTransaction(pIMsg) {}
		virtual ~PlayerTransSetComplitionState() {}

		Result OnSetComplitionState(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(SetComplitionStateRes);
	};




	/////////////////////////////////////////////////////////////////////////////
	//
	//  Debug messages
	//

	class PlayerTransSetConfigPreset : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::SetPresetGameConfigIDCmd, PlayerTransSetConfigPreset>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::SetPresetGameConfigIDCmd, PlayerTransSetConfigPreset> super;

	private:
	public:
		PlayerTransSetConfigPreset( Message::MessageData* &pIMsg )  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransSetConfigPreset() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(SetPresetGameConfigIDRes);
	};
	
	class PlayerTransGainGameResource : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GainGameResourceCmd, PlayerTransGainGameResource>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GainGameResourceCmd, PlayerTransGainGameResource> super;

	private:
	public:
		PlayerTransGainGameResource( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGainGameResource() {}

		Result OnSetPlayerInfoRes(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(GainGameResourceRes);
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	GCM register/unregister transaction
	//

	class PlayerTransRegisterGCM : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::RegisterGCMCmd, PlayerTransRegisterGCM>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::RegisterGCMCmd, PlayerTransRegisterGCM> super;

	private:
	public:
		PlayerTransRegisterGCM( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransRegisterGCM() {}

		Result OnUpdated( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(RegisterGCMRes);
	};


	class PlayerTransUnregisterGCM : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::UnregisterGCMCmd, PlayerTransUnregisterGCM>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::UnregisterGCMCmd, PlayerTransUnregisterGCM> super;

	private:
	public:
		PlayerTransUnregisterGCM( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransUnregisterGCM() {}

		Result OnUpdated( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(UnregisterGCMRes);
	};
	


	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Notifications transaction
	//
	
	class PlayerTransGetNotificationList : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetNotificationListCmd, PlayerTransGetNotificationList>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetNotificationListCmd, PlayerTransGetNotificationList> super;

	private:
	public:
		PlayerTransGetNotificationList( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetNotificationList() {}

		Result OnGetList( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(GetNotificationListRes);
	};
	
	class PlayerTransDeleteNotification : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::DeleteNotificationCmd, PlayerTransDeleteNotification>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::DeleteNotificationCmd, PlayerTransDeleteNotification> super;

	private:
	public:
		PlayerTransDeleteNotification( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransDeleteNotification() {}

		Result OnDeletedNotification( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(DeleteNotificationRes,GetNotificationID());
	};


	class PlayerTransSetNotificationRead : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::SetNotificationReadCmd, PlayerTransSetNotificationRead>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::SetNotificationReadCmd, PlayerTransSetNotificationRead> super;

	private:
	public:
		PlayerTransSetNotificationRead( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransSetNotificationRead() {}

		Result OnSetRead( Svr::TransactionResult* &pRes );
		Result OnUpdateStatus( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(SetNotificationReadRes, GetNotificationID());
	};


	class PlayerTransAcceptNotification : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::AcceptNotificationCmd, PlayerTransAcceptNotification>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::AcceptNotificationCmd, PlayerTransAcceptNotification> super;

	private:
	public:
		PlayerTransAcceptNotification(Message::MessageData* &pIMsg);//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransAcceptNotification() {}

		Result OnDeletedNotification(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(AcceptNotificationRes, GetNotificationID());
	};


	class PlayerTransNotifyS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::NotifyC2SEvt, PlayerTransNotifyS2S>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::NotifyC2SEvt, PlayerTransNotifyS2S> super;

	public:
		PlayerTransNotifyS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransNotifyS2S() {}

		// Start Transaction
		virtual Result StartTransaction();
	};
	

	

	/////////////////////////////////////////////////////////////////////////////
	//
	//	NickName transaction
	//

	class PlayerTransSetNickName : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::SetNickNameCmd, PlayerTransSetNickName>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::SetNickNameCmd, PlayerTransSetNickName> super;

	private:
		uint64_t m_TotalGem;
		uint64_t m_TotalGameMoney;

	public:
		PlayerTransSetNickName( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransSetNickName() {}

		Result OnNickChanged( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(SetNickNameRes, m_TotalGem, m_TotalGameMoney);
	};
	
	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Find player transaction
	//

	class PlayerTransFindPlayerByEMail : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::FindPlayerByEMailCmd, PlayerTransFindPlayerByEMail>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::FindPlayerByEMailCmd, PlayerTransFindPlayerByEMail> super;

	private:
		PlayerInformation m_Player;
		int m_PlayerShardID;

	public:
		PlayerTransFindPlayerByEMail( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransFindPlayerByEMail() {}

		Result OnFindPlayer( Svr::TransactionResult* &pRes );
		Result OnGetNickName(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(FindPlayerByEMailRes,m_Player);
	};


	class PlayerTransFindPlayerByPlayerID : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::FindPlayerByPlayerIDCmd, PlayerTransFindPlayerByPlayerID>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::FindPlayerByPlayerIDCmd, PlayerTransFindPlayerByPlayerID> super;

	private:
		PlayerInformation m_Player;
		int m_PlayerShardID;

	public:
		PlayerTransFindPlayerByPlayerID( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransFindPlayerByPlayerID() {}

		Result OnFindPlayer( Svr::TransactionResult* &pRes );
		Result OnGetNickName(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(FindPlayerByPlayerIDRes,m_Player);
	};


	class PlayerTransRequestPlayerStatusUpdate : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::RequestPlayerStatusUpdateCmd, PlayerTransRequestPlayerStatusUpdate>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::RequestPlayerStatusUpdateCmd, PlayerTransRequestPlayerStatusUpdate> super;

	private:

		uint m_PlayerStatusQueryCount;

	public:
		PlayerTransRequestPlayerStatusUpdate( Message::MessageData* &pIMsg );// :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransRequestPlayerStatusUpdate() {}

		Result OnPlayerShardIDRes(Svr::TransactionResult* &pRes);
		Result OnPlayerStatusUpdateRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(RequestPlayerStatusUpdateRes);
	};
	
	class PlayerTransRequestPlayerStatusUpdateS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::RequestPlayerStatusUpdateC2SEvt, PlayerTransRequestPlayerStatusUpdateS2S>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::RequestPlayerStatusUpdateC2SEvt, PlayerTransRequestPlayerStatusUpdateS2S> super;

	public:
		PlayerTransRequestPlayerStatusUpdateS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransRequestPlayerStatusUpdateS2S() {}

		// Start Transaction
		virtual Result StartTransaction();
	};
	
	class PlayerTransNotifyPlayerStatusUpdatedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt, PlayerTransNotifyPlayerStatusUpdatedS2S>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt, PlayerTransNotifyPlayerStatusUpdatedS2S> super;

	public:
		PlayerTransNotifyPlayerStatusUpdatedS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransNotifyPlayerStatusUpdatedS2S() {}

		// Start Transaction
		virtual Result StartTransaction();
	};



	class PlayerTransGetRankingList : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetRankingListCmd, PlayerTransGetRankingList>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::GetRankingListCmd, PlayerTransGetRankingList> super;

	private:

		StaticArray<TotalRankingPlayerInformation, 100> m_RankingList;

	public:
		PlayerTransGetRankingList( Message::MessageData* &pIMsg );
		virtual ~PlayerTransGetRankingList() {}

		Result OnGetRankingListRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GetRankingListRes, m_RankingList);
	};



	class PlayerTransBuyShopItemPrepare : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::BuyShopItemPrepareCmd, PlayerTransBuyShopItemPrepare>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::BuyShopItemPrepareCmd, PlayerTransBuyShopItemPrepare> super;

	private:
		const uint MAX_RETRY = 3;

		StaticArray<uint8_t, 1024> m_Signagure;
		uint m_RetryCount;

	public:
		PlayerTransBuyShopItemPrepare(Message::MessageData* &pIMsg);// : MessageTransaction(pIMsg) { m_Signagure.push_back('\0'); }
		virtual ~PlayerTransBuyShopItemPrepare() {}

		Result OnPurchaseIDChecked(Svr::TransactionResult* &pRes);

		Result GenerateSigunatureAndCheck();

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(BuyShopItemPrepareRes, GetShopItemID(), (const char*)m_Signagure.data());
	};



	class PlayerTransBuyShopItem : public Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::BuyShopItemCmd, PlayerTransBuyShopItem, sizeof(Svr::TransactionMessageHandlerType) * 4>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::NetSvrPolicyGame, Message::Game::BuyShopItemCmd, PlayerTransBuyShopItem, sizeof(Svr::TransactionMessageHandlerType) * 4> super;

	private:
		Memento<UserGamePlayerInfoSystem::MEMENTO_SIZE> m_SavedData;
		conspiracy::ShopTbl::ShopItem *m_pShopItem;

	public:
		PlayerTransBuyShopItem( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransBuyShopItem() {}

		Result OnPurchaseCheckedAndroid(Svr::TransactionResult* &pRes);
		Result OnPurchaseCheckedIOS(Svr::TransactionResult* &pRes);
		Result OnSavedToDB(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(BuyShopItemRes, GetShopItemID());
	};
	


} // namespace GameServer 
} // namespace SF 

