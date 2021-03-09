////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2020 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Stream/SFMemoryStream.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Message/PlayInstanceMsgClass.h"
#include "Protocol/Policy/PlayInstanceNetPolicy.h"

#include "Transaction/MessageRoute.h"
#include "Transaction/ServerTransaction.h"

#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "Net/SFMessage.h"


namespace SF {


		class GameEntityTransJoinGameInstance : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::GameInstance::JoinGameInstanceCmd>
		{
		public:
			using super = Svr::MessageTransaction<Svr::GameInstanceEntity, Message::GameInstance::JoinGameInstanceCmd>;

		private:

			NetAddress m_Addr4{};
			NetAddress m_Addr6{};
			UTCTimeStampMS m_TimeStamp{};

		public:
			GameEntityTransJoinGameInstance(IHeap& heap, MessageDataPtr& pIMsg)
				: super(heap, Forward<MessageDataPtr>(pIMsg))
			{}
			virtual ~GameEntityTransJoinGameInstance() = default;

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyGameInstance, JoinGameInstanceRes, RouteContext(GetOwnerEntityUID(), GetRouteContext().GetFrom()), 
				m_Addr4, m_Addr6,
				(uint32_t)(DurationCast<DurationSec>(m_TimeStamp.time_since_epoch()).count()));
		};


		class GameEntityTransLeaveGameInstance : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::GameInstance::LeaveGameInstanceCmd>
		{
		public:
			using super = MessageTransaction<Svr::GameInstanceEntity, Message::GameInstance::LeaveGameInstanceCmd>;

		private:

		public:
			GameEntityTransLeaveGameInstance(IHeap& heap, MessageDataPtr& pIMsg) :super(heap, Forward<MessageDataPtr>(pIMsg)) {}
			virtual ~GameEntityTransLeaveGameInstance() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, LeaveGameInstanceRes, GetRouteContext().GetSwaped());
		};


		class GameEntityTransPlayerMovement : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::PlayerMovementC2SEvt>
		{
		public:
			using super = MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::PlayerMovementC2SEvt>;

		private:

		public:
			GameEntityTransPlayerMovement(IHeap& heap, MessageDataPtr& pIMsg) :super(heap, Forward<MessageDataPtr>(pIMsg)) {}
			virtual ~GameEntityTransPlayerMovement() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


} // namespace SF 

