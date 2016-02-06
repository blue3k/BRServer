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
#include "ServerSystem/ServiceEntity/Game/GameServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {

	
	
	class GameInstanceTrans : public TransactionT<GameServiceEntity, GameInstanceTrans, sizeof(TransactionMessageHandlerType)*7>
	{
	public:
		typedef TransactionT<GameServiceEntity, GameInstanceTrans, sizeof(TransactionMessageHandlerType) * 7> super;

	private:

		struct GamePlayerInfo : public PlayerInformation
		{
			GamePlayerInfo()
			{
			}
		};

	public:
		GameInstanceTrans(UINT startMemberCount, UINT targetMemberCount);
		virtual ~GameInstanceTrans() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

	};





} // namespace GameServer 
} // namespace BR 
