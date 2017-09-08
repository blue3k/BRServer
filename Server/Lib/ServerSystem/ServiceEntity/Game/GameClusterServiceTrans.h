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

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GameClusterServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {


	//class GameServerTransRegisterPlayerToJoinGameServer : public ServerEntityMessageTransaction< GameClusterServiceEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 2>
	//{
	//public:
	//	typedef ServerEntityMessageTransaction< GameClusterServiceEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 2> super;

	//private:
	//	const char* m_PublicAddress;
	//	const char* m_PublicAddressIPV6;
	//	uint m_Port;
	//	EntityUID m_PlayerUID;

	//public:

	//	GameServerTransRegisterPlayerToJoinGameServer(MessageDataPtr &pIMsg);// : ServerEntityMessageTransaction(pIMsg) { SetWorkOnServerEntity(true); }
	//	virtual ~GameServerTransRegisterPlayerToJoinGameServer() {}

	//	Result OnPlayerRegisteredRes(Svr::TransactionResult* &pRes);

	//	// Start Transaction
	//	virtual Result StartTransaction() override;

	//	Policy::NetSvrPolicyGameServer* GetInterface() { return super::GetInterface<Policy::NetSvrPolicyGameServer>(); }

	//	BR_SVR_MSGTRANS_CLOSE_ARGS(RegisterPlayerToJoinGameServerRes, RouteContext(m_PlayerUID, GetRouteContext().GetFrom()), m_PublicAddress, m_PublicAddressIPV6, m_Port);
	//};


	class GameServerTransGamePlayerEntityCreatedS2CEvt : public ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityCreatedC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityCreatedC2SEvt> super;

	private:

	public:
		GameServerTransGamePlayerEntityCreatedS2CEvt(IHeap& memHeap, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memHeap, pIMsg ) {}
		virtual ~GameServerTransGamePlayerEntityCreatedS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	class GameServerTransGamePlayerEntityDeletedS2CEvt : public ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityDeletedC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< GameClusterServiceEntity, Message::ClusterServer::GamePlayerEntityDeletedC2SEvt> super;

	private:

	public:
		GameServerTransGamePlayerEntityDeletedS2CEvt(IHeap& memHeap, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction(memHeap, pIMsg ) {}
		virtual ~GameServerTransGamePlayerEntityDeletedS2CEvt() {}

		// Start Transaction
		virtual Result StartTransaction();
	};

	
} // namespace GameServer 
} // namespace SF 

