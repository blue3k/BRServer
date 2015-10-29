
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Common/Typedefs.h"

#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"



namespace BR {
namespace DB {

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameConspiracyDB Class 
	//
	
	// constructor / destructor
	GameConspiracyDB::GameConspiracyDB()
		:Svr::IServerComponent(ComponentID)
	{
	}

	GameConspiracyDB::~GameConspiracyDB()
	{
	}

	
	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Game DB Interface
	//

	HRESULT GameConspiracyDB::CreatePlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID, INT initialStamina)
	{
		HRESULT hr = S_OK;
		QueryCreatePlayerInfoCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryCreatePlayerInfoCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->InitialStamina = initialStamina;
		pQuery->Result = 0;

		QueryGetPlayerInfoData *pRawSet = pQuery;
		memset(pRawSet, 0, sizeof(QueryGetPlayerInfoData));

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	HRESULT GameConspiracyDB::GetPlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID)
	{
		HRESULT hr = S_OK;
		QueryGetPlayerInfoCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetPlayerInfoCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		QueryGetPlayerInfoData *pRawSet = pQuery;
		memset(pRawSet, 0, sizeof(QueryGetPlayerInfoData));

		pQuery->SetTransaction(Sender);

		dbChk(RequestQuery(pQuery));
		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}


	// Save player info
	HRESULT GameConspiracyDB::SetPlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																			SHORT	Level,
																			INT64	Exp,
																			INT64	GameMoney,
																			INT64	Gem,
																			SHORT	Stamina,
																			SHORT	AddedFriendSlot,
																			INT32	TotalPlayed,
																			INT32	WinPlaySC, INT32 WinPlaySM, INT32 WinPlaySS,
																			INT32	LosePlaySC, INT32 LosePlaySM, INT32 LosePlaySS,
																			INT32	WinPlayNC, INT32 WinPlayNM, INT32 WinPlayNS,
																			INT32	LosePlayNC, INT32 LosePlayNM, INT32 LosePlayNS,
		TimeStampSec	LatestActiveTime,
		TimeStampSec	LatestTickTime
																			)
	{
		HRESULT hr = S_OK;
		QuerySetPlayerInfoCmd *pQuery = nullptr;

		dbMem( pQuery = new QuerySetPlayerInfoCmd );

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		QuerySetPlayerInfoData *pDataSet = pQuery;
		memset(pDataSet, 0, sizeof(QuerySetPlayerInfoData));

		pDataSet->Level = Level;
		pDataSet->Exp = Exp;
		pDataSet->GameMoney = GameMoney;
		pDataSet->Gem = Gem;
		pDataSet->Stamina = Stamina;
		pDataSet->AddedFriendSlot = AddedFriendSlot;
		pDataSet->TotalPlayed = TotalPlayed;
		pDataSet->WinPlaySC = WinPlaySC;
		pDataSet->WinPlaySM = WinPlaySM;
		pDataSet->WinPlaySS = WinPlaySS;
		pDataSet->LosePlaySC = LosePlaySC;
		pDataSet->LosePlaySM = LosePlaySM;
		pDataSet->LosePlaySS = LosePlaySS;
		pDataSet->WinPlayNC = WinPlayNC;
		pDataSet->WinPlayNM = WinPlayNM;
		pDataSet->WinPlayNS = WinPlayNS;
		pDataSet->LosePlayNC = LosePlayNC;
		pDataSet->LosePlayNM = LosePlayNM;
		pDataSet->LosePlayNS = LosePlayNS;
		pDataSet->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
		pDataSet->LatestTickTime = LatestTickTime.time_since_epoch().count();


		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}
	HRESULT GameConspiracyDB::SavePurchaseInfoToDB(
		TransactionID Sender, UINT shardID, const PlayerID &playerID,
		SHORT	Level,
		INT64	Exp,
		INT64	GameMoney,
		INT64	Gem,
		SHORT	Stamina,
		SHORT	AddedFriendSlot,
		const Array<BYTE>& purchaseID,
		const char* purchasePlatform, const char* purchaseToken,
		TimeStampSec	LatestActiveTime,
		TimeStampSec	LatestTickTime
		)
	{
		HRESULT hr = S_OK;
		QuerySavePurchaseInfoToDBCmd *pQuery = nullptr;

		dbMem(pQuery = new QuerySavePurchaseInfoToDBCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pQuery->Level = Level;
		pQuery->Exp = Exp;
		pQuery->GameMoney = GameMoney;
		pQuery->Gem = Gem;
		pQuery->Stamina = Stamina;
		pQuery->AddedFriendSlot = AddedFriendSlot;

		if (purchaseID.GetSize() > sizeof(pQuery->PurchaseID))
			dbErr(E_INVALIDARG);
		memset(pQuery->PurchaseID, 0, sizeof(pQuery->PurchaseID));
		memcpy(pQuery->PurchaseID, purchaseID.data(), purchaseID.GetSize());

		dbChk(StrUtil::StringCpy(pQuery->PurchasePlatform, purchasePlatform));
		dbChk(StrUtil::StringCpy(pQuery->PurchaseToken, purchaseToken));
		pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
		pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();


		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	HRESULT GameConspiracyDB::CheckPurchaseID(TransactionID Sender, UINT shardID, const Array<BYTE>& purchaseID)
	{
		HRESULT hr = S_OK;
		QueryCheckPurchaseIDCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryCheckPurchaseIDCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);

		if (purchaseID.GetSize() > sizeof(pQuery->PurchaseID))
			dbErr(E_INVALIDARG);
		memset(pQuery->PurchaseID, 0, sizeof(pQuery->PurchaseID));
		memcpy(pQuery->PurchaseID, purchaseID.data(), purchaseID.GetSize());

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::SetNickName(BR::TransactionID Sender, UINT shardID, PlayerID playerID, const char* nickName)
	{
		HRESULT hr = S_OK;
		QuerySetNickNameCmd *pQuery = nullptr;

		dbMem(pQuery = new QuerySetNickNameCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->PlayerID = playerID;
		StrUtil::StringCpy(pQuery->NickName, nickName);

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}


	HRESULT GameConspiracyDB::GetNickName(BR::TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		HRESULT hr = S_OK;
		QueryGetNickNameCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetNickNameCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->PlayerID = playerID;
		pQuery->NickName[0] = '\0';
		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}


	// Save player info
	HRESULT GameConspiracyDB::UpdateGameEndCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																			SHORT	Level,
																			INT64	Exp,
																			INT64	GameMoney,
																			INT32	TotalPlayed,
																			INT32	WinPlaySC, INT32 WinPlaySM, INT32 WinPlaySS,
																			INT32	LosePlaySC, INT32 LosePlaySM, INT32 LosePlaySS,
																			INT32	WinPlayNC, INT32 WinPlayNM, INT32 WinPlayNS,
																			INT32	LosePlayNC, INT32 LosePlayNM, INT32 LosePlayNS,
																			TimeStampSec	LatestActiveTime
																			)
	{
		HRESULT hr = S_OK;
		QueryUpdateGameEndCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryUpdateGameEndCmd );

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pQuery->Level = Level;
		pQuery->Exp = Exp;
		pQuery->GameMoney = GameMoney;
		pQuery->TotalPlayed = TotalPlayed;
		pQuery->WinPlaySC = WinPlaySC;
		pQuery->WinPlaySM = WinPlaySM;
		pQuery->WinPlaySS = WinPlaySS;
		pQuery->LosePlaySC = LosePlaySC;
		pQuery->LosePlaySM = LosePlaySM;
		pQuery->LosePlaySS = LosePlaySS;
		pQuery->WinPlayNC = WinPlayNC;
		pQuery->WinPlayNM = WinPlayNM;
		pQuery->WinPlayNS = WinPlayNS;
		pQuery->LosePlayNC = LosePlayNC;
		pQuery->LosePlayNM = LosePlayNM;
		pQuery->LosePlayNS = LosePlayNS;
		pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}


	// Save player info
	HRESULT GameConspiracyDB::UpdateJoinGameCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																			INT64	Gem,
																			SHORT	Stamina,
																			SHORT	PlayerState,
																			TimeStampSec	LatestActiveTime,
																			TimeStampSec	LatestTickTime
																			)
	{
		HRESULT hr = S_OK;
		QueryUpdateJoinGameCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryUpdateJoinGameCmd );

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pQuery->Gem = Gem;
		pQuery->Stamina = Stamina;
		pQuery->PlayerState = PlayerState;
		pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
		pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();


		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}


	// Save player info
	HRESULT GameConspiracyDB::UpdateTickStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																			INT64	Gem,
																			SHORT	Stamina,
																			SHORT	PlayerState,
																			TimeStampSec	LatestActiveTime,
																			TimeStampSec	LatestTickTime
																			)
	{
		HRESULT hr = S_OK;
		QueryUpdateTickStatusCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryUpdateTickStatusCmd );

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pQuery->Gem = Gem;
		pQuery->Stamina = Stamina;
		pQuery->PlayerState = PlayerState;
		pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
		pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	
	// Save player info
	HRESULT GameConspiracyDB::GetPlayerStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID)
	{
		HRESULT hr = S_OK;
		QueryGetPlayerStatusCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryGetPlayerStatusCmd );

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->LatestActiveTime = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	
	
	// Save player info
	HRESULT GameConspiracyDB::GetPlayerQuickInfoCmd(BR::TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		HRESULT hr = S_OK;
		QueryGetPlayerQuickInfoCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryGetPlayerQuickInfoCmd );

		pQuery->SetPartitioningKey(shardID);

		pQuery->UserID = playerID;
		pQuery->Level = 0;
		pQuery->WeeklyWin = 0;
		pQuery->WeeklyLose = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}



	// Save player info
	HRESULT GameConspiracyDB::GetFriendQuickInfoCmd(BR::TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		HRESULT hr = S_OK;
		QueryGetFriendQuickInfoCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetFriendQuickInfoCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Level = 0;
		pQuery->WeeklyWin = 0;
		pQuery->WeeklyLose = 0;
		pQuery->PlayerState = 0;
		pQuery->LatestActiveTime = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction(Sender);

		dbChk(RequestQuery(pQuery));
		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}


	// Save player info
	HRESULT GameConspiracyDB::GetFriendQuickInfoWithNickCmd(BR::TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		HRESULT hr = S_OK;
		QueryGetFriendQuickInfoWithNickCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetFriendQuickInfoWithNickCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->GameNick[0] = '\0';
		pQuery->Level = 0;
		pQuery->WeeklyWin = 0;
		pQuery->WeeklyLose = 0;
		pQuery->PlayerState = 0;
		pQuery->LatestActiveTime = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction(Sender);

		dbChk(RequestQuery(pQuery));
		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}



	// Save player info
	HRESULT GameConspiracyDB::GetFriendSlotStatus(BR::TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		HRESULT hr = S_OK;
		QueryGetFriendSlotStatusCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetFriendSlotStatusCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Level = 0;
		pQuery->AddedFriendSlot = 0;
		pQuery->NumFriends = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction(Sender);

		dbChk(RequestQuery(pQuery));
		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::AddFriend(BR::TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID, UINT friendShardID, FacebookUID FriendFacebookUID)
	{
		HRESULT hr = S_OK;
		QueryAddFriendCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryAddFriendCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = accountID;
		pQuery->FriendUID = FriendUID;
		pQuery->FriendShardID = friendShardID;
		pQuery->FriendFacebookUID = FriendFacebookUID;
		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::RemoveFriend(BR::TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID)
	{
		HRESULT hr = S_OK;
		QueryRemoveFriendCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryRemoveFriendCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = accountID;
		pQuery->FriendUID = FriendUID;
		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::GetFriendList(BR::TransactionID Sender, UINT shardID, PlayerID accountID)
	{
		HRESULT hr = S_OK;
		QueryGetFriendListCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetFriendListCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = accountID;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::UpdateFriendStaminaTime(BR::TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID, TimeStampSec timeStamp)
	{
		HRESULT hr = S_OK;
		QueryUpdateFriendStaminaTimeCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryUpdateFriendStaminaTimeCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = accountID;
		pQuery->FriendUID = FriendUID;
		pQuery->TimeStamp = timeStamp.time_since_epoch().count();
		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}


	// Notifications
	HRESULT GameConspiracyDB::Notification_Add(BR::TransactionID Sender, UINT shardID, PlayerID ToUserID, BOOL isCollapsable, NotificationType messageID, INT64 messageParam0, INT64 messageParam1, const char* messageText, TimeStampSec timeStamp)
	{
		HRESULT hr = S_OK;
		QueryNotification_AddCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryNotification_AddCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = ToUserID;
		pQuery->Collapsable = isCollapsable;
		pQuery->MessageID = (UINT16)messageID;
		pQuery->MessageParam0 = messageParam0;
		pQuery->MessageParam1 = messageParam1;
		dbChk(StrUtil::StringCpy(pQuery->MessageText, messageText));
		pQuery->TimeStamp = timeStamp.time_since_epoch().count();

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::Notification_GetList(BR::TransactionID Sender, UINT shardID, PlayerID UserID)
	{
		HRESULT hr = S_OK;
		QueryNotification_GetListCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryNotification_GetListCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = UserID;

		QueryNotification_GetListSet *pSet = pQuery;
		memset(pSet, 0, sizeof(QueryNotification_GetListSet));

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::Notification_Remove(BR::TransactionID Sender, UINT shardID, PlayerID userID, INT32 notificationID)
	{
		HRESULT hr = S_OK;
		QueryNotification_RemoveCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryNotification_RemoveCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = userID;
		pQuery->NotificationID = notificationID;


		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::Notification_RemoveByMessageID(BR::TransactionID Sender, UINT shardID, PlayerID UserID, INT16 messageID)
	{
		HRESULT hr = S_OK;
		QueryNotification_RemoveByMessageIDCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryNotification_RemoveByMessageIDCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = UserID;
		pQuery->MessageID = messageID;

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::Notification_SetRead(BR::TransactionID Sender, UINT shardID, PlayerID userID, INT32 notificationID)
	{
		HRESULT hr = S_OK;
		QueryNotification_SetReadCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryNotification_SetReadCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = userID;
		pQuery->NotificationID = notificationID;


		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::SetComplitionState(BR::TransactionID Sender, UINT shardID, PlayerID userID, const char* complitionState)
	{
		HRESULT hr = S_OK;
		QuerySetComplitionStateCmd *pQuery = nullptr;

		dbMem(pQuery = new QuerySetComplitionStateCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->PlayerID = userID;
		dbChk(StrUtil::StringCpy(pQuery->ComplitionState, complitionState));


		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	HRESULT GameConspiracyDB::GetComplitionState(BR::TransactionID Sender, UINT shardID, PlayerID userID)
	{
		HRESULT hr = S_OK;
		QueryGetComplitionStateCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetComplitionStateCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->PlayerID = userID;
		dbChk(StrUtil::StringCpy(pQuery->ComplitionState, ""));


		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}



} //namespace DB
} // namespace BR

