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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {

	

	class MatchingTransGrabPlayer : public TransactionT<MatchingServiceQueueEntity, MatchingTransGrabPlayer, sizeof(TransactionMessageHandlerType) * 2>
	{
	public:
		typedef TransactionT<MatchingServiceQueueEntity, MatchingTransGrabPlayer, sizeof(TransactionMessageHandlerType) * 2> super;

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
		UINT m_MinQueueCount;
		UINT m_MaxQueueCount;

		// target queue member count
		UINT m_TargetQueueMemberCount;
		UINT m_TargetQueueComponentID;
		UINT m_MatchingMemberCount;
		PlayerRole m_RequestRole;

		//LONG m_RequestedTime;

	public:
		MatchingTransGrabPlayer(UINT matchingMemberCount, UINT targetQueueMemberCount, PlayerRole playerRole, UINT minQueueCount, UINT maxQueueCount);
		virtual ~MatchingTransGrabPlayer() {}

		////////////////////////////////////////////////////////////
		// Event handlers
		HRESULT OnTimer(TransactionResult* pRes);

		HRESULT ProcessGrabbing();
		HRESULT ReserveItem(UINT grabCount);
		HRESULT RequestDeleteItem(MatchingQueueTicket ticket);
		HRESULT OnReserveItem(TransactionResult* pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		virtual HRESULT OnCloseTransaction(HRESULT hrRes) override;
	};



	//class MatchingTransCancelPlayer : public Svr::TransactionT<MatchingServiceQueueEntity, MatchingTransCancelPlayer, sizeof(TransactionMessageHandlerType) * 2>
	//{
	//private:

	//	// target queue member count
	//	UINT m_TargetQueueMemberCount;
	//	UINT m_MatchingMemberCount;


	//public:
	//	MatchingTransCancelPlayer(UINT matchingMemberCount, UINT targetQueueMemberCount);
	//	virtual ~MatchingTransCancelPlayer() {}

	//	////////////////////////////////////////////////////////////
	//	// Event handlers
	//	HRESULT OnTimer(TransactionResult* pRes);


	//	HRESULT ProcessCanceling();
	//	HRESULT CancelReservation(MatchingQueueTicket ticket);
	//	HRESULT OnCancelReservation(TransactionResult* pRes);


	//	// Start Transaction
	//	virtual HRESULT StartTransaction();

	//};



	class MatchingTransProcessMatchedItems : public TransactionT<MasterEntity, MatchingTransProcessMatchedItems, sizeof(TransactionMessageHandlerType) * 7>
	{
	public:
		typedef TransactionT<MasterEntity, MatchingTransProcessMatchedItems, sizeof(TransactionMessageHandlerType) * 7> super;

	private:

		enum {
			MAX_MATCHING_ITEM	= 20,
			MAX_NUM_PLAYER		= 20
		};


		// Target matching member count
		BRCLASS_ATTRIBUTE(UINT, TargetMatchingMemberCount);

		// Current total member count
		BRCLASS_ATTRIBUTE(UINT, MatchedItemCount);

		INT m_PendingDequeueItem;

		UINT m_DequeuedTotalMembers;

		typedef MatchingServiceEntity::MatchingItem MatchingItem;
		typedef MatchingServiceEntity::ReservedMatchingItem ReservedMatchingItem;

		StaticArray<MatchingItem, MAX_MATCHING_ITEM> m_MatchedItems;

	public:
		MatchingTransProcessMatchedItems(UINT targetMatchingMemberCount, const Array<ReservedMatchingItem>& matchedItems);
		virtual ~MatchingTransProcessMatchedItems() {}

		////////////////////////////////////////////////////////////
		// Event handlers

		HRESULT DequeueItem(const MatchingQueueTicket& ticket);
		HRESULT OnDequeueItem(TransactionResult* pRes);

		HRESULT CreateGame();
		HRESULT OnCreateGame(TransactionResult* pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

	};






} // namespace GameServer 
} // namespace BR 

