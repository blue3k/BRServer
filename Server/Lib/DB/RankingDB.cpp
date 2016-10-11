
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
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DB/RankingDB.h"
#include "DB/RankingDBQuery.h"



namespace BR {
namespace DB { 


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	RankingDB Class 
	//
	
	// constructor / destructor
	RankingDB::RankingDB()
		:Svr::IServerComponent(ComponentID)
	{
	}


	RankingDB::~RankingDB()
	{
	}

	void RankingDB::TerminateComponent()
	{
		TerminateDB();
		Svr::IServerComponent::TerminateComponent();
	}


	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Ranking DB interface
	//

	Result RankingDB::GetRankingListCmd( TransactionID Sender, UINT32 minRanking, UINT32 rankingCount )
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetTotalRankingCmd *pQuery = nullptr;
		QueryGetTotalRankingSet *pSet = nullptr;

		dbMem( pQuery = new QueryGetTotalRankingCmd );


		pSet = pQuery;
		memset( pSet, 0, sizeof(QueryGetTotalRankingSet) );

		pQuery->MinRanking = minRanking;
		pQuery->RankingCount = rankingCount;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}


} // namespace DB
} // namespace BR


