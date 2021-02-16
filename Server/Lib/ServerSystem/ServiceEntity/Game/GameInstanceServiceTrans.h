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
#include "Net/SFMessage.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {

	
	
	class GameInstanceTrans : public TransactionT<GameServiceEntity>
	{
	public:
		typedef TransactionT<GameServiceEntity> super;

	private:

		struct GamePlayerInfo : public PlayerInformation
		{
			GamePlayerInfo()
			{
			}
		};

	public:
		GameInstanceTrans(IHeap& memoryManager, uint startMemberCount, uint targetMemberCount);
		virtual ~GameInstanceTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;

	};





} // namespace GameServer 
} // namespace SF 

