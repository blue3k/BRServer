
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	

#include "SFTypedefs.h"
#include "DB/DBClusterManager.h"
#include "Types/BrSvrTypes.h"
#include "Common/ArrayUtil.h"
#include "Types/BrGameTypes.h"
#include "ServerSystem/ServerComponent.h"

namespace BR {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameDB Class 
	//

	class GameConspiracyDB : private DBClusterManager, public Svr::IServerComponent
	{
	public:

		enum {
			ComponentID = Svr::ServerComponentID_GameDB,
		};

	private:

	public:
		// constructor / destructor
		GameConspiracyDB();
		virtual ~GameConspiracyDB();

		virtual void TerminateComponent() override;

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Game DB Interface
		//

		Result CreatePlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID, INT initialStamina);

		// Query player info
		Result GetPlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID);

		// Save player info
		Result SetPlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				SHORT	Level,
																				int64_t	Exp,
																				int64_t	GameMoney,
																				int64_t	Gem,
																				SHORT	Stamina,
																				SHORT	AddedFriendSlot,
																				int32_t	TotalPlayed,
																				int32_t	WinPlaySC, int32_t WinPlaySM, int32_t WinPlaySS,
																				int32_t	LosePlaySC, int32_t LosePlaySM, int32_t LosePlaySS,
																				int32_t	WinPlayNC, int32_t WinPlayNM, int32_t WinPlayNS,
																				int32_t	LosePlayNC, int32_t LosePlayNM, int32_t LosePlayNS,
			TimeStampSec	LatestActiveTime,
			TimeStampSec	LatestTickTime
																				);


		Result SavePurchaseInfoToDB(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				SHORT	Level,
																				int64_t	Exp,
																				int64_t	GameMoney,
																				int64_t	Gem,
																				SHORT	Stamina,
																				SHORT	AddedFriendSlot,
																				const Array<uint8_t>& purchaseID, 
																				const char* purchasePlatform, const char* purchaseToken,
			TimeStampSec	LatestActiveTime,
			TimeStampSec	LatestTickTime
																				);

		Result CheckPurchaseID(TransactionID Sender, UINT shardID, const Array<uint8_t>& purchaseID);

		// Nick name
		Result SetNickName(TransactionID Sender, UINT shardID, PlayerID playerID, const char* nickName);
		Result GetNickName(TransactionID Sender, UINT shardID, PlayerID playerID);

		// Save player info
		Result UpdateGameEndCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				SHORT	Level,
																				int64_t	Exp,
																				int64_t	GameMoney,
																				int32_t	TotalPlayed,
																				int32_t	WinPlaySC, int32_t WinPlaySM, int32_t WinPlaySS,
																				int32_t	LosePlaySC, int32_t LosePlaySM, int32_t LosePlaySS,
																				int32_t	WinPlayNC, int32_t WinPlayNM, int32_t WinPlayNS,
																				int32_t	LosePlayNC, int32_t LosePlayNM, int32_t LosePlayNS,
																				TimeStampSec	LatestActiveTime
																				);
		
		// Save player info
		Result UpdateJoinGameCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				int64_t	Gem,
																				SHORT	Stamina,
																				SHORT	PlayerState,
																				TimeStampSec	LatestActiveTime,
																				TimeStampSec	LatestTickTime
																				);

		// Save player info
		Result UpdateTickStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				int64_t	Gem,
																				SHORT	Stamina,
																				SHORT	PlayerState,
																				TimeStampSec	LatestActiveTime,
																				TimeStampSec	LatestTickTime
																				);

		// Save player info
		Result GetPlayerStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID);

		Result GetPlayerQuickInfoCmd(TransactionID Sender, UINT shardID, PlayerID playerID);

		Result GetFriendQuickInfoCmd(TransactionID Sender, UINT shardID, PlayerID playerID);
		Result GetFriendQuickInfoWithNickCmd(TransactionID Sender, UINT shardID, PlayerID playerID);

		Result GetFriendSlotStatus(TransactionID Sender, UINT shardID, PlayerID playerID);


		// Friend list
		Result AddFriend(TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID, UINT friendShardID, FacebookUID FriendFacebookUID);
		Result RemoveFriend(TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID);
		Result GetFriendList(TransactionID Sender, UINT shardID, PlayerID accountID);
		Result UpdateFriendStaminaTime(TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID, TimeStampSec timeStamp);


		// Notifications
		Result Notification_Add(TransactionID Sender, UINT shardID, PlayerID ToUserID, bool isCollapsable, NotificationType messageID, int64_t messageParam0, int64_t messageParam1, const char* messageText, TimeStampSec timeStamp);
		Result Notification_GetList(TransactionID Sender, UINT shardID, PlayerID UserID);
		Result Notification_Remove(TransactionID Sender, UINT shardID, PlayerID userID, int32_t notificationID);
		Result Notification_RemoveByMessageID(TransactionID Sender, UINT shardID, PlayerID UserID, int16_t messageID);
		Result Notification_SetRead(TransactionID Sender, UINT shardID, PlayerID userID, int32_t notificationID);

		Result SetComplitionState(TransactionID Sender, UINT shardID, PlayerID userID, const char* complitionState);
		Result GetComplitionState(TransactionID Sender, UINT shardID, PlayerID userID);
	};

} // namespace DB
}  // namespace BR
