
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "GameConspiracyDBPch.h"
#include "SFTypedefs.h"

#include "GameConspiracyDB.h"
#include "GameConspiracyQuery.h"



namespace SF {
namespace conspiracy {

	
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


	
	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Game DB Interface
	//

	Result GameConspiracyDB::CreateAccountInfoCmd(TransactionID Sender, uint shardID, const PlayerID& playerID, INT initialStamina)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCreateAccountInfoCmd* pQuery = nullptr;
		QueryGetPlayerInfoData* pRawSet = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryCreateAccountInfoCmd(GetHeap()));

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->InitialStamina = initialStamina;
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

	Result GameConspiracyDB::GetPlayerInfoCmd(TransactionID Sender, uint shardID, const PlayerID& playerID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetPlayerInfoCmd* pQuery = nullptr;
		QueryGetPlayerInfoData* pRawSet = nullptr;

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
	Result GameConspiracyDB::SetPlayerInfoCmd(TransactionID Sender, uint shardID, const PlayerID& playerID,
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
		UTCTimeStampSec	LatestActiveTime,
		UTCTimeStampSec	LatestTickTime
	)
	{
		Result hr = ResultCode::SUCCESS;
		QuerySetPlayerInfoCmd* pQuery = nullptr;
		QuerySetPlayerInfoData* pDataSet = nullptr;

		dbMem(pQuery = new(GetHeap()) QuerySetPlayerInfoCmd(GetHeap()));

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


		pQuery->SetTransaction(Sender);

		dbChk(RequestQuery(pQuery));
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::SavePurchaseInfoToDB(
		TransactionID Sender, uint shardID, const PlayerID& playerID,
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
		Result hr = ResultCode::SUCCESS;
		QuerySavePurchaseInfoToDBCmd* pQuery = nullptr;

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


		pQuery->SetTransaction(Sender);

		dbChk(RequestQuery(pQuery));
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result GameConspiracyDB::CheckPurchaseID(TransactionID Sender, uint shardID, const Array<uint8_t>& purchaseID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCheckPurchaseIDCmd* pQuery = nullptr;

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
																			UTCTimeStampSec	LatestActiveTime
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
																			UTCTimeStampSec	LatestActiveTime,
																			UTCTimeStampSec	LatestTickTime
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
																			UTCTimeStampSec	LatestActiveTime,
																			UTCTimeStampSec	LatestTickTime
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

	
	Result GameConspiracyDB::UpdateFriendStaminaTime(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID, UTCTimeStampSec timeStamp)
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



} //namespace conspiracy
} // namespace SF

