////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"

#include "ServerSystem/MessageRoute.h"

#include "GameServerClass.h"

#include "GameInstance/GamePlayerEntity.h"
#include "Net/Message.h"

//#include "PlayerInfoManager.h"


namespace BR {
namespace GameServer {


	// Close zone instance
	class PlayerTransCloseInstance : public Svr::TransactionT<GamePlayerEntity, PlayerTransCloseInstance,sizeof(Svr::TransactionMessageHandlerType)*6>
	{
	private:
		typedef Svr::TransactionT<GamePlayerEntity, PlayerTransCloseInstance, sizeof(Svr::TransactionMessageHandlerType) * 6> super;

		INT m_WaitingTransactions;

	public:
		PlayerTransCloseInstance();
		virtual ~PlayerTransCloseInstance() {}

		Result UpdateDBRes(Svr::TransactionResult* &pRes);
		Result DeleteLoginSessionRes(Svr::TransactionResult* &pRes);
		Result UnregisterMatchingRes(Svr::TransactionResult* &pRes);
		Result LeaveGameRes(Svr::TransactionResult* &pRes);
		Result LeavePartyRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		virtual Result OnCloseTransaction( Result hrRes );

	};
	





} // namespace GameServer 
} // namespace BR 

