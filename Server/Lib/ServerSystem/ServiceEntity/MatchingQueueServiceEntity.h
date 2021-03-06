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

#include "MemoryManager/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/SFHashTable2.h"
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

	class MatchingQueueServiceEntity : public ShardedClusterServiceEntity
	{
	public:

		typedef ShardedClusterServiceEntity super;

		// Queue item
		struct QueueItem : public MatchingQueueItem
		{
			// Queue Item ID
			LocalUID		QueueItemID;

			// Reserver UID
			EntityUID		Reserver;

			// Reserved time
			TimeStampMS			ReservedTime;

			// Cancel is pending?
			bool			PendingCancel;


			QueueItem()
				: QueueItemID(0)
				, Reserver(0)
				, ReservedTime(TimeStampMS::min())
				, PendingCancel(false)
			{
			}

			MatchingQueueTicket GetTicket(const EntityUID& ownerEntityUID)
			{
				return MatchingQueueTicket(ownerEntityUID, QueueItemID);
			}

		};

		typedef HashTable2<uint64_t, QueueItem*> QueueItemIDTable;


	private:

		// Item ID table
		QueueItemIDTable		m_ItemIDTable;

		// Matching queue
		PageQueue<QueueItem*>	m_MainQueue;
		PageQueue<QueueItem*>	m_SecondaryQueue;

		PageQueue<QueueItem*>	m_ReservedQueue;

		// UID generator
		LocalUIDGenerator		m_UIDGenerator;

		SyncCounter				m_ItemCounter;
		Util::TimeStampTimer	m_WorkloadUpdateTimer;

		PerformanceCounterRaw<uint64_t> m_QueuedItemCount;

	protected:


	public:

		MatchingQueueServiceEntity( GameID gameID, ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::Slave);
		~MatchingQueueServiceEntity();


		static const char* GetCounterNameFromClusterID(ClusterID clusterID);


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		Result InitializeEntity( EntityID newEntityID ) override;

		// clear transaction
		virtual Result ClearEntity() override;

		Result TickUpdate(TimerAction *pAction = nullptr) override;

		Result UpdateResevationQueue();
		Result UpdateMatchigQueue();

		// Register message handler for this component
		virtual Result RegisterServiceMessageHandler() override;

	private:
		//////////////////////////////////////////////////////////////////////////
		//
		//	internal operations
		//

		Result CancelItem(QueueItem *pItem);

	public:
		//////////////////////////////////////////////////////////////////////////
		//
		//	Queue operations
		//

		// Add new Entity
		Result Enqueue( EntityUID registerUID, PlayerID registerID, uint numPlayer, const MatchingPlayerInformation* players, MatchingQueueTicket& ticket );

		// Update ticket 
		Result UpdateTicket( const MatchingQueueTicket& ticket, const EntityUID& UIDFrom, const EntityUID& UIDTo );

		// Cancel enqueued ticket 
		Result CancelTicket( const MatchingQueueTicket& ticket );

		// Reserve a item in the queue from the top
		Result ReserveItem( EntityUID reserverUID, uint &numPlayersInItem, MatchingQueueTicket& ticket );

		// Cancel a reservation
		Result CancelReservation(const MatchingQueueTicket& ticket );

		// Dequeue reserved item
		Result DequeueItem( const MatchingQueueTicket& ticket, MatchingQueueItem &item );

		// Delete item by error
		Result DeleteItem(const MatchingQueueTicket& ticket, MatchingQueueItem &item);


	};





}; // namespace Svr
}; // namespace SF




