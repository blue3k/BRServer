
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2020 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "DBPch.h"
#include "SFTypedefs.h"

#include "GameDB.h"
#include "GameQuery.h"



namespace SF {
	namespace DB {


		//////////////////////////////////////////////////////////////////////////////////
		//
		//	GameDB Class 
		//

		// constructor / destructor
		GameDB::GameDB()
		{
		}

		GameDB::~GameDB()
		{
		}

		void GameDB::TerminateComponent()
		{
			TerminateDB();
		}


		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Game DB Interface
		//

		Result GameDB::CreatePlayerInfoCmd(TransactionID Sender, uint shardID, const PlayerID& playerID)
		{
			FunctionContext hr;

			UniquePtr<QueryCreatePlayerInfoCmd> pQuery(new(GetHeap()) QueryCreatePlayerInfoCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->PlayerID = playerID;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}

		Result GameDB::GetPlayerInfoCmd(TransactionID Sender, uint shardID, const PlayerID& playerID)
		{
			FunctionContext hr;

			UniquePtr<QueryGetPlayerInfoCmd> pQuery(new(GetHeap()) QueryGetPlayerInfoCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->PlayerID = playerID;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}


		// Save player info
		Result GameDB::SetPlayerInfoCmd(TransactionID Sender, uint shardID, const PlayerID& playerID,
			UTCTimeStampSec	LatestActiveTime,
			UTCTimeStampSec	LatestTickTime,
			const Array<NamedVariableBox>& Variables)
		{
			FunctionContext hr;

			UniquePtr<QuerySetPlayerInfoCmd> pQuery(new(GetHeap()) QuerySetPlayerInfoCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->PlayerID = playerID;
			pQuery->Result = 0;

			pQuery->Attributes = Variables;
			pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
			pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}

		Result GameDB::SavePurchaseInfoToDB(
			TransactionID Sender, uint shardID, const PlayerID &playerID,
			int16_t	Level,
			int64_t	Exp,
			int64_t	GameMoney,
			int64_t	Gem,
			int16_t	Stamina,
			int16_t	AddedFriendSlot,
			const Array<uint8_t>& purchaseID,
			const char* purchasePlatform, const char* purchaseToken,
			UTCTimeStampSec	LatestActiveTime,
			UTCTimeStampSec	LatestTickTime
			)
		{
			FunctionContext hr;

			UniquePtr<QuerySavePurchaseInfoToDBCmd> pQuery(new(GetHeap()) QuerySavePurchaseInfoToDBCmd(GetHeap()));
			dbCheckMem(pQuery);

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
				dbError(ResultCode::INVALID_ARG);

			pQuery->PurchaseID = purchaseID;
			pQuery->PurchasePlatform = purchasePlatform;
			pQuery->PurchaseToken = purchaseToken;
			pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
			pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();


			pQuery->SetTransaction( Sender );

			dbCheck( RequestQuery( pQuery ) );

			return hr;
		}

		Result GameDB::CheckPurchaseID(TransactionID Sender, uint shardID, const Array<uint8_t>& purchaseID)
		{
			FunctionContext hr;

			UniquePtr<QueryCheckPurchaseIDCmd> pQuery(new(GetHeap()) QueryCheckPurchaseIDCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);

			if (purchaseID.size() > sizeof(pQuery->PurchaseID))
				dbError(ResultCode::INVALID_ARG);

			pQuery->PurchaseID = purchaseID;

			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			return hr;
		}

		Result GameDB::SetNickName(TransactionID Sender, uint shardID, PlayerID playerID, const char* nickName)
		{
			FunctionContext hr;

			UniquePtr<QuerySetNickNameCmd> pQuery(new(GetHeap()) QuerySetNickNameCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->PlayerID = playerID;
			StrUtil::StringCopy(pQuery->NickName, nickName);

			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			return hr;
		}


		Result GameDB::GetNickName(TransactionID Sender, uint shardID, PlayerID playerID)
		{
			FunctionContext hr;

			UniquePtr<QueryGetNickNameCmd> pQuery(new(GetHeap()) QueryGetNickNameCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->PlayerID = playerID;
			pQuery->NickName[0] = '\0';
			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			return hr;
		}


		//// Save player info
		//Result GameDB::UpdateGameEndCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
		//																		int16_t	Level,
		//																		int64_t	Exp,
		//																		int64_t	GameMoney,
		//																		int32_t	TotalPlayed,
		//																		int32_t	WinPlaySC, int32_t WinPlaySM, int32_t WinPlaySS,
		//																		int32_t	LosePlaySC, int32_t LosePlaySM, int32_t LosePlaySS,
		//																		int32_t	WinPlayNC, int32_t WinPlayNM, int32_t WinPlayNS,
		//																		int32_t	LosePlayNC, int32_t LosePlayNM, int32_t LosePlayNS,
		//																		UTCTimeStampSec	LatestActiveTime
		//																		)
		//{
		//	Result hr = ResultCode::SUCCESS;
		//	QueryUpdateGameEndCmd *pQuery = nullptr;

		//	dbMem( pQuery = new(GetHeap()) QueryUpdateGameEndCmd(GetHeap()));

		//	pQuery->SetPartitioningKey(shardID);

		//	pQuery->PlayerID = playerID;
		//	pQuery->Result = 0;

		//	pQuery->Level = Level;
		//	pQuery->Exp = Exp;
		//	pQuery->GameMoney = GameMoney;
		//	pQuery->TotalPlayed = TotalPlayed;
		//	pQuery->WinPlaySC = WinPlaySC;
		//	pQuery->WinPlaySM = WinPlaySM;
		//	pQuery->WinPlaySS = WinPlaySS;
		//	pQuery->LosePlaySC = LosePlaySC;
		//	pQuery->LosePlaySM = LosePlaySM;
		//	pQuery->LosePlaySS = LosePlaySS;
		//	pQuery->WinPlayNC = WinPlayNC;
		//	pQuery->WinPlayNM = WinPlayNM;
		//	pQuery->WinPlayNS = WinPlayNS;
		//	pQuery->LosePlayNC = LosePlayNC;
		//	pQuery->LosePlayNM = LosePlayNM;
		//	pQuery->LosePlayNS = LosePlayNS;
		//	pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();

		//	pQuery->SetTransaction( Sender );

		//	dbChk( RequestQuery( pQuery ) );
		//	pQuery = nullptr;

		//Proc_End:

		//	IHeap::Delete(pQuery);

		//	return hr;
		//}


		//// Save player info
		//Result GameDB::UpdateJoinGameCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
		//																		int64_t	Gem,
		//																		int16_t	Stamina,
		//																		int16_t	PlayerState,
		//																		UTCTimeStampSec	LatestActiveTime,
		//																		UTCTimeStampSec	LatestTickTime
		//																		)
		//{
		//	Result hr = ResultCode::SUCCESS;
		//	QueryUpdateJoinGameCmd *pQuery = nullptr;

		//	dbMem( pQuery = new(GetHeap()) QueryUpdateJoinGameCmd(GetHeap()));

		//	pQuery->SetPartitioningKey(shardID);

		//	pQuery->PlayerID = playerID;
		//	pQuery->Result = 0;

		//	pQuery->Gem = Gem;
		//	pQuery->Stamina = Stamina;
		//	pQuery->PlayerState = PlayerState;
		//	pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
		//	pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();


		//	pQuery->SetTransaction( Sender );

		//	dbChk( RequestQuery( pQuery ) );
		//	pQuery = nullptr;

		//Proc_End:

		//	IHeap::Delete(pQuery);

		//	return hr;
		//}


		//// Save player info
		//Result GameDB::UpdateTickStatusCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
		//																		int64_t	Gem,
		//																		int16_t	Stamina,
		//																		int16_t	PlayerState,
		//																		UTCTimeStampSec	LatestActiveTime,
		//																		UTCTimeStampSec	LatestTickTime
		//																		)
		//{
		//	Result hr = ResultCode::SUCCESS;
		//	QueryUpdateTickStatusCmd *pQuery = nullptr;

		//	dbMem( pQuery = new(GetHeap()) QueryUpdateTickStatusCmd(GetHeap()));

		//	pQuery->SetPartitioningKey(shardID);

		//	pQuery->PlayerID = playerID;
		//	pQuery->Result = 0;

		//	pQuery->Gem = Gem;
		//	pQuery->Stamina = Stamina;
		//	pQuery->PlayerState = PlayerState;
		//	pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
		//	pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();

		//	pQuery->SetTransaction( Sender );

		//	dbChk( RequestQuery( pQuery ) );
		//	pQuery = nullptr;

		//Proc_End:

		//	IHeap::Delete(pQuery);

		//	return hr;
		//}


		// Save player info
		Result GameDB::GetPlayerStatusCmd(TransactionID Sender, uint shardID, const PlayerID& playerID)
		{
			UniquePtr<QueryGetPlayerStatusCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryGetPlayerStatusCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->PlayerID = playerID;
			pQuery->LatestActiveTime = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}



		// Save player info
		Result GameDB::GetPlayerQuickInfoCmd(TransactionID Sender, uint shardID, PlayerID playerID)
		{
			UniquePtr<QueryGetPlayerQuickInfoCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryGetPlayerQuickInfoCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->UserID = playerID;
			pQuery->Level = 0;
			pQuery->WeeklyWin = 0;
			pQuery->WeeklyLose = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}



		// Save player info
		Result GameDB::GetFriendQuickInfoCmd(TransactionID Sender, uint shardID, PlayerID playerID)
		{
			UniquePtr<QueryGetFriendQuickInfoCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryGetFriendQuickInfoCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->PlayerID = playerID;
			pQuery->Level = 0;
			pQuery->WeeklyWin = 0;
			pQuery->WeeklyLose = 0;
			pQuery->PlayerState = 0;
			pQuery->LatestActiveTime = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}


		// Save player info
		Result GameDB::GetFriendQuickInfoWithNickCmd(TransactionID Sender, uint shardID, PlayerID playerID)
		{
			UniquePtr<QueryGetFriendQuickInfoWithNickCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryGetFriendQuickInfoWithNickCmd(GetHeap()));
			dbCheckMem(pQuery);

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

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}



		// Save player info
		Result GameDB::GetFriendSlotStatus(TransactionID Sender, uint shardID, PlayerID playerID)
		{
			UniquePtr<QueryGetFriendSlotStatusCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryGetFriendSlotStatusCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->PlayerID = playerID;
			pQuery->Level = 0;
			pQuery->AddedFriendSlot = 0;
			pQuery->NumFriends = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}

		Result GameDB::AddFriend(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID, uint friendShardID, FacebookUID FriendFacebookUID)
		{
			UniquePtr<QueryAddFriendCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryAddFriendCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->UserID = accountID;
			pQuery->FriendUID = FriendUID;
			pQuery->FriendShardID = friendShardID;
			pQuery->FriendFacebookUID = FriendFacebookUID;
			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result GameDB::RemoveFriend(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID)
		{
			UniquePtr<QueryRemoveFriendCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryRemoveFriendCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->UserID = accountID;
			pQuery->FriendUID = FriendUID;
			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result GameDB::GetFriendList(TransactionID Sender, uint shardID, PlayerID accountID)
		{
			UniquePtr<QueryGetFriendListCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryGetFriendListCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->UserID = accountID;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}


		// Notifications
		Result GameDB::Notification_Add(TransactionID Sender, uint shardID, PlayerID ToUserID, bool isCollapsable, NotificationType messageID, int64_t messageParam0, int64_t messageParam1, const char* messageText, UTCTimeStampSec timeStamp)
		{
			UniquePtr<QueryNotification_AddCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryNotification_AddCmd(GetHeap()));
			dbCheckMem(pQuery);

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

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result GameDB::Notification_GetList(TransactionID Sender, uint shardID, PlayerID UserID)
		{
			UniquePtr<QueryNotification_GetListCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryNotification_GetListCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->UserID = UserID;

			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result GameDB::Notification_Remove(TransactionID Sender, uint shardID, PlayerID userID, int32_t notificationID)
		{
			UniquePtr<QueryNotification_RemoveCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryNotification_RemoveCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->UserID = userID;
			pQuery->NotificationID = notificationID;


			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result GameDB::Notification_RemoveByMessageID(TransactionID Sender, uint shardID, PlayerID UserID, int16_t messageID)
		{
			UniquePtr<QueryNotification_RemoveByMessageIDCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryNotification_RemoveByMessageIDCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->UserID = UserID;
			pQuery->MessageID = messageID;

			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result GameDB::Notification_SetRead(TransactionID Sender, uint shardID, PlayerID userID, int32_t notificationID)
		{
			UniquePtr<QueryNotification_SetReadCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryNotification_SetReadCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->UserID = userID;
			pQuery->NotificationID = notificationID;


			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result GameDB::SetComplitionState(TransactionID Sender, uint shardID, PlayerID userID, const char* complitionState)
		{
			UniquePtr<QuerySetComplitionStateCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QuerySetComplitionStateCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->PlayerID = userID;
			pQuery->ComplitionState = complitionState;


			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result GameDB::GetComplitionState(TransactionID Sender, uint shardID, PlayerID userID)
		{
			UniquePtr<QueryGetComplitionStateCmd> pQuery;
			FunctionContext hr;

			pQuery.reset(new(GetHeap()) QueryGetComplitionStateCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->PlayerID = userID;
			pQuery->ComplitionState = "";


			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}



	} //namespace DB
} // namespace SF

