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
#include "Protocol/ClusterServerMsgClass.h"
#include "Protocol/ClusterServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
namespace Svr {

	

	class MatchingTransGrabPlayer : public TransactionT<MatchingServiceQueueEntity>
	{
	public:
		typedef TransactionT<MatchingServiceQueueEntity> super;

	private:

		enum {
			GRAB_MAX = 200,

			TICK_TIME = 100,
#ifdef DEBUG
			GRAB_RETRY_TIME = 7000,
#else
			GRAB_RETRY_TIME = 400,
#endif
		};

		typedef MatchingServiceEntity::ReservedMatchingItem ReservedMatchingItem;

		// Target enqueue member count
		uint m_MinQueueCount;
		uint m_MaxQueueCount;

		// target queue member count
		uint m_TargetQueueMemberCount;
		ClusterID m_TargetQueueClusterID = ClusterID::None;
		uint m_MatchingMemberCount;
		PlayerRole m_RequestRole;

		//long m_RequestedTime;

	public:
		MatchingTransGrabPlayer(IHeap& memoryManager, uint matchingMemberCount, uint targetQueueMemberCount, PlayerRole playerRole, uint minQueueCount, uint maxQueueCount);
		virtual ~MatchingTransGrabPlayer() {}

		////////////////////////////////////////////////////////////
		// Event handlers
		Result OnTimer(TransactionResult* pRes);

		Result ProcessGrabbing();
		Result ReserveItem(uint grabCount);
		Result RequestDeleteItem(MatchingQueueTicket ticket);
		Result OnReserveItem(TransactionResult* pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		virtual Result OnCloseTransaction(Result hrRes) override;
	};



	//class MatchingTransCancelPlayer : public Svr::TransactionT<MatchingServiceQueueEntity, MatchingTransCancelPlayer, sizeof(TransactionMessageHandlerType) * 2>
	//{
	//private:

	//	// target queue member count
	//	uint m_TargetQueueMemberCount;
	//	uint m_MatchingMemberCount;


	//public:
	//	MatchingTransCancelPlayer(uint matchingMemberCount, uint targetQueueMemberCount);
	//	virtual ~MatchingTransCancelPlayer() {}

	//	////////////////////////////////////////////////////////////
	//	// Event handlers
	//	Result OnTimer(TransactionResult* pRes);


	//	Result ProcessCanceling();
	//	Result CancelReservation(MatchingQueueTicket ticket);
	//	Result OnCancelReservation(TransactionResult* pRes);


	//	// Start Transaction
	//	virtual Result StartTransaction();

	//};



	class MatchingTransProcessMatchedItems : public TransactionT<MasterEntity>
	{
	public:
		typedef TransactionT<MasterEntity> super;

	private:

		enum {
			MAX_MATCHING_ITEM	= 20,
			MAX_NUM_PLAYER		= 20
		};


		// Target matching member count
		uint m_TargetMatchingMemberCount;

		// Current total member count
		uint m_MatchedItemCount;

		int m_PendingDequeueItem;

		uint m_DequeuedTotalMembers;

		typedef MatchingServiceEntity::MatchingItem MatchingItem;
		typedef MatchingServiceEntity::ReservedMatchingItem ReservedMatchingItem;

		StaticArray<MatchingItem, MAX_MATCHING_ITEM> m_MatchedItems;

	public:
		MatchingTransProcessMatchedItems(IHeap& memoryManager, uint targetMatchingMemberCount, const Array<ReservedMatchingItem>& matchedItems);
		virtual ~MatchingTransProcessMatchedItems() {}

		uint GetTargetMatchingMemberCount() { return m_TargetMatchingMemberCount; }

		uint GetMatchedItemCount() { return m_MatchedItemCount; }


		////////////////////////////////////////////////////////////
		// Event handlers

		Result DequeueItem(const MatchingQueueTicket& ticket);
		Result OnDequeueItem(TransactionResult* pRes);

		Result CreateGame();
		Result OnCreateGame(TransactionResult* pRes);

		// Start Transaction
		virtual Result StartTransaction();

	};






} // namespace GameServer 
} // namespace SF 

