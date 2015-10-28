////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "Common/BrSvrTypes.h"
#include "Common/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MasterEntity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"


namespace BR {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class MatchingServiceEntity : public ShardedClusterServiceEntity
	{
	public:

		enum { 
			MIN_TRANSACTIONS			= 1,
			//MAX_NUMBER_OF_KEYS		= ((UINT)ClusterID::MatchingQueue_Game_11 - (UINT)ClusterID::MatchingQueue_Game_1) + 1,

			TIME_START_MATCHING			= 5*1000,
			TIME_REMATCHING_ONFAIL		= 2*1000,

			TIME_BOTMATCHING_WAIT		= 20*1000,

			MAX_NUM_PLAYER				= 20,
			MAX_PREPARED_PLAYER			= 40,

			MIN_ITEM_RESERVATION		= 16,
			MAX_ITEM_RESERVATION		= 24,

			MAX_QUEUE_COUNT				= 11,
		};


		// Reserved item type
		struct ReservedMatchingItem {
			MatchingQueueTicket			MatchingTicket;
			UINT						MemberCount;
			PlayerRole                  RequestedRole;

			ReservedMatchingItem()
				: MemberCount(0)
				, RequestedRole(PlayerRole::None)
			{
			}

			bool operator == (const ReservedMatchingItem& src) const { return MatchingTicket == src.MatchingTicket; }
			bool operator == (void* src) const { assert(src == nullptr);  return MatchingTicket == 0; }
			bool operator != (void* src) const { assert(src == nullptr);  return MatchingTicket != 0; }

			ReservedMatchingItem& operator = (const ReservedMatchingItem& src);
			ReservedMatchingItem& operator = (ReservedMatchingItem&& src);
			ReservedMatchingItem& operator = (void* src) { assert(src == nullptr); MatchingTicket = 0;  MemberCount = 0;  return *this; }

		};

		// Reserved matching queue
		class MatchingQueueInterface
		{
		public:
			MatchingQueueInterface() {}
			virtual ~MatchingQueueInterface() {}

			virtual void ResetForMatch() {}
			virtual HRESULT Dequeue(Array<ReservedMatchingItem>& items, UINT numDequeue) = 0;
			virtual UINT GetEnqueueCount() = 0;
		};

		// single query wraper
		class MatchingQueue_Single : public MatchingQueueInterface
		{
		private:
			PageQueue<ReservedMatchingItem>* m_pQueuePtr;

		public:

			//MatchingQueue_Single();
			MatchingQueue_Single(PageQueue<ReservedMatchingItem>* pQueuePtr);

			virtual HRESULT Dequeue(Array<ReservedMatchingItem>& items, UINT numDequeue) override;
			virtual UINT GetEnqueueCount() override;
		};

		// Multiple queue with matching count balance
		class MatchingQueue_Multiple : public MatchingQueueInterface
		{
		public:

			struct QueueItem
			{
				PageQueue<ReservedMatchingItem>* pQueue;
				UINT                             MaxAllowPerMatch;
				PlayerRole                       RequestRole;


				QueueItem()
					: pQueue(nullptr)
					, MaxAllowPerMatch(0)
					, RequestRole(PlayerRole::None)
				{
				}


				bool operator == (const QueueItem& src) const;

				QueueItem& operator = (QueueItem&& src);

				static const QueueItem NullValue;
			};


		private:
			StaticArray<QueueItem, MAX_QUEUE_COUNT> m_pQueuePtr;
			UINT m_StartQueueIndex;

		public:

			MatchingQueue_Multiple();

			HRESULT AddQueue(PageQueue<ReservedMatchingItem>* pQueuePtr, UINT maxPerMatch, PlayerRole requestRole);

			virtual void ResetForMatch() override;
			virtual HRESULT Dequeue(Array<ReservedMatchingItem>& items, UINT numDequeue) override;
			virtual UINT GetEnqueueCount() override;
		};


		struct MatchingItem {
			MatchingQueueTicket			MatchingTicket;
			EntityUID					RegisterEntityUID;
			PlayerRole                  RequestedRole;
			UINT						MemberCount;
			MatchingPlayerInformation	Players[MAX_NUM_PLAYER];

			MatchingItem()
				:MemberCount(0), RequestedRole(PlayerRole::None)
			{
			}

			bool operator == (const MatchingItem& src) const { return MatchingTicket == src.MatchingTicket; }

			MatchingItem& operator = (const MatchingItem& src);
			MatchingItem& operator = (MatchingItem&& src);

			static const MatchingItem NullValue;
		};

	private:

		// Matching target member count
		BRCLASS_ATTRIBUTE_READONLY(UINT, TargetMatchingMemberCount);
		BRCLASS_ATTRIBUTE_READONLY(UINT, MaxMatchingQueue);

		BRCLASS_ATTRIBUTE_READONLY(bool, IsUseBot);

		TimeStampMS m_WaitingBotMatchingStart;

		// Reservation state
		UINT m_ReservationStartFrom;

		WeakPointerT<Entity> m_pQueueEntity;
		StaticArray<MatchingQueueInterface*, MAX_QUEUE_COUNT> m_MatchingReserevedQueues;

		PerformanceCounterRaw<UINT64> m_ItemCounts[MAX_QUEUE_COUNT];

		PerformanceCounterTickPerSec<INT64> m_MatchedCount;

	protected:
		HRESULT GetMatchingPatterTable(UINT targetMatchingMemberCount, UINT& numTableEntries, const UINT* &pTable);

		HRESULT UpdateMatching();
		HRESULT UpdateBotMatching();

		UINT GetGrabbedPlayerCount(Array<ReservedMatchingItem>& grabbedItems);

	public:

		MatchingServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, const char* parameters = nullptr );
		~MatchingServiceEntity();

		// We are not going to use hashed key
		virtual UINT KeyHash( UINT64 key ) { return (UINT)key; }

		
		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual HRESULT InitializeEntity(EntityID newEntityID) override;

		// clear transaction
		virtual HRESULT ClearEntity() override;

		// TickUpdate 
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;

	};




	class MatchingWatcherServiceEntity : public RingClusterServiceEntity, public IServerComponent
	{
	public:
		MatchingWatcherServiceEntity( ClusterID clusterID, UINT componentID );
		~MatchingWatcherServiceEntity();
	};




	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	QueueCacheEntity class
	//

	class MatchingServiceQueueEntity : public MasterEntity
	{
	public:
		enum {
			MAX_QUEUE_COUNT = 11,
		};

	private:

		// cached members by member queue
		PageQueue<MatchingServiceEntity::ReservedMatchingItem> m_ReservedItemQueue[MAX_QUEUE_COUNT];

		INT m_ReservedItemQueueTransaction[MAX_QUEUE_COUNT];

		// Target enqueue member count
		UINT m_MinQueueCount;
		UINT m_MaxQueueCount;

		// target queue member count
		UINT m_MatchingMemberCount;



	public:
		MatchingServiceQueueEntity(UINT matchingMemberCount, UINT minQueueCount, UINT maxQueueCount);
		~MatchingServiceQueueEntity();

		MemoryAllocator& GetAllocator() { return STDAllocator::GetInstance(); }

		UINT GetMinQueueCount()															{ return m_MinQueueCount; }
		UINT GetMaxQueueCount()															{ return m_MaxQueueCount; }
		PageQueue<MatchingServiceEntity::ReservedMatchingItem>& GetReservedItemQueue(UINT iMemberCount, PlayerRole requestRole);

		void TransactionStartedForQueue(UINT iMemberCount)								{ m_ReservedItemQueueTransaction[iMemberCount]++; }
		void TransactionClosedForQueue(UINT iMemberCount)								{ m_ReservedItemQueueTransaction[iMemberCount]--; if (m_ReservedItemQueueTransaction[iMemberCount] < 0) m_ReservedItemQueueTransaction[iMemberCount] = 0; }
		INT GetTransactionStartedForQueue(UINT iMemberCount)							{ return m_ReservedItemQueueTransaction[iMemberCount]; }

		virtual HRESULT InitializeEntity(EntityID newEntityID) override;

		// TickUpdate 
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr) override;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MatchingServiceHandleMatchedEntity class
	//

	class MatchingServiceHandleMatchedEntity : public MasterEntity
	{
	public:

	public:
		MatchingServiceHandleMatchedEntity();
		~MatchingServiceHandleMatchedEntity();

		virtual HRESULT InitializeEntity(EntityID newEntityID) override;
	};



}; // namespace Svr
}; // namespace BR




