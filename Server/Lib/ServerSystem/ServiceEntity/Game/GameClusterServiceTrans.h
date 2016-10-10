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
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {


	//class GameServerTransRegisterPlayerToJoinGameServer : public ServerEntityMessageTransaction< GameClusterServiceEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 2>
	//{
	//public:
	//	typedef ServerEntityMessageTransaction< GameClusterServiceEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 2> super;

	//private:
	//	const char* m_PublicAddress;
	//	const char* m_PublicAddressIPV6;
	//	UINT m_Port;
	//	EntityUID m_PlayerUID;

	//public:

	//	GameServerTransRegisterPlayerToJoinGameServer(Message::MessageData* &pIMsg);// : ServerEntityMessageTransaction(pIMsg) { SetWorkOnServerEntity(true); }
	//	virtual ~GameServerTransRegisterPlayerToJoinGameServer() {}

	//	Result OnPlayerRegisteredRes(Svr::TransactionResult* &pRes);

	//	// Start Transaction
	//	virtual Result StartTransaction() override;

	//	Policy::ISvrPolicyGameServer* GetPolicy() { return super::GetPolicy<Policy::ISvrPolicyGameServer>(); }

	//	BR_SVR_MSGTRANS_CLOSE_ARGS(RegisterPlayerToJoinGameServerRes, RouteContext(m_PlayerUID, GetRouteContext().GetFrom()), m_PublicAddress, m_PublicAddressIPV6, m_Port);
	//};


	class GameServerTransGamePlayerEntityCreatedS2CEvt : public ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityCreatedC2SEvt, GameServerTransGamePlayerEntityCreatedS2CEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityCreatedC2SEvt, GameServerTransGamePlayerEntityCreatedS2CEvt> super;

	private:

	public:
		GameServerTransGamePlayerEntityCreatedS2CEvt( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~GameServerTransGamePlayerEntityCreatedS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	class GameServerTransGamePlayerEntityDeletedS2CEvt : public ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityDeletedC2SEvt, GameServerTransGamePlayerEntityDeletedS2CEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityDeletedC2SEvt, GameServerTransGamePlayerEntityDeletedS2CEvt> super;

	private:

	public:
		GameServerTransGamePlayerEntityDeletedS2CEvt( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~GameServerTransGamePlayerEntityDeletedS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
} // namespace GameServer 
} // namespace BR 

