
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Game DB
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "DB/GameConspiracyQuery.h"
#include "DBTrace.h"
#include "Common/MemoryPool.h"



BRDB_DEFINE_QUERY_IMPL(QueryGetPlayerScoreList);

BRDB_DEFINE_QUERY_IMPL(QueryCreatePlayerInfo);
BRDB_DEFINE_QUERY_IMPL(QueryGetPlayerInfo);
BRDB_DEFINE_QUERY_IMPL(QuerySetPlayerInfo);

BRDB_DEFINE_QUERY_IMPL(QueryUpdateGameEnd);
BRDB_DEFINE_QUERY_IMPL(QueryUpdateJoinGame);
BRDB_DEFINE_QUERY_IMPL(QueryUpdateTickStatus);

BRDB_DEFINE_QUERY_IMPL(QueryGetPlayerStatus);

BRDB_DEFINE_QUERY_IMPL(QueryGetNickName);
BRDB_DEFINE_QUERY_IMPL(QuerySetNickName);

BRDB_DEFINE_QUERY_IMPL(QueryGetPlayerQuickInfo);
BRDB_DEFINE_QUERY_IMPL(QueryGetFriendQuickInfo);
BRDB_DEFINE_QUERY_IMPL(QueryGetFriendQuickInfoWithNick);

BRDB_DEFINE_QUERY_IMPL(QueryGetFriendSlotStatus);


BRDB_DEFINE_QUERY_IMPL(QueryAddFriend);
BRDB_DEFINE_QUERY_IMPL(QueryRemoveFriend);
BRDB_DEFINE_QUERY_IMPL(QueryGetFriendList);
BRDB_DEFINE_QUERY_IMPL(QueryUpdateFriendStaminaTime);

BRDB_DEFINE_QUERY_IMPL(QueryNotification_Add);
BRDB_DEFINE_QUERY_IMPL(QueryNotification_GetList);
BRDB_DEFINE_QUERY_IMPL(QueryNotification_Remove);
BRDB_DEFINE_QUERY_IMPL(QueryNotification_RemoveByMessageID);
BRDB_DEFINE_QUERY_IMPL(QueryNotification_SetRead);
