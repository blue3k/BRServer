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

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Entity/MasterEntity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "PerformanceCounter/PerformanceCounter.h"


namespace SF {
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
			//MAX_NUMBER_OF_KEYS		= ((uint)ClusterID::MatchingQueue_Game_11 - (uint)ClusterID::MatchingQueue_Game_1) + 1,

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
			uint						MemberCount;
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
			virtual uint GetEnqueueCount() = 0;
		};

		// single query wrapper
		class MatchingQueue_Single : public MatchingQueueInterface
		{
		private:
			PageQueue<ReservedMatchingItem>* m_pQueuePtr = nullptr;

		public:

			//MatchingQueue_Single();
			MatchingQueue_Single(PageQueue<ReservedMatchingItem>* pQueuePtr);

			virtual Result Dequeue(Array<ReservedMatchingItem>& items) override;
			virtual uint GetEnqueueCount() override;
		};

		// Multiple queue with matching count balance
		class MatchingQueue_Multiple : public MatchingQueueInterface
		{
		public:

			struct QueueItem
			{
				PageQueue<ReservedMatchingItem>* pQueue;
				uint                             MaxAllowPerMatch;
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
			IHeap& m_Heap;
			StaticArray<QueueItem, MAX_QUEUE_COUNT> m_pQueuePtr;
			uint m_StartQueueIndex;

		public:

			MatchingQueue_Multiple(IHeap& memMgr);

			IHeap& GetHeap() { return m_Heap; }

			Result AddQueue(PageQueue<ReservedMatchingItem>* pQueuePtr, uint maxPerMatch, PlayerRole requestRole);

			virtual void ResetForMatch() override;
			virtual Result Dequeue(Array<ReservedMatchingItem>& items) override;
			virtual uint GetEnqueueCount() override;
		};


		struct MatchingItem {
			MatchingQueueTicket			MatchingTicket;
			EntityUID					RegisterEntityUID;
			PlayerRole                  RequestedRole;
			uint						MemberCount;
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
		uint m_TargetMatchingMemberCount = 0;
		uint m_MaxMatchingQueue = 0;

		bool m_IsUseBot;

		TimeStampMS m_WaitingBotMatchingStart;

		// Reservation state
		uint m_ReservationStartFrom = 0;

		WeakPointerT<Entity> m_pQueueEntity;
		StaticArray<MatchingQueueInterface*, MAX_QUEUE_COUNT> m_MatchingReserevedQueues;

		PerformanceCounterRaw<uint64_t> m_ItemCounts[MAX_QUEUE_COUNT];

		PerformanceCounterTickPerSec<int64_t> m_MatchedCount;

	protected:
		Result GetMatchingPatterTable(uint targetMatchingMemberCount, uint& numTableEntries, const uint* &pTable);

		Result UpdateMatching();
		Result UpdateBotMatching();

		uint GetGrabbedPlayerCount(Array<ReservedMatchingItem>& grabbedItems);

	public:

		MatchingServiceEntity( GameID gameID, ClusterID clusterID, bool useBot = true, ClusterMembership initialMembership = ClusterMembership::Slave );
		~MatchingServiceEntity();

		// We are not going to use hashed key
		virtual uint KeyHash( uint64_t key ) { return (uint)key; }

		uint GetTargetMatchingMemberCount() { return m_TargetMatchingMemberCount; }
		uint GetMaxMatchingQueue() { return m_MaxMatchingQueue; }

		bool GetIsUseBot() { return m_IsUseBot; }


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
		PageQueue<MatchingServiceEntity::ReservedMatchingItem>* m_ReservedItemQueue[MAX_QUEUE_COUNT] = { nullptr, };

		int m_ReservedItemQueueTransaction[MAX_QUEUE_COUNT];

		// target queue member count
		uint m_MatchingMemberCount;

		// Target enqueue member count
		uint m_MinQueueCount;
		uint m_MaxQueueCount;



	public:
		MatchingServiceQueueEntity(uint matchingMemberCount, uint minQueueCount, uint maxQueueCount);
		~MatchingServiceQueueEntity();


		uint GetMinQueueCount()															{ return m_MinQueueCount; }
		uint GetMaxQueueCount()															{ return m_MaxQueueCount; }
		PageQueue<MatchingServiceEntity::ReservedMatchingItem>& GetReservedItemQueue(uint iMemberCount, PlayerRole requestRole);

		void TransactionStartedForQueue(uint iMemberCount)								{ m_ReservedItemQueueTransaction[iMemberCount]++; }
		void TransactionClosedForQueue(uint iMemberCount)								{ m_ReservedItemQueueTransaction[iMemberCount]--; if (m_ReservedItemQueueTransaction[iMemberCount] < 0) m_ReservedItemQueueTransaction[iMemberCount] = 0; }
		INT GetTransactionStartedForQueue(uint iMemberCount)							{ return m_ReservedItemQueueTransaction[iMemberCount]; }

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
}; // namespace SF




