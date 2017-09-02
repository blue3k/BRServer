
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
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

	class GameTransactionDB : private DBClusterManager, public Svr::IServerComponent
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


		virtual void TerminateComponent() override;

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
