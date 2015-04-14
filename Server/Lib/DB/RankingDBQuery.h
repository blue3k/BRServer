
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : MadK
//
// Description : Ranking DB queries
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "DB/Query.h"
#include "DB/DBConst.h"
#include "DB/QueryConst.h"


namespace BR {
namespace DB {
	
	
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query Message Code
	//

	enum MsgRankingDBCode
	{
		MCODE_QueryGetTotalRanking = 1,
	}; // enum MsgCode



	struct QueryGetTotalRankingSet
	{
		INT32 RankingID;
		INT32 Ranking;
		float WinRate;
		INT32 Win;
		INT32 Lose;
		INT64 PlayerID;
		INT64 FBUID;
		char  NickName[BINSIZE_NAME];
		INT32 Level;
	};

	class QueryGetTotalRanking : public QueryGetTotalRankingSet, public QueryBase
	{
	public:
		INT32 MinRanking;
		INT32 RankingCount;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetTotalRanking,2)
			BRDB_COLUMN_ENTRY(MinRanking)
			BRDB_COLUMN_ENTRY(RankingCount)
		BRDB_END_PARAM_MAP()
		
		BRDB_BEGIN_RESULT_MAP(QueryGetTotalRanking,9)
			BRDB_COLUMN_ENTRY(RankingID)
			BRDB_COLUMN_ENTRY(Ranking)
			BRDB_COLUMN_ENTRY(WinRate)
			BRDB_COLUMN_ENTRY(Win)
			BRDB_COLUMN_ENTRY(Lose)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(FBUID)
			BRDB_COLUMN_ENTRY(NickName)
			BRDB_COLUMN_ENTRY(Level)
		BRDB_END_RESULT_MAP()

		BRDB_QUERYSTRING( "spGetTotalRanking", BRDB_PARAM_2 )
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(POLICY_RANKINGDB,QueryGetTotalRanking, QueryGetTotalRankingSet);

	
}  // namespace DB
}  // namespace BR
