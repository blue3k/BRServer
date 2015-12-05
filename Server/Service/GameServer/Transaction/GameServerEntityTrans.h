////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/ServerTransaction.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "GameServerClass.h"


namespace BR {
namespace GameServer {

	

	class GameServerTransRegisterPlayerToJoinGameServer : public Svr::ServerEntityMessageTransaction< Svr::ServerEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer, sizeof(Svr::TransactionMessageHandlerType)*2>
	{
	public:
		typedef Svr::ServerEntityMessageTransaction< Svr::ServerEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 2> super;

	private:
		NetAddress m_PublicAddress;
		EntityUID m_PlayerUID;

	public:

		GameServerTransRegisterPlayerToJoinGameServer(Message::MessageData* &pIMsg);// : ServerEntityMessageTransaction(pIMsg) { SetWorkOnServerEntity(true); }
		virtual ~GameServerTransRegisterPlayerToJoinGameServer() {}

		HRESULT OnPlayerRegisteredRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction() override;

		Policy::ISvrPolicyGameServer* GetPolicy() { return GetMyOwner()->GetPolicy<Policy::ISvrPolicyGameServer>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(RegisterPlayerToJoinGameServerRes, RouteContext(m_PlayerUID, GetRouteContext().GetFrom()), m_PublicAddress);
	};



};// namespace GameServer 
};// namespace BR 

