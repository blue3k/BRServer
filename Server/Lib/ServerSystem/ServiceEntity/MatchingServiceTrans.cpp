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
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"

#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"
#include "Protocol/Policy/PartyMatchingQueueIPolicy.h"
#include "Protocol/Policy/PartyMatchingIPolicy.h"

#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/MatchingServiceUtil.h"
#include "ServerSystem/ServiceEntity/MatchingServiceTrans.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServerSystem/ServiceEntity/GameInstanceManagerServiceEntity.h"

#include "ServerSystem/ServerService/PartyMatchingQueueService.h"
#include "ServerSystem/ServerService/GameInstanceManagerService.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingTransGrabPlayer);
BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingTransProcessMatchedItems);
//BR_MEMORYPOOL_IMPLEMENT(Svr::MatchingPartyTrans);



namespace BR {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingTransGrabPlayer::MatchingTransGrabPlayer(UINT matchingMemberCount, UINT targetQueueMemberCount, PlayerRole playerRole, UINT minQueueCount, UINT maxQueueCount)
		: super(TransactionID())
		, m_MinQueueCount(minQueueCount)
		, m_MaxQueueCount(maxQueueCount)
		, m_TargetQueueMemberCount(targetQueueMemberCount)
		, m_MatchingMemberCount(matchingMemberCount)
		, m_RequestRole(playerRole)
	{
		Assert(m_TargetQueueMemberCount);
		Assert(m_MinQueueCount);
		Assert(m_MaxQueueCount);
		Assert(m_MaxQueueCount > m_MinQueueCount);

		m_TargetQueueComponentID = MatchingUtil::GetQueueComponentID(matchingMemberCount, targetQueueMemberCount, m_RequestRole);

		SetPrintTrace(false);

		BR_TRANS_MESSAGE(TimerResult, { return OnTimer(pRes); });
		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::ReserveItemsRes, { return OnReserveItem(pRes); });
	}

	HRESULT MatchingTransGrabPlayer::OnTimer(TransactionResult* pRes)
	{
		ProcessGrabbing();

		return S_SYSTEM_OK;
	}

	HRESULT MatchingTransGrabPlayer::ProcessGrabbing()
	{
		HRESULT hr = S_SYSTEM_OK;
		UINT itemsInQueue = 0;

		if (itemsInQueue < m_MinQueueCount)
		{
			auto grabCount = std::min(m_MaxQueueCount - m_MinQueueCount, (UINT)GRAB_MAX);

			hr = ReserveItem(grabCount);
			if (hr == E_SVR_NOITEM_INQUEUE || hr == E_SVR_CLUSTER_NOTREADY)
			{
				goto Proc_End;
			}

			//svrChk(hr);
			if (FAILED(hr))
				goto Proc_End;

			ClearTimer();
		}
		else
		{
			// Nothing for now
			SetTimer(DurationMS(TICK_TIME));
		}

	Proc_End:

		if (FAILED(hr))
		{
			SetTimer(DurationMS(GRAB_RETRY_TIME));
		}
		

		return S_SYSTEM_OK;
	}

	HRESULT MatchingTransGrabPlayer::ReserveItem(UINT grabCount)
	{
		HRESULT hr = S_SYSTEM_OK;
		ServerServiceInformation *pService = nullptr;
		RingClusterServiceEntity *pServiceEntity = nullptr;

		svrChkPtr(pServiceEntity = GetServerComponent<RingClusterServiceEntity>(m_TargetQueueComponentID));

		if (FAILED(pServiceEntity->GetService(pService)))
		{
			return E_SVR_CLUSTER_NOTREADY;
		}

		// NOTE: Workload bug, just try to grab
		if (pService->GetWorkload() > 0)
		{
			svrTrace(Svr::TRC_MATCHING, "Try to make a reservation. Matching:{0}, TargetQueueCompo:{1}, expected {2}", m_MatchingMemberCount, m_TargetQueueComponentID, pService->GetWorkload());
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
				svrTrace(Trace::TRC_ERROR, "Matching queue watcher query is failed, matching queueID:{0}, ServiceEntityUID:{1}, ClusterID:{2}", m_TargetQueueComponentID, pServiceEntity->GetEntityUID(), pServiceEntity->GetClusterID());
			}
			else
			{
				svrTrace(Trace::TRC_ERROR, "Matching queue watcher query is failed, matching queueID:{0}", m_TargetQueueComponentID);
			}
		}

		return hr;
	}


	HRESULT MatchingTransGrabPlayer::RequestDeleteItem(MatchingQueueTicket ticket)
	{
		HRESULT hr = S_SYSTEM_OK;
		ServerEntity *pServerEntity = nullptr;

		svrTrace(Svr::TRC_MATCHING, "Request Delete item Matching:{0}, MatchingQueueID:{1}, MTicket:{2}", m_MatchingMemberCount, m_TargetQueueComponentID, ticket);

		svrChk(GetServerComponent<ServerEntityManager>()->GetServerEntity(ticket.QueueUID.GetServerID(), pServerEntity));

		// 2. Get service entity list in the cluster
		svrChk(pServerEntity->GetPolicy<Policy::IPolicyPartyMatchingQueue>()->MatchingItemErrorC2SEvt(RouteContext(GetMyOwner()->GetEntityUID(), ticket.QueueUID), 0, ticket));

	Proc_End:

		return hr;
	}


	HRESULT MatchingTransGrabPlayer::OnReserveItem(TransactionResult* pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		Message::PartyMatchingQueue::ReserveItemsRes msgRes;

		//m_RequestedTime = 0;

		if (FAILED(pRes->GetHRESULT()))
		{
			// 
			hr = pRes->GetHRESULT();
			goto Proc_End;
		}

		svrChk(msgRes.ParseIMsg(((MessageResult*)pRes)->GetMessage()));

		{
			auto numItems = std::min(msgRes.GetNumberOfPlayersInTheItem().GetSize(), msgRes.GetMatchingTicket().GetSize());
			auto& pNumPlayersInItems = msgRes.GetNumberOfPlayersInTheItem();
			auto& pMatchingTickets = msgRes.GetMatchingTicket();

			svrTrace(Svr::TRC_MATCHING, "{2} items are cached, Matching:{0}, MatchingQueueCompo:{1}, Count:{2}", m_MatchingMemberCount, m_TargetQueueComponentID, numItems);

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
					auto& reservedQueue = GetMyOwner()->GetReservedItemQueue(newItem.MemberCount, m_RequestRole);
					svrChk(reservedQueue.Enqueue(newItem));
				}
			}

		}


		ProcessGrabbing();

	Proc_End:

		if (FAILED(hr))
		{
			SetTimer(DurationMS(GRAB_RETRY_TIME));
		}
		else
		{
			ClearTimer();
		}

		return S_SYSTEM_OK;
	}

	// Start Transaction
	HRESULT MatchingTransGrabPlayer::StartTransaction()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(super::StartTransaction());

		GetMyOwner()->TransactionStartedForQueue(m_TargetQueueMemberCount);

		ProcessGrabbing();

	Proc_End:

		return hr;
	}

	HRESULT MatchingTransGrabPlayer::OnCloseTransaction(HRESULT hrRes)
	{
		GetMyOwner()->TransactionClosedForQueue(m_TargetQueueMemberCount);

		return super::OnCloseTransaction(hrRes);
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingTransProcessMatchedItems::MatchingTransProcessMatchedItems(UINT targetMatchingMemberCount, const Array<ReservedMatchingItem>& matchedItems)
		: TransactionT(TransactionID())
		, m_TargetMatchingMemberCount(targetMatchingMemberCount)
	{
		Assert(m_TargetMatchingMemberCount > 0);
		Assert(m_MatchedItemCount > 0);

		SetPrintTrace(false);

		for (UINT iItem = 0; iItem < matchedItems.GetSize(); iItem++)
		{
			MatchingItem item;
			item.MatchingTicket = matchedItems[iItem].MatchingTicket;
			item.MemberCount = matchedItems[iItem].MemberCount;
			item.RequestedRole = matchedItems[iItem].RequestedRole;
			m_MatchedItems.push_back(item);
		}

		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::DequeueItemRes, { return OnDequeueItem(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstanceManager::CreateGameRes, { return OnCreateGame(pRes); });
	}


	HRESULT MatchingTransProcessMatchedItems::DequeueItem(const MatchingQueueTicket& ticket)
	{
		HRESULT hr = S_SYSTEM_OK;
		ServerEntity *pServerEntity = nullptr;

		svrTrace(Svr::TRC_MATCHING, "Dequeue item Matching:{0}, MTicket:{1}", GetTargetMatchingMemberCount(), ticket);

		svrChk(GetServerComponent<ServerEntityManager>()->GetServerEntity(ticket.QueueUID.GetServerID(), pServerEntity));

		// 2. Get service entity list in the cluster
		svrChk(pServerEntity->GetPolicy<Policy::IPolicyPartyMatchingQueue>()->DequeueItemCmd(GetTransID(), RouteContext(GetMyOwner()->GetEntityUID(), ticket.QueueUID), 0, ticket));

		m_PendingDequeueItem++;

	Proc_End:

		return hr;
	}

	HRESULT MatchingTransProcessMatchedItems::OnDequeueItem(TransactionResult* pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		Message::PartyMatchingQueue::DequeueItemRes msgRes;

		m_PendingDequeueItem--;

		// Maybe canceled?
		if (SUCCEEDED(pRes->GetHRESULT()))
		{
			svrChk(msgRes.ParseIMsg(((MessageResult*)pRes)->GetMessage()));

			AssertRel(msgRes.GetPlayers().GetSize() <= MAX_NUM_PLAYER);

			for (UINT iItem = 0; iItem < m_MatchedItems.GetSize(); iItem++)
			{
				if (m_MatchedItems[iItem].MatchingTicket != msgRes.GetMatchingTicket())
					continue;

				m_DequeuedTotalMembers += (UINT)msgRes.GetPlayers().GetSize();
				m_MatchedItems[iItem].MemberCount = (UINT)msgRes.GetPlayers().GetSize();
				memcpy(m_MatchedItems[iItem].Players, msgRes.GetPlayers().data(), sizeof(MatchingPlayerInformation)*msgRes.GetPlayers().GetSize());
				for (UINT iPlayer = 0; iPlayer < m_MatchedItems[iItem].MemberCount; iPlayer++)
				{
					m_MatchedItems[iItem].Players[iPlayer].RequestedRole = m_MatchedItems[iItem].RequestedRole;
				}
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
				CloseTransaction(S_SYSTEM_OK);
		}

		return hr;
	}

	HRESULT MatchingTransProcessMatchedItems::CreateGame()
	{
		HRESULT hr = S_SYSTEM_OK;
		ServerServiceInformation *pService = nullptr;

		svrTrace(Svr::TRC_MATCHING, "Creating game Matching:{0}", GetTargetMatchingMemberCount());

		svrChk(GetServerComponent<GameInstanceManagerWatcherServiceEntity>()->GetService(pService));

		// 2. Get service entity list in the cluster
		svrChk(pService->GetService<GameInstanceManagerService>()->CreateGameCmd(GetTransID(), 0, m_TargetMatchingMemberCount - m_DequeuedTotalMembers, m_TargetMatchingMemberCount));

	Proc_End:

		if (FAILED(hr))
			SetTimer(DurationMS(1000));

		return hr;
	}

	HRESULT MatchingTransProcessMatchedItems::OnCreateGame(TransactionResult* pRes)
	{
		HRESULT hr = S_SYSTEM_OK;
		Message::GameInstanceManager::CreateGameRes msgRes;
		GameInsUID gameUID;
		UINT notifiedPlayerCount = 0;


		svrChk(pRes->GetHRESULT());

		svrChk(msgRes.ParseIMsg(((MessageResult*)pRes)->GetMessage()));

		gameUID = msgRes.GetRouteContext().GetFrom();


		// Send Game information to players
		for (size_t iItem = 0; iItem < m_MatchedItems.GetSize(); iItem++)
		{
			auto& reservedMember = m_MatchedItems[iItem];

			ServerEntity *pServerEntity = nullptr;
			bool notifiedToRegister = false;
			for (UINT member = 0; member < reservedMember.MemberCount; member++)
			{
				if (reservedMember.Players[member].PlayerUID == reservedMember.RegisterEntityUID) notifiedToRegister = true;

				if (FAILED(GetServerComponent<ServerEntityManager>()->GetServerEntity(reservedMember.Players[member].PlayerUID.GetServerID(), pServerEntity)))
				{
					// skip this player
					svrTrace(Trace::TRC_ERROR, "Failed to find Server entity({0}) while broadcasting for a player({1})", reservedMember.Players[member].PlayerUID.GetServerID(), reservedMember.Players[member].PlayerID);
					continue;
				}

				notifiedPlayerCount++;
				pServerEntity->GetPolicy<Policy::ISvrPolicyPartyMatching>()->PlayerGameMatchedS2CEvt(
					RouteContext(GetOwnerEntityUID(), reservedMember.Players[member].PlayerUID), 0,
					reservedMember.Players[member].PlayerID, gameUID, reservedMember.RequestedRole);
			}

			// this should be a party, or canceled item
			if (!notifiedToRegister)
			{
				if (FAILED(GetServerComponent<ServerEntityManager>()->GetServerEntity(reservedMember.RegisterEntityUID.GetServerID(), pServerEntity)))
				{
					// skip this player
					svrTrace(Trace::TRC_ERROR, "Failed to find Server entity({0}) while broadcasting", reservedMember.RegisterEntityUID.GetServerID());
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
			if (SUCCEEDED(GetServerComponent<ServerEntityManager>()->GetServerEntity(gameUID.GetServerID(), pServerEntity)))
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
		HRESULT hr = S_SYSTEM_OK;

		m_PendingDequeueItem = 0;
		m_DequeuedTotalMembers = 0;

		svrChk(super::StartTransaction());

		for (size_t iItem = 0; iItem < m_MatchedItems.GetSize(); iItem++)
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






};// namespace Svr 
};// namespace BR 

