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
#include "Protocol/GameInstanceMsgClass.h"
#include "Protocol/GameInstanceNetPolicy.h"
#include "Protocol/PlayInstanceMsgClass.h"
#include "Protocol/PlayInstanceNetPolicy.h"

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
			GameEntityTransJoinGameInstance(IHeap& heap, const MessageDataPtr& pIMsg)
				: super(heap, pIMsg)
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
			GameEntityTransLeaveGameInstance(IHeap& heap, const MessageDataPtr& pIMsg) :super(heap, pIMsg) {}
			virtual ~GameEntityTransLeaveGameInstance() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyGameInstance, LeaveGameInstanceRes, GetRouteContext().GetSwaped());
		};


		class GameInstanceTransPlayerMovement : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::PlayerMovementC2SEvt>
		{
		public:
			using super = MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::PlayerMovementC2SEvt>;

		private:

		public:
			GameInstanceTransPlayerMovement(IHeap& heap, const MessageDataPtr& pIMsg) :super(heap, pIMsg) {}
			virtual ~GameInstanceTransPlayerMovement() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


		class GameInstanceTransClientSyncReliable : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::ClientSyncReliableC2SEvt>
		{
		public:
			using super = MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::ClientSyncReliableC2SEvt>;

		private:

		public:
			GameInstanceTransClientSyncReliable(IHeap& heap, const MessageDataPtr& pIMsg) :super(heap, pIMsg)
			{
				SetDirectProcess(true);
			}
			virtual ~GameInstanceTransClientSyncReliable() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};


		class GameInstanceTransClientSync : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::ClientSyncC2SEvt>
		{
		public:
			using super = MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::ClientSyncC2SEvt>;

		private:

		public:
			GameInstanceTransClientSync(IHeap& heap, const MessageDataPtr& pIMsg) :super(heap, pIMsg)
			{
				SetDirectProcess(true);
			}
			virtual ~GameInstanceTransClientSync() {}

			// Start Transaction
			virtual Result StartTransaction() override;
		};




		class GameInstanceTransOccupyMapObject : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::OccupyMapObjectCmd>
		{
		public:
			using super = MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::OccupyMapObjectCmd>;

		private:

		public:
			GameInstanceTransOccupyMapObject(IHeap& heap, const MessageDataPtr& pIMsg) :super(heap, pIMsg) {}
			virtual ~GameInstanceTransOccupyMapObject() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyPlayInstance, OccupyMapObjectRes, GetPlayInstanceUID(), GetPlayerID(), GetMapObjectId());
		};

		class GameInstanceTransUnoccupyMapObject : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::UnoccupyMapObjectCmd>
		{
		public:
			using super = MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::UnoccupyMapObjectCmd>;

		private:

		public:
			GameInstanceTransUnoccupyMapObject(IHeap& heap, const MessageDataPtr& pIMsg) :super(heap, pIMsg) {}
			virtual ~GameInstanceTransUnoccupyMapObject() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyPlayInstance, UnoccupyMapObjectRes, GetPlayInstanceUID(), GetPlayerID(), GetMapObjectId());
		};

		class GameInstanceTransUseMapObject : public Svr::MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::UseMapObjectCmd>
		{
		public:
			using super = MessageTransaction<Svr::GameInstanceEntity, Message::PlayInstance::UseMapObjectCmd>;

		private:
			VariableTable m_ResultAttributes;

		public:
			GameInstanceTransUseMapObject(IHeap& heap, const MessageDataPtr& pIMsg)
				: super(heap, pIMsg)
				, m_ResultAttributes(heap)
			{}
			virtual ~GameInstanceTransUseMapObject() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyPlayInstance, UseMapObjectRes, GetPlayInstanceUID(), GetPlayerID(), GetMapObjectId(), m_ResultAttributes);
		};


} // namespace SF 
