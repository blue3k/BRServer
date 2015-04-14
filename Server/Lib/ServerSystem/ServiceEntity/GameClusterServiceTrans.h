////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : game cluster entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/GameClusterServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {


	class GameServerTransGamePlayerEntityCreatedS2CEvt : public ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityCreatedC2SEvt, GameServerTransGamePlayerEntityCreatedS2CEvt>
	{
	private:

	public:
		GameServerTransGamePlayerEntityCreatedS2CEvt( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~GameServerTransGamePlayerEntityCreatedS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	class GameServerTransGamePlayerEntityDeletedS2CEvt : public ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityDeletedC2SEvt, GameServerTransGamePlayerEntityDeletedS2CEvt>
	{
	private:

	public:
		GameServerTransGamePlayerEntityDeletedS2CEvt( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~GameServerTransGamePlayerEntityDeletedS2CEvt() {}

		// Start Transaction
		virtual HRESULT StartTransaction();
	};

	
} // namespace GameServer 
} // namespace BR 

