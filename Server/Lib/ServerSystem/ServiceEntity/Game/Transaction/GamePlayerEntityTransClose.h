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
#include "Transaction/Transaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Protocol/GameMsgClass.h"
#include "Protocol/GameNetPolicy.h"
#include "Protocol/GameServerMsgClass.h"
#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameInstanceMsgClass.h"
#include "Protocol/GameInstanceNetPolicy.h"

#include "Transaction/MessageRoute.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Net/SFMessage.h"




namespace SF {
namespace Svr {


	// Close zone instance
	class PlayerTransCloseInstance : public Svr::TransactionT<GamePlayerEntity>
	{
	private:
		using super = Svr::TransactionT<GamePlayerEntity>;

		INT m_WaitingTransactions;

	public:
		PlayerTransCloseInstance(IHeap& heap);
		virtual ~PlayerTransCloseInstance() {}

		Result UpdateDBRes(Svr::TransactionResult* pRes);
		Result DeleteLoginSessionRes(Svr::TransactionResult* pRes);
		Result UnregisterMatchingRes(Svr::TransactionResult* pRes);
		Result LeaveGameInstanceRes(Svr::TransactionResult* pRes);
		Result LeavePartyRes(Svr::TransactionResult* pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		virtual Result OnCloseTransaction( Result hrRes ) override;

	};
	





} // namespace GameServer 
} // namespace SF 

