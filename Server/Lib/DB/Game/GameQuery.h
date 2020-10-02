
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
		MCODE_QueryGameDBNull,
		// Game
		MCODE_QueryCreatePlayerInfo,
		MCODE_QueryGetPlayerInfo,
		MCODE_QuerySetPlayerInfo,
		MCODE_QueryGetPlayerStatus,
		MCODE_QueryGetNickName,
		MCODE_QuerySetNickName,
		MCODE_QueryGetPlayerQuickInfo,
		MCODE_QueryGetFriendQuickInfo,
		MCODE_QueryGetFriendQuickInfoWithNick,
		MCODE_QueryGetFriendSlotStatus,
		MCODE_QueryAddFriend,
		MCODE_QueryRemoveFriend,
		MCODE_QueryGetFriendList,
		MCODE_QueryNotification_Add,
		MCODE_QueryNotification_GetList,
		MCODE_QueryNotification_Remove,
		MCODE_QueryNotification_RemoveByMessageID,
		MCODE_QueryNotification_SetRead,
		MCODE_QuerySetComplitionState,
		MCODE_QueryGetComplitionState,

		MCODE_QueryGameDB_Base,

		//MCODE_QuerySavePurchaseInfoToDB = MCODE_QueryGameDB_Base,
		//MCODE_QueryCheckPurchaseID,
		//MCODE_QueryUpdateGameEnd,
		//MCODE_QueryUpdateJoinGame,
		//MCODE_QueryUpdateTickStatus,
		//MCODE_QueryUpdateFriendStaminaTime,

	}; // enum MsgCode



	class QueryCreatePlayerInfo : public DB::QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;
		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryCreatePlayerInfo, "spCreatePlayerInfo")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryCreatePlayerInfo);


	class QueryGetPlayerInfo : public DB::QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerInfo, "spGetPlayerInfo")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
			BRDB_END_PARAM_MAP()



	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryGetPlayerInfo);



	class QuerySetPlayerInfo : public DB::QueryBase
	{
	public:
		// Player ID
		int64_t	PlayerID;
		int32_t	LatestActiveTime;
		int64_t	LatestTickTime;

		DynamicArray<NamedVariableBox> Attributes;

		// result
		int32_t	Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QuerySetPlayerInfo, "spSetPlayerInfo")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestActiveTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, LatestTickTime)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, Attributes)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
			BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QuerySetPlayerInfo);


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
	//	QuerySetNickName class
	//

	class QuerySetNickName : public DB::QueryBase
	{
	public:

		int64_t PlayerID;

		char NickName[DB::Const::MAX_USERNAME];
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QuerySetNickName, "spSetNickName")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, NickName)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
			BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QuerySetNickName);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryGetNickName class
	//

	class QueryGetNickName : public DB::QueryBase
	{
	public:

		int64_t PlayerID;

		char NickName[DB::Const::MAX_USERNAME];
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetNickName, "spGetNickName")
			BRDB_PARAM_ENTRY(DB::ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, NickName)
			BRDB_PARAM_ENTRY(DB::ParamIO::Output, Result)
			BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMEDB, QueryGetNickName);


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

		//BRDB_BEGIN_RESULT_MAP(QueryGetFriendList)
		//	BRDB_COLUMN_ENTRY(FriendUID)
		//	BRDB_COLUMN_ENTRY(FriendShardID)
		//	BRDB_COLUMN_ENTRY(FriendFacebookUID)
		//	BRDB_COLUMN_ENTRY(FriendStaminaTime)
		//BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryGetFriendList);




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



	class QueryNotification_GetList : public QueryBase
	{
	public:
		int64_t UserID;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryNotification_GetList, "spNotification_GetList")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

		//BRDB_BEGIN_RESULT_MAP(QueryNotification_GetList)
		//	BRDB_COLUMN_ENTRY(NotificationID)
		//	BRDB_COLUMN_ENTRY(MessageID)
		//	BRDB_COLUMN_ENTRY(MessageParam0)
		//	BRDB_COLUMN_ENTRY(MessageParam1)
		//	BRDB_COLUMN_ENTRY(MessageText)
		//	BRDB_COLUMN_ENTRY(IsRead)
		//	BRDB_COLUMN_ENTRY(TimeStamp)
		//BRDB_END_RESULT_MAP()

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_GAMEDB, QueryNotification_GetList);



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
