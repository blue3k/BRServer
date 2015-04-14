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
#include "Common/BRBaseTypes.h"
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
#include "Common/Message.h"

//#include "PlayerInfoManager.h"


namespace BR {
namespace GameServer {


	// Close zone instance
	class PlayerTransCloseInstance : public Svr::TransactionT<GamePlayerEntity, PlayerTransCloseInstance,sizeof(Svr::TransactionMessageHandlerType)*6>
	{
	private:
		INT m_WaitingTransactions;

	public:
		PlayerTransCloseInstance();
		virtual ~PlayerTransCloseInstance() {}

		HRESULT UpdateDBRes(Svr::TransactionResult* &pRes);
		HRESULT DeleteLoginSessionRes(Svr::TransactionResult* &pRes);
		HRESULT UnregisterMatchingRes(Svr::TransactionResult* &pRes);
		HRESULT LeaveGameRes(Svr::TransactionResult* &pRes);
		HRESULT LeavePartyRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		virtual HRESULT OnCloseTransaction( HRESULT hrRes );

	};
	





} // namespace GameServer 
} // namespace BR 

