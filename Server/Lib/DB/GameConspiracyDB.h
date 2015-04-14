
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
#include "DB/QueryManager.h"
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

	class GameConspiracyDB : private QueryManager, public Svr::IServerComponent
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

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Game DB Interface
		//

		HRESULT CreatePlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID);

		// Query player info
		HRESULT GetPlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID);

		// Save player info
		HRESULT SetPlayerInfoCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
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
																				INT32	LatestActiveTime,
																				INT64	LatestTickTime
																				);

		// Nick name
		HRESULT SetNickName(BR::TransactionID Sender, UINT shardID, PlayerID playerID, const char* nickName);
		HRESULT GetNickName(BR::TransactionID Sender, UINT shardID, PlayerID playerID);

		// Save player info
		HRESULT UpdateGameEndCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				SHORT	Level,
																				INT64	Exp,
																				INT64	GameMoney,
																				INT32	TotalPlayed,
																				INT32	WinPlaySC, INT32 WinPlaySM, INT32 WinPlaySS,
																				INT32	LosePlaySC, INT32 LosePlaySM, INT32 LosePlaySS,
																				INT32	WinPlayNC, INT32 WinPlayNM, INT32 WinPlayNS,
																				INT32	LosePlayNC, INT32 LosePlayNM, INT32 LosePlayNS,
																				INT32	LatestActiveTime
																				);
		
		// Save player info
		HRESULT UpdateJoinGameCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				INT64	Gem,
																				SHORT	Stamina,
																				SHORT	PlayerState,
																				INT32	LatestActiveTime,
																				INT64	LatestTickTime
																				);

		// Save player info
		HRESULT UpdateTickStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID,
																				INT64	Gem,
																				SHORT	Stamina,
																				SHORT	PlayerState,
																				INT32	LatestActiveTime,
																				INT64	LatestTickTime
																				);

		// Save player info
		HRESULT GetPlayerStatusCmd(TransactionID Sender, UINT shardID, const PlayerID &playerID);

		HRESULT GetPlayerQuickInfoCmd(BR::TransactionID Sender, UINT shardID, PlayerID playerID);

		HRESULT GetFriendQuickInfoCmd(BR::TransactionID Sender, UINT shardID, PlayerID playerID);
		HRESULT GetFriendQuickInfoWithNickCmd(BR::TransactionID Sender, UINT shardID, PlayerID playerID);

		HRESULT GetFriendSlotStatus(BR::TransactionID Sender, UINT shardID, PlayerID playerID);


		// Friend list
		HRESULT AddFriend(BR::TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID, UINT friendShardID, FacebookUID FriendFacebookUID);
		HRESULT RemoveFriend(BR::TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID);
		HRESULT GetFriendList(BR::TransactionID Sender, UINT shardID, PlayerID accountID);
		HRESULT UpdateFriendStaminaTime(BR::TransactionID Sender, UINT shardID, PlayerID accountID, PlayerID FriendUID, UINT64 timeStamp);


		// Notifications
		HRESULT Notification_Add(BR::TransactionID Sender, UINT shardID, PlayerID ToUserID, BOOL isCollapsable, NotificationType messageID, INT64 messageParam0, INT64 messageParam1, const char* messageText, INT64 timeStamp);
		HRESULT Notification_GetList(BR::TransactionID Sender, UINT shardID, PlayerID UserID);
		HRESULT Notification_Remove(BR::TransactionID Sender, UINT shardID, PlayerID userID, INT32 notificationID);
		HRESULT Notification_RemoveByMessageID(BR::TransactionID Sender, UINT shardID, PlayerID UserID, INT16 messageID);
		HRESULT Notification_SetRead(BR::TransactionID Sender, UINT shardID, PlayerID userID, INT32 notificationID);

	};

} // namespace DB
}  // namespace BR
