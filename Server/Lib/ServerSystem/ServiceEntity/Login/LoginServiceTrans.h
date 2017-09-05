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
#include "ServerSystem/ServiceEntity/Login/LoginServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {

	
	
	class LoginPartyTrans : public TransactionT<LoginServiceEntity, LoginPartyTrans>
	{
	public:
		typedef TransactionT<LoginServiceEntity, LoginPartyTrans> super;

	private:

		struct LoginPlayerInfo : public PlayerInformation
		{
			LoginPlayerInfo()
			{
			}
		};

	public:
		LoginPartyTrans(IMemoryManager& memoryManager, uint startMemberCount, uint targetMemberCount);
		virtual ~LoginPartyTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

	};





} // namespace GameServer 
} // namespace SF 

