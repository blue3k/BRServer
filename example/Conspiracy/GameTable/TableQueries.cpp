////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Account DB
//
////////////////////////////////////////////////////////////////////////////////

#include "TablePCH.h"
#include "TableQueries.h"

namespace SF {
	namespace DB {
		BRDB_DEFINE_QUERY_IMPL(QueryTableVersionTbl);

		BRDB_DEFINE_QUERY_IMPL(QueryGameConfigTbl);

		BRDB_DEFINE_QUERY_IMPL(QueryLevelTbl);
		BRDB_DEFINE_QUERY_IMPL(QueryOrganicTbl);

		BRDB_DEFINE_QUERY_IMPL(QueryShopTbl);
		BRDB_DEFINE_QUERY_IMPL(QueryRewardTbl);

		BRDB_DEFINE_QUERY_IMPL(QueryBotTalkTable);
	}
}