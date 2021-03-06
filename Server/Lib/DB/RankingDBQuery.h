
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Ranking DB queries
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
		String  NickName;
		int32_t Level;
	};

	class QueryGetTotalRanking : public QueryGetTotalRankingSet, public QueryBase
	{
	public:
		int32_t MinRanking;
		int32_t RankingCount;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetTotalRanking, "spGetTotalRanking")
			BRDB_PARAM_ENTRY(ParamIO::Input, MinRanking)
			BRDB_PARAM_ENTRY(ParamIO::Input, RankingCount)
		BRDB_END_PARAM_MAP()
		
		//BRDB_BEGIN_RESULT_MAP(QueryGetTotalRanking)
		//	BRDB_COLUMN_ENTRY(RankingID)
		//	BRDB_COLUMN_ENTRY(Ranking)
		//	BRDB_COLUMN_ENTRY(WinRate)
		//	BRDB_COLUMN_ENTRY(Win)
		//	BRDB_COLUMN_ENTRY(Lose)
		//	BRDB_COLUMN_ENTRY(PlayerID)
		//	BRDB_COLUMN_ENTRY(FBUID)
		//	BRDB_COLUMN_ENTRY(NickName)
		//	BRDB_COLUMN_ENTRY(Level)
		//BRDB_END_RESULT_MAP()
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_RANKINGDB,QueryGetTotalRanking);




	class QueryUpdateRankingScore : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t FBUID;
		String  NickName;
		int32_t Level;

		uint64_t Score;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateRankingScore, "spUpdateRankingScore")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, FBUID)
			BRDB_PARAM_ENTRY(ParamIO::Input, NickName)
			BRDB_PARAM_ENTRY(ParamIO::Input, Level)
			BRDB_PARAM_ENTRY(ParamIO::Input, Score)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_RANKINGDB, QueryUpdateRankingScore);

	



	struct QueryGetRankersSet
	{
		int64_t PlayerID;
		int64_t FBUID;
		String  NickName;
		int32_t Level;

		uint64_t Score;
	};

	class QueryGetRankers : public QueryGetRankersSet, public QueryBase
	{
	public:
		int32_t BaseIndex;
		int32_t RequestCount;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetRankers, "spGetRankers")
			BRDB_PARAM_ENTRY(ParamIO::Input, BaseIndex)
			BRDB_PARAM_ENTRY(ParamIO::Input, RequestCount)
		BRDB_END_PARAM_MAP()

		//BRDB_BEGIN_RESULT_MAP(QueryGetRankersSet)
		//	BRDB_COLUMN_ENTRY(PlayerID)
		//	BRDB_COLUMN_ENTRY(FBUID)
		//	BRDB_COLUMN_ENTRY(NickName)
		//	BRDB_COLUMN_ENTRY(Level)
		//	BRDB_COLUMN_ENTRY(Score)
		//BRDB_END_RESULT_MAP()
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_RANKINGDB, QueryGetRankers);



}  // namespace DB
}  // namespace SF
