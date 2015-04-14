////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"

#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"
#include "Protocol/Policy/PartyMatchingIPolicy.h"

#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/MatchingServiceTrans.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"

#include "ServerSystem/ServerService/PartyMatchingQueueService.h"
#include "ServerSystem/ServerService/GameInstanceManagerService.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingTransGrabPlayer);
//BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingTransCancelPlayer);
BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingTransProcessMatchedItems);
BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingPartyTrans);



namespace BR {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingTransGrabPlayer::MatchingTransGrabPlayer(UINT matchingMemberCount, UINT targetQueueMemberCount, UINT minQueueCount, UINT maxQueueCount)
		: TransactionT(TransactionID())
		, m_TargetQueueMemberCount(targetQueueMemberCount)
		, m_MinQueueCount(minQueueCount)
		, m_MaxQueueCount(maxQueueCount)
		, m_MatchingMemberCount(matchingMemberCount)
	{
		Assert(m_TargetQueueMemberCount);
		Assert(m_MinQueueCount);
		Assert(m_MaxQueueCount);
		Assert(m_MaxQueueCount > m_MinQueueCount);

		SetPrintTrace(false);

		BR_TRANS_MESSAGE(TimerResult, { return OnTimer(pRes); });
		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::ReserveItemsRes, { return OnReserveItem(pRes); });
	}

	HRESULT MatchingTransGrabPlayer::OnTimer(TransactionResult* pRes)
	{
		ProcessGrabbing();

		return S_OK;
	}

	HRESULT MatchingTransGrabPlayer::ProcessGrabbing()
	{
		HRESULT hr = S_OK;
		UINT itemsInQueue = 0;

		if (itemsInQueue < m_MinQueueCount
			//&& m_RequestedTime == 0
			)
		{
			auto grabCount = std::min(m_MaxQueueCount - m_MinQueueCount, (UINT)GRAB_MAX);

			hr = ReserveItem(grabCount);
			if (hr == E_SVR_NOITEM_INQUEUE || hr == E_SVR_CLUSTER_NOTREADY)
			{
				goto Proc_End;
			}

			svrChk(hr);

			//m_RequestedTime = Util::Time.GetTimeMs();
			ClearTimer();
		}
		else
		{
			// Nothing for now
			SetTimer(TICK_TIME);
		}

	Proc_End:

		if (FAILED(hr))
		{
			SetTimer(GRAB_RETRY_TIME);
		}
		

		return S_OK;
	}

	HRESULT MatchingTransGrabPlayer::ReserveItem(UINT grabCount)
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pService = nullptr;
		MatchingQueueWatcherServiceEntity *pServiceEntity = nullptr;

		int componentID = MatchingQueueWatcherServiceEntity::GetQueueComponentID(m_MatchingMemberCount, m_TargetQueueMemberCount);

		svrChkPtr(pServiceEntity = GetServerComponent<MatchingQueueWatcherServiceEntity>(componentID));

		if (FAILED(pServiceEntity->GetService(pService)))
		{
			return E_SVR_CLUSTER_NOTREADY;
		}

		// NOTE: Workload bug, just try to grab
		if (pService->GetWorkload() > 0)
		{
			svrTrace(Svr::TRC_MATCHING, "Try to make a reservation. Matching:%0%, TargetQueue:%1%, expected %2%", m_MatchingMemberCount, m_TargetQueueMemberCount, pService->GetWorkload());
			svrChk(pService->GetService<Svr::PartyMatchingQueueService>()->ReserveItemsCmd(GetTransID(), 0, grabCount));
		}
		else
		{
			return E_SVR_NOITEM_INQUEUE;
		}

	Proc_End:

		if (FAILED(hr))
		{
			if (pServiceEntity != nullptr)
			{
				svrTrace(Trace::TRC_ERROR, "Matching queue watcher query is failed, matching member count:%0%, ServiceEntityUID:%1%, ClusterID:%2%", m_TargetQueueMemberCount, pServiceEntity->GetEntityUID(), pServiceEntity->GetClusterID());
			}
			else
			{
				svrTrace(Trace::TRC_ERROR, "Matching queue watcher query is failed, matching member count:%0%", m_TargetQueueMemberCount);
			}
		}

		return hr;

	}

	HRESULT MatchingTransGrabPlayer::RequestDeleteItem(MatchingQueueTicket ticket)
	{
		HRESULT hr = S_OK;
		ServerEntity *pServerEntity = nullptr;

		svrTrace(Svr::TRC_MATCHING, "Request Delete item Matching:%0%, MatchingQueue:%1%, MTicket:%2%", m_MatchingMemberCount, m_TargetQueueMemberCount, ticket);

		svrChk(GetServerComponent<ServerEntityManager>()->GetServerEntity(ticket.QueueUID.SvrID, pServerEntity));

		// 2. Get service entity list in the cluster
		svrChk(pServerEntity->GetPolicy<Policy::IPolicyPartyMatchingQueue>()->MatchingItemErrorC2SEvt(RouteContext(GetMyOwner()->GetEntityUID(), ticket.QueueUID), 0, ticket));

	Proc_End:

		return hr;
	}

	HRESULT MatchingTransGrabPlayer::OnReserveItem(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::PartyMatchingQueue::ReserveItemsRes msgRes;

		//m_RequestedTime = 0;

		if (FAILED(pRes->GetHRESULT()))
		{
			// 
			hr = pRes->GetHRESULT();
			goto Proc_End;
		}

		svrChk(msgRes.ParseIMsg(((MessageResult*)pRes)->GetMessage()));


		auto numItems = std::min(msgRes.GetNumberOfPlayersInTheItem().GetSize(), msgRes.GetMatchingTicket().GetSize());
		auto& pNumPlayersInItems = msgRes.GetNumberOfPlayersInTheItem();
		auto& pMatchingTickets = msgRes.GetMatchingTicket();

		svrTrace(Svr::TRC_MATCHING, "%2% items are cached, Matching:%0%, MatchingQueue:%1%, Count:%2%", m_MatchingMemberCount, m_TargetQueueMemberCount, numItems);

		for (UINT iItem = 0; iItem < numItems; iItem++)
		{
			ReservedMatchingItem newItem;

			newItem.MemberCount = pNumPlayersInItems[iItem];
			newItem.MatchingTicket = pMatchingTickets[iItem];

			if (newItem.MemberCount == 0)
			{
				// Request delete for invalid items
				RequestDeleteItem(newItem.MatchingTicket);
			}
			else
			{
				// This will gurantee that the items will be queued by actual member count
				auto& reservedQueue = GetMyOwner()->GetReservedItemQueue(newItem.MemberCount);
				svrChk(reservedQueue.Enqueue(newItem));
			}
		}


		ProcessGrabbing();

	Proc_End:

		if (FAILED(hr))
		{
			SetTimer(GRAB_RETRY_TIME);
		}
		else
		{
			ClearTimer();
		}

		return S_OK;
	}

	// Start Transaction
	HRESULT MatchingTransGrabPlayer::StartTransaction()
	{
		HRESULT hr = S_OK;

		//m_RequestedTime = 0;// Util::Time.GetTimeMs();

		svrChk(__super::StartTransaction());

		GetMyOwner()->TransactionStartedForQueue(m_TargetQueueMemberCount);

		ProcessGrabbing();

	Proc_End:

		return hr;
	}

	HRESULT MatchingTransGrabPlayer::OnCloseTransaction(HRESULT hrRes)
	{
		GetMyOwner()->TransactionClosedForQueue(m_TargetQueueMemberCount);

		return __super::OnCloseTransaction(hrRes);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//

	//	
	//MatchingTransCancelPlayer::MatchingTransCancelPlayer(UINT matchingMemberCount, UINT targetQueueMemberCount)
	//	: TransactionT(TransactionID())
	//	, m_TargetQueueMemberCount(targetQueueMemberCount)
	//	, m_MatchingMemberCount(matchingMemberCount)
	//{
	//	Assert(m_TargetQueueMemberCount);

	//	SetPrintTrace(false);

	//	BR_TRANS_MESSAGE(TimerResult, { return OnTimer(pRes); });
	//	BR_TRANS_MESSAGE(Message::PartyMatchingQueue::CancelReservationsRes, { return OnCancelReservation(pRes); });
	//}

	//HRESULT MatchingTransCancelPlayer::OnTimer(TransactionResult* pRes)
	//{
	//	ProcessCanceling();

	//	return S_OK;
	//}

	//HRESULT MatchingTransCancelPlayer::ProcessCanceling()
	//{

	//}

	//HRESULT MatchingTransCancelPlayer::CancelReservation(MatchingQueueTicket ticket)
	//{

	//}

	//HRESULT MatchingTransCancelPlayer::OnCancelReservation(TransactionResult* pRes)
	//{

	//}


	//// Start Transaction
	//HRESULT MatchingTransCancelPlayer::StartTransaction()
	//{

	//}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingTransProcessMatchedItems::MatchingTransProcessMatchedItems(UINT targetMatchingMemberCount, const Array<ReservedMatchingItem>& matchedItems)
		: TransactionT(TransactionID())
		, m_TargetMatchingMemberCount(targetMatchingMemberCount)
	{
		Assert(m_TargetMatchingMemberCount);
		Assert(m_MatchedItemCount);

		SetPrintTrace(false);

		for (int iItem = 0; iItem < matchedItems.GetSize(); iItem++)
		{
			MatchingItem item;
			item.MatchingTicket = matchedItems[iItem].MatchingTicket;
			item.MemberCount = matchedItems[iItem].MemberCount;
			m_MatchedItems.push_back(item);
		}

		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::DequeueItemRes, { return OnDequeueItem(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstanceManager::CreateGameRes, { return OnCreateGame(pRes); });
	}


	HRESULT MatchingTransProcessMatchedItems::DequeueItem(const MatchingQueueTicket& ticket)
	{
		HRESULT hr = S_OK;
		ServerEntity *pServerEntity = nullptr;

		svrTrace(Svr::TRC_MATCHING, "Dequeue item Matching:%0%, MTicket:%1%", GetTargetMatchingMemberCount(), ticket);

		svrChk(GetServerComponent<ServerEntityManager>()->GetServerEntity(ticket.QueueUID.SvrID, pServerEntity));

		// 2. Get service entity list in the cluster
		svrChk(pServerEntity->GetPolicy<Policy::IPolicyPartyMatchingQueue>()->DequeueItemCmd(GetTransID(), RouteContext(GetMyOwner()->GetEntityUID(), ticket.QueueUID), 0, ticket));

		m_PendingDequeueItem++;

	Proc_End:

		return hr;
	}

	HRESULT MatchingTransProcessMatchedItems::OnDequeueItem(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::PartyMatchingQueue::DequeueItemRes msgRes;
		const ServiceInformation *currentMaster = nullptr, *currentSlave = nullptr;

		m_PendingDequeueItem--;

		// Maybe canceled?
		if (SUCCEEDED(pRes->GetHRESULT()))
		{
			svrChk(msgRes.ParseIMsg(((MessageResult*)pRes)->GetMessage()));

			AssertRel(msgRes.GetPlayers().GetSize() <= MAX_NUM_PLAYER);

			for (int iItem = 0; iItem < m_MatchedItems.GetSize(); iItem++)
			{
				if (m_MatchedItems[iItem].MatchingTicket != msgRes.GetMatchingTicket())
					continue;

				m_DequeuedTotalMembers += (UINT)msgRes.GetPlayers().GetSize();
				m_MatchedItems[iItem].MemberCount = (UINT)msgRes.GetPlayers().GetSize();
				memcpy(m_MatchedItems[iItem].Players, msgRes.GetPlayers().data(), sizeof(MatchingPlayerInformation)*msgRes.GetPlayers().GetSize());
				m_MatchedItems[iItem].RegisterEntityUID = msgRes.GetRegisterUID();
				break;
			}
		}
		else
		{
		}


	Proc_End:

		if (m_PendingDequeueItem <= 0)
		{
			if (m_DequeuedTotalMembers > 0)
				CreateGame();
			else
				CloseTransaction(S_OK);
		}

		return hr;
	}

	HRESULT MatchingTransProcessMatchedItems::CreateGame()
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pService = nullptr;

		svrTrace(Svr::TRC_MATCHING, "Creating game Matching:%0%", GetTargetMatchingMemberCount());

		svrChk(GetServerComponent<GameInstanceManagerWatcherServiceEntity>()->GetService(pService));

		// 2. Get service entity list in the cluster
		svrChk(pService->GetService<GameInstanceManagerService>()->CreateGameCmd(GetTransID(), 0, m_TargetMatchingMemberCount - m_DequeuedTotalMembers, m_TargetMatchingMemberCount));

	Proc_End:

		if (FAILED(hr))
			SetTimer(1000);

		return hr;
	}

	HRESULT MatchingTransProcessMatchedItems::OnCreateGame(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::GameInstanceManager::CreateGameRes msgRes;
		const ServiceInformation *currentMaster = nullptr, *currentSlave = nullptr;
		GameInsUID gameUID;
		UINT notifiedPlayerCount = 0;


		svrChk(pRes->GetHRESULT());

		svrChk(msgRes.ParseIMsg(((MessageResult*)pRes)->GetMessage()));

		gameUID = msgRes.GetRouteContext().From;


		// Send Game information to players
		for (int iItem = 0; iItem < m_MatchedItems.GetSize(); iItem++)
		{
			auto& reservedMember = m_MatchedItems[iItem];

			ServerEntity *pServerEntity = nullptr;
			bool notifiedToRegister = false;
			for (UINT member = 0; member < reservedMember.MemberCount; member++)
			{
				if (reservedMember.Players[member].PlayerUID == reservedMember.RegisterEntityUID) notifiedToRegister = true;

				if (FAILED(GetServerComponent<ServerEntityManager>()->GetServerEntity(reservedMember.Players[member].PlayerUID.SvrID, pServerEntity)))
				{
					// skip this player
					svrTrace(Trace::TRC_ERROR, "Failed to find Server entity(%0%) while broadcasting for a player(%1%)", reservedMember.Players[member].PlayerUID.SvrID, reservedMember.Players[member].PlayerID);
					continue;
				}

				notifiedPlayerCount++;
				pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatching>()->PlayerGameMatchedS2CEvt(
					RouteContext(GetOwnerEntityUID(), reservedMember.Players[member].PlayerUID), 0,
					reservedMember.Players[member].PlayerID, gameUID);
			}

			// this should be a party, or canceled item
			if (!notifiedToRegister)
			{
				if (FAILED(GetServerComponent<ServerEntityManager>()->GetServerEntity(reservedMember.RegisterEntityUID.SvrID, pServerEntity)))
				{
					// skip this player
					svrTrace(Trace::TRC_ERROR, "Failed to find Server entity(%0%) while broadcasting", reservedMember.RegisterEntityUID.SvrID);
					continue;
				}

				notifiedPlayerCount++;
				pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatching>()->PartyGameMatchedS2CEvt(
					RouteContext(GetOwnerEntityUID(), reservedMember.RegisterEntityUID), 0);
			}
		}


	Proc_End:

		// clean up empty game instance
		if (notifiedPlayerCount == 0)
		{
			ServerEntity *pServerEntity = nullptr;
			if (SUCCEEDED(GetServerComponent<ServerEntityManager>()->GetServerEntity(gameUID.SvrID, pServerEntity)))
			{
				auto pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameInstance>();
				if (pPolicy != nullptr)
					pPolicy->DeleteGameC2SEvt(RouteContext(GetOwnerEntityUID(), gameUID));
			}

		}

		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	HRESULT MatchingTransProcessMatchedItems::StartTransaction()
	{
		HRESULT hr = S_OK;

		m_PendingDequeueItem = 0;
		m_DequeuedTotalMembers = 0;

		svrChk(__super::StartTransaction());

		for (int iItem = 0; iItem < m_MatchedItems.GetSize(); iItem++)
		{
			DequeueItem(m_MatchedItems[iItem].MatchingTicket);
		}


	Proc_End:

		if (FAILED(hr) || m_PendingDequeueItem <= 0)
		{
			// We need to retry until it's successed
			CloseTransaction(hr);
		}

		return hr;
	}






	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingPartyTrans::MatchingPartyTrans(UINT startMemberCount, UINT targetMemberCount)
		: TransactionT( TransactionID() )
		,m_QueryMemberCount(startMemberCount)
		,m_TargetMemberCount(targetMemberCount)
	{
		Assert(m_QueryMemberCount);
		Assert(m_TargetMemberCount);

		SetPrintTrace(false);

		BR_TRANS_MESSAGE( TimerResult, { return OnTimer(pRes); });
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::ReserveItemRes,			{ return OnReserveItem(pRes); });
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::CancelReservationRes,	{ return OnCancelReservation(pRes); });
		BR_TRANS_MESSAGE( Message::PartyMatchingQueue::DequeueItemRes,			{ return OnDequeueItem(pRes); });
		BR_TRANS_MESSAGE( Message::GameInstanceManager::CreateGameRes,			{ return OnCreateGame(pRes); });
	}

	HRESULT MatchingPartyTrans::OnTimer(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;

		switch( m_Step )
		{
		case Step::Grabbing:
			hr = ProcessGrabbing();
			if( hr == E_SVR_NOITEM_INQUEUE )
			{
				hr = S_OK;
				goto Proc_End;
			}
			svrChk(hr);
			break;
		case Step::Canceling:
			if( GetProcessingIndex() >= 0 && GetProcessingIndex() < (INT)m_ReservedMember.size() )
			{
				svrChk( CancelReservation(m_ReservedMember[GetProcessingIndex()].ReservationTicket) );
			}
			else
			{
				CloseTransaction(hr);
			}
			break;
		case Step::CreateGame:
			CreateGame();
			break;
		};

	Proc_End:

		if(FAILED(hr))
		{
			// If we failed even though we tried with time delay. We should be better to close this job at here
			CloseTransaction(hr);
		}

		return hr;
	}

	HRESULT MatchingPartyTrans::ProcessGrabbing()
	{
		HRESULT hr = S_OK;
		bool bUseBot = false;

		m_GrabTryCount++;

		if (/*GetGrabTryCount() > MAX_GRAB_TRY ||*/ Util::TimeSince(m_MatchingTryTimeOut) > 0 )
		{
			if (!GetMyOwner()->GetIsUseBot() || GetCurrentMemberCount() == 0)
			{
				SetProcessingIndex(0);

				svrChk(ProcessCanceling());

				if (GetCurrentMemberCount() > 0)
				{
					svrTrace(Svr::TRC_INFO, "Failed to grab, Not enought player, %0%/%1%", GetCurrentMemberCount(), GetTargetMemberCount());
				}
				goto Proc_End;
			}
			else
			{
				bUseBot = true;
			}
		}

		if (!bUseBot && GetCurrentMemberCount() < GetTargetMemberCount())
		{
			UINT grabbingCount = GetCurrentGrabbing();
			if( grabbingCount <= 0 )
			{
				// For first time try maximum one
				grabbingCount = GetTargetMemberCount() - GetCurrentMemberCount();
			}
			else
			{
				grabbingCount--;
				grabbingCount = Util::Min( GetTargetMemberCount() - GetCurrentMemberCount(), grabbingCount );
				if( grabbingCount == 0 )
					grabbingCount = GetTargetMemberCount() - GetCurrentMemberCount();
			}

			if( grabbingCount >= GetTargetMemberCount() )
				grabbingCount = GetTargetMemberCount() - 1;

			SetCurrentGrabbing( grabbingCount );
			hr = ReserveItem( grabbingCount );
			if( hr == E_SVR_NOITEM_INQUEUE || hr == E_SVR_CLUSTER_NOTREADY )
			{
				goto Proc_End;
			}

			svrChk(hr);
		}
		else
		{
			// we've done grabbing perfectly, Start dequeuing them
			m_Step = Step::Dequeuing;
			SetProcessingIndex(0);
			svrChk( DequeueItem( m_ReservedMember[GetProcessingIndex()].ReservationTicket ) );
		}

	Proc_End:

		if( FAILED(hr) )
		{
			SetTimer(GRAB_RETRY_TIME);
		}

		return S_OK;
	}

	HRESULT MatchingPartyTrans::ProcessCanceling()
	{
		HRESULT hr = S_OK;

		m_Step = Step::Canceling;
		if( m_ProcessingIndex >= 0 && m_ProcessingIndex < (INT)m_ReservedMember.size() )
		{
			svrChk( CancelReservation( m_ReservedMember[GetProcessingIndex()].ReservationTicket ) );
		}
		else
		{
			CloseTransaction(hr);
		}

	Proc_End:

		if( FAILED(hr) )
		{
			SetTimer(1000);
		}

		return hr;
	}

	HRESULT MatchingPartyTrans::ProcessDequeuing()
	{
		HRESULT hr = S_OK;

		if( GetProcessingIndex() < (INT)m_ReservedMember.size() )
		{
			svrChk( DequeueItem( m_ReservedMember[GetProcessingIndex()].ReservationTicket ) );
		}
		else
		{
			// Dequeue done, matched! Create a game to send them
			svrChk( CreateGame() );
		}

	Proc_End:

		if( FAILED(hr) )
		{
			SetTimer(1000);
		}

		return hr;
	}

	HRESULT MatchingPartyTrans::ReserveItem(UINT memberCount)
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pService = nullptr;
		MatchingQueueWatcherServiceEntity *pServiceEntity = nullptr;

		int componentID = GetClusterComponentIDFromMemberCount(memberCount);

		svrChkPtr( pServiceEntity = GetServerComponent<MatchingQueueWatcherServiceEntity>(componentID) );

		svrChk( pServiceEntity->GetService( pService ) );
		if( pService->GetWorkload() > 0 )
		{
			svrTrace( Svr::TRC_MATCHING, "Make a reservation (%0%) Matching:%1%, TargetQueue:%2%", GetOwnerEntityUID(), GetTargetMemberCount(), memberCount );
			svrChk( pService->GetService<Svr::PartyMatchingQueueService>()->ReserveItemCmd( GetTransID(), 0 ) );
		}
		else
		{
			return E_SVR_NOITEM_INQUEUE;
		}

	Proc_End:

		if(FAILED(hr))
		{
			if( pServiceEntity != nullptr )
			{
				svrTrace( Trace::TRC_ERROR, "Matching queue watcher query is failed, matching member count:%0%, ServiceEntityUID:%1%, ClusterID:%2%", memberCount, pServiceEntity->GetEntityUID(), pServiceEntity->GetClusterID() );
			}
			else
			{
				svrTrace( Trace::TRC_ERROR, "Matching queue watcher query is failed, matching member count:%0%", memberCount );
			}
		}

		return hr;
	}

	HRESULT MatchingPartyTrans::OnReserveItem(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::PartyMatchingQueue::ReserveItemRes msgRes;
		ReservedMember newItem;

		if( FAILED(pRes->GetHRESULT()) )
		{
			if( GetCurrentGrabbing() == 0 )
				SetCurrentGrabbing( GetQueryMemberCount() );

			hr = ProcessGrabbing();
			if( hr == E_SVR_NOITEM_INQUEUE )
			{
				svrTrace( Svr::TRC_INFO, "Failed to grab item E_SVR_NOITEM_INQUEUE line:%0%", __LINE__ );
				goto Proc_End;
			}

			svrChk(hr);
			goto Proc_End;
		}

		svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );

		newItem.MemberCount = msgRes.GetNumberOfPlayersInTheItem();
		newItem.ReservationTicket = msgRes.GetMatchingTicket();

		svrChk( m_ReservedMember.push_back(newItem) );

		newItem.ReservationTicket.QueueItemID = 0;

		SetCurrentMemberCount( GetCurrentMemberCount() + newItem.MemberCount );

		// reset when succeeded
		SetGrabTryCount(0);

		hr = ProcessGrabbing();
		if( hr == E_SVR_NOITEM_INQUEUE )
		{
			svrTrace( Svr::TRC_INFO, "Failed to grab item E_SVR_NOITEM_INQUEUE, line:%0%", __LINE__ );
			goto Proc_End;
		}
		svrChk(hr);

	Proc_End:

		// If failed at something, retry after 1 sec
		if( FAILED(hr) )
		{
			// If we made a reservation but failed, we don't any choice here
			if( newItem.ReservationTicket.QueueItemID.UID != 0 )
			{
				m_Step = Step::Canceling;
				SetProcessingIndex( -1 );
				CancelReservation(newItem.ReservationTicket);
			}
			else
			{
				// retry after 1sec
				SetTimer( 1000 );
			}
		}

		// Hide error log for this error
		if( hr == E_SVR_NOITEM_INQUEUE )
			return S_OK;

		return hr;
	}


	HRESULT MatchingPartyTrans::CancelReservation( MatchingQueueTicket ticket )
	{
		HRESULT hr = S_OK;
		ServerEntity *pServerEntity = nullptr;

		svrTrace( Svr::TRC_MATCHING, "Canceling reservation (%0%) Matching:%1%, MTicket:%2%", GetOwnerEntityUID(), GetTargetMemberCount(), ticket );

		svrChk( GetServerComponent<ServerEntityManager>()->GetServerEntity( ticket.QueueUID.SvrID, pServerEntity ) );

		// 2. Get service entity list in the cluster
		svrChk( pServerEntity->GetPolicy<Policy::IPolicyPartyMatchingQueue>()->CancelReservationCmd( GetTransID(), RouteContext(GetMyOwner()->GetEntityUID(), ticket.QueueUID), 0, ticket ) );

	Proc_End:

		return hr;
	}

	HRESULT MatchingPartyTrans::OnCancelReservation(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::PartyMatchingQueue::CancelReservationRes msgRes;
		ReservedMember newItem;

		svrChk(pRes->GetHRESULT());

		svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );

		if( m_ProcessingIndex >= 0 && m_ProcessingIndex < (INT)m_ReservedMember.size() )
			m_ReservedMember[m_ProcessingIndex].ReservationTicket = 0;
		m_ProcessingIndex++;

		svrChk( ProcessCanceling() );


	Proc_End:

		// If failed at something, retry after 1 sec
		if( FAILED(hr) )
		{
			// retry after 1sec
			SetTimer( 1000 );
		}

		return hr;
	}


	HRESULT MatchingPartyTrans::DequeueItem( MatchingQueueTicket ticket )
	{
		HRESULT hr = S_OK;
		ServerEntity *pServerEntity = nullptr;

		svrTrace( Svr::TRC_MATCHING, "Dequeue item (%0%) Matching:%1%, MTicket:%2%", GetOwnerEntityUID(), GetTargetMemberCount(), ticket );

		svrChk( GetServerComponent<ServerEntityManager>()->GetServerEntity( ticket.QueueUID.SvrID, pServerEntity ) );

		// 2. Get service entity list in the cluster
		svrChk( pServerEntity->GetPolicy<Policy::IPolicyPartyMatchingQueue>()->DequeueItemCmd( GetTransID(), RouteContext(GetMyOwner()->GetEntityUID(), ticket.QueueUID), 0, ticket ) );

	Proc_End:

		return hr;
	}

	HRESULT MatchingPartyTrans::OnDequeueItem(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::PartyMatchingQueue::DequeueItemRes msgRes;
		const ServiceInformation *currentMaster = nullptr, *currentSlave = nullptr;

		if( m_Step == Step::Dequeuing )
		{
			// Maybe canceled?
			if( SUCCEEDED(pRes->GetHRESULT()) )
			{
				svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );

				AssertRel(msgRes.GetPlayers().GetSize() <= MAX_NUM_PLAYER);
				Assert(m_ReservedMember[m_ProcessingIndex].MemberCount == (UINT)msgRes.GetPlayers().GetSize());

				m_ReservedMember[m_ProcessingIndex].MemberCount = (UINT)msgRes.GetPlayers().GetSize();
				memcpy(m_ReservedMember[m_ProcessingIndex].Players, msgRes.GetPlayers().data(), sizeof(MatchingPlayerInformation)*msgRes.GetPlayers().GetSize());

				m_ReservedMember[m_ProcessingIndex].RegisterEntityUID = msgRes.GetRegisterUID();
			}
			else
			{
				// We failed to dequeue an item
				// TODO: need to recover from this
				m_ReservedMember[m_ProcessingIndex].MemberCount = 0;
				m_ReservedMember[m_ProcessingIndex].RegisterEntityUID = 0;
			}

			Assert(m_ProcessingIndex >= 0 && m_ProcessingIndex < (INT)m_ReservedMember.size());

			m_ProcessingIndex++;
		}

		svrChk( ProcessDequeuing() );

	Proc_End:

		// If failed at something, retry after 1 sec
		if( FAILED(hr) )
			SetTimer( 1000 );

		return hr;
	}

	HRESULT MatchingPartyTrans::CreateGame()
	{
		HRESULT hr = S_OK;
		ServerServiceInformation *pService = nullptr;

		if (m_CurrentMemberCount <= 0 || m_CurrentMemberCount > m_TargetMemberCount)
		{
			svrErr(E_UNEXPECTED);
		}

		m_Step = Step::CreateGame;

		svrTrace( Svr::TRC_MATCHING, "Creating game (%0%) Matching:%1%", GetOwnerEntityUID(), GetTargetMemberCount() );

		svrChk( GetServerComponent<GameInstanceManagerWatcherServiceEntity>()->GetService( pService ) );

		// 2. Get service entity list in the cluster
		svrChk(pService->GetService<GameInstanceManagerService>()->CreateGameCmd(GetTransID(), 0, m_TargetMemberCount - m_CurrentMemberCount, m_TargetMemberCount));

	Proc_End:

		if( FAILED(hr) )
			SetTimer(1000);

		return hr;
	}

	HRESULT MatchingPartyTrans::OnCreateGame(TransactionResult* pRes)
	{
		HRESULT hr = S_OK;
		Message::GameInstanceManager::CreateGameRes msgRes;
		const ServiceInformation *currentMaster = nullptr, *currentSlave = nullptr;
		GameInsUID gameUID;
		UINT nitifiedPlayerCount = 0;

		svrChk(pRes->GetHRESULT());

		svrChk( msgRes.ParseIMsg( ((MessageResult*)pRes)->GetMessage() ) );

		gameUID = msgRes.GetRouteContext().From;

		// Send Game information to players
		m_ReservedMember.Foreach( [&]( ReservedMember& reservedMember ) -> HRESULT
		{
			ServerEntity *pServerEntity = nullptr;
			bool notifiedToRegister = false;
			for( UINT member = 0; member < reservedMember.MemberCount; member++ )
			{
				if( reservedMember.Players[member].PlayerUID == reservedMember.RegisterEntityUID ) notifiedToRegister = true;

				if( FAILED(GetServerComponent<ServerEntityManager>()->GetServerEntity( reservedMember.Players[member].PlayerUID.SvrID, pServerEntity )) )
				{
					// skip this player
					svrTrace( Trace::TRC_ERROR, "Failed to find Server entity(%0%) while broadcasting for a player(%1%)", reservedMember.Players[member].PlayerUID.SvrID, reservedMember.Players[member].PlayerID );
					continue;
				}

				nitifiedPlayerCount++;
				pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatching>()->PlayerGameMatchedS2CEvt( 
					RouteContext(GetOwnerEntityUID(),reservedMember.Players[member].PlayerUID), 0, 
					reservedMember.Players[member].PlayerID, gameUID );
			}

			// this should be a party, or canceled item
			if( !notifiedToRegister )
			{
				// Canceled member items can be 0
				//Assert( reservedMember.MemberCount >= 1 );


				if( FAILED(GetServerComponent<ServerEntityManager>()->GetServerEntity( reservedMember.RegisterEntityUID.SvrID, pServerEntity )) )
				{
					// skip this player
					svrTrace( Trace::TRC_ERROR, "Failed to find Server entity(%0%) while broadcasting", reservedMember.RegisterEntityUID.SvrID );
					return S_OK;
				}

				nitifiedPlayerCount++;
				pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatching>()->PartyGameMatchedS2CEvt( 
					RouteContext(GetOwnerEntityUID(),reservedMember.RegisterEntityUID), 0 );
			}

			return S_OK;
		});


	Proc_End:

		// clean up empty game instance
		if (nitifiedPlayerCount == 0)
		{
			ServerEntity *pServerEntity = nullptr;
			if (SUCCEEDED(GetServerComponent<ServerEntityManager>()->GetServerEntity(gameUID.SvrID, pServerEntity)))
			{
				auto pPolicy = pServerEntity->GetPolicy<Policy::IPolicyGameInstance>();
				if (pPolicy != nullptr)
					pPolicy->DeleteGameC2SEvt(RouteContext(GetOwnerEntityUID(), gameUID));
			}

		}

		CloseTransaction(hr);

		//// If failed at something, retry after 1 sec
		//if( FAILED(hr) )
		//	SetTimer( 1000 );

		return hr;
	}

	// Start Transaction
	HRESULT MatchingPartyTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		m_MatchingTryTimeOut = Util::Time.GetTimeMs() + GRAB_TRY_TIMEOUT;

		svrChk( __super::StartTransaction() );


		if( m_TargetMemberCount <= 0 || m_TargetMemberCount > GameConst::MAX_GAMEPLAYER )
			svrErr(E_GAME_INAVLID_PLAYER_COUNT);

		if( m_QueryMemberCount <= 0 || m_QueryMemberCount > GameConst::MAX_GAMEPLAYER || m_QueryMemberCount >= m_TargetMemberCount )
			svrErr(E_GAME_INAVLID_PLAYER_COUNT);

		m_Step = Step::Grabbing;
		SetCurrentGrabbing(0);
		SetGrabTryCount(0);
		SetProcessingIndex(0);

		SetCurrentMemberCount(0);
		m_ReservedMember.Clear();

		hr = ReserveItem(GetQueryMemberCount());
		if( hr == E_SVR_NOITEM_INQUEUE )
		{
			hr = S_OK;
			SetTimer(1000);
		}

	Proc_End:

		if( FAILED(hr) )
		{
			// We need to retry until it's successed
			CloseTransaction(hr);
		}

		return hr;
	}
	
	////////////////////////////////////////////////////////////
	// Helpers
	UINT MatchingPartyTrans::GetClusterComponentIDFromMemberCount( UINT count )
	{
		return MatchingQueueWatcherServiceEntity::GetQueueComponentID( GetTargetMemberCount(), count );
	}


};// namespace Svr 
};// namespace BR 

