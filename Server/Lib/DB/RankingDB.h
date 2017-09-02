
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
	

#include "SFTypedefs.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "DB/DBClusterManager.h"
#include "ServerSystem/ServerComponent.h"

#include "DB/RankingDBQuery.h"


namespace SF {
namespace DB {



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	RankingDBServer Class 
	//

	class RankingDB : protected DBClusterManager, public Svr::IServerComponent
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

		virtual void TerminateComponent() override;

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Ranking DB interface
		//

		Result GetRankingListCmd( TransactionID Sender, uint32_t minRanking, uint32_t rankingCount );


		Result UpdateRankingScoreCmd(TransactionID Sender, int64_t playerID, int64_t fBUID, const char *nickName, int32_t level, uint64_t score);

		QueryGetRankersCmd* GetRankers(int32_t baseIndex, int32_t requestCount);

	};


} // namespace DB
} // namespace SF
