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
#include "Common/BrMemory.h"
#include "Common/GameConst.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceUtil.h"
#include "ServerSystem/ServiceEntity/MatchingServiceEntity.h"
#include "ServerSystem/ServiceEntity/MatchingServiceTrans.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"



namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	static const UINT g_MatchingPattern4[] = 
	{
		// 1  2  3
		0, 4, 0, 0,
		0, 2, 1, 0,
		0, 1, 0, 1,
		// repeteided pattern for matching balance
		0, 1, 0, 1,
		0, 1, 0, 1,
		0, 1, 0, 1,

		// repeated pattern for circulation
		0, 4, 0, 0,
		0, 2, 1, 0,
		0, 1, 0, 1,
		// repeteided pattern for matching balance
		0, 1, 0, 1,
		0, 1, 0, 1,
		0, 1, 0, 1,
	};

	static const UINT g_MatchingPattern8[] =
	{
		////   1  2  3  4  5  6  7
		//{ 0, 8, 0, 0, 0, 0, 0, 0 },
		//{ 0, 6, 1, 0, 0, 0, 0, 0 },
		//{ 0, 5, 0, 1, 0, 0, 0, 0 },
		//{ 0, 4, 0, 0, 1, 0, 0, 0 },
		//{ 0, 4, 2, 0, 0, 0, 0, 0 },
		//{ 0, 3, 0, 0, 0, 1, 0, 0 },
		//{ 0, 3, 1, 1, 0, 0, 0, 0 },
		//{ 0, 2, 0, 0, 0, 0, 1, 0 },
		//{ 0, 2, 1, 0, 1, 0, 0, 0 },
		//{ 0, 2, 0, 2, 0, 0, 0, 0 },
		//{ 0, 2, 3, 0, 0, 0, 0, 0 },
		//{ 0, 1, 0, 0, 0, 0, 0, 1 },
		//{ 0, 1, 1, 0, 0, 1, 0, 0 },
		//{ 0, 1, 0, 1, 1, 0, 0, 0 },
		//{ 0, 1, 2, 1, 0, 0, 0, 0 },
		//{ 0, 0, 4, 0, 0, 0, 0, 0 },
		//{ 0, 0, 2, 0, 1, 0, 0, 0 },
		//{ 0, 0, 1, 0, 0, 0, 1, 0 },
		//{ 0, 0, 1, 2, 0, 0, 0, 0 },
		//{ 0, 0, 0, 1, 0, 1, 0, 0 },
		//{ 0, 0, 0, 0, 2, 0, 0, 0 },

		0, 8, 0, 0, 0, 0, 0, 0,
		0, 6, 1, 0, 0, 0, 0, 0,
		0, 5, 0, 1, 0, 0, 0, 0,
		0, 4, 0, 0, 1, 0, 0, 0,
		0, 4, 2, 0, 0, 0, 0, 0,
		0, 3, 0, 0, 0, 1, 0, 0,
		0, 3, 1, 1, 0, 0, 0, 0,
		0, 2, 0, 0, 0, 0, 1, 0,
		0, 2, 1, 0, 1, 0, 0, 0,
		0, 2, 0, 2, 0, 0, 0, 0,
		0, 2, 3, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 1, 0, 0, 1, 0, 0,
		0, 1, 0, 1, 1, 0, 0, 0,
		0, 1, 2, 1, 0, 0, 0, 0,
		0, 0, 4, 0, 0, 0, 0, 0,
		0, 0, 2, 0, 1, 0, 0, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 2, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,

		// repeteided patterns for matching balance
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,

		// repeated pattern for circulation
		0, 8, 0, 0, 0, 0, 0, 0,
		0, 6, 1, 0, 0, 0, 0, 0,
		0, 5, 0, 1, 0, 0, 0, 0,
		0, 4, 0, 0, 1, 0, 0, 0,
		0, 4, 2, 0, 0, 0, 0, 0,
		0, 3, 0, 0, 0, 1, 0, 0,
		0, 3, 1, 1, 0, 0, 0, 0,
		0, 2, 0, 0, 0, 0, 1, 0,
		0, 2, 1, 0, 1, 0, 0, 0,
		0, 2, 0, 2, 0, 0, 0, 0,
		0, 2, 3, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 1, 0, 0, 1, 0, 0,
		0, 1, 0, 1, 1, 0, 0, 0,
		0, 1, 2, 1, 0, 0, 0, 0,
		0, 0, 4, 0, 0, 0, 0, 0,
		0, 0, 2, 0, 1, 0, 0, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 2, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,

		// repeteided patterns for matching balance
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 0, 1,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0,
	};


	MatchingServiceEntity::ReservedMatchingItem& MatchingServiceEntity::ReservedMatchingItem::operator = (const ReservedMatchingItem& src)
	{
		MatchingTicket = src.MatchingTicket;
		MemberCount = src.MemberCount;
		RequestedRole = src.RequestedRole;

		return *this;
	}

	MatchingServiceEntity::ReservedMatchingItem& MatchingServiceEntity::ReservedMatchingItem::operator = (ReservedMatchingItem&& src)
	{
		MatchingTicket = src.MatchingTicket;
		MemberCount = src.MemberCount;
		RequestedRole = src.RequestedRole;

		src.MatchingTicket = 0;
		src.MemberCount = 0;
		src.RequestedRole = PlayerRole::None;

		return *this;
	}



	MatchingServiceEntity::MatchingItem& MatchingServiceEntity::MatchingItem::operator = (const MatchingItem& src)
	{
		MatchingTicket = src.MatchingTicket;
		MemberCount = src.MemberCount;
		RequestedRole = src.RequestedRole;

		return *this;
	}

	MatchingServiceEntity::MatchingItem& MatchingServiceEntity::MatchingItem::operator = (MatchingItem&& src)
	{
		MatchingTicket = src.MatchingTicket;
		MemberCount = src.MemberCount;
		RequestedRole = src.RequestedRole;

		src.MatchingTicket = 0;
		src.MemberCount = 0;
		src.RequestedRole = PlayerRole::None;

		return *this;
	}


	/////////////////////////////////////////////////////////////
	//
	// MatchingQueue_Single
	//


	MatchingServiceEntity::MatchingQueue_Single::MatchingQueue_Single(PageQueue<ReservedMatchingItem>* pQueuePtr)
		: m_pQueuePtr(pQueuePtr)
	{
		AssertRel(pQueuePtr != nullptr);
	}

	Result MatchingServiceEntity::MatchingQueue_Single::Dequeue(Array<ReservedMatchingItem>& items)
	{
		ReservedMatchingItem item;
		if(!(m_pQueuePtr->Dequeue(item))) return ResultCode::FAIL;
		return items.push_back(item);
	}

	UINT MatchingServiceEntity::MatchingQueue_Single::GetEnqueueCount()
	{
		return (UINT)m_pQueuePtr->GetEnqueCount();
	}





	/////////////////////////////////////////////////////////////
	//
	// MatchingQueue_Multiple
	//
	const MatchingServiceEntity::MatchingQueue_Multiple::QueueItem MatchingServiceEntity::MatchingQueue_Multiple::QueueItem::NullValue;

	bool MatchingServiceEntity::MatchingQueue_Multiple::QueueItem::operator == (const QueueItem& src) const
	{
		return pQueue == src.pQueue;
	}

	MatchingServiceEntity::MatchingQueue_Multiple::QueueItem& MatchingServiceEntity::MatchingQueue_Multiple::QueueItem::operator = (QueueItem&& src)
	{
		pQueue = src.pQueue;
		MaxAllowPerMatch = src.MaxAllowPerMatch;
		RequestRole = src.RequestRole;

		src.pQueue = nullptr;
		src.MaxAllowPerMatch = 0;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingServiceEntity::MatchingQueue_Multiple::MatchingQueue_Multiple()
		: m_StartQueueIndex(0)
	{
	}

	Result MatchingServiceEntity::MatchingQueue_Multiple::AddQueue(PageQueue<ReservedMatchingItem>* pQueuePtr, UINT maxPerMatch, PlayerRole requestRole)
	{
		Result hr = ResultCode::SUCCESS;

		QueueItem item;
		item.MaxAllowPerMatch = maxPerMatch;
		item.pQueue = pQueuePtr;
		item.RequestRole = requestRole;

		svrChk(m_pQueuePtr.push_back(std::forward<QueueItem>(item)));

	Proc_End:

		return hr;
	}

	void MatchingServiceEntity::MatchingQueue_Multiple::ResetForMatch()
	{
	}

	Result MatchingServiceEntity::MatchingQueue_Multiple::Dequeue(Array<ReservedMatchingItem>& items)
	{
		Result hr = ResultCode::SUCCESS;
		UINT missTry = 0;
		StaticArray<UINT, MAX_QUEUE_COUNT> pickedNumbers;

		pickedNumbers.SetSize(m_pQueuePtr.GetSize());
		memset(pickedNumbers.data(), 0, sizeof(UINT) * pickedNumbers.GetSize());

		UINT iDequeue = 0;
		for (; iDequeue == 0 && missTry < m_pQueuePtr.GetSize();)
		{
			ReservedMatchingItem item;
			UINT currentQueue = m_StartQueueIndex;
			auto pickedNumber = pickedNumbers[currentQueue];
			if (pickedNumber < m_pQueuePtr[currentQueue].MaxAllowPerMatch)
			{
				if ((m_pQueuePtr[currentQueue].pQueue->Dequeue(item)))
				{
					item.RequestedRole = m_pQueuePtr[currentQueue].RequestRole;
					svrChk(items.push_back(item));
					iDequeue++;
					missTry = 0;
				}
				else
					missTry++;
			}
			else
			{
				missTry++;
			}
			m_StartQueueIndex = (decltype(m_StartQueueIndex))((currentQueue + 1) % m_pQueuePtr.GetSize());
		}

	Proc_End:

		return iDequeue > 0 ? ResultCode::SUCCESS : ResultCode::FAIL;
	}

	UINT MatchingServiceEntity::MatchingQueue_Multiple::GetEnqueueCount()
	{
		UINT totalCount = 0;

		for (UINT iQueue = 0; iQueue < m_pQueuePtr.GetSize(); iQueue++)
		{
			totalCount += std::min((UINT)m_pQueuePtr[iQueue].pQueue->GetEnqueCount(), m_pQueuePtr[iQueue].MaxAllowPerMatch);
		}

		return totalCount;
	}


	/////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingServiceEntity::MatchingServiceEntity(ClusterID clusterID, ClusterMembership initialMembership, bool useBot)
		: ShardedClusterServiceEntity(clusterID, initialMembership )
		, m_IsUseBot(true)
		, m_MatchedCount("Matched")
	{
		switch(clusterID)
		{
		case ClusterID::Matching_Game_4:
			m_TargetMatchingMemberCount = 4;
			break;
		case ClusterID::Matching_Game_8:
			m_TargetMatchingMemberCount = 8;
			break;
		default:
			Assert(false);
			m_TargetMatchingMemberCount = 0;
			break;
		}

		m_MaxMatchingQueue = m_TargetMatchingMemberCount + 2; // 


		m_IsUseBot = useBot;

		SetTickInterval(DurationMS(1));
	}

	MatchingServiceEntity::~MatchingServiceEntity()
	{
	}

	Result MatchingServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		char strName[128];
		auto entityManager = GetServerComponent<EntityManager>();
		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		auto matchingQueueCount = MatchingUtil::GetQueueCount(m_TargetMatchingMemberCount) + 1;
		MatchingServiceQueueEntity* newQueueEntity = nullptr;

		svrChk(ShardedClusterServiceEntity::InitializeEntity(newEntityID) );

		m_ReservationStartFrom = 1;
		m_WaitingBotMatchingStart = TimeStampMS::min();


		newQueueEntity = new MatchingServiceQueueEntity(m_TargetMatchingMemberCount, MIN_ITEM_RESERVATION, MAX_ITEM_RESERVATION);
		svrChkPtr(newQueueEntity);

		// TODO: Map queues
		m_MatchingReserevedQueues.push_back(nullptr);
		for (UINT iQueue = 1; iQueue < (UINT)matchingQueueCount; iQueue++)
		{
			if (iQueue == 1)
			{
				auto queueInterface = new MatchingQueue_Multiple();
				queueInterface->AddQueue(&newQueueEntity->GetReservedItemQueue(iQueue, PlayerRole::None), MAX_NUM_PLAYER, PlayerRole::None);
				if (m_TargetMatchingMemberCount == 4)
				{
					queueInterface->AddQueue(&newQueueEntity->GetReservedItemQueue(1, PlayerRole::Seer), 0, PlayerRole::Seer);
					queueInterface->AddQueue(&newQueueEntity->GetReservedItemQueue(1, PlayerRole::Werewolf), 1, PlayerRole::Werewolf);
				}
				else // m_TargetMatchingMemberCount == 8
				{
					queueInterface->AddQueue(&newQueueEntity->GetReservedItemQueue(1, PlayerRole::Seer), 1, PlayerRole::Seer);
					queueInterface->AddQueue(&newQueueEntity->GetReservedItemQueue(1, PlayerRole::Werewolf), 2, PlayerRole::Werewolf);
				}
				svrChk(m_MatchingReserevedQueues.push_back(queueInterface));

			}
			else
			{
				PageQueue<ReservedMatchingItem>* queue = &newQueueEntity->GetReservedItemQueue(iQueue, PlayerRole::None);
				svrChk(m_MatchingReserevedQueues.push_back(new MatchingQueue_Single(queue)));
			}
			


			if (pInstance != nullptr)
			{
				StrUtil::Format(strName, "Matching Cache: {0}-{1}", m_TargetMatchingMemberCount, iQueue);
				m_ItemCounts[iQueue - 1].SetCounterName(strName);

				pInstance->AddCounter(&m_ItemCounts[iQueue - 1]);
			}
		}

		StrUtil::Format(strName, "Matching matched : {0}", m_TargetMatchingMemberCount);
		m_MatchedCount.SetCounterName(strName);
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_MatchedCount);
		}

		svrChk(entityManager->AddEntity(EntityFaculty::Service, newQueueEntity));

		m_pQueueEntity = SharedPointerT<Entity>(newQueueEntity);

	Proc_End:

		return hr;
	}

	// clear transaction
	Result MatchingServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ShardedClusterServiceEntity::ClearEntity() );

		// we should close queue entity
		if (m_pQueueEntity != nullptr)
		{
			SharedPointerT<Entity> queueEntity;
			m_pQueueEntity.GetSharedPointer(queueEntity);
			if (queueEntity != nullptr)
			{
				queueEntity->TerminateEntity();
			}
		}

	Proc_End:

		return hr;
	}

	Result MatchingServiceEntity::GetMatchingPatterTable(UINT targetMatchingMemberCount, UINT& numTableEntries, const UINT* &pTable)
	{
		switch (targetMatchingMemberCount)
		{
		case 4:
			numTableEntries = (UINT)countof(g_MatchingPattern4) / targetMatchingMemberCount;
			pTable = g_MatchingPattern4;
			break;
		case 8:
			numTableEntries = (UINT)countof(g_MatchingPattern8) / targetMatchingMemberCount;
			pTable = g_MatchingPattern8;
			break;
		default:
			return ResultCode::UNEXPECTED;
		};

		// table count will be smaller one
		numTableEntries >>= 1;
		// Apply offset
		pTable += targetMatchingMemberCount * m_ReservationStartFrom;

		return ResultCode::SUCCESS;
	}

	Result MatchingServiceEntity::UpdateMatching()
	{
		Result hr = ResultCode::SUCCESS;
		Transaction *pTrans = nullptr;
		UINT targetMatchingMemberCount = m_TargetMatchingMemberCount;
		UINT numPatterns = 0;
		const UINT* pMatchingPatternTable = nullptr;
		StaticArray<ReservedMatchingItem, MAX_PREPARED_PLAYER> grabbedItems;
		UINT itemCountPerQueue[MAX_QUEUE_COUNT];
		UINT numItemsInQueues = 0;
		const UINT* pCurMatchingPattern = nullptr;
		const UINT* pMatchingPattern = nullptr;


		memset(itemCountPerQueue, 0, sizeof(itemCountPerQueue));

		svrChk(GetMatchingPatterTable(targetMatchingMemberCount, numPatterns, pMatchingPatternTable));

		// increase pattern offset after getting pattern table
		m_ReservationStartFrom++;
		if (m_ReservationStartFrom >= numPatterns)
			m_ReservationStartFrom = 0;


		// cache queue item count
		for (UINT iQueue = 1; iQueue < targetMatchingMemberCount; iQueue++)
		{
			itemCountPerQueue[iQueue] = (UINT)m_MatchingReserevedQueues[iQueue]->GetEnqueueCount();
			numItemsInQueues += itemCountPerQueue[iQueue];
		}

		// no itmes to match
		if (numItemsInQueues == 0)
			goto Proc_End;

		// Pick matching pattern
		pCurMatchingPattern = pMatchingPatternTable;
		for (UINT iPattern = 0; iPattern < numPatterns; iPattern++, pCurMatchingPattern += targetMatchingMemberCount)
		{
			//pCurMatchingPattern[0];

			bool bPatternMat = true;
			for (UINT iQueue = 1; iQueue < targetMatchingMemberCount; iQueue++)
			{
				if (itemCountPerQueue[iQueue] < pCurMatchingPattern[iQueue])
				{
					bPatternMat = false;
					break;
				}
			}
			if (!bPatternMat)
				continue;

			pMatchingPattern = pCurMatchingPattern;
			// we've found best suit matching pattern
			break;
		}


		if (pMatchingPattern == nullptr)
		{
			if (numItemsInQueues > 0 && GetIsUseBot())
				return UpdateBotMatching();

			return hr;
		}


		for (UINT iQueue = 1; iQueue < targetMatchingMemberCount; iQueue++)
		{
			UINT numItemsToDequeue = pMatchingPattern[iQueue];
			for (UINT iItem = 0; iItem < numItemsToDequeue; iItem++)
			{
				if (!(m_MatchingReserevedQueues[iQueue]->Dequeue(grabbedItems)))
				{
					Assert(false);
				}
			}
		}

		GetGrabbedPlayerCount(grabbedItems);


		m_WaitingBotMatchingStart = TimeStampMS::min();

		++m_MatchedCount;
		pTrans = new MatchingTransProcessMatchedItems(targetMatchingMemberCount, grabbedItems);
		svrMem(pTrans);
		svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));


	Proc_End:

		return hr;
	}

	Result MatchingServiceEntity::UpdateBotMatching()
	{
		Result hr = ResultCode::SUCCESS;
		Transaction *pTrans = nullptr;
		UINT targetMatchingMemberCount = m_TargetMatchingMemberCount;
		StaticArray<ReservedMatchingItem, MAX_PREPARED_PLAYER> grabbedItems;
		UINT grabbedPlayerCount = 0;

		if (m_WaitingBotMatchingStart == TimeStampMS::min())
		{
			m_WaitingBotMatchingStart = Util::Time.GetTimeMs();
			return hr;
		}

		if (Util::TimeSince(m_WaitingBotMatchingStart) < DurationMS(TIME_BOTMATCHING_WAIT))
			return hr;

		// grab one
		INT iQueue = 1;
		for (; iQueue < (INT)m_TargetMatchingMemberCount; iQueue++)
		{
			if ((m_MatchingReserevedQueues[iQueue]->Dequeue(grabbedItems)))
			{
				break;
			}
		}

		grabbedPlayerCount = GetGrabbedPlayerCount(grabbedItems);


		for (iQueue = m_TargetMatchingMemberCount - grabbedPlayerCount; grabbedPlayerCount < m_TargetMatchingMemberCount && iQueue > 0;)
		{
			if ((m_MatchingReserevedQueues[iQueue]->Dequeue(grabbedItems)))
			{
				grabbedPlayerCount = GetGrabbedPlayerCount(grabbedItems);
				iQueue = m_TargetMatchingMemberCount - grabbedPlayerCount;
			}
			else
			{
				iQueue--;
			}
		}


		m_WaitingBotMatchingStart = TimeStampMS::min();

		if (grabbedPlayerCount > 0)
		{
			++m_MatchedCount;
			pTrans = new MatchingTransProcessMatchedItems(targetMatchingMemberCount, grabbedItems);
			svrMem(pTrans);
			svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));
		}

	Proc_End:

		return hr;
	}

	UINT MatchingServiceEntity::GetGrabbedPlayerCount(Array<ReservedMatchingItem>& grabbedItems)
	{
		UINT grabbedPlayerCount = 0;
		for (UINT iGrab = 0; iGrab < grabbedItems.GetSize(); iGrab++)
		{
			grabbedPlayerCount += grabbedItems[iGrab].MemberCount;
		}
		return grabbedPlayerCount;
	}

	Result MatchingServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		hr = ShardedClusterServiceEntity::TickUpdate(pAction);
		if (hr == Result(ResultCode::SUCCESS_FALSE))
			return hr;
		svrChk(hr);

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			goto Proc_End;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			goto Proc_End;


		UpdateMatching();

		// update perf counters
		for (UINT iQueue = 1; iQueue < m_TargetMatchingMemberCount; iQueue++)
		{
			m_ItemCounts[iQueue - 1] = m_MatchingReserevedQueues[iQueue]->GetEnqueueCount();
		}



	Proc_End:

		return hr;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Matching operations
	//





	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MatchingWatcherServiceEntity class
	//


	MatchingWatcherServiceEntity::MatchingWatcherServiceEntity( ClusterID clusterID, UINT componentID )
		:RingClusterServiceEntity(clusterID, ClusterMembership::StatusWatcher)
		,IServerComponent(componentID)
	{
	}

	MatchingWatcherServiceEntity::~MatchingWatcherServiceEntity()
	{
	}




	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MatchingWatcherServiceEntity class
	//

	MatchingServiceQueueEntity::MatchingServiceQueueEntity(UINT matchingMemberCount, UINT minQueueCount, UINT maxQueueCount)
		: m_MatchingMemberCount(matchingMemberCount)
		, m_MinQueueCount(minQueueCount)
		, m_MaxQueueCount(maxQueueCount)
	{

	}

	MatchingServiceQueueEntity::~MatchingServiceQueueEntity()
	{

	}

	PageQueue<MatchingServiceEntity::ReservedMatchingItem>& MatchingServiceQueueEntity::GetReservedItemQueue(UINT iMemberCount, PlayerRole requestRole)
	{
		if (iMemberCount == 1 && m_MatchingMemberCount > 2)
		{
			if (requestRole == PlayerRole::Werewolf)   return m_ReservedItemQueue[(UINT)m_MatchingMemberCount + 0];
			else if (requestRole == PlayerRole::Seer)  return m_ReservedItemQueue[(UINT)m_MatchingMemberCount + 1];
		}

		return m_ReservedItemQueue[iMemberCount];
	}

	Result MatchingServiceQueueEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr = ResultCode::SUCCESS;

		memset(m_ReservedItemQueueTransaction, 0, sizeof m_ReservedItemQueueTransaction);

		svrChk(MasterEntity::InitializeEntity(newEntityID));

	Proc_End:

		return hr;
	}

	// TickUpdate 
	Result MatchingServiceQueueEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		hr = MasterEntity::TickUpdate(pAction);
		if (hr == Result(ResultCode::SUCCESS_FALSE))
			return hr;

		// don't update query when the server isn't running
		if (BrServer::GetInstance()->GetServerState() != Svr::ServerState::RUNNING)
			return hr;

		if (GetPendingTransactionCount() == 0 && GetActiveTransactionCount() < (m_MatchingMemberCount-1))
		{
			UINT minComponentID, maxComponentID;
			svrChk(MatchingUtil::GetQueueComponentIDMinMax(m_MatchingMemberCount, minComponentID, maxComponentID));

			for (UINT iQueue = 1, compoID = minComponentID; compoID <= maxComponentID; iQueue++, compoID++)
			{
				if (GetTransactionStartedForQueue(iQueue) <= 0)
				{
					auto playerRole = MatchingUtil::GetPlayerRoleFromQueueComponentID(compoID);
					auto targetMemberCount = MatchingUtil::GetPartyMemberCountFromQueueComponentID(compoID);
					Transaction *pTrans = new MatchingTransGrabPlayer(m_MatchingMemberCount, targetMemberCount, playerRole, m_MinQueueCount, m_MaxQueueCount);
					svrMem(pTrans);
					svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));
				}
			}

		}

	Proc_End:

		return hr;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MatchingWatcherServiceEntity class
	//

	MatchingServiceHandleMatchedEntity::MatchingServiceHandleMatchedEntity()
	{
	}

	MatchingServiceHandleMatchedEntity::~MatchingServiceHandleMatchedEntity()
	{
	}

	Result MatchingServiceHandleMatchedEntity::InitializeEntity(EntityID newEntityID)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(InitializeEntity(newEntityID));

	Proc_End:

		return hr;
	}




}; // namespace Svr {
}; // namespace BR {



