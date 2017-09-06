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
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServerSystem/ServiceEntity/Game/GameServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {



	class GamePartyTrans : public TransactionT<GameServiceEntity>
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
		GamePartyTrans(IMemoryManager& memoryManager, uint startMemberCount, uint targetMemberCount);
		virtual ~GamePartyTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

	};





} // namespace GameServer 
} // namespace SF 

