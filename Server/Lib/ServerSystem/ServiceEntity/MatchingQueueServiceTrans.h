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
#include "Memory/MemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/Message.h"
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
		RegisterPartyMatchingTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~RegisterPartyMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyPartyMatchingQueue, RegisterPartyMatchingRes, GetRouteContext().GetSwaped(), m_MatchingTicket);
	};

	
	class RegisterPlayerMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPlayerMatchingCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPlayerMatchingCmd> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		RegisterPlayerMatchingTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~RegisterPlayerMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();


		BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyPartyMatchingQueue, RegisterPlayerMatchingRes, GetRouteContext().GetSwaped(), m_MatchingTicket);
	};
	
	
	class UpdateMatchingEntityUIDTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd> super;

	private:

	public:
		UpdateMatchingEntityUIDTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~UpdateMatchingEntityUIDTrans() {}

		// Start Transaction
		virtual Result StartTransaction();


		BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyPartyMatchingQueue, UpdateMatchingEntityUIDRes, GetRouteContext().GetSwaped());
	};


	class UnregisterMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UnregisterMatchingCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UnregisterMatchingCmd> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		UnregisterMatchingTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~UnregisterMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyPartyMatchingQueue, UnregisterMatchingRes, GetRouteContext().GetSwaped());
	};


	
	
	class ReserveItemTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemCmd> super;

	private:
		uint m_NumPlayersInTheTicket;
		MatchingQueueTicket m_MatchingTicket;

	public:
		ReserveItemTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~ReserveItemTrans() {}

		// Start Transaction
		virtual Result StartTransaction();


		BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyPartyMatchingQueue, ReserveItemRes, GetRouteContext().GetSwaped(), m_NumPlayersInTheTicket, m_MatchingTicket);
	};



	class MatchingQueueReserveItemsTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemsCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemsCmd> super;

	private:
		StaticArray<uint,40> m_NumPlayersInTheTicket;
		StaticArray<MatchingQueueTicket,40> m_MatchingTicket;

	public:
		MatchingQueueReserveItemsTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg)
			: ClusterEntityMessageTransaction(memMgr, pIMsg)
			, m_NumPlayersInTheTicket(memMgr)
			, m_MatchingTicket(memMgr)
		{ SetPrintTrace(false); }
		virtual ~MatchingQueueReserveItemsTrans() {}

		// Start Transaction
		virtual Result StartTransaction();


		BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyPartyMatchingQueue, ReserveItemsRes, GetRouteContext().GetSwaped(), m_NumPlayersInTheTicket, m_MatchingTicket);
	};


	
	class CancelReservationTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::CancelReservationCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::CancelReservationCmd> super;

	private:

	public:
		CancelReservationTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~CancelReservationTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		
		BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyPartyMatchingQueue, CancelReservationRes, GetRouteContext().GetSwaped());
	};
	
	
	class DequeueItemTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::DequeueItemCmd>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::DequeueItemCmd> super;

	private:
		MatchingQueueItem m_matchingQueueItem;

	public:
		DequeueItemTrans(IMemoryManager& memMgr, MessageDataPtr &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~DequeueItemTrans() {}

		// Start Transaction
		virtual Result StartTransaction();


		BR_SVR_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyPartyMatchingQueue, DequeueItemRes, GetRouteContext().GetSwaped(), GetMatchingTicket(), m_matchingQueueItem.RegisterUID, m_matchingQueueItem.RegisterID, LinkedArray<MatchingPlayerInformation>((uint)countof(m_matchingQueueItem.Players), m_matchingQueueItem.NumPlayers, m_matchingQueueItem.Players));
	};



	class MatchingQueueTransMatchingItemError : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::MatchingItemErrorC2SEvt>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::MatchingItemErrorC2SEvt> super;

	private:

	public:
		MatchingQueueTransMatchingItemError(IMemoryManager& memMgr, MessageDataPtr &pIMsg) : ClusterEntityMessageTransaction(memMgr, pIMsg) { SetPrintTrace(false); }
		virtual ~MatchingQueueTransMatchingItemError() {}

		// Start Transaction
		virtual Result StartTransaction();

	};


} // namespace GameServer 
} // namespace SF 

