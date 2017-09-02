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
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace SF {
namespace Svr {

	
	class RegisterPartyMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPartyMatchingCmd, RegisterPartyMatchingTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPartyMatchingCmd, RegisterPartyMatchingTrans> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		RegisterPartyMatchingTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~RegisterPartyMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyPartyMatchingQueue* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(RegisterPartyMatchingRes, GetRouteContext().GetSwaped(), m_MatchingTicket);
	};

	
	class RegisterPlayerMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPlayerMatchingCmd, RegisterPlayerMatchingTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::RegisterPlayerMatchingCmd, RegisterPlayerMatchingTrans> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		RegisterPlayerMatchingTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~RegisterPlayerMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyPartyMatchingQueue* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(RegisterPlayerMatchingRes, GetRouteContext().GetSwaped(), m_MatchingTicket);
	};
	
	
	class UpdateMatchingEntityUIDTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd, UpdateMatchingEntityUIDTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd, UpdateMatchingEntityUIDTrans> super;

	private:

	public:
		UpdateMatchingEntityUIDTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~UpdateMatchingEntityUIDTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyPartyMatchingQueue* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE(UpdateMatchingEntityUIDRes, GetRouteContext().GetSwaped());
	};


	class UnregisterMatchingTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UnregisterMatchingCmd, UnregisterMatchingTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::UnregisterMatchingCmd, UnregisterMatchingTrans> super;

	private:
		MatchingQueueTicket m_MatchingTicket;

	public:
		UnregisterMatchingTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) {}
		virtual ~UnregisterMatchingTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyPartyMatchingQueue* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE(UnregisterMatchingRes, GetRouteContext().GetSwaped());
	};


	
	
	class ReserveItemTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemCmd, ReserveItemTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemCmd, ReserveItemTrans> super;

	private:
		uint m_NumPlayersInTheTicket;
		MatchingQueueTicket m_MatchingTicket;

	public:
		ReserveItemTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~ReserveItemTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyPartyMatchingQueue* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(ReserveItemRes, GetRouteContext().GetSwaped(), m_NumPlayersInTheTicket, m_MatchingTicket);
	};



	class MatchingQueueReserveItemsTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemsCmd, MatchingQueueReserveItemsTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::ReserveItemsCmd, MatchingQueueReserveItemsTrans> super;

	private:
		StaticArray<uint,40> m_NumPlayersInTheTicket;
		StaticArray<MatchingQueueTicket,40> m_MatchingTicket;

	public:
		MatchingQueueReserveItemsTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(memMgr, pIMsg) { SetPrintTrace(false); }
		virtual ~MatchingQueueReserveItemsTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyPartyMatchingQueue* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(ReserveItemsRes, GetRouteContext().GetSwaped(), m_NumPlayersInTheTicket, m_MatchingTicket);
	};


	
	class CancelReservationTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::CancelReservationCmd, CancelReservationTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::CancelReservationCmd, CancelReservationTrans> super;

	private:

	public:
		CancelReservationTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~CancelReservationTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyPartyMatchingQueue* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE(CancelReservationRes, GetRouteContext().GetSwaped());
	};
	
	
	class DequeueItemTrans : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::DequeueItemCmd, DequeueItemTrans>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::DequeueItemCmd, DequeueItemTrans> super;

	private:
		MatchingQueueItem m_matchingQueueItem;

	public:
		DequeueItemTrans(IMemoryManager& memMgr, Message::MessageData* &pIMsg ) : ClusterEntityMessageTransaction( memMgr, pIMsg ) { SetPrintTrace(false); }
		virtual ~DequeueItemTrans() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::NetSvrPolicyPartyMatchingQueue* GetInterface()	{ return ServerEntityMessageTransaction::GetInterface<Policy::NetSvrPolicyPartyMatchingQueue>(); }

		BR_SVR_MSGTRANS_CLOSE_ARGS(DequeueItemRes, GetRouteContext().GetSwaped(), GetMatchingTicket(), m_matchingQueueItem.RegisterUID, m_matchingQueueItem.RegisterID, LinkedArray<MatchingPlayerInformation>((uint)countof(m_matchingQueueItem.Players), m_matchingQueueItem.NumPlayers, m_matchingQueueItem.Players));
	};



	class MatchingQueueTransMatchingItemError : public ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::MatchingItemErrorC2SEvt, MatchingQueueTransMatchingItemError>
	{
	public:
		typedef ClusterEntityMessageTransaction< MatchingQueueServiceEntity, Message::PartyMatchingQueue::MatchingItemErrorC2SEvt, MatchingQueueTransMatchingItemError> super;

	private:

	public:
		MatchingQueueTransMatchingItemError(IMemoryManager& memMgr, Message::MessageData* &pIMsg) : ClusterEntityMessageTransaction(memMgr, pIMsg) { SetPrintTrace(false); }
		virtual ~MatchingQueueTransMatchingItemError() {}

		// Start Transaction
		virtual Result StartTransaction();

	};


} // namespace GameServer 
} // namespace SF 

