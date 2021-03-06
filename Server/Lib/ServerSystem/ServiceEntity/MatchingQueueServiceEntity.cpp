////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "MemoryManager/SFMemory.h"
#include "Net/SFNetDef.h"

#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingQueueServiceTrans.h"
#include "SvrTrace.h"
#include "SvrConst.h"

#include "PerformanceCounter/PerformanceCounterClient.h"

#include "ServiceEntity/MatchingServiceUtil.h"


namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	MatchingQueueServiceEntity::MatchingQueueServiceEntity(GameID gameID, ClusterID clusterID, ClusterMembership initialMembership)
		: super(gameID, clusterID, initialMembership )
		, m_ItemIDTable(GetHeap())
		, m_MainQueue(GetHeap())
		, m_SecondaryQueue(GetHeap())
		, m_ReservedQueue(GetHeap())
		, m_ItemCounter(0)
		, m_QueuedItemCount(GetCounterNameFromClusterID(clusterID))
	{
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::RegisterPartyMatchingCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) RegisterPartyMatchingTrans(GetHeap(),pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::RegisterPlayerMatchingCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) RegisterPlayerMatchingTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) UpdateMatchingEntityUIDTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::UnregisterMatchingCmd)			{ svrMemReturn(pNewTrans = new(GetHeap()) UnregisterMatchingTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::ReserveItemCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) ReserveItemTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::ReserveItemsCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) MatchingQueueReserveItemsTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::CancelReservationCmd)			{ svrMemReturn(pNewTrans = new(GetHeap()) CancelReservationTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::DequeueItemCmd)					{ svrMemReturn(pNewTrans = new(GetHeap()) DequeueItemTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::MatchingItemErrorC2SEvt)			{ svrMemReturn(pNewTrans = new(GetHeap()) MatchingQueueTransMatchingItemError(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
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

	Result MatchingQueueServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();

		svrChk(super::InitializeEntity(newEntityID) );

		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_QueuedItemCount);
		}

		m_WorkloadUpdateTimer.SetTimer(DurationMS(Const::WORKLOAD_UPDATE_TIME));

	Proc_End:

		return hr;
	}
	
	// clear transaction
	Result MatchingQueueServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		m_ItemIDTable.for_each([](const uint64_t& key, const QueueItem* pItem)
		{
			Util::SafeDelete(pItem);
			return true;
		});
		m_ItemIDTable.Clear();


		m_MainQueue.ClearQueue();
		m_SecondaryQueue.ClearQueue();
		m_ReservedQueue.ClearQueue();

		svrChk(super::ClearEntity() );

	Proc_End:

		return hr;
	}
	

	Result MatchingQueueServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::TickUpdate(pAction) );

		// Update workload
		if( m_WorkloadUpdateTimer.CheckTimer() )
		{
			m_WorkloadUpdateTimer.SetTimer(DurationMS(Const::WORKLOAD_UPDATE_TIME));
			SetWorkload( (uint)m_ItemCounter );
		}

		m_QueuedItemCount = m_ItemIDTable.size();

		UpdateResevationQueue();

		UpdateMatchigQueue();

	Proc_End:

		return hr;
	}

	Result MatchingQueueServiceEntity::UpdateResevationQueue()
	{
		Result hr = ResultCode::SUCCESS;
		
		// if any old reserved item is found remove it
		uint numItems = (uint)m_ReservedQueue.GetEnqueCount();
		for( uint iItems = 0; iItems < numItems; iItems++ )
		{
			QueueItem *pItem = nullptr;

			if( !(m_ReservedQueue.Dequeue( pItem )) )
				continue;

			// If deleted
			if( pItem->NumPlayers == 0 )
			{
				QueueItem* erasedValue = nullptr;
				if ((m_ItemIDTable.Erase(pItem->QueueItemID.UID, erasedValue)))
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
				svrTrace( Error, "Timeout for a reserved matching queue item, Reserver:{0}, {1}", pItem->Reserver, pItem->ReservedTime );
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



	Result MatchingQueueServiceEntity::UpdateMatchigQueue()
	{
		Result hr = ResultCode::SUCCESS;

		
		// clean up front
		QueueItem *pItem = nullptr;
		while( (m_MainQueue.GetFront(pItem)) )
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
		while( (m_SecondaryQueue.GetFront(pItem)) )
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
	Result MatchingQueueServiceEntity::RegisterServiceMessageHandler()
	{
		super::RegisterServiceMessageHandler();

		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::RegisterPartyMatchingCmd) { svrMemReturn(pNewTrans = new(GetHeap()) RegisterPartyMatchingTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::RegisterPlayerMatchingCmd) { svrMemReturn(pNewTrans = new(GetHeap()) RegisterPlayerMatchingTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd) { svrMemReturn(pNewTrans = new(GetHeap()) UpdateMatchingEntityUIDTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::UnregisterMatchingCmd) { svrMemReturn(pNewTrans = new(GetHeap()) UnregisterMatchingTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::ReserveItemCmd) { svrMemReturn(pNewTrans = new(GetHeap()) ReserveItemTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::ReserveItemsCmd) { svrMemReturn(pNewTrans = new(GetHeap()) MatchingQueueReserveItemsTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::CancelReservationCmd) { svrMemReturn(pNewTrans = new(GetHeap()) CancelReservationTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::DequeueItemCmd) { svrMemReturn(pNewTrans = new(GetHeap()) DequeueItemTrans(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::PartyMatchingQueue::MatchingItemErrorC2SEvt) { svrMemReturn(pNewTrans = new(GetHeap()) MatchingQueueTransMatchingItemError(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		return ResultCode::SUCCESS;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	internal operations
	//

	Result MatchingQueueServiceEntity::CancelItem(QueueItem *pItem)
	{
		Result hr = ResultCode::SUCCESS;
		MessageEndpoint* requesterEndpoint{};

		if (pItem == nullptr)
			return ResultCode::INVALID_ARG;

		if ((pItem->NumPlayers != 0 && !pItem->PendingCancel) || pItem->ReservedTime == TimeStampMS::min())
		{
			hr = ResultCode::INVALID_STATE;
			goto Proc_End;
		}

		// If delete is required by canceling
		requesterEndpoint = Service::MessageEndpointManager->GetEndpoint(pItem->RegisterUID);
		
		if (pItem->NumPlayers > 1)
		{
			NetSvrPolicyPartyMatchingQueue(requesterEndpoint).PartyMatchingCanceledS2CEvt(RouteContext(GetEntityUID(), pItem->RegisterUID), 0, pItem->RegisterUID, pItem->GetTicket(GetEntityUID()));
		}
		else
		{
			NetSvrPolicyPartyMatchingQueue(requesterEndpoint).PlayerMatchingCanceledS2CEvt(RouteContext(GetEntityUID(), pItem->RegisterUID), 0, pItem->RegisterID, pItem->GetTicket(GetEntityUID()));
		}

	Proc_End:

		pItem->NumPlayers = 0;
		QueueItem *erasedValue = nullptr;
		if ((m_ItemIDTable.Erase(pItem->QueueItemID, erasedValue)))
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
	Result MatchingQueueServiceEntity::Enqueue( EntityUID registerUID, PlayerID registerID, uint numPlayer, const MatchingPlayerInformation* players, MatchingQueueTicket& ticket )
	{
		Result hr = ResultCode::SUCCESS;
		QueueItem *pNewItem = nullptr;

		svrMem( pNewItem = new(GetHeap()) QueueItem );

		svrChk( pNewItem->SetQueueItem( registerUID, registerID, numPlayer, players ) );
		pNewItem->QueueItemID = m_UIDGenerator.NewUID();

		svrChk(m_ItemIDTable.Insert(pNewItem->QueueItemID, pNewItem));
		svrChk( m_MainQueue.Enqueue( pNewItem ) );

		ticket.QueueItemID = pNewItem->QueueItemID;
		ticket.QueueUID = GetEntityUID();

		m_ItemCounter.fetch_add(1, std::memory_order_relaxed);

	Proc_End:


		if(!(hr))
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
	Result MatchingQueueServiceEntity::UpdateTicket( const MatchingQueueTicket& ticket, const EntityUID& UIDFrom, const EntityUID& UIDTo )
	{
		Result hr = ResultCode::SUCCESS;
		QueueItem *pItem = nullptr;
		MatchingPlayerInformation *pPlayers = nullptr;

		svrChk(m_ItemIDTable.Find(ticket.QueueItemID, pItem));

		// Just mark the item as a canceled one for now
		if (pItem->RegisterUID == UIDFrom)
		{
			pItem->RegisterUID = UIDTo;
		}

		pPlayers = pItem->Players;
		for (uint player = 0; player < pItem->NumPlayers; player++, pPlayers++)
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

	Result MatchingQueueServiceEntity::CancelTicket( const MatchingQueueTicket& ticket )
	{
		Result hr = ResultCode::SUCCESS;
		QueueItem *pItem = nullptr;

		if (!(m_ItemIDTable.Find(ticket.QueueItemID, pItem)))
		{
			return ResultCode::SVR_INVALID_QUEUEITEM;
		}

		// already canceled
		if (pItem->NumPlayers == 0)
			return ResultCode::SVR_QUEUEITEM_CANCELED;

		if (pItem->Reserver.UID != 0)
		{
			// just mark cancel for now
			pItem->PendingCancel = true;

			return ResultCode::SUCCESS_SVR_QUEUEITEM_CANCEL_PENDED;
		}
		else
		{
			hr = CancelItem(pItem);
		}


	//Proc_End:

		return hr;
	}

	// Reserve a item int the queue from the top
	Result MatchingQueueServiceEntity::ReserveItem( EntityUID reserverUID, uint &numPlayersInItem, MatchingQueueTicket& ticket )
	{
		Result hr = ResultCode::SUCCESS;
		QueueItem *pItem = nullptr;

		do {
			if( !( m_SecondaryQueue.Dequeue( pItem ) ) )
			{
				hr = m_MainQueue.Dequeue( pItem );
				if(!( hr ))
				{
					hr = ResultCode::SVR_NOITEM_INQUEUE;
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
	Result MatchingQueueServiceEntity::CancelReservation( const MatchingQueueTicket& ticket )
	{
		Result hr = ResultCode::SUCCESS;
		QueueItem *pItem = nullptr;

		svrChk(m_ItemIDTable.Find(ticket.QueueItemID, pItem));


		pItem->Reserver = 0;

		std::atomic_thread_fence(std::memory_order_seq_cst);

		pItem->ReservedTime = TimeStampMS::min();

	Proc_End:

		return hr;
	}

	// Dequeue reserved item
	Result MatchingQueueServiceEntity::DequeueItem( const MatchingQueueTicket& ticket, MatchingQueueItem &item )
	{
		Result hr = ResultCode::SUCCESS;
		QueueItem *pItem = nullptr, *erasedValue = nullptr;

		svrChk(m_ItemIDTable.Find(ticket.QueueItemID, pItem));


		// If canceled
		if( pItem->NumPlayers == 0 )
		{
			svrErr(ResultCode::SVR_QUEUEITEM_CANCELED);
		}

		item = *pItem;

		// This item will be deleted in TickUpdate
		pItem->ReservedTime = TimeStampMS::min();
		pItem->Reserver = 0;
		pItem->QueueItemID = 0;
		pItem->NumPlayers = 0;

		if ((m_ItemIDTable.Erase(ticket.QueueItemID, erasedValue)))
		{
			Assert(pItem == erasedValue);
		}

		m_ItemCounter.fetch_sub(1, std::memory_order_relaxed);

	Proc_End:

		return hr;
	}

	Result MatchingQueueServiceEntity::DeleteItem(const MatchingQueueTicket& ticket, MatchingQueueItem &item)
	{
		return DequeueItem(ticket, item);
	}





}; // namespace Svr {
}; // namespace SF {



