
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
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


	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Ranking DB interface
	//

	HRESULT RankingDB::GetRankingListCmd( BR::TransactionID Sender, UINT32 minRanking, UINT32 rankingCount )
	{
		HRESULT hr = S_OK;
		QueryGetTotalRankingCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryGetTotalRankingCmd );


		QueryGetTotalRankingSet *pSet = pQuery;
		memset( pSet, 0, sizeof(QueryGetTotalRankingSet) );

		pQuery->MinRanking = minRanking;
		pQuery->RankingCount = rankingCount;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}


} // namespace DB
} // namespace BR


