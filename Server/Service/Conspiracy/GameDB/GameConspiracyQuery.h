
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Game DB query data
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "DB/Query.h"
#include "DB/DBConst.h"
#include "DB/QueryConst.h"
#include "Game/GameQuery.h"

namespace SF {
namespace conspiracy {

	
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game DB Query Message Code
	//

	enum 
	{
		MCODE_QueryConspiracyNull,
		// Game
		MCODE_QueryCreatePlayerInfo,
		MCODE_QueryGetPlayerInfo,
		MCODE_QuerySetPlayerInfo,
		MCODE_QuerySavePurchaseInfoToDB,
		MCODE_QueryCheckPurchaseID,
		MCODE_QueryGetNickName,
		MCODE_QuerySetNickName,
		MCODE_QueryUpdateGameEnd,
		MCODE_QueryUpdateJoinGame,
		MCODE_QueryUpdateTickStatus,
		MCODE_QueryGetPlayerStatus,
		MCODE_QueryGetPlayerQuickInfo,
		MCODE_QueryGetFriendQuickInfo,
		MCODE_QueryGetFriendQuickInfoWithNick,
		MCODE_QueryGetFriendSlotStatus,
		MCODE_QueryAddFriend,
		MCODE_QueryRemoveFriend,
		MCODE_QueryGetFriendList,
		MCODE_QueryUpdateFriendStaminaTime,
		MCODE_QueryNotification_Add,
		MCODE_QueryNotification_GetList,
		MCODE_QueryNotification_Remove,
		MCODE_QueryNotification_RemoveByMessageID,
		MCODE_QueryNotification_SetRead,
		MCODE_QuerySetComplitionState,
		MCODE_QueryGetComplitionState,

	}; // enum MsgCode



	struct PlayerGameScore
	{
		int32_t	TotalPlayed;
		int32_t	WinPlaySC;
		int32_t	WinPlaySM;
		int32_t	WinPlaySS;
		int32_t	LosePlaySC;
		int32_t	LosePlaySM;
		int32_t	LosePlaySS;
		int32_t	WinPlayNC;
		int32_t	WinPlayNM;
		int32_t	WinPlayNS;
		int32_t	LosePlayNC;
		int32_t	LosePlayNM;
		int32_t	LosePlayNS;
	};


	class QuerySavePurchaseInfoToDB : public DB::QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;
		int16_t	Level;
		int64_t	Exp;
		int64_t	GameMoney;
		int64_t	Gem;
		int16_t	Stamina;
		int16_t	AddedFriendSlot;
		StaticArray<uint8_t,256>    PurchaseID;
		String  PurchasePlatform;
		String  PurchaseToken;
		int32_t	LatestActiveTime;
		int64_t	LatestTickTime;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QuerySavePurchaseInfoToDB, "spSavePurchaseInfoToDB")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Level)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Exp)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, GameMoney)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Gem)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Stamina)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, AddedFriendSlot)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PurchaseID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PurchasePlatform)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PurchaseToken)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestTickTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QuerySavePurchaseInfoToDB);


	class QueryCheckPurchaseID : public DB::QueryBase
	{
	public:
		// Player ID
		DynamicArray<uint8_t>    PurchaseID;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryCheckPurchaseID, "spCheckPurchaseID")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PurchaseID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryCheckPurchaseID);





	class QueryUpdateGameEnd : public DB::QueryBase, public PlayerGameScore
	{
	public:
		// Player ID
		int64_t	PlayerID;
		int16_t	Level;
		int64_t	Exp;
		int64_t	GameMoney;
		int32_t	LatestActiveTime;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateGameEnd, "spUpdateGameEnd")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Level)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Exp)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, GameMoney)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, TotalPlayed)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, WinPlaySC)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, WinPlaySM)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, WinPlaySS)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LosePlaySC)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LosePlaySM)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LosePlaySS)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, WinPlayNC)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, WinPlayNM)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, WinPlayNS)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LosePlayNC)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LosePlayNM)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LosePlayNS)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QueryUpdateGameEnd);



	class QueryUpdateJoinGame : public DB::QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;
		int64_t	Gem;
		int16_t	Stamina;
		int16_t	PlayerState;
		int32_t	LatestActiveTime;
		int64_t	LatestTickTime;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateJoinGame, "spUpdateJoinGame")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Gem)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Stamina)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerState)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestTickTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QueryUpdateJoinGame);



	class QueryUpdateTickStatus : public DB::QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;
		int64_t	Gem;
		int16_t	Stamina;
		int16_t	PlayerState;
		int32_t	LatestActiveTime;
		int64_t	LatestTickTime;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateTickStatus, "spUpdateTickStatus")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Gem)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Stamina)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerState)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestTickTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QueryUpdateTickStatus);
	


	class QueryUpdateFriendStaminaTime : public DB::QueryBase
	{
	public:

		int64_t UserID;
		int64_t FriendUID;
		int64_t TimeStamp;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateFriendStaminaTime, "spFriend_UpdateStaminaTime")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, FriendUID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, TimeStamp)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryUpdateFriendStaminaTime);



} // namespace DB
}  // namespace SF
