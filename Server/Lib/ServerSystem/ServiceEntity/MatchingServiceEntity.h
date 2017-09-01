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


#include "SFTypedefs.h"
#include "Common/ClassUtil.h"
#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MasterEntity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"

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

			TIME_BOTMATCHING_WAIT		= 5*1000,

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

			ReservedMatchingItem(void* ptr)
				: MemberCount(0)
				, RequestedRole(PlayerRole::None)
			{
				assert(ptr == nullptr);
			}

			ReservedMatchingItem()
				: MemberCount(0)
				, RequestedRole(PlayerRole::None)
			{
			}

			ReservedMatchingItem(const ReservedMatchingItem& src)
				: MemberCount(0)
				, RequestedRole(PlayerRole::None)
			{
				MatchingTicket = src.MatchingTicket;
				MemberCount = src.MemberCount;
				RequestedRole = src.RequestedRole;
			}

			bool operator == (const ReservedMatchingItem& src) const { return MatchingTicket == src.MatchingTicket; }
			bool operator != (const ReservedMatchingItem& src) const { return MatchingTicket != src.MatchingTicket; }

			ReservedMatchingItem& operator = (const ReservedMatchingItem& src);
			ReservedMatchingItem& operator = (ReservedMatchingItem&& src);

		};

		// Reserved matching queue
		class MatchingQueueInterface
		{
		public:
			MatchingQueueInterface() {}
			virtual ~MatchingQueueInterface() {}

			virtual void ResetForMatch() {}
			virtual Result Dequeue(Array<ReservedMatchingItem>& items) = 0;
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

			virtual Result Dequeue(Array<ReservedMatchingItem>& items) override;
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

			Result AddQueue(PageQueue<ReservedMatchingItem>* pQueuePtr, UINT maxPerMatch, PlayerRole requestRole);

			virtual void ResetForMatch() override;
			virtual Result Dequeue(Array<ReservedMatchingItem>& items) override;
			virtual UINT GetEnqueueCount() override;
		};


		struct MatchingItem {
			MatchingQueueTicket			MatchingTicket;
			EntityUID					RegisterEntityUID;
			PlayerRole                  RequestedRole;
			UINT						MemberCount;
			MatchingPlayerInformation	Players[MAX_NUM_PLAYER];

			MatchingItem(void* ptr = nullptr)
				: RequestedRole(PlayerRole::None)
				, MemberCount(0)
			{
				assert(ptr == nullptr);
			}

			bool operator == (const MatchingItem& src) const { return MatchingTicket == src.MatchingTicket; }

			MatchingItem& operator = (const MatchingItem& src);
			MatchingItem& operator = (MatchingItem&& src);
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

		PerformanceCounterRaw<uint64_t> m_ItemCounts[MAX_QUEUE_COUNT];

		PerformanceCounterTickPerSec<int64_t> m_MatchedCount;

	protected:
		Result GetMatchingPatterTable(UINT targetMatchingMemberCount, UINT& numTableEntries, const UINT* &pTable);

		Result UpdateMatching();
		Result UpdateBotMatching();

		UINT GetGrabbedPlayerCount(Array<ReservedMatchingItem>& grabbedItems);

	public:

		MatchingServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher, bool useBot = true );
		~MatchingServiceEntity();

		// We are not going to use hashed key
		virtual UINT KeyHash( uint64_t key ) { return (UINT)key; }

		
		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		virtual Result InitializeEntity(EntityID newEntityID) override;

		// clear transaction
		virtual Result ClearEntity() override;

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;

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

		// target queue member count
		UINT m_MatchingMemberCount;

		// Target enqueue member count
		UINT m_MinQueueCount;
		UINT m_MaxQueueCount;



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

		virtual Result InitializeEntity(EntityID newEntityID) override;

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;
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

		virtual Result InitializeEntity(EntityID newEntityID) override;
	};



}; // namespace Svr
}; // namespace BR




