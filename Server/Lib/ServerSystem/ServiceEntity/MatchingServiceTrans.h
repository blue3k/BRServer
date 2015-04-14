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
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/Policy/ClusterServerIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {

	

	class MatchingTransGrabPlayer : public Svr::TransactionT<MatchingServiceQueueEntity, MatchingTransGrabPlayer, sizeof(TransactionMessageHandlerType) * 2>
	{
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
		UINT m_MatchingMemberCount;

		//LONG m_RequestedTime;

	public:
		MatchingTransGrabPlayer(UINT matchingMemberCount, UINT targetQueueMemberCount, UINT minQueueCount, UINT maxQueueCount);
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



	class MatchingTransProcessMatchedItems : public Svr::TransactionT<MasterEntity, MatchingTransProcessMatchedItems, sizeof(TransactionMessageHandlerType) * 7>
	{
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




	class MatchingPartyTrans : public Svr::TransactionT<MatchingServiceEntity, MatchingPartyTrans, sizeof(TransactionMessageHandlerType)*7>
	{
	private:

		enum {
			MAX_GRAB_TRY = 15,				// Big number is ok because we are going to use bot
			MAX_NUM_PLAYER = 20,

			GRAB_RETRY_TIME = 400,			// Small number will be enable bot match often
			GRAB_TRY_TIMEOUT = 7 * 1000,	// Bot will be matched after this limit
		};

		enum class Step {
			Grabbing,
			Dequeuing,
			Canceling,
			Matched,
			CreateGame,
		} m_Step;


		ULONG m_MatchingTryTimeOut;

		BRCLASS_ATTRIBUTE(UINT,QueryMemberCount);
		// Target matching member count
		BRCLASS_ATTRIBUTE(UINT,TargetMemberCount);
		// Current total member count
		BRCLASS_ATTRIBUTE(UINT,CurrentMemberCount);

		BRCLASS_ATTRIBUTE(INT,CurrentGrabbing);
		BRCLASS_ATTRIBUTE(UINT,GrabTryCount);

		BRCLASS_ATTRIBUTE(INT,ProcessingIndex);

		struct ReservedMember {
			MatchingQueueTicket			ReservationTicket;
			EntityUID					RegisterEntityUID;
			UINT						MemberCount;
			MatchingPlayerInformation	Players[MAX_NUM_PLAYER];

			ReservedMember()
				:MemberCount(0)
			{
			}
		};
		StaticArray<ReservedMember,MAX_NUM_PLAYER> m_ReservedMember;

	public:
		MatchingPartyTrans(UINT startMemberCount, UINT targetMemberCount);
		virtual ~MatchingPartyTrans() {}

		////////////////////////////////////////////////////////////
		// Event handlers
		HRESULT OnTimer(TransactionResult* pRes);

		HRESULT ProcessGrabbing();
		HRESULT ReserveItem(UINT memberCount);
		HRESULT OnReserveItem(TransactionResult* pRes);

		HRESULT ProcessCanceling();
		HRESULT CancelReservation( MatchingQueueTicket ticket );
		HRESULT OnCancelReservation(TransactionResult* pRes);

		HRESULT ProcessDequeuing();
		HRESULT DequeueItem(MatchingQueueTicket ticket);
		HRESULT OnDequeueItem(TransactionResult* pRes);

		HRESULT CreateGame();
		HRESULT OnCreateGame(TransactionResult* pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		////////////////////////////////////////////////////////////
		// Helpers
		UINT GetClusterComponentIDFromMemberCount( UINT count );
	};





} // namespace GameServer 
} // namespace BR 

