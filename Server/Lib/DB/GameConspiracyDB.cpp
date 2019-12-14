
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "DBPch.h"
#include "SFTypedefs.h"

#include "DB/GameConspiracyDB.h"
#include "DB/GameConspiracyQuery.h"



namespace SF {
namespace DB {

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameConspiracyDB Class 
	//
	
	// constructor / destructor
	GameConspiracyDB::GameConspiracyDB()
	{
	}

	GameConspiracyDB::~GameConspiracyDB()
	{
	}

	void GameConspiracyDB::TerminateComponent()
	{
		TerminateDB();
	}

	
	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Game DB Interface
	//

	Result GameConspiracyDB::CreatePlayerInfoCmd(TransactionID Sender, uint shardID, const PlayerID &playerID, INT initialStamina)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCreatePlayerInfoCmd *pQuery = nullptr;
		QueryGetPlayerInfoData *pRawSet = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryCreatePlayerInfoCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->InitialStamina = initialStamina;
		pQuery->Result = 0;

		pRawSet = pQuery;
		*pRawSet = {};

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::GetPlayerInfoCmd(TransactionID Sender, uint shardID, const PlayerID &playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetPlayerInfoCmd *pQuery = nullptr;
		QueryGetPlayerInfoData *pRawSet = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryGetPlayerInfoCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pRawSet = pQuery;
		*pRawSet = {};

		pQuery->SetTransaction(Sender);

		dbChk(RequestQuery(pQuery));
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	// Save player info
	Result GameConspiracyDB::SetPlayerInfoCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
																			int16_t	Level,
																			int64_t	Exp,
																			int64_t	GameMoney,
																			int64_t	Gem,
																			int16_t	Stamina,
																			int16_t	AddedFriendSlot,
																			int32_t	TotalPlayed,
																			int32_t	WinPlaySC, int32_t WinPlaySM, int32_t WinPlaySS,
																			int32_t	LosePlaySC, int32_t LosePlaySM, int32_t LosePlaySS,
																			int32_t	WinPlayNC, int32_t WinPlayNM, int32_t WinPlayNS,
																			int32_t	LosePlayNC, int32_t LosePlayNM, int32_t LosePlayNS,
		TimeStampSec	LatestActiveTime,
		TimeStampSec	LatestTickTime
																			)
	{
		Result hr = ResultCode::SUCCESS;
		QuerySetPlayerInfoCmd *pQuery = nullptr;
		QuerySetPlayerInfoData *pDataSet = nullptr;

		dbMem( pQuery = new(GetHeap()) QuerySetPlayerInfoCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pDataSet = pQuery;
		*pDataSet = {};

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

		IHeap::Delete(pQuery);

		return hr;
	}
	Result GameConspiracyDB::SavePurchaseInfoToDB(
		TransactionID Sender, uint shardID, const PlayerID &playerID,
		int16_t	Level,
		int64_t	Exp,
		int64_t	GameMoney,
		int64_t	Gem,
		int16_t	Stamina,
		int16_t	AddedFriendSlot,
		const Array<uint8_t>& purchaseID,
		const char* purchasePlatform, const char* purchaseToken,
		TimeStampSec	LatestActiveTime,
		TimeStampSec	LatestTickTime
		)
	{
		Result hr = ResultCode::SUCCESS;
		QuerySavePurchaseInfoToDBCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QuerySavePurchaseInfoToDBCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->Result = 0;

		pQuery->Level = Level;
		pQuery->Exp = Exp;
		pQuery->GameMoney = GameMoney;
		pQuery->Gem = Gem;
		pQuery->Stamina = Stamina;
		pQuery->AddedFriendSlot = AddedFriendSlot;

		if (purchaseID.size() > sizeof(pQuery->PurchaseID))
			dbErr(ResultCode::INVALID_ARG);

		pQuery->PurchaseID = purchaseID;
		pQuery->PurchasePlatform, purchasePlatform;
		pQuery->PurchaseToken, purchaseToken;
		pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
		pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();


		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::CheckPurchaseID(TransactionID Sender, uint shardID, const Array<uint8_t>& purchaseID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCheckPurchaseIDCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryCheckPurchaseIDCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);

		if (purchaseID.size() > sizeof(pQuery->PurchaseID))
			dbErr(ResultCode::INVALID_ARG);

		pQuery->PurchaseID = purchaseID;

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::SetNickName(TransactionID Sender, uint shardID, PlayerID playerID, const char* nickName)
	{
		Result hr = ResultCode::SUCCESS;
		QuerySetNickNameCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QuerySetNickNameCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->PlayerID = playerID;
		StrUtil::StringCopy(pQuery->NickName, nickName);

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	Result GameConspiracyDB::GetNickName(TransactionID Sender, uint shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetNickNameCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryGetNickNameCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->PlayerID = playerID;
		pQuery->NickName[0] = '\0';
		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	// Save player info
	Result GameConspiracyDB::UpdateGameEndCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
																			int16_t	Level,
																			int64_t	Exp,
																			int64_t	GameMoney,
																			int32_t	TotalPlayed,
																			int32_t	WinPlaySC, int32_t WinPlaySM, int32_t WinPlaySS,
																			int32_t	LosePlaySC, int32_t LosePlaySM, int32_t LosePlaySS,
																			int32_t	WinPlayNC, int32_t WinPlayNM, int32_t WinPlayNS,
																			int32_t	LosePlayNC, int32_t LosePlayNM, int32_t LosePlayNS,
																			TimeStampSec	LatestActiveTime
																			)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateGameEndCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryUpdateGameEndCmd(GetHeap()));

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

		IHeap::Delete(pQuery);

		return hr;
	}


	// Save player info
	Result GameConspiracyDB::UpdateJoinGameCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
																			int64_t	Gem,
																			int16_t	Stamina,
																			int16_t	PlayerState,
																			TimeStampSec	LatestActiveTime,
																			TimeStampSec	LatestTickTime
																			)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateJoinGameCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryUpdateJoinGameCmd(GetHeap()));

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

		IHeap::Delete(pQuery);

		return hr;
	}


	// Save player info
	Result GameConspiracyDB::UpdateTickStatusCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
																			int64_t	Gem,
																			int16_t	Stamina,
																			int16_t	PlayerState,
																			TimeStampSec	LatestActiveTime,
																			TimeStampSec	LatestTickTime
																			)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateTickStatusCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryUpdateTickStatusCmd(GetHeap()));

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

		IHeap::Delete(pQuery);

		return hr;
	}

	
	// Save player info
	Result GameConspiracyDB::GetPlayerStatusCmd(TransactionID Sender, uint shardID, const PlayerID &playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetPlayerStatusCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryGetPlayerStatusCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->LatestActiveTime = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	
	
	// Save player info
	Result GameConspiracyDB::GetPlayerQuickInfoCmd(TransactionID Sender, uint shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetPlayerQuickInfoCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryGetPlayerQuickInfoCmd(GetHeap()));

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

		IHeap::Delete(pQuery);

		return hr;
	}



	// Save player info
	Result GameConspiracyDB::GetFriendQuickInfoCmd(TransactionID Sender, uint shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetFriendQuickInfoCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryGetFriendQuickInfoCmd(GetHeap()));

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

		IHeap::Delete(pQuery);

		return hr;
	}


	// Save player info
	Result GameConspiracyDB::GetFriendQuickInfoWithNickCmd(TransactionID Sender, uint shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetFriendQuickInfoWithNickCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryGetFriendQuickInfoWithNickCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->GameNick = "";
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

		IHeap::Delete(pQuery);

		return hr;
	}



	// Save player info
	Result GameConspiracyDB::GetFriendSlotStatus(TransactionID Sender, uint shardID, PlayerID playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetFriendSlotStatusCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryGetFriendSlotStatusCmd(GetHeap()));

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

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::AddFriend(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID, uint friendShardID, FacebookUID FriendFacebookUID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryAddFriendCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryAddFriendCmd(GetHeap()));

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

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::RemoveFriend(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryRemoveFriendCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryRemoveFriendCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = accountID;
		pQuery->FriendUID = FriendUID;
		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::GetFriendList(TransactionID Sender, uint shardID, PlayerID accountID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetFriendListCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryGetFriendListCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = accountID;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::UpdateFriendStaminaTime(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID, TimeStampSec timeStamp)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateFriendStaminaTimeCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryUpdateFriendStaminaTimeCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = accountID;
		pQuery->FriendUID = FriendUID;
		pQuery->TimeStamp = timeStamp.time_since_epoch().count();
		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	// Notifications
	Result GameConspiracyDB::Notification_Add(TransactionID Sender, uint shardID, PlayerID ToUserID, bool isCollapsable, NotificationType messageID, int64_t messageParam0, int64_t messageParam1, const char* messageText, TimeStampSec timeStamp)
	{
		Result hr = ResultCode::SUCCESS;
		QueryNotification_AddCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryNotification_AddCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = ToUserID;
		pQuery->Collapsable = isCollapsable ? 1 : 0;
		pQuery->MessageID = (uint16_t)messageID;
		pQuery->MessageParam0 = messageParam0;
		pQuery->MessageParam1 = messageParam1;
		pQuery->MessageText = messageText;
		pQuery->TimeStamp = timeStamp.time_since_epoch().count();

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::Notification_GetList(TransactionID Sender, uint shardID, PlayerID UserID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryNotification_GetListCmd *pQuery = nullptr;
		QueryNotification_GetListSet *pSet = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryNotification_GetListCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = UserID;

		pSet = pQuery;
		*pSet = {};

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::Notification_Remove(TransactionID Sender, uint shardID, PlayerID userID, int32_t notificationID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryNotification_RemoveCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryNotification_RemoveCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = userID;
		pQuery->NotificationID = notificationID;


		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::Notification_RemoveByMessageID(TransactionID Sender, uint shardID, PlayerID UserID, int16_t messageID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryNotification_RemoveByMessageIDCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryNotification_RemoveByMessageIDCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = UserID;
		pQuery->MessageID = messageID;

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::Notification_SetRead(TransactionID Sender, uint shardID, PlayerID userID, int32_t notificationID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryNotification_SetReadCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryNotification_SetReadCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->UserID = userID;
		pQuery->NotificationID = notificationID;


		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::SetComplitionState(TransactionID Sender, uint shardID, PlayerID userID, const char* complitionState)
	{
		Result hr = ResultCode::SUCCESS;
		QuerySetComplitionStateCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QuerySetComplitionStateCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->PlayerID = userID;
		pQuery->ComplitionState = complitionState;


		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::GetComplitionState(TransactionID Sender, uint shardID, PlayerID userID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetComplitionStateCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryGetComplitionStateCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->SetTransaction(Sender);
		pQuery->PlayerID = userID;
		pQuery->ComplitionState = "";


		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}



} //namespace DB
} // namespace SF

