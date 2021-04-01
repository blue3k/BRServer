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
#include "MemoryManager/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/SFMessage.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "Transaction/ServerTransaction.h"
#include "Protocol/GameServerMsgClass.h"
#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameMsgClass.h"
#include "Protocol/GameNetPolicy.h"

// deprecated
#include "Protocol/ClusterServerMsgClass.h"
#include "Protocol/ClusterServerNetPolicy.h"

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

		//BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyGameServer, RegisterPlayerToJoinGameServerRes, RouteContext(m_PlayerUID, super::GetRouteContext().GetFrom()), m_PublicAddress, m_PublicAddressIPV6, m_Port);
		virtual Result OnCloseTransaction(Result hrRes) override
		{
			Result hr = ResultCode::SUCCESS; 
			RouteContext routeContext(m_PlayerUID, super::GetRouteContext().GetFrom());
			svrCheck(NetSvrPolicyGameServer(super::GetRemoteEndpoint()).RegisterPlayerToJoinGameServerRes(routeContext, super::GetTransactionID(), hrRes, m_PublicAddress, m_PublicAddressIPV6, m_Port));

			super::OnCloseTransaction(hrRes);
		return hr;
		}

	};


	extern template class GameServerTransRegisterPlayerToJoinGameServer<GameServiceEntity>;
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

		BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameServer, RegisterPlayerToJoinGameServerOnPlayerEntityRes, GetRouteContext().GetSwaped());
	};






} // namespace Svr
} // namespace SF 

