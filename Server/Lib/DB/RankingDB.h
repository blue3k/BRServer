
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : MadK
//
// Description : RankingDB Interface
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrCommonTypes.h"
#include "DB/QueryManager.h"
#include "Common/SvrPolicyID.h"
#include "ServerSystem/ServerComponent.h"


namespace BR {
namespace DB {



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	RankingDBServer Class 
	//

	class RankingDB : protected QueryManager, public Svr::IServerComponent
	{
	public:

		enum {
			ComponentID = Svr::ServerComponentID_RankingDB,
		};

	private:

	public:
		// constructor / destructor
		RankingDB();
		virtual ~RankingDB();

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Ranking DB interface
		//

		HRESULT GetRankingListCmd( TransactionID Sender, UINT32 minRanking, UINT32 rankingCount );


	};


} // namespace DB
} // namespace BR
