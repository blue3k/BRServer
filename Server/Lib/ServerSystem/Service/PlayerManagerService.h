////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : Server cluster service
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "String/SFFixedStringDB.h"




namespace SF {

	namespace Svr {
		class ClusteredServiceEntity;
	}
	

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Player manager service
	//

	class PlayerManagerService
	{
	protected:

	public:

		PlayerManagerService()
		{}

		virtual ~PlayerManagerService()
		{
		}

		virtual void Clear() {}

		// Create or update player id
		virtual Result CreatePlayer(GameID gameID, PlayerID playerID, EntityUID entityUID) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result DeletePlayer(GameID gameID, PlayerID playerID) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result FindPlayer(GameID gameID, PlayerID playerID, EntityUID& entityUID) { return ResultCode::NOT_IMPLEMENTED; }

		// Use server gameID for search
		virtual Result CreatePlayer(PlayerID playerID, EntityUID entityUID) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result DeletePlayer(PlayerID playerID) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result FindPlayer(PlayerID playerID, EntityUID& entityUID) { return ResultCode::NOT_IMPLEMENTED; }

	};

	

}; // namespace SF

