
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
#include "DB/DBClusterManager.h"
#include "Common/SvrPolicyID.h"
#include "ServerSystem/ServerComponent.h"


namespace BR {
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

		Result GetRankingListCmd( TransactionID Sender, UINT32 minRanking, UINT32 rankingCount );


	};


} // namespace DB
} // namespace BR
