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
#include "Common/Memento.h"
#include "Common/ArrayUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"



#include "Table/Conspiracy/ShopTbl.h"

#include "ServerSystem/MessageRoute.h"

#include "GameServerClass.h"

#include "GameInstance/GamePlayerEntity.h"
#include "Common/Message.h"

#include "GameSystem/UserGamePlayerInfoSystem.h"


namespace BR {

	namespace DB{
		struct QueryGetPlayerInfoData;
	};

namespace GameServer {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//

	class PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity : public Svr::UserTransactionS2SCmd< GamePlayerEntity, Policy::ISvrPolicyGameServer, Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd, PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity>
	{
	public:
		typedef Svr::UserTransactionS2SCmd< GamePlayerEntity, Policy::ISvrPolicyGameServer, Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd, PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity> super;

	public:
		PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity(Message::MessageData* &pIMsg) : UserTransactionS2SCmd(pIMsg) {}
		virtual ~PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity() {}

		// Start Transaction
		virtual HRESULT StartTransaction() override;

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(RegisterPlayerToJoinGameServerOnPlayerEntityRes, GetRouteContext().GetSwaped());
	};


	class PlayerTransJoinGameServer : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::JoinGameServerCmd, PlayerTransJoinGameServer, sizeof(Svr::TransactionMessageHandlerType)*6>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::JoinGameServerCmd, PlayerTransJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 6> super;

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

		HRESULT OnGameServerJoined( Svr::TransactionResult* &pRes );
		HRESULT OnJoinPartyRes( Svr::TransactionResult* &pRes );
		HRESULT OnCreatePlayerGameDataRes(Svr::TransactionResult* &pRes);
		HRESULT OnGetPlayerGameDataRes(Svr::TransactionResult* &pRes);

		HRESULT SetPlayerGameData(const DB::QueryGetPlayerInfoData &playerData);

		HRESULT RegisterToPlayerManager();

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(JoinGameServerRes,m_PlayerNick, m_GameUID, GetMyOwner()->GetPartyUID(), m_PartyLeaderID, m_MatchingTicket);
	};

	
	class PlayerTransGetUserGamePlayerInfo : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetUserGamePlayerInfoCmd, PlayerTransGetUserGamePlayerInfo>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetUserGamePlayerInfoCmd, PlayerTransGetUserGamePlayerInfo> super;

	private:
		struct {
			SHORT Level;
			INT64 Exp;
			INT64 GameMoney;
			INT64 Gem;
			SHORT Stamina;
			UINT32 LastUpdateTime;
			INT32 TotalPlayed;
			INT32 WinPlaySC;
			INT32 WinPlaySM;
			INT32 WinPlaySS;
			INT32 LosePlaySC;
			INT32 LosePlaySM;
			INT32 LosePlaySS;
			INT32 WinPlayNC;
			INT32 WinPlayNM;
			INT32 WinPlayNS;
			INT32 LosePlayNC;
			INT32 LosePlayNM;
			INT32 LosePlayNS;
			INT32 WeeklyWin;
			INT32 WeeklyLose;
		} m_Result;


	public:
		PlayerTransGetUserGamePlayerInfo( Message::MessageData* &pIMsg )  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetUserGamePlayerInfo() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

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
	
	
	
	class PlayerTransGetGamePlayerInfo : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetGamePlayerInfoCmd, PlayerTransGetGamePlayerInfo>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetGamePlayerInfoCmd, PlayerTransGetGamePlayerInfo> super;

	private:
		struct {
			SHORT Level;
			INT32 TotalPlayed;
			INT32 WinPlaySC;
			INT32 WinPlaySM;
			INT32 WinPlaySS;
			INT32 LosePlaySC;
			INT32 LosePlaySM;
			INT32 LosePlaySS;
			INT32 WinPlayNC;
			INT32 WinPlayNM;
			INT32 WinPlayNS;
			INT32 LosePlayNC;
			INT32 LosePlayNM;
			INT32 LosePlayNS;
			INT32 WeeklyWin;
			INT32 WeeklyLose;
		} m_Result;

	public:
		PlayerTransGetGamePlayerInfo( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetGamePlayerInfo() {}

		HRESULT OnGetPlayerShardID(Svr::TransactionResult* &pRes);
		HRESULT OnGetGamePlayerInfo( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

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



	class PlayerTransGetComplitionState : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetComplitionStateCmd, PlayerTransGetComplitionState>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetComplitionStateCmd, PlayerTransGetComplitionState> super;

	private:

		BRCLASS_ATTRIBUTE_STRING(ComplitionState, GameConst::MAX_COMPLITIONSTATE);

	public:
		PlayerTransGetComplitionState(Message::MessageData* &pIMsg);// : MessageTransaction(pIMsg) {}
		virtual ~PlayerTransGetComplitionState() {}

		HRESULT OnGetComplitionState(Svr::TransactionResult* &pRes);


		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GetComplitionStateRes, m_ComplitionState);
	};



	class PlayerTransSetComplitionState : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::SetComplitionStateCmd, PlayerTransSetComplitionState>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::SetComplitionStateCmd, PlayerTransSetComplitionState> super;

	public:
		PlayerTransSetComplitionState(Message::MessageData* &pIMsg);// : MessageTransaction(pIMsg) {}
		virtual ~PlayerTransSetComplitionState() {}

		HRESULT OnSetComplitionState(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(SetComplitionStateRes);
	};




	/////////////////////////////////////////////////////////////////////////////
	//
	//  Debug messages
	//

	class PlayerTransSetConfigPreset : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::SetPresetGameConfigIDCmd, PlayerTransSetConfigPreset>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::SetPresetGameConfigIDCmd, PlayerTransSetConfigPreset> super;

	private:
	public:
		PlayerTransSetConfigPreset( Message::MessageData* &pIMsg )  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransSetConfigPreset() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(SetPresetGameConfigIDRes);
	};
	
	class PlayerTransGainGameResource : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GainGameResourceCmd, PlayerTransGainGameResource>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GainGameResourceCmd, PlayerTransGainGameResource> super;

	private:
	public:
		PlayerTransGainGameResource( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGainGameResource() {}

		HRESULT OnSetPlayerInfoRes(  Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(GainGameResourceRes);
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	GCM register/unregister transaction
	//

	class PlayerTransRegisterGCM : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::RegisterGCMCmd, PlayerTransRegisterGCM>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::RegisterGCMCmd, PlayerTransRegisterGCM> super;

	private:
	public:
		PlayerTransRegisterGCM( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransRegisterGCM() {}

		HRESULT OnUpdated( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(RegisterGCMRes);
	};


	class PlayerTransUnregisterGCM : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::UnregisterGCMCmd, PlayerTransUnregisterGCM>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::UnregisterGCMCmd, PlayerTransUnregisterGCM> super;

	private:
	public:
		PlayerTransUnregisterGCM( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransUnregisterGCM() {}

		HRESULT OnUpdated( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(UnregisterGCMRes);
	};
	


	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Notifications transaction
	//
	
	class PlayerTransGetNotificationList : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetNotificationListCmd, PlayerTransGetNotificationList>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetNotificationListCmd, PlayerTransGetNotificationList> super;

	private:
	public:
		PlayerTransGetNotificationList( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransGetNotificationList() {}

		HRESULT OnGetList( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(GetNotificationListRes);
	};
	
	class PlayerTransDeleteNotification : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::DeleteNotificationCmd, PlayerTransDeleteNotification>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::DeleteNotificationCmd, PlayerTransDeleteNotification> super;

	private:
	public:
		PlayerTransDeleteNotification( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransDeleteNotification() {}

		HRESULT OnDeletedNotification( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(DeleteNotificationRes,GetNotificationID());
	};


	class PlayerTransSetNotificationRead : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::SetNotificationReadCmd, PlayerTransSetNotificationRead>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::SetNotificationReadCmd, PlayerTransSetNotificationRead> super;

	private:
	public:
		PlayerTransSetNotificationRead( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransSetNotificationRead() {}

		HRESULT OnSetRead( Svr::TransactionResult* &pRes );
		HRESULT OnUpdateStatus( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(SetNotificationReadRes, GetNotificationID());
	};


	class PlayerTransAcceptNotification : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AcceptNotificationCmd, PlayerTransAcceptNotification>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::AcceptNotificationCmd, PlayerTransAcceptNotification> super;

	private:
	public:
		PlayerTransAcceptNotification(Message::MessageData* &pIMsg);//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransAcceptNotification() {}

		HRESULT OnDeletedNotification(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(AcceptNotificationRes, GetNotificationID());
	};


	class PlayerTransNotifyS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::NotifyC2SEvt, PlayerTransNotifyS2S>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::NotifyC2SEvt, PlayerTransNotifyS2S> super;

	public:
		PlayerTransNotifyS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransNotifyS2S() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	

	

	/////////////////////////////////////////////////////////////////////////////
	//
	//	NickName transaction
	//

	class PlayerTransSetNickName : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::SetNickNameCmd, PlayerTransSetNickName>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::SetNickNameCmd, PlayerTransSetNickName> super;

	private:
		UINT64 m_TotalGem;
		UINT64 m_TotalGameMoney;

	public:
		PlayerTransSetNickName( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransSetNickName() {}

		HRESULT OnNickChanged( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(SetNickNameRes, m_TotalGem, m_TotalGameMoney);
	};
	
	
	/////////////////////////////////////////////////////////////////////////////
	//
	//	Find player transaction
	//

	class PlayerTransFindPlayerByEMail : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::FindPlayerByEMailCmd, PlayerTransFindPlayerByEMail>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::FindPlayerByEMailCmd, PlayerTransFindPlayerByEMail> super;

	private:
		PlayerInformation m_Player;
		int m_PlayerShardID;

	public:
		PlayerTransFindPlayerByEMail( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransFindPlayerByEMail() {}

		HRESULT OnFindPlayer( Svr::TransactionResult* &pRes );
		HRESULT OnGetNickName(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(FindPlayerByEMailRes,m_Player);
	};


	class PlayerTransFindPlayerByPlayerID : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::FindPlayerByPlayerIDCmd, PlayerTransFindPlayerByPlayerID>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::FindPlayerByPlayerIDCmd, PlayerTransFindPlayerByPlayerID> super;

	private:
		PlayerInformation m_Player;
		int m_PlayerShardID;

	public:
		PlayerTransFindPlayerByPlayerID( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransFindPlayerByPlayerID() {}

		HRESULT OnFindPlayer( Svr::TransactionResult* &pRes );
		HRESULT OnGetNickName(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(FindPlayerByPlayerIDRes,m_Player);
	};


	class PlayerTransRequestPlayerStatusUpdate : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::RequestPlayerStatusUpdateCmd, PlayerTransRequestPlayerStatusUpdate>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::RequestPlayerStatusUpdateCmd, PlayerTransRequestPlayerStatusUpdate> super;

	private:

		UINT m_PlayerStatusQueryCount;

	public:
		PlayerTransRequestPlayerStatusUpdate( Message::MessageData* &pIMsg );// :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransRequestPlayerStatusUpdate() {}

		HRESULT OnPlayerShardIDRes(Svr::TransactionResult* &pRes);
		HRESULT OnPlayerStatusUpdateRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(RequestPlayerStatusUpdateRes);
	};
	
	class PlayerTransRequestPlayerStatusUpdateS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::RequestPlayerStatusUpdateC2SEvt, PlayerTransRequestPlayerStatusUpdateS2S>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::RequestPlayerStatusUpdateC2SEvt, PlayerTransRequestPlayerStatusUpdateS2S> super;

	public:
		PlayerTransRequestPlayerStatusUpdateS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransRequestPlayerStatusUpdateS2S() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};
	
	class PlayerTransNotifyPlayerStatusUpdatedS2S : public Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt, PlayerTransNotifyPlayerStatusUpdatedS2S>
	{
	public:
		typedef Svr::UserTransactionS2SEvt< GamePlayerEntity, Policy::ISvrPolicyGame, Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt, PlayerTransNotifyPlayerStatusUpdatedS2S> super;

	public:
		PlayerTransNotifyPlayerStatusUpdatedS2S( Message::MessageData* &pIMsg ):UserTransactionS2SEvt(pIMsg) {}
		virtual ~PlayerTransNotifyPlayerStatusUpdatedS2S() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};



	class PlayerTransGetRankingList : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetRankingListCmd, PlayerTransGetRankingList>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::GetRankingListCmd, PlayerTransGetRankingList> super;

	private:

		StaticArray<TotalRankingPlayerInformation, 100> m_RankingList;

	public:
		PlayerTransGetRankingList( Message::MessageData* &pIMsg );
		virtual ~PlayerTransGetRankingList() {}

		HRESULT OnGetRankingListRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(GetRankingListRes, m_RankingList);
	};



	class PlayerTransBuyShopItemPrepare : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::BuyShopItemPrepareCmd, PlayerTransBuyShopItemPrepare>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::BuyShopItemPrepareCmd, PlayerTransBuyShopItemPrepare> super;

	private:
		const UINT MAX_RETRY = 3;

		StaticArray<BYTE, 1024> m_Signagure;
		UINT m_RetryCount;

	public:
		PlayerTransBuyShopItemPrepare(Message::MessageData* &pIMsg);// : MessageTransaction(pIMsg) { m_Signagure.push_back('\0'); }
		virtual ~PlayerTransBuyShopItemPrepare() {}

		HRESULT OnPurchaseIDChecked(Svr::TransactionResult* &pRes);

		HRESULT GenerateSigunatureAndCheck();

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(BuyShopItemPrepareRes, GetShopItemID(), (const char*)m_Signagure.data());
	};



	class PlayerTransBuyShopItem : public Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::BuyShopItemCmd, PlayerTransBuyShopItem, sizeof(Svr::TransactionMessageHandlerType) * 4>
	{
	public:
		typedef Svr::MessageTransaction< GamePlayerEntity, Policy::ISvrPolicyGame, Message::Game::BuyShopItemCmd, PlayerTransBuyShopItem, sizeof(Svr::TransactionMessageHandlerType) * 4> super;

	private:
		Memento<UserGamePlayerInfoSystem::MEMENTO_SIZE> m_SavedData;
		conspiracy::ShopTbl::ShopItem *m_pShopItem;

	public:
		PlayerTransBuyShopItem( Message::MessageData* &pIMsg );//  :MessageTransaction( pIMsg ) {}
		virtual ~PlayerTransBuyShopItem() {}

		HRESULT OnPurchaseCheckedAndroid(Svr::TransactionResult* &pRes);
		HRESULT OnPurchaseCheckedIOS(Svr::TransactionResult* &pRes);
		HRESULT OnSavedToDB(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(BuyShopItemRes, GetShopItemID());
	};
	


} // namespace GameServer 
} // namespace BR 

