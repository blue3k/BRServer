////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/Memory.h"
#include "Net/NetDef.h"

#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceTrans.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/ServerEntityManager.h"

#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"

#include "ServerSystem/ServiceEntity/MatchingServiceUtil.h"

BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingQueueServiceEntity::QueueItem);


namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	MatchingQueueServiceEntity::MatchingQueueServiceEntity(ClusterID clusterID, ClusterMembership initialMembership)
		: RingClusterServiceEntity(clusterID, initialMembership )
		, IServerComponent(MatchingUtil::GetComponentIDFromClusterID(clusterID))
		, m_ItemCounter(0)
		, m_QueuedItemCount(GetCounterNameFromClusterID(clusterID))
	{
	}

	MatchingQueueServiceEntity::~MatchingQueueServiceEntity()
	{
	}

	const char* MatchingQueueServiceEntity::GetCounterNameFromClusterID(ClusterID clusterID)
	{
		switch (clusterID)
		{
		case ClusterID::MatchingQueue_Game_4x1:  return "MatchingQueue_Game_4x1";
		case ClusterID::MatchingQueue_Game_4x2:  return "MatchingQueue_Game_4x2";
		case ClusterID::MatchingQueue_Game_4x3:  return "MatchingQueue_Game_4x3";
		case ClusterID::MatchingQueue_Game_4x1S: return "MatchingQueue_Game_4x1S";
		case ClusterID::MatchingQueue_Game_4x1W: return "MatchingQueue_Game_4x1W";
		case ClusterID::MatchingQueue_Game_8x1:  return "MatchingQueue_Game_8x1";
		case ClusterID::MatchingQueue_Game_8x2:  return "MatchingQueue_Game_8x2";
		case ClusterID::MatchingQueue_Game_8x3:  return "MatchingQueue_Game_8x3";
		case ClusterID::MatchingQueue_Game_8x4:  return "MatchingQueue_Game_8x4";
		case ClusterID::MatchingQueue_Game_8x5:  return "MatchingQueue_Game_8x5";
		case ClusterID::MatchingQueue_Game_8x6:  return "MatchingQueue_Game_8x6";
		case ClusterID::MatchingQueue_Game_8x7:  return "MatchingQueue_Game_8x7";
		case ClusterID::MatchingQueue_Game_8x1S: return "MatchingQueue_Game_8x1S";
		case ClusterID::MatchingQueue_Game_8x1W: return "MatchingQueue_Game_8x1W";
		default:
			break;
		}

		return "Unknown";
	}

	HRESULT MatchingQueueServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;
		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();

		svrChk(RingClusterServiceEntity::InitializeEntity(newEntityID) );

		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_QueuedItemCount);
		}

		m_WorkloadUpdateTimer.SetTimer(DurationMS(Const::WORKLOAD_UPDATE_TIME));

	Proc_End:

		return hr;
	}
	
	// clear transaction
	HRESULT MatchingQueueServiceEntity::ClearEntity()
	{
		HRESULT hr = S_OK;

		m_ItemIDTable.Foreach([](const UINT64& key, const QueueItem* pItem)
		{
			Util::SafeDelete(pItem);
			return true;
		});
		m_ItemIDTable.Clear();


		m_MainQueue.ClearQueue();
		m_SecondaryQueue.ClearQueue();
		m_ReservedQueue.ClearQueue();

		svrChk(RingClusterServiceEntity::ClearEntity() );

	Proc_End:

		return hr;
	}
	

	HRESULT MatchingQueueServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
		ServiceEntityUIDMap::iterator itService;

		svrChk(RingClusterServiceEntity::TickUpdate(pAction) );

		// Update workload
		if( m_WorkloadUpdateTimer.CheckTimer() )
		{
			m_WorkloadUpdateTimer.SetTimer(DurationMS(Const::WORKLOAD_UPDATE_TIME));
			SetWorkload( (UINT)m_ItemCounter );
		}

		m_QueuedItemCount = m_ItemIDTable.GetItemCount();

		UpdateResevationQueue();

		UpdateMatchigQueue();

	Proc_End:

		return hr;
	}

	HRESULT MatchingQueueServiceEntity::UpdateResevationQueue()
	{
		HRESULT hr = S_OK;
		
		// if any old reserved item is found remove it
		UINT numItems = (UINT)m_ReservedQueue.GetEnqueCount();
		for( UINT iItems = 0; iItems < numItems; iItems++ )
		{
			QueueItem *pItem = nullptr;

			if( FAILED(m_ReservedQueue.Dequeue( pItem )) )
				continue;

			// If deleted
			if( pItem->NumPlayers == 0 )
			{
				QueueItem* erasedValue = nullptr;
				if (SUCCEEDED(m_ItemIDTable.Erase(pItem->QueueItemID.UID, erasedValue)))
				{
					Assert(erasedValue == pItem);
				}
				Util::SafeDelete(pItem);
				continue;
			}

			// Canceled
			if( pItem->ReservedTime == TimeStampMS::min() )
			{
				m_SecondaryQueue.Enqueue( pItem );
				continue;
			}

			if( Util::TimeSince(pItem->ReservedTime) > DurationMS(Const::MATCHING_QUEUE_RESERVATION_TIMEOUT))
			{
				// return back to secondary queue so that it can be selected again
				svrTrace( Trace::TRC_ERROR, "Timeout for a reserved matching queue item, Reserver:{0}, {1}", pItem->Reserver, pItem->ReservedTime );
				pItem->Reserver = 0;
				pItem->ReservedTime = TimeStampMS::min();
				m_SecondaryQueue.Enqueue( pItem );
			}
			else
			{
				// NOTE: need to check timeout of waiting item

				// return back to waiting queue
				m_ReservedQueue.Enqueue( pItem );
			}
		}

	
	//Proc_End:

		return hr;
	}



	HRESULT MatchingQueueServiceEntity::UpdateMatchigQueue()
	{
		HRESULT hr = S_OK;

		
		// clean up front
		QueueItem *pItem = nullptr;
		while( SUCCEEDED(m_MainQueue.GetFront(pItem)) )
		{
			// If delete is required by canceling
			if ((pItem->NumPlayers == 0 || pItem->PendingCancel) && pItem->ReservedTime == TimeStampMS::min())
			{
				m_MainQueue.Dequeue(pItem);
				CancelItem(pItem);
				//m_ItemIDTable.erase( pItem ); // Erase again for safe
				Util::SafeDelete(pItem);
			}
			else
				break;
		}

		// clean up front
		pItem = nullptr;
		while( SUCCEEDED(m_SecondaryQueue.GetFront(pItem)) )
		{
			// If delete is required by canceling
			if ((pItem->NumPlayers == 0 || pItem->PendingCancel) && pItem->ReservedTime == TimeStampMS::min())
			{
				m_SecondaryQueue.Dequeue(pItem);
				CancelItem(pItem);
				//m_ItemIDTable.erase( pItem ); // Erase again for safe
				Util::SafeDelete(pItem);
			}
			else
				break;
		}

		
	//Proc_End:

		return hr;
	}


	// Register message handler for this component
	HRESULT MatchingQueueServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		RingClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity );

		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::RegisterPartyMatchingCmd)				{ svrMemReturn(pNewTrans = new RegisterPartyMatchingTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::RegisterPlayerMatchingCmd)			{ svrMemReturn(pNewTrans = new RegisterPlayerMatchingTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd)			{ svrMemReturn(pNewTrans = new UpdateMatchingEntityUIDTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::UnregisterMatchingCmd)				{ svrMemReturn(pNewTrans = new UnregisterMatchingTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::ReserveItemCmd)						{ svrMemReturn(pNewTrans = new ReserveItemTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::ReserveItemsCmd)						{ svrMemReturn(pNewTrans = new MatchingQueueReserveItemsTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::CancelReservationCmd)					{ svrMemReturn(pNewTrans = new CancelReservationTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::DequeueItemCmd)						{ svrMemReturn(pNewTrans = new DequeueItemTrans(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::MatchingItemErrorC2SEvt)				{ svrMemReturn(pNewTrans = new MatchingQueueTransMatchingItemError(pMsgData)); return S_OK; } );

		return S_OK;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	internal operations
	//

	HRESULT MatchingQueueServiceEntity::CancelItem(QueueItem *pItem)
	{
		HRESULT hr = S_OK;
		ServerEntity* pServerEntity = nullptr;
		Policy::ISvrPolicyPartyMatchingQueue* pPolicyMatchingQueueSvr = nullptr;

		if (pItem == nullptr)
			return E_INVALIDARG;

		if ((pItem->NumPlayers != 0 && !pItem->PendingCancel) || pItem->ReservedTime != TimeStampMS::min())
			return E_INVALID_STATE;

		// If delete is required by canceling
		svrChk((GetServerComponent<ServerEntityManager>()->GetServerEntity(pItem->RegisterUID.GetServerID(), pServerEntity)));
		pPolicyMatchingQueueSvr = pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatchingQueue>();
		svrChkPtr(pPolicyMatchingQueueSvr);

		if (pItem->NumPlayers > 1)
		{
			pPolicyMatchingQueueSvr->PartyMatchingCanceledS2CEvt(RouteContext(GetEntityUID(), pItem->RegisterUID), 0, pItem->RegisterUID, pItem->GetTicket(GetEntityUID()));
		}
		else
		{
			pPolicyMatchingQueueSvr->PlayerMatchingCanceledS2CEvt(RouteContext(GetEntityUID(), pItem->RegisterUID), 0, pItem->RegisterID, pItem->GetTicket(GetEntityUID()));
		}

	Proc_End:

		pItem->NumPlayers = 0;
		QueueItem *erasedValue = nullptr;
		if (SUCCEEDED(m_ItemIDTable.Erase(pItem->QueueItemID, erasedValue)))
		{
			Assert(erasedValue == pItem);
		}

		m_ItemCounter.fetch_sub(1, std::memory_order_relaxed);

		return hr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Queue operations
	//

	// Add new Entity
	HRESULT MatchingQueueServiceEntity::Enqueue( EntityUID registerUID, PlayerID registerID, UINT numPlayer, const MatchingPlayerInformation* players, MatchingQueueTicket& ticket )
	{
		HRESULT hr = S_OK;
		QueueItem *pNewItem = nullptr;

		svrMem( pNewItem = new QueueItem );

		svrChk( pNewItem->SetQueueItem( registerUID, registerID, numPlayer, players ) );
		pNewItem->QueueItemID = m_UIDGenerator.NewUID();

		svrChk(m_ItemIDTable.Insert(pNewItem->QueueItemID, pNewItem));
		svrChk( m_MainQueue.Enqueue( pNewItem ) );

		ticket.QueueItemID = pNewItem->QueueItemID;
		ticket.QueueUID = GetEntityUID();

		m_ItemCounter.fetch_add(1, std::memory_order_relaxed);

	Proc_End:


		if(FAILED(hr))
		{
			if( pNewItem != nullptr )
			{
				QueueItem* erasedValue = nullptr;
				m_ItemIDTable.Erase( pNewItem->QueueItemID, erasedValue );
				Util::SafeDelete(pNewItem);
			}
		}

		return hr;
	}

	// Update ticket 
	HRESULT MatchingQueueServiceEntity::UpdateTicket( const MatchingQueueTicket& ticket, const EntityUID& UIDFrom, const EntityUID& UIDTo )
	{
		HRESULT hr = S_OK;
		QueueItem *pItem = nullptr;
		MatchingPlayerInformation *pPlayers = nullptr;

		svrChk(m_ItemIDTable.Find(ticket.QueueItemID, pItem));

		// Just mark the item as a canceled one for now
		if (pItem->RegisterUID == UIDFrom)
		{
			pItem->RegisterUID = UIDTo;
		}

		pPlayers = pItem->Players;
		for (UINT player = 0; player < pItem->NumPlayers; player++, pPlayers++)
		{
			if( pPlayers->PlayerUID == UIDFrom )
			{
				pPlayers->PlayerUID = UIDTo;
				break;
			}
		}

	Proc_End:

		return hr;
	}

	HRESULT MatchingQueueServiceEntity::CancelTicket( const MatchingQueueTicket& ticket )
	{
		HRESULT hr = S_OK;
		QueueItem *pItem = nullptr;

		if (FAILED(m_ItemIDTable.Find(ticket.QueueItemID, pItem)))
		{
			return E_SVR_INVALID_QUEUEITEM;
		}

		// already canceled
		if (pItem->NumPlayers == 0)
			return E_SVR_QUEUEITEM_CANCELED;

		if (pItem->Reserver != 0)
		{
			// just mark cancel for now
			pItem->PendingCancel = true;

			return S_SVR_QUEUEITEM_CANCEL_PENDED;
		}
		else
		{
			svrChk( CancelItem(pItem) );
		}


	Proc_End:

		return hr;
	}

	// Reserve a item int the queue from the top
	HRESULT MatchingQueueServiceEntity::ReserveItem( EntityUID reserverUID, UINT &numPlayersInItem, MatchingQueueTicket& ticket )
	{
		HRESULT hr = S_OK;
		QueueItem *pItem = nullptr;

		do {
			if( FAILED( m_SecondaryQueue.Dequeue( pItem ) ) )
			{
				hr = m_MainQueue.Dequeue( pItem );
				if(FAILED( hr ))
				{
					hr = E_SVR_NOITEM_INQUEUE;
					goto Proc_End;
				}
			}

			// If canceled
			if( pItem->NumPlayers == 0 )
			{
				// Reserved queue updater will release this item
				svrChk( m_ReservedQueue.Enqueue( pItem ) );
				continue;
			}

			Assert(pItem->Reserver.UID==0);
			Assert(pItem->ReservedTime== TimeStampMS::min());

			pItem->Reserver = reserverUID;
			pItem->ReservedTime = Util::Time.GetTimeMs();

			numPlayersInItem = pItem->NumPlayers;
			ticket = pItem->GetTicket(GetEntityUID());

			// push it to reserved list queue
			svrChk( m_ReservedQueue.Enqueue( pItem ) );

		} while( pItem->NumPlayers == 0 );

	Proc_End:

		return hr;
	}

	// Cancel a reservation
	HRESULT MatchingQueueServiceEntity::CancelReservation( const MatchingQueueTicket& ticket )
	{
		HRESULT hr = S_OK;
		QueueItem *pItem = nullptr;

		svrChk(m_ItemIDTable.Find(ticket.QueueItemID, pItem));


		pItem->Reserver = 0;

		std::atomic_thread_fence(std::memory_order_seq_cst);

		pItem->ReservedTime = TimeStampMS::min();

	Proc_End:

		return hr;
	}

	// Dequeue reserved item
	HRESULT MatchingQueueServiceEntity::DequeueItem( const MatchingQueueTicket& ticket, MatchingQueueItem &item )
	{
		HRESULT hr = S_OK;
		QueueItem *pItem = nullptr, *erasedValue = nullptr;

		svrChk(m_ItemIDTable.Find(ticket.QueueItemID, pItem));


		// If canceled
		if( pItem->NumPlayers == 0 )
		{
			svrErr(E_SVR_QUEUEITEM_CANCELED);
		}

		item = *pItem;

		// This item will be deleted in TickUpdate
		pItem->ReservedTime = TimeStampMS::min();
		pItem->Reserver = 0;
		pItem->QueueItemID = 0;
		pItem->NumPlayers = 0;

		if (SUCCEEDED(m_ItemIDTable.Erase(ticket.QueueItemID, erasedValue)))
		{
			Assert(pItem == erasedValue);
		}

		m_ItemCounter.fetch_sub(1, std::memory_order_relaxed);

	Proc_End:

		return hr;
	}

	HRESULT MatchingQueueServiceEntity::DeleteItem(const MatchingQueueTicket& ticket, MatchingQueueItem &item)
	{
		return DequeueItem(ticket, item);
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MatchingQueueWatcherServiceEntity class
	//


	MatchingQueueWatcherServiceEntity::MatchingQueueWatcherServiceEntity( ClusterID clusterID, UINT componentID )
		:RingClusterServiceEntity(clusterID, ClusterMembership::StatusWatcher)
		,IServerComponent(componentID)
	{
	}


	MatchingQueueWatcherServiceEntity::~MatchingQueueWatcherServiceEntity()
	{
	}



}; // namespace Svr {
}; // namespace BR {



