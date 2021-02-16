
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author: Kyungkun Ko
//
// Description : Game DB
//
////////////////////////////////////////////////////////////////////////////////


#include "DBPch.h"
#include "GameQuery.h"
#include "DBTrace.h"
#include "MemoryManager/SFMemoryPool.h"


namespace SF {
	namespace DB {

		BRDB_DEFINE_QUERY_IMPL(QueryCreatePlayerInfo);
		BRDB_DEFINE_QUERY_IMPL(QueryGetPlayerInfo);
		BRDB_DEFINE_QUERY_IMPL(QuerySetPlayerInfo);

		BRDB_DEFINE_QUERY_IMPL(QueryGetNickName);
		BRDB_DEFINE_QUERY_IMPL(QuerySetNickName);

		BRDB_DEFINE_QUERY_IMPL(QueryCreateCharacter);
		BRDB_DEFINE_QUERY_IMPL(QueryDeleteCharacter);
		BRDB_DEFINE_QUERY_IMPL(QueryGetCharacterList);
		BRDB_DEFINE_QUERY_IMPL(QueryGetCharacter);
		BRDB_DEFINE_QUERY_IMPL(QuerySaveCharacter);

		BRDB_DEFINE_QUERY_IMPL(QuerySavePurchaseInfoToDB);
		BRDB_DEFINE_QUERY_IMPL(QueryCheckPurchaseID);

		BRDB_DEFINE_QUERY_IMPL(QueryGetPlayerStatus);

		BRDB_DEFINE_QUERY_IMPL(QueryGetPlayerQuickInfo);
		BRDB_DEFINE_QUERY_IMPL(QueryGetFriendQuickInfo);
		BRDB_DEFINE_QUERY_IMPL(QueryGetFriendQuickInfoWithNick);

		BRDB_DEFINE_QUERY_IMPL(QueryGetFriendSlotStatus);


		BRDB_DEFINE_QUERY_IMPL(QueryAddFriend);
		BRDB_DEFINE_QUERY_IMPL(QueryRemoveFriend);
		BRDB_DEFINE_QUERY_IMPL(QueryGetFriendList);

		BRDB_DEFINE_QUERY_IMPL(QueryNotification_Add);
		BRDB_DEFINE_QUERY_IMPL(QueryNotification_GetList);
		BRDB_DEFINE_QUERY_IMPL(QueryNotification_Remove);
		BRDB_DEFINE_QUERY_IMPL(QueryNotification_RemoveByMessageID);
		BRDB_DEFINE_QUERY_IMPL(QueryNotification_SetRead);

		BRDB_DEFINE_QUERY_IMPL(QuerySetComplitionState);
		BRDB_DEFINE_QUERY_IMPL(QueryGetComplitionState);

	}
}
