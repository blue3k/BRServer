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
#include "Common/BrMemory.h"
#include "Common/BrSvrTypes.h"
#include "Common/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable2.h"
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

	class MatchingQueueServiceEntity : public RingClusterServiceEntity, public IServerComponent
	{
	public:

		// Queue item
		struct QueueItem : public MatchingQueueItem, public MemoryPoolObject<QueueItem>
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

		typedef Hash::HashTable2<UINT64, QueueItem*> QueueItemIDTable;


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

		PerformanceCounterRaw<UINT64> m_QueuedItemCount;

	protected:


	public:

		MatchingQueueServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
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
		virtual Result RegisterServiceMessageHandler( ServerEntity *pServerEntity ) override;

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
		Result Enqueue( EntityUID registerUID, PlayerID registerID, UINT numPlayer, const MatchingPlayerInformation* players, MatchingQueueTicket& ticket );

		// Update ticket 
		Result UpdateTicket( const MatchingQueueTicket& ticket, const EntityUID& UIDFrom, const EntityUID& UIDTo );

		// Cancel enqueued ticket 
		Result CancelTicket( const MatchingQueueTicket& ticket );

		// Reserve a item in the queue from the top
		Result ReserveItem( EntityUID reserverUID, UINT &numPlayersInItem, MatchingQueueTicket& ticket );

		// Cancel a reservation
		Result CancelReservation(const MatchingQueueTicket& ticket );

		// Dequeue reserved item
		Result DequeueItem( const MatchingQueueTicket& ticket, MatchingQueueItem &item );

		// Delete item by error
		Result DeleteItem(const MatchingQueueTicket& ticket, MatchingQueueItem &item);
	};





	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MatchingQueueWatcherServiceEntity class
	//

	class MatchingQueueWatcherServiceEntity : public RingClusterServiceEntity, public IServerComponent
	{
	public:

	public:

		MatchingQueueWatcherServiceEntity( ClusterID clusterID, UINT componentID );
		~MatchingQueueWatcherServiceEntity();

	};



}; // namespace Svr
}; // namespace BR




