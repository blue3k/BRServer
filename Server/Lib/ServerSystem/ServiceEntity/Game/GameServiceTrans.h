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
#include "Memory/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/SFMessage.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Transaction/ServerTransaction.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"


namespace SF {
namespace Svr {



	class GamePartyTrans : public TransactionT<GameServiceEntity>
	{
	public:
		typedef TransactionT<GameServiceEntity> super;

	private:

		struct GamePlayerInfo : public PlayerInformation
		{
			GamePlayerInfo()
			{
			}
		};

	public:
		GamePartyTrans(IHeap& heap, uint startMemberCount, uint targetMemberCount);
		virtual ~GamePartyTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

	};





	template<class ProcessEntity>
	class GameServerTransRegisterPlayerToJoinGameServer : public Svr::ServerEntityMessageTransaction< ProcessEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd>
	{
	public:

		using super = Svr::ServerEntityMessageTransaction< ProcessEntity, Message::GameServer::RegisterPlayerToJoinGameServerCmd>;

	private:
		const char* m_PublicAddress;
		const char* m_PublicAddressIPV6;
		uint m_Port;
		EntityUID m_PlayerUID;

	public:

		GameServerTransRegisterPlayerToJoinGameServer(IHeap& heap, MessageDataPtr &pIMsg);
		virtual ~GameServerTransRegisterPlayerToJoinGameServer() {}

		Result OnPlayerRegisteredRes(Svr::TransactionResult* pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyGameServer, RegisterPlayerToJoinGameServerRes, RouteContext(m_PlayerUID, super::GetRouteContext().GetFrom()), m_PublicAddress, m_PublicAddressIPV6, m_Port);
	};


	extern template class GameServerTransRegisterPlayerToJoinGameServer<Svr::GameServiceEntity>;
	extern template class GameServerTransRegisterPlayerToJoinGameServer<Svr::GamePlayerEntity>;




	class PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity : public Svr::UserTransactionS2SCmd< GamePlayerEntity, Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd>
	{
	public:
		typedef Svr::UserTransactionS2SCmd< GamePlayerEntity, Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd> super;

	public:
		PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity(IHeap& heap, MessageDataPtr &pIMsg) : UserTransactionS2SCmd(heap, pIMsg) {}
		virtual ~PlayerTransRegisterPlayerToJoinGameServerOnPlayerEntity() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameServer, RegisterPlayerToJoinGameServerOnPlayerEntityRes, GetRouteContext().GetSwaped());
	};




} // namespace GameServer 
} // namespace SF 

