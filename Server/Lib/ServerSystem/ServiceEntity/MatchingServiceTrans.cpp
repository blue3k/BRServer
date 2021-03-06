////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Protocol/GameInstanceNetPolicy.h"
#include "Protocol/GameInstanceManagerMsgClass.h"
#include "Protocol/PartyMatchingQueueMsgClass.h"
#include "Protocol/PartyMatchingQueueNetPolicy.h"
#include "Protocol/PartyMatchingNetPolicy.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/MatchingServiceUtil.h"
#include "ServiceEntity/MatchingServiceTrans.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"


#include "Protocol/PartyMatchingQueueService.h"
#include "Protocol/GameInstanceManagerService.h"





namespace SF {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingTransGrabPlayer::MatchingTransGrabPlayer(IHeap& memoryManager, uint matchingMemberCount, uint targetQueueMemberCount, PlayerRole playerRole, uint minQueueCount, uint maxQueueCount)
		: super(memoryManager, TransactionID())
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

		m_TargetQueueClusterID = MatchingUtil::GetQueueClusterID(matchingMemberCount, targetQueueMemberCount, m_RequestRole);

		SetPrintTrace(false);

		BR_TRANS_MESSAGE(TimerResult, { return OnTimer(pRes); });
		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::ReserveItemsRes, { return OnReserveItem(pRes); });
	}

	Result MatchingTransGrabPlayer::OnTimer(TransactionResult* pRes)
	{
		ProcessGrabbing();

		return ResultCode::SUCCESS;
	}

	Result MatchingTransGrabPlayer::ProcessGrabbing()
	{
		Result hr = ResultCode::SUCCESS;
		uint itemsInQueue = 0;

		if (itemsInQueue < m_MinQueueCount)
		{
			auto grabCount = std::min(m_MaxQueueCount - m_MinQueueCount, (uint)GRAB_MAX);

			hr = ReserveItem(grabCount);
			if (hr == Result(ResultCode::SVR_NOITEM_INQUEUE) || hr == Result(ResultCode::SVR_CLUSTER_NOTREADY))
			{
				goto Proc_End;
			}

			//svrChk(hr);
			if (!(hr))
				goto Proc_End;

			ClearTimer();
		}
		else
		{
			// Nothing for now
			SetTimer(DurationMS(TICK_TIME));
		}

	Proc_End:

		if (!(hr))
		{
			SetTimer(DurationMS(GRAB_RETRY_TIME));
		}
		

		return ResultCode::SUCCESS;
	}

	Result MatchingTransGrabPlayer::ReserveItem(uint grabCount)
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<ServerServiceInformation> pService;
		ClusteredServiceEntity *pServiceEntity = nullptr;

		if(!Service::ServiceDirectory->GetRandomService(Service::ServerConfig->GameClusterID, m_TargetQueueClusterID, pService))
		{
			return ResultCode::SVR_CLUSTER_NOTREADY;
		}

		// NOTE: Workload bug, just try to grab
		//if (pService->GetWorkload() > 0)
		{
			svrTrace(SVR_DBGMATCHING, "Try to make a reservation. Matching:{0}, TargetQueueCompo:{1}", m_MatchingMemberCount, m_TargetQueueClusterID);
			svrChk(pService->GetService<PartyMatchingQueueService>()->ReserveItemsCmd(GetTransID(), 0, grabCount));
		}
		//else
		//{
		//	return ResultCode::SVR_NOITEM_INQUEUE;
		//}

	Proc_End:

		if (!(hr))
		{
			if (pServiceEntity != nullptr)
			{
				svrTrace(Error, "Matching queue watcher query is failed, matching queueID:{0}, ServiceEntityUID:{1}, ClusterID:{2}", m_TargetQueueClusterID, pServiceEntity->GetEntityUID(), pServiceEntity->GetClusterID());
			}
			else
			{
				svrTrace(Error, "Matching queue watcher query is failed, matching queueID:{0}", m_TargetQueueClusterID);
			}
		}

		return hr;
	}


	Result MatchingTransGrabPlayer::RequestDeleteItem(MatchingQueueTicket ticket)
	{
		Result hr = ResultCode::SUCCESS;

		svrTrace(SVR_MATCHING, "Request Delete item Matching:{0}, MatchingQueueID:{1}, MTicket:{2}", m_MatchingMemberCount, m_TargetQueueClusterID, ticket);

		auto ticketEndpoint = Service::MessageEndpointManager->GetEndpoint(ticket.QueueUID);
		//svrChk(Service::ServerEntityManager->GetServerEntity(ticket.QueueUID.GetServerID(), pServerEntity));

		// 2. Get service entity list in the cluster
		svrChk(NetPolicyPartyMatchingQueue(ticketEndpoint).MatchingItemErrorC2SEvt(RouteContext(GetMyOwner()->GetEntityUID(), ticket.QueueUID), 0, ticket));

	Proc_End:

		return hr;
	}


	Result MatchingTransGrabPlayer::OnReserveItem(TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Message::PartyMatchingQueue::ReserveItemsRes msgRes;

		//m_RequestedTime = 0;

		if (!(pRes->GetResult()))
		{
			// 
			hr = pRes->GetResult();
			goto Proc_End;
		}

		svrChk(msgRes.ParseMessage(*((MessageResult*)pRes)->GetMessage()));

		{
			auto numItems = std::min(msgRes.GetNumberOfPlayersInTheItem().size(), msgRes.GetMatchingTicket().size());
			auto& pNumPlayersInItems = msgRes.GetNumberOfPlayersInTheItem();
			auto& pMatchingTickets = msgRes.GetMatchingTicket();

			svrTrace(SVR_MATCHING, "{2} items are cached, Matching:{0}, MatchingQueueCompo:{1}, Count:{2}", m_MatchingMemberCount, m_TargetQueueClusterID, numItems);

			for (uint iItem = 0; iItem < numItems; iItem++)
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

		if (!(hr))
		{
			SetTimer(DurationMS(GRAB_RETRY_TIME));
		}
		else
		{
			ClearTimer();
		}

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	Result MatchingTransGrabPlayer::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		GetMyOwner()->TransactionStartedForQueue(m_TargetQueueMemberCount);

		ProcessGrabbing();

	Proc_End:

		return hr;
	}

	Result MatchingTransGrabPlayer::OnCloseTransaction(Result hrRes)
	{
		GetMyOwner()->TransactionClosedForQueue(m_TargetQueueMemberCount);

		return super::OnCloseTransaction(hrRes);
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingTransProcessMatchedItems::MatchingTransProcessMatchedItems(IHeap& memoryManager, uint targetMatchingMemberCount, const Array<ReservedMatchingItem>& matchedItems)
		: TransactionT(memoryManager, TransactionID())
		, m_TargetMatchingMemberCount(targetMatchingMemberCount)
		, m_MatchedItems(GetHeap())
	{
		Assert(m_TargetMatchingMemberCount > 0);
		Assert(m_MatchedItemCount > 0);

		SetPrintTrace(false);

		for (uint iItem = 0; iItem < matchedItems.size(); iItem++)
		{
			MatchingItem item;
			item.MatchingTicket = matchedItems[iItem].MatchingTicket;
			item.MemberCount = matchedItems[iItem].MemberCount;
			item.RequestedRole = matchedItems[iItem].RequestedRole;
			m_MatchedItems.push_back(item);
		}

		BR_TRANS_MESSAGE(Message::PartyMatchingQueue::DequeueItemRes, { return OnDequeueItem(pRes); });
		BR_TRANS_MESSAGE(Message::GameInstanceManager::CreateGameInstanceRes, { return OnCreateGame(pRes); });
	}


	Result MatchingTransProcessMatchedItems::DequeueItem(const MatchingQueueTicket& ticket)
	{
		Result hr = ResultCode::SUCCESS;

		svrTrace(SVR_MATCHING, "Dequeue item Matching:{0}, MTicket:{1}", GetTargetMatchingMemberCount(), ticket);

		auto ticketEndpoint = Service::MessageEndpointManager->GetEndpoint(ticket.QueueUID);
		//svrChk(Service::ServerEntityManager->GetServerEntity(ticket.QueueUID.GetServerID(), pServerEntity));

		// 2. Get service entity list in the cluster
		svrChk(NetPolicyPartyMatchingQueue(ticketEndpoint).DequeueItemCmd(RouteContext(GetMyOwner()->GetEntityUID(), ticket.QueueUID), GetTransID(), 0, ticket));

		m_PendingDequeueItem++;

	Proc_End:

		return hr;
	}

	Result MatchingTransProcessMatchedItems::OnDequeueItem(TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Message::PartyMatchingQueue::DequeueItemRes msgRes;

		m_PendingDequeueItem--;

		// Maybe canceled?
		if ((pRes->GetResult()))
		{
			svrChk(msgRes.ParseMessage(*((MessageResult*)pRes)->GetMessage()));

			AssertRel(msgRes.GetPlayers().size() <= MAX_NUM_PLAYER);

			for (uint iItem = 0; iItem < m_MatchedItems.size(); iItem++)
			{
				if (m_MatchedItems[iItem].MatchingTicket != msgRes.GetMatchingTicket())
					continue;

				m_DequeuedTotalMembers += (uint)msgRes.GetPlayers().size();
				m_MatchedItems[iItem].MemberCount = (uint)msgRes.GetPlayers().size();
				memcpy(m_MatchedItems[iItem].Players, msgRes.GetPlayers().data(), sizeof(MatchingPlayerInformation)*msgRes.GetPlayers().size());
				for (uint iPlayer = 0; iPlayer < m_MatchedItems[iItem].MemberCount; iPlayer++)
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

		// If we received all answers
		if (m_PendingDequeueItem <= 0)
		{
			// if total member is greater than zero
			if (m_DequeuedTotalMembers > 0)
			{
				// Create game
				if (!CreateGame())
				{
					// Game create is failed
					svrTrace(SVR_MATCHING, "Creating Game instance is failed item Matching:{0}", GetTargetMatchingMemberCount());
					CloseTransaction(ResultCode::GAME_NOTREADY);
				}
			}
			else
			{
				// if total member is zero, we don't have to care. just finish it
				CloseTransaction(ResultCode::SUCCESS);
			}
		}

		return hr;
	}

	Result MatchingTransProcessMatchedItems::CreateGame()
	{
		ScopeContext hr([this](Result hr)
			{
				if (!hr)
					SetTimer(DurationMS(1000));
			});
		SharedPointerT<ServerServiceInformation> pService;

		svrTrace(SVR_MATCHING, "Creating game Matching:{0}", GetTargetMatchingMemberCount());

		svrCheck(Service::ServiceDirectory->GetRandomService(Service::ServerConfig->GameClusterID, ClusterID::GameInstanceManager, pService));

		VariableTable attributes(GetHeap());
		attributes.SetValue("NumBot"_crc, (uint16_t)(m_TargetMatchingMemberCount - m_DequeuedTotalMembers));
		attributes.SetValue("MaxPlayer"_crc, m_TargetMatchingMemberCount);

		// 2. Get service entity list in the cluster
		svrCheck(pService->GetService<GameInstanceManagerService>()->CreateGameInstanceCmd(GetTransID(), 0, 1, attributes));

		return hr;
	}

	Result MatchingTransProcessMatchedItems::OnCreateGame(TransactionResult* pRes)
	{
		Result hr = ResultCode::SUCCESS;
		Message::GameInstanceManager::CreateGameInstanceRes msgRes;
		GameInsUID gameUID;
		uint notifiedPlayerCount = 0;


		svrChk(pRes->GetResult());

		svrChk(msgRes.ParseMessage(*((MessageResult*)pRes)->GetMessage()));

		gameUID = msgRes.GetRouteContext().GetFrom();


		// Send Game information to players
		for (size_t iItem = 0; iItem < m_MatchedItems.size(); iItem++)
		{
			auto& reservedMember = m_MatchedItems[iItem];

			bool notifiedToRegister = false;
			for (uint member = 0; member < reservedMember.MemberCount; member++)
			{
				if (reservedMember.Players[member].PlayerUID == reservedMember.RegisterEntityUID) notifiedToRegister = true;

				// FIXME: player entity might be fails
				auto playerEndpoint = Service::MessageEndpointManager->GetEndpoint(reservedMember.Players[member].PlayerUID);

				notifiedPlayerCount++;
				NetSvrPolicyPartyMatching(playerEndpoint).PlayerGameMatchedS2CEvt(
					RouteContext(GetOwnerEntityUID(), reservedMember.Players[member].PlayerUID), 0,
					reservedMember.Players[member].PlayerID, gameUID, (uint8_t)reservedMember.RequestedRole);
			}

			// this should be a party, or canceled item
			if (!notifiedToRegister)
			{
				auto requesterEndpoint = Service::MessageEndpointManager->GetEndpoint(reservedMember.RegisterEntityUID);

				//if (!(Service::ServerEntityManager->GetServerEntity(reservedMember.RegisterEntityUID.GetServerID(), pServerEntity)))
				//{
				//	// skip this player
				//	svrTrace(Error, "Failed to find Server entity({0}) while broadcasting", reservedMember.RegisterEntityUID.GetServerID());
				//	continue;
				//}

				notifiedPlayerCount++;
				NetSvrPolicyPartyMatching(requesterEndpoint).PartyGameMatchedS2CEvt(
					RouteContext(GetOwnerEntityUID(), reservedMember.RegisterEntityUID), 0);
			}
		}


	Proc_End:

		// clean up empty game instance
		if (notifiedPlayerCount == 0)
		{
			// Not joined game instance will be cleaned up automatically with timeout
			//auto gameEndpoint = Service::MessageEndpointManager->GetEndpoint(gameUID);
			//if (gameEndpoint)
			//{
			//	NetPolicyGameInstance(gameEndpoint).DeleteGameC2SEvt(RouteContext(GetOwnerEntityUID(), gameUID));
			//}
		}

		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	Result MatchingTransProcessMatchedItems::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		m_PendingDequeueItem = 0;
		m_DequeuedTotalMembers = 0;

		svrChk(super::StartTransaction());

		for (size_t iItem = 0; iItem < m_MatchedItems.size(); iItem++)
		{
			DequeueItem(m_MatchedItems[iItem].MatchingTicket);
		}


	Proc_End:

		if (!(hr) || m_PendingDequeueItem <= 0)
		{
			// We need to retry until it's successed
			CloseTransaction(hr);
		}

		return hr;
	}

}// namespace Svr 
}// namespace SF 

