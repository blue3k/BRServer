
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "DBPch.h"
#include "SFTypedefs.h"
#include "String/SFStrUtil.h"
#include "DB/RankingDB.h"
#include "DB/RankingDBQuery.h"



namespace SF {
	namespace DB {


		//////////////////////////////////////////////////////////////////////////////////
		//
		//	RankingDB Class 
		//

		// constructor / destructor
		RankingDB::RankingDB()
		{
		}


		RankingDB::~RankingDB()
		{
		}

		void RankingDB::TerminateComponent()
		{
			TerminateDB();
		}


		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Ranking DB interface
		//

		Result RankingDB::GetRankingListCmd(TransactionID Sender, uint32_t minRanking, uint32_t rankingCount)
		{
			ScopeContext hr;

			UniquePtr<QueryGetTotalRankingCmd> pQuery(new(GetHeap()) QueryGetTotalRankingCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->MinRanking = minRanking;
			pQuery->RankingCount = rankingCount;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}



		Result RankingDB::UpdateRankingScoreCmd(TransactionID Sender, int64_t playerID, int64_t fBUID, const char* nickName, int32_t level, uint64_t score)
		{
			ScopeContext hr;

			UniquePtr<QueryUpdateRankingScoreCmd> pQuery(new(GetHeap()) QueryUpdateRankingScoreCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->PlayerID = playerID;
			pQuery->FBUID = fBUID;
			pQuery->NickName = nickName;
			pQuery->Level = level;
			pQuery->Score = score;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));

			return hr;
		}


		QueryGetRankersCmd* RankingDB::GetRankers(int32_t baseIndex, int32_t requestCount)
		{
			ScopeContext hr;

			UniquePtr<QueryGetRankersCmd> pQuery(new(GetHeap()) QueryGetRankersCmd(GetHeap()));
			if (!pQuery)
				return nullptr;

			pQuery->BaseIndex = baseIndex;
			pQuery->RequestCount = requestCount;

			if (!RequestQuerySync(pQuery.get()))
				return nullptr;

			return pQuery.release();
		}


	} // namespace DB
} // namespace SF


