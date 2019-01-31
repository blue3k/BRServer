
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	

#include "SFTypedefs.h"
#include "DB/DBClusterManager.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFArray.h"
#include "Types/BrGameTypes.h"
#include "Component/ServerComponent.h"

namespace SF {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameDB Class 
	//

	class GameTransactionDB : public DBClusterManager
	{
	public:

		enum {
			ComponentID = Svr::ServerComponentID_GameTransactionDB,
		};

	private:

	public:
		// constructor / destructor
		GameTransactionDB();
		virtual ~GameTransactionDB();


		Result InitializeComponent() { return ResultCode::SUCCESS; }
		virtual void TerminateComponent() ;

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	DB Interface
		//

		// Transaction log must be closed safely
		// Add game log
		Result AddGameLog(uint shardID, const PlayerID &playerID, TimeStampSec gameTime, TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue, const char* logMessage);

	};

} // namespace DB
}  // namespace SF
