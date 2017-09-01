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
#include "ServerSystem/ServerTransaction.h"
#include "Memory/MemoryPool.h"
#include "Types/BrBaseTypes.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "GameServerClass.h"
#include "GameInstance/GameClusterServiceEntity.h"
#include "GameInstance/GamePlayerEntity.h"

namespace BR {
namespace GameServer {



	template<class ProcessEntity>
	class GameServerTransRegisterPlayerToJoinGameServer : public Svr::ServerEntityMessageTransaction< ProcessEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer<ProcessEntity>, sizeof(Svr::TransactionMessageHandlerType)*2>
	{
	public:
		typedef Svr::ServerEntityMessageTransaction< ProcessEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd, GameServerTransRegisterPlayerToJoinGameServer, sizeof(Svr::TransactionMessageHandlerType) * 2> super;

	private:
		const char* m_PublicAddress;
		const char* m_PublicAddressIPV6;
		UINT m_Port;
		EntityUID m_PlayerUID;

	public:

		GameServerTransRegisterPlayerToJoinGameServer(Message::MessageData* &pIMsg);
		virtual ~GameServerTransRegisterPlayerToJoinGameServer() {}

		Result OnPlayerRegisteredRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		Policy::ISvrPolicyGameServer* GetPolicy() { return super::template GetPolicy<Policy::ISvrPolicyGameServer>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(RegisterPlayerToJoinGameServerRes, RouteContext(m_PlayerUID, super::GetRouteContext().GetFrom()), m_PublicAddress, m_PublicAddressIPV6, m_Port);
	};


	extern template class GameServerTransRegisterPlayerToJoinGameServer<GameClusterServiceEntity>;
	extern template class GameServerTransRegisterPlayerToJoinGameServer<GamePlayerEntity>;


};// namespace GameServer 
};// namespace BR 

