
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


namespace SF {
namespace DB {

	
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game DB Query Message Code
	//

	enum 
	{
		MCODE_QueryConspiracyNull,
		// Game
		MCODE_QueryGetPlayerScoreList,
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

	struct QueryGetPlayerInfoData : public PlayerGameScore
	{
		String  GameNick;
		uint8_t	Grade;
		int16_t	Level;
		int64_t	Exp;
		int64_t	GameMoney;
		int64_t	Gem;
		int16_t	Stamina;
		int16_t	AddedFriendSlot;
		int32_t	WeeklyPlayWin;
		int32_t	WeeklyPlayLose;
		int64_t	LatestTickTime;
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	// QueryGetPlayerScore
	//

	struct QueryGetPlayerScoreRawSet
	{
		int32_t m_PlayerID;
		String m_UName;
		int16_t m_Gender;
	};

	class QueryGetPlayerScoreList : public QueryGetPlayerScoreRawSet,  public QueryBase
	{
	public:
		int64_t m_AccountID;
		int32_t m_Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerScoreList, "spGetScoreList")
			BRDB_PARAM_ENTRY(ParamIO::Input, m_AccountID)
			BRDB_PARAM_ENTRY(ParamIO::Output, m_Result)
		BRDB_END_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryGetPlayerScoreList)
			BRDB_COLUMN_ENTRY(m_PlayerID)
			BRDB_COLUMN_ENTRY(m_UName)
			BRDB_COLUMN_ENTRY(m_Gender)
		BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryGetPlayerScoreList, QueryGetPlayerScoreRawSet);


	class QueryCreatePlayerInfo : public QueryGetPlayerInfoData, public QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;
		INT		InitialStamina;
		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryCreatePlayerInfo, "spCreatePlayerInfo")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, InitialStamina)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()


		BRDB_BEGIN_RESULT_MAP(QueryGetPlayerInfoData)
			BRDB_COLUMN_ENTRY(GameNick)
			BRDB_COLUMN_ENTRY(Grade)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(Exp)
			BRDB_COLUMN_ENTRY(GameMoney)
			BRDB_COLUMN_ENTRY(Gem)
			BRDB_COLUMN_ENTRY(Stamina)
			BRDB_COLUMN_ENTRY(AddedFriendSlot)
			BRDB_COLUMN_ENTRY(TotalPlayed)
			BRDB_COLUMN_ENTRY(WinPlaySC)
			BRDB_COLUMN_ENTRY(WinPlaySM)
			BRDB_COLUMN_ENTRY(WinPlaySS)
			BRDB_COLUMN_ENTRY(LosePlaySC)
			BRDB_COLUMN_ENTRY(LosePlaySM)
			BRDB_COLUMN_ENTRY(LosePlaySS)
			BRDB_COLUMN_ENTRY(WinPlayNC)
			BRDB_COLUMN_ENTRY(WinPlayNM)
			BRDB_COLUMN_ENTRY(WinPlayNS)
			BRDB_COLUMN_ENTRY(LosePlayNC)
			BRDB_COLUMN_ENTRY(LosePlayNM)
			BRDB_COLUMN_ENTRY(LosePlayNS)
			BRDB_COLUMN_ENTRY(WeeklyPlayWin)
			BRDB_COLUMN_ENTRY(WeeklyPlayLose)
			BRDB_COLUMN_ENTRY(LatestTickTime)
		BRDB_END_RESULT_MAP()
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryCreatePlayerInfo, QueryGetPlayerInfoData);


	class QueryGetPlayerInfo : public QueryGetPlayerInfoData, public QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerInfo, "spGetPlayerInfo")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()


		BRDB_BEGIN_RESULT_MAP(QueryGetPlayerInfoData)
			BRDB_COLUMN_ENTRY(GameNick)
			BRDB_COLUMN_ENTRY(Grade)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(Exp)
			BRDB_COLUMN_ENTRY(GameMoney)
			BRDB_COLUMN_ENTRY(Gem)
			BRDB_COLUMN_ENTRY(Stamina)
			BRDB_COLUMN_ENTRY(AddedFriendSlot)
			BRDB_COLUMN_ENTRY(TotalPlayed)
			BRDB_COLUMN_ENTRY(WinPlaySC)
			BRDB_COLUMN_ENTRY(WinPlaySM)
			BRDB_COLUMN_ENTRY(WinPlaySS)
			BRDB_COLUMN_ENTRY(LosePlaySC)
			BRDB_COLUMN_ENTRY(LosePlaySM)
			BRDB_COLUMN_ENTRY(LosePlaySS)
			BRDB_COLUMN_ENTRY(WinPlayNC)
			BRDB_COLUMN_ENTRY(WinPlayNM)
			BRDB_COLUMN_ENTRY(WinPlayNS)
			BRDB_COLUMN_ENTRY(LosePlayNC)
			BRDB_COLUMN_ENTRY(LosePlayNM)
			BRDB_COLUMN_ENTRY(LosePlayNS)
			BRDB_COLUMN_ENTRY(WeeklyPlayWin)
			BRDB_COLUMN_ENTRY(WeeklyPlayLose)
			BRDB_COLUMN_ENTRY(LatestTickTime)
		BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryGetPlayerInfo, QueryGetPlayerInfoData);


	
	struct QuerySetPlayerInfoData : public PlayerGameScore
	{
		int16_t	Level;
		int64_t	Exp;
		int64_t	GameMoney;
		int64_t	Gem;
		int16_t	Stamina;
		int16_t	AddedFriendSlot;
		int32_t	LatestActiveTime;
		int64_t	LatestTickTime;
	};

	class QuerySetPlayerInfo : public QuerySetPlayerInfoData, public QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QuerySetPlayerInfo, "spSetPlayerInfo")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, Level)
			BRDB_PARAM_ENTRY(ParamIO::Input, Exp)
			BRDB_PARAM_ENTRY(ParamIO::Input, GameMoney)
			BRDB_PARAM_ENTRY(ParamIO::Input, Gem)
			BRDB_PARAM_ENTRY(ParamIO::Input, Stamina)
			BRDB_PARAM_ENTRY(ParamIO::Input, AddedFriendSlot)
			BRDB_PARAM_ENTRY(ParamIO::Input, TotalPlayed)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlaySC)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlaySM)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlaySS)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlaySC)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlaySM)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlaySS)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlayNC)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlayNM)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlayNS)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlayNC)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlayNM)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlayNS)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestTickTime)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QuerySetPlayerInfo);



	class QuerySavePurchaseInfoToDB : public QueryBase
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
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, Level)
			BRDB_PARAM_ENTRY(ParamIO::Input, Exp)
			BRDB_PARAM_ENTRY(ParamIO::Input, GameMoney)
			BRDB_PARAM_ENTRY(ParamIO::Input, Gem)
			BRDB_PARAM_ENTRY(ParamIO::Input, Stamina)
			BRDB_PARAM_ENTRY(ParamIO::Input, AddedFriendSlot)
			BRDB_PARAM_ENTRY(ParamIO::Input, PurchaseID)
			BRDB_PARAM_ENTRY(ParamIO::Input, PurchasePlatform)
			BRDB_PARAM_ENTRY(ParamIO::Input, PurchaseToken)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestTickTime)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QuerySavePurchaseInfoToDB);


	class QueryCheckPurchaseID : public QueryBase
	{
	public:
		// Player ID
		DynamicArray<uint8_t>    PurchaseID;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryCheckPurchaseID, "spCheckPurchaseID")
			BRDB_PARAM_ENTRY(ParamIO::Input, PurchaseID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryCheckPurchaseID);





	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QuerySetNickName class
	//

	class QuerySetNickName : public QueryBase
	{
	public:

		int64_t PlayerID;

		char NickName[Const::MAX_USERNAME];
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QuerySetNickName, "spSetNickName")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, NickName)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QuerySetNickName);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryGetNickName class
	//

	class QueryGetNickName : public QueryBase
	{
	public:

		int64_t PlayerID;

		char NickName[Const::MAX_USERNAME];
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetNickName, "spGetNickName")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, NickName)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryGetNickName);



	class QueryUpdateGameEnd : public QueryBase, public PlayerGameScore
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
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, Level)
			BRDB_PARAM_ENTRY(ParamIO::Input, Exp)
			BRDB_PARAM_ENTRY(ParamIO::Input, GameMoney)
			BRDB_PARAM_ENTRY(ParamIO::Input, TotalPlayed)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlaySC)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlaySM)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlaySS)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlaySC)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlaySM)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlaySS)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlayNC)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlayNM)
			BRDB_PARAM_ENTRY(ParamIO::Input, WinPlayNS)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlayNC)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlayNM)
			BRDB_PARAM_ENTRY(ParamIO::Input, LosePlayNS)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QueryUpdateGameEnd);



	class QueryUpdateJoinGame : public QueryBase
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
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, Gem)
			BRDB_PARAM_ENTRY(ParamIO::Input, Stamina)
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerState)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestTickTime)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QueryUpdateJoinGame);



	class QueryUpdateTickStatus : public QueryBase
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
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, Gem)
			BRDB_PARAM_ENTRY(ParamIO::Input, Stamina)
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerState)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(ParamIO::Input, LatestTickTime)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QueryUpdateTickStatus);
	


	class QueryGetPlayerStatus : public QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;
		int16_t	PlayerState;
		int32_t	LatestActiveTime;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerStatus, "spGetPlayerStatus")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, PlayerState)
			BRDB_PARAM_ENTRY(ParamIO::Output, LatestActiveTime)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QueryGetPlayerStatus);




	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryGetPlayerQuickInfo class
	//


	class QueryGetPlayerQuickInfo : public QueryBase
	{
	public:

		int64_t UserID;
		int32_t Level;
		int32_t WeeklyWin;
		int32_t WeeklyLose;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerQuickInfo, "spGetPlayerQuickInfo")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Level)
			BRDB_PARAM_ENTRY(ParamIO::Output, WeeklyWin)
			BRDB_PARAM_ENTRY(ParamIO::Output, WeeklyLose)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB,QueryGetPlayerQuickInfo);


	class QueryGetFriendQuickInfo : public QueryBase
	{
	public:

		int64_t PlayerID;
		int32_t Level;
		int32_t WeeklyWin;
		int32_t WeeklyLose;
		int16_t	PlayerState;
		int32_t	LatestActiveTime;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetFriendQuickInfo, "spGetFriendQuickInfo")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Level)
			BRDB_PARAM_ENTRY(ParamIO::Output, WeeklyWin)
			BRDB_PARAM_ENTRY(ParamIO::Output, WeeklyLose)
			BRDB_PARAM_ENTRY(ParamIO::Output, PlayerState)
			BRDB_PARAM_ENTRY(ParamIO::Output, LatestActiveTime)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
			BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryGetFriendQuickInfo);



	class QueryGetFriendQuickInfoWithNick : public QueryBase
	{
	public:

		int64_t PlayerID;
		String GameNick;
		int32_t Level;
		int32_t WeeklyWin;
		int32_t WeeklyLose;
		int16_t	PlayerState;
		int32_t	LatestActiveTime;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetFriendQuickInfoWithNick, "spGetFriendQuickInfoWithNick")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, GameNick)
			BRDB_PARAM_ENTRY(ParamIO::Output, Level)
			BRDB_PARAM_ENTRY(ParamIO::Output, WeeklyWin)
			BRDB_PARAM_ENTRY(ParamIO::Output, WeeklyLose)
			BRDB_PARAM_ENTRY(ParamIO::Output, PlayerState)
			BRDB_PARAM_ENTRY(ParamIO::Output, LatestActiveTime)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
			BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryGetFriendQuickInfoWithNick);



	class QueryGetFriendSlotStatus : public QueryBase
	{
	public:

		int64_t PlayerID;

		int32_t Level;
		int32_t AddedFriendSlot;
		int32_t NumFriends;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetFriendSlotStatus, "spGetFriendSlotStatus")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Level)
			BRDB_PARAM_ENTRY(ParamIO::Output, AddedFriendSlot)
			BRDB_PARAM_ENTRY(ParamIO::Output, NumFriends)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryGetFriendSlotStatus);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryAddFriend class
	//

	class QueryAddFriend : public QueryBase
	{
	public:

		int64_t UserID;

		int64_t FriendUID;
		int64_t FriendFacebookUID;
		int32_t FriendShardID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryAddFriend, "spFriend_Add")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Input, FriendUID)
			BRDB_PARAM_ENTRY(ParamIO::Input, FriendShardID)
			BRDB_PARAM_ENTRY(ParamIO::Input, FriendFacebookUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryAddFriend);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryRemoveFriend class
	//

	class QueryRemoveFriend : public QueryBase
	{
	public:

		int64_t UserID;

		int64_t FriendUID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryRemoveFriend, "spFriend_Remove")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Input, FriendUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryRemoveFriend);


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryGetFriendList class
	//

	struct QueryGetFriendListSet
	{
		int64_t FriendUID;
		int32_t FriendShardID;
		int64_t FriendFacebookUID;
		int64_t FriendStaminaTime;
	};

	class QueryGetFriendList : public QueryGetFriendListSet, public QueryBase
	{
	public:
		int64_t UserID;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetFriendList, "spFriend_List")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
		BRDB_END_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryGetFriendList)
			BRDB_COLUMN_ENTRY(FriendUID)
			BRDB_COLUMN_ENTRY(FriendShardID)
			BRDB_COLUMN_ENTRY(FriendFacebookUID)
			BRDB_COLUMN_ENTRY(FriendStaminaTime)
		BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryGetFriendList, QueryGetFriendListSet);



	class QueryUpdateFriendStaminaTime : public QueryBase
	{
	public:

		int64_t UserID;
		int64_t FriendUID;
		int64_t TimeStamp;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateFriendStaminaTime, "spFriend_UpdateStaminaTime")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Input, FriendUID)
			BRDB_PARAM_ENTRY(ParamIO::Input, TimeStamp)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryUpdateFriendStaminaTime);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Notification classes
	//

	class QueryNotification_Add : public QueryBase
	{
	public:

		int64_t UserID;
		int8_t Collapsable;
		uint16_t MessageID;
		int64_t MessageParam0;
		int64_t MessageParam1;
		String MessageText;
		int64_t TimeStamp;

		int32_t NotificationID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_Add, "spNotification_Add")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Input, Collapsable)
			BRDB_PARAM_ENTRY(ParamIO::Input, MessageID)
			BRDB_PARAM_ENTRY(ParamIO::Input, MessageParam0)
			BRDB_PARAM_ENTRY(ParamIO::Input, MessageParam1)
			BRDB_PARAM_ENTRY(ParamIO::Input, MessageText)
			BRDB_PARAM_ENTRY(ParamIO::Input, TimeStamp)
			BRDB_PARAM_ENTRY(ParamIO::Output, NotificationID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_Add);


	struct QueryNotification_GetListSet
	{
		int32_t NotificationID;
		int16_t MessageID;
		int64_t MessageParam0;
		int64_t MessageParam1;
		String MessageText;
		int64_t TimeStamp;
		uint8_t IsRead;
	};

	class QueryNotification_GetList : public QueryNotification_GetListSet, public QueryBase
	{
	public:
		int64_t UserID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_GetList, "spNotification_GetList")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryNotification_GetList)
			BRDB_COLUMN_ENTRY(NotificationID)
			BRDB_COLUMN_ENTRY(MessageID)
			BRDB_COLUMN_ENTRY(MessageParam0)
			BRDB_COLUMN_ENTRY(MessageParam1)
			BRDB_COLUMN_ENTRY(MessageText)
			BRDB_COLUMN_ENTRY(IsRead)
			BRDB_COLUMN_ENTRY(TimeStamp)
		BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_GetList, QueryNotification_GetListSet);



	class QueryNotification_Remove : public QueryBase
	{
	public:
		int64_t UserID;
		int32_t NotificationID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_Remove, "spNotification_Remove")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Input, NotificationID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_Remove);


	class QueryNotification_RemoveByMessageID : public QueryBase
	{
	public:
		int64_t UserID;
		int16_t MessageID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_RemoveByMessageID, "spNotification_RemoveByMessageID")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Input, MessageID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_RemoveByMessageID);


	class QueryNotification_SetRead : public QueryBase
	{
	public:
		int64_t UserID;
		int32_t NotificationID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_SetRead, "spNotification_SetRead")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Input, NotificationID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_SetRead);



	class QuerySetComplitionState : public QueryBase
	{
	public:
		int64_t PlayerID;
		String ComplitionState;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QuerySetComplitionState, "spSetComplitionState")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, ComplitionState)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QuerySetComplitionState);


	class QueryGetComplitionState : public QueryBase
	{
	public:
		int64_t PlayerID;
		String ComplitionState;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetComplitionState, "spGetComplitionState")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, ComplitionState)
			BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryGetComplitionState);


} // namespace DB
}  // namespace SF
