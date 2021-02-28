
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Game DB
//
////////////////////////////////////////////////////////////////////////////////


#include "GameConspiracyDBPch.h"
#include "GameConspiracyQuery.h"
#include "DBTrace.h"
#include "MemoryManager/SFMemoryPool.h"


namespace SF {
	namespace conspiracy {

		BRDB_DEFINE_QUERY_IMPL(QueryUpdateGameEnd);
		BRDB_DEFINE_QUERY_IMPL(QueryUpdateJoinGame);
		BRDB_DEFINE_QUERY_IMPL(QueryUpdateTickStatus);

		BRDB_DEFINE_QUERY_IMPL(QueryUpdateFriendStaminaTime);

	}
}
