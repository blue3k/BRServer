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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/Login/LoginServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {

	
	
	class LoginPartyTrans : public TransactionT<LoginServiceEntity, LoginPartyTrans, sizeof(TransactionMessageHandlerType)*7>
	{
	public:
		typedef TransactionT<LoginServiceEntity, LoginPartyTrans, sizeof(TransactionMessageHandlerType) * 7> super;

	private:

		struct LoginPlayerInfo : public PlayerInformation
		{
			LoginPlayerInfo()
			{
			}
		};

	public:
		LoginPartyTrans(UINT startMemberCount, UINT targetMemberCount);
		virtual ~LoginPartyTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

	};





} // namespace GameServer 
} // namespace BR 

