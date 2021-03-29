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
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {

	
	class RegisterPartyMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPartyMatchingCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPartyMatchingCmd> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		RegisterPartyMatchingTrans(IHeap& memMgr, const MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~RegisterPartyMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyPartyMatchingQueue, RegisterPartyMatchingRes, GetRouteContext().GetSwaped(), m_MatchingTicket);
	};

	
	class RegisterPlayerMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPlayerMatchingCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPlayerMatchingCmd> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		RegisterPlayerMatchingTrans(IHeap& memMgr, const MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~RegisterPlayerMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;


		BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyPartyMatchingQueue, RegisterPlayerMatchingRes, GetRouteContext().GetSwaped(), m_MatchingTicket);
	};
	
	
	class UpdateMatchingEntityUIDTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd> super;

	private:

	public:
		UpdateMatchingEntityUIDTrans(IHeap& memMgr, const MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~UpdateMatchingEntityUIDTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;


		BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyPartyMatchingQueue, UpdateMatchingEntityUIDRes, GetRouteContext().GetSwaped());
	};


	class UnregisterMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UnregisterMatchingCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UnregisterMatchingCmd> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		UnregisterMatchingTrans(IHeap& memMgr, const MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~UnregisterMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyPartyMatchingQueue, UnregisterMatchingRes, GetRouteContext().GetSwaped());
	};


	
	
	class ReserveItemTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemCmd> super;

	private:
		uint m_NumPlayersInTheTicket;
		MatchingQueueTicket m_MatchingTicket;

	public:
		ReserveItemTrans(IHeap& memMgr, const MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~ReserveItemTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;


		BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyPartyMatchingQueue, ReserveItemRes, GetRouteContext().GetSwaped(), m_NumPlayersInTheTicket, m_MatchingTicket);
	};



	class MatchingQueueReserveItemsTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemsCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemsCmd> super;

	private:
		StaticArray<uint,40> m_NumPlayersInTheTicket;
		StaticArray<MatchingQueueTicket,40> m_MatchingTicket;

	public:
		MatchingQueueReserveItemsTrans(IHeap& memMgr, const MessageDataPtr &pIMsg)
			: ClusterEntityMessageTransaction(memMgr, pIMsg)
			, m_NumPlayersInTheTicket(memMgr)
			, m_MatchingTicket(memMgr)
		{ SetPrintTrace(false); }
		virtual ~MatchingQueueReserveItemsTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;


		BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyPartyMatchingQueue, ReserveItemsRes, GetRouteContext().GetSwaped(), m_NumPlayersInTheTicket, m_MatchingTicket);
	};


	
	class CancelReservationTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::CancelReservationCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::CancelReservationCmd> super;

	private:

	public:
		CancelReservationTrans(IHeap& memMgr, const MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~CancelReservationTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;

		
		BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyPartyMatchingQueue, CancelReservationRes, GetRouteContext().GetSwaped());
	};
	
	
	class DequeueItemTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::DequeueItemCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::DequeueItemCmd> super;

	private:
		MatchingQueueItem m_matchingQueueItem;

	public:
		DequeueItemTrans(IHeap& memMgr, const MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~DequeueItemTrans() {}

		// Start Transaction
		virtual Result StartTransaction() override;


		BR_SVR_MSGTRANS_CLOSE_ARGS(NetSvrPolicyPartyMatchingQueue, DequeueItemRes, GetRouteContext().GetSwaped(), GetMatchingTicket(), m_matchingQueueItem.RegisterUID, m_matchingQueueItem.RegisterID, ArrayView<MatchingPlayerInformation>((uint)countof(m_matchingQueueItem.Players), m_matchingQueueItem.NumPlayers, m_matchingQueueItem.Players));
	};



	class MatchingQueueTransMatchingItemError : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::MatchingItemErrorC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::MatchingItemErrorC2SEvt> super;

	private:

	public:
		MatchingQueueTransMatchingItemError(IHeap& memMgr, const MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(memMgr, pIMsg) { SetPrintTrace(false); }
		virtual ~MatchingQueueTransMatchingItemError() {}

		// Start Transaction
		virtual Result StartTransaction() override;

	};


} // namespace GameServer 
} // namespace SF 

