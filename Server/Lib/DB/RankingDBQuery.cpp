////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Ranking DB
//
////////////////////////////////////////////////////////////////////////////////

#include "DBPch.h"
#include "RankingDBQuery.h"


namespace SF {
	namespace DB {
		BRDB_DEFINE_QUERY_IMPL(QueryGetTotalRanking);
		BRDB_DEFINE_QUERY_IMPL(QueryUpdateRankingScore);
		BRDB_DEFINE_QUERY_IMPL(QueryGetRankers);
	}
}
