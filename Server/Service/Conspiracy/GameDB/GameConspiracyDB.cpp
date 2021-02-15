
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

		// Save player info
		Result GameConspiracyDB::UpdateGameEndCmd(TransactionID Sender, uint shardID, const PlayerID& playerID,
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
			ScopeContext hr;

			SFUniquePtr<QueryUpdateGameEndCmd> pQuery(new(GetHeap()) QueryUpdateGameEndCmd(GetHeap()));
			dbCheckMem(pQuery);

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

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}


		// Save player info
		Result GameConspiracyDB::UpdateJoinGameCmd(TransactionID Sender, uint shardID, const PlayerID& playerID,
			int64_t	Gem,
			int16_t	Stamina,
			int16_t	PlayerState,
			UTCTimeStampSec	LatestActiveTime,
			UTCTimeStampSec	LatestTickTime
		)
		{
			ScopeContext hr;

			SFUniquePtr<QueryUpdateJoinGameCmd> pQuery(new(GetHeap()) QueryUpdateJoinGameCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->PlayerID = playerID;
			pQuery->Result = 0;

			pQuery->Gem = Gem;
			pQuery->Stamina = Stamina;
			pQuery->PlayerState = PlayerState;
			pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
			pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();


			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}


		// Save player info
		Result GameConspiracyDB::UpdateTickStatusCmd(TransactionID Sender, uint shardID, const PlayerID& playerID,
			int64_t	Gem,
			int16_t	Stamina,
			int16_t	PlayerState,
			UTCTimeStampSec	LatestActiveTime,
			UTCTimeStampSec	LatestTickTime
		)
		{
			ScopeContext hr;

			SFUniquePtr<QueryUpdateTickStatusCmd> pQuery(new(GetHeap()) QueryUpdateTickStatusCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->PlayerID = playerID;
			pQuery->Result = 0;

			pQuery->Gem = Gem;
			pQuery->Stamina = Stamina;
			pQuery->PlayerState = PlayerState;
			pQuery->LatestActiveTime = LatestActiveTime.time_since_epoch().count();
			pQuery->LatestTickTime = LatestTickTime.time_since_epoch().count();

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}


		Result GameConspiracyDB::UpdateFriendStaminaTime(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID, UTCTimeStampSec timeStamp)
		{
			ScopeContext hr;

			SFUniquePtr<QueryUpdateFriendStaminaTimeCmd> pQuery(new(GetHeap()) QueryUpdateFriendStaminaTimeCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetPartitioningKey(shardID);

			pQuery->SetTransaction(Sender);
			pQuery->UserID = accountID;
			pQuery->FriendUID = FriendUID;
			pQuery->TimeStamp = timeStamp.time_since_epoch().count();
			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			return hr;
		}



	} //namespace conspiracy
} // namespace SF

