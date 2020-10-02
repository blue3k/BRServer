
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

namespace SF {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameDB Class 
	//

	class GameDB : public DBClusterManager
	{
	public:

		enum {
			ComponentID = Svr::ServerComponentID_GameDB,
		};

	public:
		// constructor / destructor
		GameDB();
		virtual ~GameDB();

		Result InitializeComponent() { return ResultCode::SUCCESS; }
		virtual void TerminateComponent() ;

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Game DB Interface
		//


		virtual Result CreatePlayerInfoCmd(TransactionID Sender, uint shardID, const AccountID& playerID);

		// Query account info
		virtual Result GetPlayerInfoCmd(TransactionID Sender, uint shardID, const AccountID& playerID);

		// Save player info
		virtual Result SetPlayerInfoCmd(TransactionID Sender, uint shardID, const AccountID& playerID,
			UTCTimeStampSec	LatestActiveTime,
			UTCTimeStampSec	LatestTickTime,
			const Array<NamedVariableBox>& Variables
		) = 0;


		// Nick name
		Result SetNickName(TransactionID Sender, uint shardID, PlayerID playerID, const char* nickName);
		Result GetNickName(TransactionID Sender, uint shardID, PlayerID playerID);

		// Save player info
		Result GetPlayerStatusCmd(TransactionID Sender, uint shardID, const PlayerID &playerID);

		Result GetPlayerQuickInfoCmd(TransactionID Sender, uint shardID, PlayerID playerID);

		Result GetFriendQuickInfoCmd(TransactionID Sender, uint shardID, PlayerID playerID);
		Result GetFriendQuickInfoWithNickCmd(TransactionID Sender, uint shardID, PlayerID playerID);

		Result GetFriendSlotStatus(TransactionID Sender, uint shardID, PlayerID playerID);


		// Friend list
		Result AddFriend(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID, uint friendShardID, FacebookUID FriendFacebookUID);
		Result RemoveFriend(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID);
		Result GetFriendList(TransactionID Sender, uint shardID, PlayerID accountID);
		//Result UpdateFriendStaminaTime(TransactionID Sender, uint shardID, PlayerID accountID, PlayerID FriendUID, UTCTimeStampSec timeStamp);


		// Notifications
		Result Notification_Add(TransactionID Sender, uint shardID, PlayerID ToUserID, bool isCollapsable, NotificationType messageID, int64_t messageParam0, int64_t messageParam1, const char* messageText, UTCTimeStampSec timeStamp);
		Result Notification_GetList(TransactionID Sender, uint shardID, PlayerID UserID);
		Result Notification_Remove(TransactionID Sender, uint shardID, PlayerID userID, int32_t notificationID);
		Result Notification_RemoveByMessageID(TransactionID Sender, uint shardID, PlayerID UserID, int16_t messageID);
		Result Notification_SetRead(TransactionID Sender, uint shardID, PlayerID userID, int32_t notificationID);

		Result SetComplitionState(TransactionID Sender, uint shardID, PlayerID userID, const char* complitionState);
		Result GetComplitionState(TransactionID Sender, uint shardID, PlayerID userID);
	};

} // namespace DB
}  // namespace SF
