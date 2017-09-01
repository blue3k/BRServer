
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
		MCODE_QueryUpdateRankingScore,
		MCODE_QueryGetRankers,
	}; // enum MsgCode



	struct QueryGetTotalRankingSet
	{
		int32_t RankingID;
		int32_t Ranking;
		float WinRate;
		int32_t Win;
		int32_t Lose;
		int64_t PlayerID;
		int64_t FBUID;
		char  NickName[BINSIZE_NAME];
		int32_t Level;
	};

	class QueryGetTotalRanking : public QueryGetTotalRankingSet, public QueryBase
	{
	public:
		int32_t MinRanking;
		int32_t RankingCount;

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

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_RANKINGDB,QueryGetTotalRanking, QueryGetTotalRankingSet);




	class QueryUpdateRankingScore : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t FBUID;
		char  NickName[BINSIZE_NAME];
		int32_t Level;

		uint64_t Score;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateRankingScore, 5)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(FBUID)
			BRDB_COLUMN_ENTRY(NickName)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(Score)
		BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spUpdateRankingScore", BRDB_PARAM_5)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_RANKINGDB, QueryUpdateRankingScore);

	



	struct QueryGetRankersSet
	{
		int64_t PlayerID;
		int64_t FBUID;
		char  NickName[BINSIZE_NAME];
		int32_t Level;

		uint64_t Score;
	};

	class QueryGetRankers : public QueryGetRankersSet, public QueryBase
	{
	public:
		int32_t BaseIndex;
		int32_t RequestCount;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetRankers, 2)
			BRDB_COLUMN_ENTRY(BaseIndex)
			BRDB_COLUMN_ENTRY(RequestCount)
			BRDB_END_PARAM_MAP()

			BRDB_BEGIN_RESULT_MAP(QueryGetRankersSet, 5)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(FBUID)
			BRDB_COLUMN_ENTRY(NickName)
			BRDB_COLUMN_ENTRY(Level)
			BRDB_COLUMN_ENTRY(Score)
			BRDB_END_RESULT_MAP()

			BRDB_QUERYSTRING("spGetRankers", BRDB_PARAM_2)
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_RANKINGDB, QueryGetRankers, QueryGetRankersSet);



}  // namespace DB
}  // namespace BR
