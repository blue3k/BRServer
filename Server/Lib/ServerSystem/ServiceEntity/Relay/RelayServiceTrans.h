////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
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
#include "Protocol/ClusterServerMsgClass.h"
#include "Protocol/ClusterServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Relay/RelayServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {

	
	
	class RelayInstanceTrans : public TransactionT<RelayServiceEntity>
	{
	public:
		typedef TransactionT<RelayServiceEntity> super;

	private:


	public:
		RelayInstanceTrans(IHeap& memoryManager);
		virtual ~RelayInstanceTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

	};





} // namespace GameServer 
} // namespace SF 

