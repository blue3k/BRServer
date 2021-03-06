
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
#include "DB/LoginSessionQuery.h"
#include "DBTrace.h"
#include "MemoryManager/SFMemoryPool.h"



namespace SF {
	namespace DB {
		BRDB_DEFINE_QUERY_IMPL(QueryRegisterAuthTicket);
		BRDB_DEFINE_QUERY_IMPL(QueryReplaceLoginSession);
		BRDB_DEFINE_QUERY_IMPL(QueryDeleteLoginSession);

		BRDB_DEFINE_QUERY_IMPL(QueryConnectedToGameServer);
		BRDB_DEFINE_QUERY_IMPL(QueryValidateGameServerSession);
		BRDB_DEFINE_QUERY_IMPL(QueryGameServerHeartbeat);

		BRDB_DEFINE_QUERY_IMPL(QueryFindPlayer);

	}
}
