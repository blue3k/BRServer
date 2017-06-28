
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
	

#include "Common/Typedefs.h"
#include "DB/DBClusterManager.h"
#include "Common/BrSvrTypes.h"
#include "Common/ArrayUtil.h"
#include "Common/BrGameTypes.h"
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
																				);


		Result SavePurchaseInfoToDB(TransactionID Sender, UINT shardID, const PlayerID &playerID,
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
																				);

		Result CheckPurchaseID(TransactionID Sender, UINT shardID, const Array<BYTE>& purchaseID);

		// Nick name
		Result SetNickName(TransactionID Sender, UINT shardID, PlayerID playerID, const char* nickName);
		Result GetNickName(TransactionID Sender, UINT shardID, PlayerID playerID);

		// Save player info
		Result UpdateGameEndCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				SHORT	Level,
																				INT64	Exp,
																				INT64	GameMoney,
																				INT32	TotalPlayed,
																				INT32	WinPlaySC, INT32 WinPlaySM, INT32 WinPlaySS,
																				INT32	LosePlaySC, INT32 LosePlaySM, INT32 LosePlaySS,
																				INT32	WinPlayNC, INT32 WinPlayNM, INT32 WinPlayNS,
																				INT32	LosePlayNC, INT32 LosePlayNM, INT32 LosePlayNS,
																				TimeStampSec	LatestActiveTime
																				);
		
		// Save player info
		Result UpdateJoinGameCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				INT64	Gem,
																				SHORT	Stamina,
																				SHORT	PlayerState,
																				TimeStampSec	LatestActiveTime,
																				TimeStampSec	LatestTickTime
																				);

		// Save player info
		Result UpdateTickStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				INT64	Gem,
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
		Result Notification_Add(TransactionID Sender, UINT shardID, PlayerID ToUserID, bool isCollapsable, NotificationType messageID, INT64 messageParam0, INT64 messageParam1, const char* messageText, TimeStampSec timeStamp);
		Result Notification_GetList(TransactionID Sender, UINT shardID, PlayerID UserID);
		Result Notification_Remove(TransactionID Sender, UINT shardID, PlayerID userID, INT32 notificationID);
		Result Notification_RemoveByMessageID(TransactionID Sender, UINT shardID, PlayerID UserID, INT16 messageID);
		Result Notification_SetRead(TransactionID Sender, UINT shardID, PlayerID userID, INT32 notificationID);

		Result SetComplitionState(TransactionID Sender, UINT shardID, PlayerID userID, const char* complitionState);
		Result GetComplitionState(TransactionID Sender, UINT shardID, PlayerID userID);
	};

} // namespace DB
}  // namespace BR
