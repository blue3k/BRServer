
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SFTypedefs.h"
#include "String/StrUtil.h"
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

	Result RankingDB::GetRankingListCmd( TransactionID Sender, uint32_t minRanking, uint32_t rankingCount )
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetTotalRankingCmd *pQuery = nullptr;
		QueryGetTotalRankingSet *pSet = nullptr;

		dbMem( pQuery = new(GetMemoryManager()) QueryGetTotalRankingCmd );


		pSet = pQuery;
		memset( pSet, 0, sizeof(QueryGetTotalRankingSet) );

		pQuery->MinRanking = minRanking;
		pQuery->RankingCount = rankingCount;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}



	Result RankingDB::UpdateRankingScoreCmd(TransactionID Sender, int64_t playerID, int64_t fBUID, const char *nickName, int32_t level, uint64_t score)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateRankingScoreCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetMemoryManager()) QueryUpdateRankingScoreCmd);

		pQuery->PlayerID = playerID;
		pQuery->FBUID = fBUID;
		StrUtil::StringCpy(pQuery->NickName, nickName);
		pQuery->Level = level;
		pQuery->Score = score;

		pQuery->SetTransaction(Sender);

		dbChk(RequestQuery(pQuery));
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	QueryGetRankersCmd* RankingDB::GetRankers(int32_t baseIndex, int32_t requestCount)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetRankersCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetMemoryManager()) QueryGetRankersCmd);

		pQuery->BaseIndex = baseIndex;
		pQuery->RequestCount = requestCount;

		dbChk(RequestQuerySync(pQuery));

	Proc_End:

		if (!(hr))
		{
			IHeap::Delete(pQuery);
			pQuery = nullptr;
		}

		return pQuery;
	}


} // namespace DB
} // namespace SF


