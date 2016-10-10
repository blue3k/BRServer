
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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

	void GameConspiracyDB::TerminateComponent()
	{
		TerminateDB();
		Svr::IServerComponent::TerminateComponent();
	}

	
	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Game DB Interface
	//

	Result GameConspiracyDB::CreatePlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID, INT initialStamina)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCreatePlayerInfoCmd *pQuery = nullptr;
		QueryGetPlayerInfoData *pRawSet = nullptr;

		dbMem(pQuery = new QueryCreatePlayerInfoCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->InitialStamina = initialStamina;
		pQuery->Result = 0;

		pRawSet = pQuery;
		memset(pRawSet, 0, sizeof(QueryGetPlayerInfoData));

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	Result GameConspiracyDB::GetPlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetPlayerInfoCmd *pQuery = nullptr;
		QueryGetPlayerInfoData *pRawSet = nullptr;

		dbMem(pQuery = new QueryGetPlayerInfoCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pRawSet = pQuery;
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
	Result GameConspiracyDB::SetPlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
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
		Result hr = ResultCode::SUCCESS;
		QuerySetPlayerInfoCmd *pQuery = nullptr;
		QuerySetPlayerInfoData *pDataSet = nullptr;

		dbMem( pQuery = new QuerySetPlayerInfoCmd );

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pDataSet = pQuery;
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
	Result GameConspiracyDB::SavePurchaseInfoToDB(
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
		Result hr = ResultCode::SUCCESS;
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
			dbErr(ResultCode::INVALID_ARG);
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

	Result GameConspiracyDB::CheckPurchaseID(TransactionID Sender, UINT shardID, const Array<BYTE>& purchaseID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCheckPurchaseIDCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryCheckPurchaseIDCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);

		if (purchaseID.GetSize() > sizeof(pQuery->PurchaseID))
			dbErr(ResultCode::INVALID_ARG);
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

	Result GameConspiracyDB::SetNickName(TransactionID Sender, UINT shardID, PlayerID playerID, const char* nickName)
	{
		Result hr = ResultCode::SUCCESS;
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


	Result GameConspiracyDB::GetNickName(TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::UpdateGameEndCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
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
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::UpdateJoinGameCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																			INT64	Gem,
																			SHORT	Stamina,
																			SHORT	PlayerState,
																			TimeStampSec	LatestActiveTime,
																			TimeStampSec	LatestTickTime
																			)
	{
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::UpdateTickStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																			INT64	Gem,
																			SHORT	Stamina,
																			SHORT	PlayerState,
																			TimeStampSec	LatestActiveTime,
																			TimeStampSec	LatestTickTime
																			)
	{
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::GetPlayerStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID)
	{
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::GetPlayerQuickInfoCmd(TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::GetFriendQuickInfoCmd(TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::GetFriendQuickInfoWithNickCmd(TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::GetFriendSlotStatus(TransactionID Sender, UINT shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
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

	Result GameConspiracyDB::AddFriend(TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID, UINT friendShardID, FacebookUID FriendFacebookUID)
	{
		Result hr = ResultCode::SUCCESS;
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

	Result GameConspiracyDB::RemoveFriend(TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID)
	{
		Result hr = ResultCode::SUCCESS;
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

	Result GameConspiracyDB::GetFriendList(TransactionID Sender, UINT shardID, PlayerID accountID)
	{
		Result hr = ResultCode::SUCCESS;
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

	Result GameConspiracyDB::UpdateFriendStaminaTime(TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID, TimeStampSec timeStamp)
	{
		Result hr = ResultCode::SUCCESS;
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
	Result GameConspiracyDB::Notification_Add(TransactionID Sender, UINT shardID, PlayerID ToUserID, bool isCollapsable, NotificationType messageID, INT64 messageParam0, INT64 messageParam1, const char* messageText, TimeStampSec timeStamp)
	{
		Result hr = ResultCode::SUCCESS;
		QueryNotification_AddCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryNotification_AddCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = ToUserID;
		pQuery->Collapsable = isCollapsable ? 1 : 0;
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

	Result GameConspiracyDB::Notification_GetList(TransactionID Sender, UINT shardID, PlayerID UserID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryNotification_GetListCmd *pQuery = nullptr;
		QueryNotification_GetListSet *pSet = nullptr;

		dbMem(pQuery = new QueryNotification_GetListCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = UserID;

		pSet = pQuery;
		memset(pSet, 0, sizeof(QueryNotification_GetListSet));

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (FAILED(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}

	Result GameConspiracyDB::Notification_Remove(TransactionID Sender, UINT shardID, PlayerID userID, INT32 notificationID)
	{
		Result hr = ResultCode::SUCCESS;
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

	Result GameConspiracyDB::Notification_RemoveByMessageID(TransactionID Sender, UINT shardID, PlayerID UserID, INT16 messageID)
	{
		Result hr = ResultCode::SUCCESS;
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

	Result GameConspiracyDB::Notification_SetRead(TransactionID Sender, UINT shardID, PlayerID userID, INT32 notificationID)
	{
		Result hr = ResultCode::SUCCESS;
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

	Result GameConspiracyDB::SetComplitionState(TransactionID Sender, UINT shardID, PlayerID userID, const char* complitionState)
	{
		Result hr = ResultCode::SUCCESS;
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

	Result GameConspiracyDB::GetComplitionState(TransactionID Sender, UINT shardID, PlayerID userID)
	{
		Result hr = ResultCode::SUCCESS;
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

