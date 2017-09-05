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

#include "SFTypedefs.h"
#include "Transaction/ServerTransaction.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "GameServerClass.h"
#include "GameInstance/GameClusterServiceEntity.h"
#include "GameInstance/GamePlayerEntity.h"

namespace SF {
namespace GameServer {



	template<class ProcessEntity>
	class GameServerTransRegisterPlayerToJoinGameServer : public Svr::ServerEntityMessageTransaction< ProcessEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>, sizeof(Svr::TransactionMessageHandlerType)*2>
	{
	public:
		typedef Svr::ServerEntityMessageTransaction< ProcessEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 2> super;

	private:
		const char* m_PublicAddress;
		const char* m_PublicAddressIPV6;
		uint m_Port;
		EntityUID m_PlayerUID;

	public:

		GameServerTransRegisterPlayerToJoinGameServer(MessageDataPtr &pIMsg);
		virtual ~GameServerTransRegisterPlayerToJoinGameServer() {}

		Result OnPlayerRegisteredRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		Policy::NetSvrPolicyGameServer* GetInterface() { return super::template GetInterface<Policy::NetSvrPolicyGameServer>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(RegisterPlayerToJoinGameServerRes, RouteContext(m_PlayerUID, super::GetRouteContext().GetFrom()), m_PublicAddress, m_PublicAddressIPV6, m_Port);
	};


	extern template class GameServerTransRegisterPlayerToJoinGameServer<GameClusterServiceEntity>;
	extern template class GameServerTransRegisterPlayerToJoinGameServer<GamePlayerEntity>;


};// namespace GameServer 
};// namespace SF 

