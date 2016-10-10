
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
	

#include "Common/Typedefs.h"
#include "DB/QueryManager.h"
#include "Common/BrSvrTypes.h"
#include "Common/ArrayUtil.h"
#include "Common/BrGameTypes.h"
#include "ServerSystem/ServerComponent.h"

namespace BR {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameDB Class 
	//

	class GameTransactionDB : private QueryManager, public Svr::IServerComponent
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
		Result AddGameLog(UINT shardID, const PlayerID &playerID, TimeStampSec gameTime, TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue, const char* logMessage);

	};

} // namespace DB
}  // namespace BR
