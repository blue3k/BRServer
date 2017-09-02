
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
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
		char    GameNick[Const::MAX_USERNAME];
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
		uint8_t m_UName[BINSIZE_NAME];
		int16_t m_Gender;
	};

	class QueryGetPlayerScoreList : public QueryGetPlayerScoreRawSet,  public QueryBase
	{
	public:
		int64_t m_AccountID;
		int32_t m_Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerScoreList,2)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(m_AccountID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(m_Result)
		BRDB_END_PARAM_MAP()

		BRDB_BEGIN_RESULT_MAP(QueryGetPlayerScoreList,3)
			BRDB_COLUMN_ENTRY(m_PlayerID)
			BRDB_COLUMN_ENTRY(m_UName)
			BRDB_COLUMN_ENTRY(m_Gender)
		BRDB_END_RESULT_MAP()

		BRDB_QUERYSTRING( "spGetScoreList", BRDB_PARAM_2 )
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
		BRDB_BEGIN_PARAM_MAP(QueryCreatePlayerInfo, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(InitialStamina)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()


		BRDB_BEGIN_RESULT_MAP(QueryGetPlayerInfoData, 24)
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


		BRDB_QUERYSTRING( "spCreatePlayerInfo", BRDB_PARAM_3 )
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
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerInfo, 2)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()


			BRDB_BEGIN_RESULT_MAP(QueryGetPlayerInfoData, 24)
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


			BRDB_QUERYSTRING("spGetPlayerInfo", BRDB_PARAM_2)
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
		BRDB_BEGIN_PARAM_MAP(QuerySetPlayerInfo,23)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
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
			BRDB_COLUMN_ENTRY(LatestActiveTime)
			BRDB_COLUMN_ENTRY(LatestTickTime)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()


		BRDB_QUERYSTRING( "spSetPlayerInfo", BRDB_PARAM_23 )
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
		uint8_t    PurchaseID[Const::MAX_PURCHASEID];
		char    PurchasePlatform[Const::MAX_PLATFORM_NAME];
		char    PurchaseToken[Const::MAX_PURCHASETOKEN];
		int32_t	LatestActiveTime;
		int64_t	LatestTickTime;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QuerySavePurchaseInfoToDB, 13)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(Exp)
			BRDB_COLUMN_ENTRY(GameMoney)
			BRDB_COLUMN_ENTRY(Gem)
			BRDB_COLUMN_ENTRY(Stamina)
			BRDB_COLUMN_ENTRY(AddedFriendSlot)
			BRDB_COLUMN_ENTRY(PurchaseID)
			BRDB_COLUMN_ENTRY(PurchasePlatform)
			BRDB_COLUMN_ENTRY(PurchaseToken)
			BRDB_COLUMN_ENTRY(LatestActiveTime)
			BRDB_COLUMN_ENTRY(LatestTickTime)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()


		BRDB_QUERYSTRING( "spSavePurchaseInfoToDB", BRDB_PARAM_13 )
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QuerySavePurchaseInfoToDB);


	class QueryCheckPurchaseID : public QueryBase
	{
	public:
		// Player ID
		uint8_t    PurchaseID[Const::MAX_PURCHASEID];

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryCheckPurchaseID, 2)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PurchaseID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()


			BRDB_QUERYSTRING("spCheckPurchaseID", BRDB_PARAM_2)
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
		BRDB_BEGIN_PARAM_MAP(QuerySetNickName, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(NickName)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spSetNickName", BRDB_PARAM_3)
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
		BRDB_BEGIN_PARAM_MAP(QueryGetNickName, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(NickName)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spGetNickName", BRDB_PARAM_3)
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
		BRDB_BEGIN_PARAM_MAP(QueryUpdateGameEnd,19)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(Exp)
			BRDB_COLUMN_ENTRY(GameMoney)
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
			BRDB_COLUMN_ENTRY(LatestActiveTime)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spUpdateGameEnd", BRDB_PARAM_19 )
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
		BRDB_BEGIN_PARAM_MAP(QueryUpdateJoinGame,7)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(Gem)
			BRDB_COLUMN_ENTRY(Stamina)
			BRDB_COLUMN_ENTRY(PlayerState)
			BRDB_COLUMN_ENTRY(LatestActiveTime)
			BRDB_COLUMN_ENTRY(LatestTickTime)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spUpdateJoinGame", BRDB_PARAM_7 )
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
		BRDB_BEGIN_PARAM_MAP(QueryUpdateTickStatus,7)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(Gem)
			BRDB_COLUMN_ENTRY(Stamina)
			BRDB_COLUMN_ENTRY(PlayerState)
			BRDB_COLUMN_ENTRY(LatestActiveTime)
			BRDB_COLUMN_ENTRY(LatestTickTime)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spUpdateTickStatus", BRDB_PARAM_7 )
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
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerStatus,4)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(PlayerState)
			BRDB_COLUMN_ENTRY(LatestActiveTime)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spGetPlayerStatus", BRDB_PARAM_4 )
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
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerQuickInfo,5)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(WeeklyWin)
			BRDB_COLUMN_ENTRY(WeeklyLose)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()


		BRDB_QUERYSTRING( "spGetPlayerQuickInfo", BRDB_PARAM_5 )
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
		BRDB_BEGIN_PARAM_MAP(QueryGetFriendQuickInfo, 7)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(WeeklyWin)
			BRDB_COLUMN_ENTRY(WeeklyLose)
			BRDB_COLUMN_ENTRY(PlayerState)
			BRDB_COLUMN_ENTRY(LatestActiveTime)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()


			BRDB_QUERYSTRING("spGetFriendQuickInfo", BRDB_PARAM_7)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryGetFriendQuickInfo);



	class QueryGetFriendQuickInfoWithNick : public QueryBase
	{
	public:

		int64_t PlayerID;
		char GameNick[Const::MAX_USERNAME];
		int32_t Level;
		int32_t WeeklyWin;
		int32_t WeeklyLose;
		int16_t	PlayerState;
		int32_t	LatestActiveTime;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetFriendQuickInfoWithNick, 8)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(GameNick)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(WeeklyWin)
			BRDB_COLUMN_ENTRY(WeeklyLose)
			BRDB_COLUMN_ENTRY(PlayerState)
			BRDB_COLUMN_ENTRY(LatestActiveTime)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()


			BRDB_QUERYSTRING("spGetFriendQuickInfoWithNick", BRDB_PARAM_8)
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
		BRDB_BEGIN_PARAM_MAP(QueryGetFriendSlotStatus, 5)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(AddedFriendSlot)
			BRDB_COLUMN_ENTRY(NumFriends)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()


			BRDB_QUERYSTRING("spGetFriendSlotStatus", BRDB_PARAM_5)
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
		BRDB_BEGIN_PARAM_MAP(QueryAddFriend, 5)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_COLUMN_ENTRY(FriendUID)
			BRDB_COLUMN_ENTRY(FriendShardID)
			BRDB_COLUMN_ENTRY(FriendFacebookUID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spFriend_Add", BRDB_PARAM_5)
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
		BRDB_BEGIN_PARAM_MAP(QueryRemoveFriend, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_COLUMN_ENTRY(FriendUID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spFriend_Remove", BRDB_PARAM_3)
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
		BRDB_BEGIN_PARAM_MAP(QueryGetFriendList, 1)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_END_PARAM_MAP()

			BRDB_BEGIN_RESULT_MAP(QueryGetFriendList, 4)
			BRDB_COLUMN_ENTRY(FriendUID)
			BRDB_COLUMN_ENTRY(FriendShardID)
			BRDB_COLUMN_ENTRY(FriendFacebookUID)
			BRDB_COLUMN_ENTRY(FriendStaminaTime)
			BRDB_END_RESULT_MAP()


			BRDB_QUERYSTRING("spFriend_List", BRDB_PARAM_1)
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
		BRDB_BEGIN_PARAM_MAP(QueryUpdateFriendStaminaTime, 4)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_COLUMN_ENTRY(FriendUID)
			BRDB_COLUMN_ENTRY(TimeStamp)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spFriend_UpdateStaminaTime", BRDB_PARAM_4)
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
		char MessageText[Const::MAX_NOTIFICATION_TEXT];
		int64_t TimeStamp;

		int32_t NotificationID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_Add, 9)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_COLUMN_ENTRY(Collapsable)
			BRDB_COLUMN_ENTRY(MessageID)
			BRDB_COLUMN_ENTRY(MessageParam0)
			BRDB_COLUMN_ENTRY(MessageParam1)
			BRDB_COLUMN_ENTRY(MessageText)
			BRDB_COLUMN_ENTRY(TimeStamp)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(NotificationID)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spNotification_Add", BRDB_PARAM_9)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_Add);


	struct QueryNotification_GetListSet
	{
		int32_t NotificationID;
		int16_t MessageID;
		int64_t MessageParam0;
		int64_t MessageParam1;
		char MessageText[Const::MAX_NOTIFICATION_TEXT];
		int64_t TimeStamp;
		uint8_t IsRead;
	};

	class QueryNotification_GetList : public QueryNotification_GetListSet, public QueryBase
	{
	public:
		int64_t UserID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_GetList, 2)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_BEGIN_RESULT_MAP(QueryNotification_GetList, 7)
			BRDB_COLUMN_ENTRY(NotificationID)
			BRDB_COLUMN_ENTRY(MessageID)
			BRDB_COLUMN_ENTRY(MessageParam0)
			BRDB_COLUMN_ENTRY(MessageParam1)
			BRDB_COLUMN_ENTRY(MessageText)
			BRDB_COLUMN_ENTRY(IsRead)
			BRDB_COLUMN_ENTRY(TimeStamp)
			BRDB_END_RESULT_MAP()


			BRDB_QUERYSTRING("spNotification_GetList", BRDB_PARAM_2)
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_GetList, QueryNotification_GetListSet);



	class QueryNotification_Remove : public QueryBase
	{
	public:
		int64_t UserID;
		int32_t NotificationID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_Remove, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_COLUMN_ENTRY(NotificationID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spNotification_Remove", BRDB_PARAM_3)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_Remove);


	class QueryNotification_RemoveByMessageID : public QueryBase
	{
	public:
		int64_t UserID;
		int16_t MessageID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_RemoveByMessageID, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_COLUMN_ENTRY(MessageID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spNotification_RemoveByMessageID", BRDB_PARAM_3)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_RemoveByMessageID);


	class QueryNotification_SetRead : public QueryBase
	{
	public:
		int64_t UserID;
		int32_t NotificationID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_SetRead, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_COLUMN_ENTRY(NotificationID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spNotification_SetRead", BRDB_PARAM_3)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_SetRead);



	class QuerySetComplitionState : public QueryBase
	{
	public:
		int64_t PlayerID;
		char ComplitionState[Const::MAX_COMPLITIONSTATE];

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QuerySetComplitionState, 2)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(ComplitionState)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spSetComplitionState", BRDB_PARAM_2)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QuerySetComplitionState);


	class QueryGetComplitionState : public QueryBase
	{
	public:
		int64_t PlayerID;
		char ComplitionState[Const::MAX_COMPLITIONSTATE];

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetComplitionState, 2)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(ComplitionState)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spGetComplitionState", BRDB_PARAM_2)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryGetComplitionState);


} // namespace DB
}  // namespace SF
