
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	

#include "SFTypedefs.h"
#include "DB/DBClusterManager.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFArray.h"
#include "Types/BrGameTypes.h"
#include "Component/ServerComponent.h"
#include "Game/GameDB.h"

namespace SF {
namespace conspiracy {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameDB Class 
	//

	class GameConspiracyDB : public DB::GameDB
	{
	public:

	public:
		// constructor / destructor
		GameConspiracyDB();
		virtual ~GameConspiracyDB();


	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Game DB Interface
		//



		Result SavePurchaseInfoToDB(TransactionID Sender, uint shardID, const PlayerID& playerID,
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
		);

		Result CheckPurchaseID(TransactionID Sender, uint shardID, const Array<uint8_t>& purchaseID);

		// Save player info
		Result UpdateGameEndCmd(TransactionID Sender, uint shardID, 
			const PlayerID &playerID,
			int16_t	Level,
			int64_t	Exp,
			int64_t	GameMoney,
			int32_t	TotalPlayed,
			int32_t	WinPlaySC, int32_t WinPlaySM, int32_t WinPlaySS,
			int32_t	LosePlaySC, int32_t LosePlaySM, int32_t LosePlaySS,
			int32_t	WinPlayNC, int32_t WinPlayNM, int32_t WinPlayNS,
			int32_t	LosePlayNC, int32_t LosePlayNM, int32_t LosePlayNS,
			UTCTimeStampSec	LatestActiveTime
			);
		
		// Save player info
		Result UpdateJoinGameCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
																				int64_t	Gem,
																				int16_t	Stamina,
																				int16_t	PlayerState,
			UTCTimeStampSec	LatestActiveTime,
																				UTCTimeStampSec	LatestTickTime
																				);

		// Save player info
		Result UpdateTickStatusCmd(TransactionID Sender, uint shardID, const PlayerID &playerID,
			int64_t	Gem,
			int16_t	Stamina,
			int16_t	PlayerState,
			UTCTimeStampSec	LatestActiveTime,
			UTCTimeStampSec	LatestTickTime
			);

		Result UpdateFriendStaminaTime(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID, UTCTimeStampSec timeStamp);
	};

} // namespace DB
}  // namespace SF
