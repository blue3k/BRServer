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
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {

	
	class RegisterPartyMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPartyMatchingCmd, RegisterPartyMatchingTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPartyMatchingCmd, RegisterPartyMatchingTrans, 1> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		RegisterPartyMatchingTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~RegisterPartyMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyPartyMatchingQueue* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(RegisterPartyMatchingRes, GetRouteContext().GetSwaped(), m_MatchingTicket);
	};

	
	class RegisterPlayerMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPlayerMatchingCmd, RegisterPlayerMatchingTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPlayerMatchingCmd, RegisterPlayerMatchingTrans, 1> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		RegisterPlayerMatchingTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~RegisterPlayerMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyPartyMatchingQueue* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(RegisterPlayerMatchingRes, GetRouteContext().GetSwaped(), m_MatchingTicket);
	};
	
	
	class UpdateMatchingEntityUIDTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd, UpdateMatchingEntityUIDTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd, UpdateMatchingEntityUIDTrans, 1> super;

	private:

	public:
		UpdateMatchingEntityUIDTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~UpdateMatchingEntityUIDTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyPartyMatchingQueue* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE(UpdateMatchingEntityUIDRes, GetRouteContext().GetSwaped());
	};


	class UnregisterMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UnregisterMatchingCmd, UnregisterMatchingTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UnregisterMatchingCmd, UnregisterMatchingTrans, 1> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		UnregisterMatchingTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) {}
		virtual ~UnregisterMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyPartyMatchingQueue* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE(UnregisterMatchingRes, GetRouteContext().GetSwaped());
	};


	
	
	class ReserveItemTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemCmd, ReserveItemTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemCmd, ReserveItemTrans, 1> super;

	private:
		UINT m_NumPlayersInTheTicket;
		MatchingQueueTicket m_MatchingTicket;

	public:
		ReserveItemTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) { SetPrintTrace(false); }
		virtual ~ReserveItemTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyPartyMatchingQueue* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(ReserveItemRes, GetRouteContext().GetSwaped(), m_NumPlayersInTheTicket, m_MatchingTicket);
	};



	class MatchingQueueReserveItemsTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemsCmd, MatchingQueueReserveItemsTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemsCmd, MatchingQueueReserveItemsTrans, 1> super;

	private:
		StaticArray<UINT,40> m_NumPlayersInTheTicket;
		StaticArray<MatchingQueueTicket,40> m_MatchingTicket;

	public:
		MatchingQueueReserveItemsTrans(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) { SetPrintTrace(false); }
		virtual ~MatchingQueueReserveItemsTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyPartyMatchingQueue* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(ReserveItemsRes, GetRouteContext().GetSwaped(), m_NumPlayersInTheTicket, m_MatchingTicket);
	};


	
	class CancelReservationTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::CancelReservationCmd, CancelReservationTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::CancelReservationCmd, CancelReservationTrans, 1> super;

	private:

	public:
		CancelReservationTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) { SetPrintTrace(false); }
		virtual ~CancelReservationTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyPartyMatchingQueue* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE(CancelReservationRes, GetRouteContext().GetSwaped());
	};
	
	
	class DequeueItemTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::DequeueItemCmd, DequeueItemTrans, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::DequeueItemCmd, DequeueItemTrans, 1> super;

	private:
		MatchingQueueItem m_matchingQueueItem;

	public:
		DequeueItemTrans( Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( pIMsg ) { SetPrintTrace(false); }
		virtual ~DequeueItemTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyPartyMatchingQueue* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(DequeueItemRes, GetRouteContext().GetSwaped(), GetMatchingTicket(), m_matchingQueueItem.RegisterUID, m_matchingQueueItem.RegisterID, LinkedArray<MatchingPlayerInformation>((UINT)countof(m_matchingQueueItem.Players), m_matchingQueueItem.NumPlayers, m_matchingQueueItem.Players));
	};



	class MatchingQueueTransMatchingItemError : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::MatchingItemErrorC2SEvt, MatchingQueueTransMatchingItemError, 1>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::MatchingItemErrorC2SEvt, MatchingQueueTransMatchingItemError, 1> super;

	private:

	public:
		MatchingQueueTransMatchingItemError(Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(pIMsg) { SetPrintTrace(false); }
		virtual ~MatchingQueueTransMatchingItemError() {}

		// Start Transaction
		virtual Result StartTransaction();

	};


} // namespace GameServer 
} // namespace BR 

