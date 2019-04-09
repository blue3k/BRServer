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
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Memory/SFMemory.h"
#include "GameConst.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/MatchingServiceUtil.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/MatchingServiceTrans.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Entity/EntityManager.h"
#include "PerformanceCounter/PerformanceCounterClient.h"



namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	static const uint g_MatchingPattern4[] = 
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

	static const uint g_MatchingPattern8[] =
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

	uint MatchingServiceEntity::MatchingQueue_Single::GetEnqueueCount()
	{
		return (uint)m_pQueuePtr->GetEnqueCount();
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

	MatchingServiceEntity::MatchingQueue_Multiple::MatchingQueue_Multiple(IHeap& memMgr)
		: m_Heap(memMgr)
		, m_pQueuePtr(memMgr)
		, m_StartQueueIndex(0)
	{
	}

	Result MatchingServiceEntity::MatchingQueue_Multiple::AddQueue(PageQueue<ReservedMatchingItem>* pQueuePtr, uint maxPerMatch, PlayerRole requestRole)
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
		uint missTry = 0;
		StaticArray<uint, MAX_QUEUE_COUNT> pickedNumbers(GetHeap());

		pickedNumbers.resize(m_pQueuePtr.size());
		memset(pickedNumbers.data(), 0, sizeof(uint) * pickedNumbers.size());

		uint iDequeue = 0;
		for (; iDequeue == 0 && missTry < m_pQueuePtr.size();)
		{
			ReservedMatchingItem item;
			uint currentQueue = m_StartQueueIndex;
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
			m_StartQueueIndex = (decltype(m_StartQueueIndex))((currentQueue + 1) % m_pQueuePtr.size());
		}

	Proc_End:

		return iDequeue > 0 ? ResultCode::SUCCESS : ResultCode::FAIL;
	}

	uint MatchingServiceEntity::MatchingQueue_Multiple::GetEnqueueCount()
	{
		uint totalCount = 0;

		for (uint iQueue = 0; iQueue < m_pQueuePtr.size(); iQueue++)
		{
			totalCount += std::min((uint)m_pQueuePtr[iQueue].pQueue->GetEnqueCount(), m_pQueuePtr[iQueue].MaxAllowPerMatch);
		}

		return totalCount;
	}


	/////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingServiceEntity::MatchingServiceEntity(GameID gameID, ClusterID clusterID, bool useBot, ClusterMembership initialMembership)
		: ShardedClusterServiceEntity(gameID, clusterID, initialMembership )
		, m_IsUseBot(true)
		, m_MatchingReserevedQueues(GetHeap())
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
		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		auto matchingQueueCount = MatchingUtil::GetQueueCount(m_TargetMatchingMemberCount) + 1;
		MatchingServiceQueueEntity* newQueueEntity = nullptr;

		svrChk(ShardedClusterServiceEntity::InitializeEntity(newEntityID) );

		m_ReservationStartFrom = 1;
		m_WaitingBotMatchingStart = TimeStampMS::min();

		switch (m_TargetMatchingMemberCount)
		{
		case 4:
			svrChk(Service::ClusterManager->RegisterClustereWatchers(GetGameID(), ClusterID::MatchingQueue_Game_4x1, ClusterID::MatchingQueue_Game_4x1W));
			break;
		case 8:
			svrChk(Service::ClusterManager->RegisterClustereWatchers(GetGameID(), ClusterID::MatchingQueue_Game_8x1, ClusterID::MatchingQueue_Game_8x1W));
			break;
		default:
			assert(false); // not supported
			break;
		}

		svrChk(Service::ClusterManager->SetWatchForCluster(GetGameID(), ClusterID::GameInstanceManager, true));


		newQueueEntity = new(GetHeap()) MatchingServiceQueueEntity(m_TargetMatchingMemberCount, MIN_ITEM_RESERVATION, MAX_ITEM_RESERVATION);
		svrChkPtr(newQueueEntity);

		// Map queues
		m_MatchingReserevedQueues.push_back(nullptr);
		for (uint iQueue = 1; iQueue < (uint)matchingQueueCount; iQueue++)
		{
			if (iQueue == 1)
			{
				auto queueInterface = new(GetHeap()) MatchingQueue_Multiple(GetHeap());
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
				svrChk(m_MatchingReserevedQueues.push_back(new(GetHeap()) MatchingQueue_Single(queue)));
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

		svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, newQueueEntity));

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

	Result MatchingServiceEntity::GetMatchingPatterTable(uint targetMatchingMemberCount, uint& numTableEntries, const uint* &pTable)
	{
		switch (targetMatchingMemberCount)
		{
		case 4:
			numTableEntries = (uint)countof(g_MatchingPattern4) / targetMatchingMemberCount;
			pTable = g_MatchingPattern4;
			break;
		case 8:
			numTableEntries = (uint)countof(g_MatchingPattern8) / targetMatchingMemberCount;
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
		TransactionPtr pTrans;
		uint targetMatchingMemberCount = m_TargetMatchingMemberCount;
		uint numPatterns = 0;
		const uint* pMatchingPatternTable = nullptr;
		StaticArray<ReservedMatchingItem, MAX_PREPARED_PLAYER> grabbedItems(GetHeap());
		uint itemCountPerQueue[MAX_QUEUE_COUNT];
		uint numItemsInQueues = 0;
		const uint* pCurMatchingPattern = nullptr;
		const uint* pMatchingPattern = nullptr;


		memset(itemCountPerQueue, 0, sizeof(itemCountPerQueue));

		svrChk(GetMatchingPatterTable(targetMatchingMemberCount, numPatterns, pMatchingPatternTable));

		// increase pattern offset after getting pattern table
		m_ReservationStartFrom++;
		if (m_ReservationStartFrom >= numPatterns)
			m_ReservationStartFrom = 0;


		// cache queue item count
		for (uint iQueue = 1; iQueue < targetMatchingMemberCount; iQueue++)
		{
			itemCountPerQueue[iQueue] = (uint)m_MatchingReserevedQueues[iQueue]->GetEnqueueCount();
			numItemsInQueues += itemCountPerQueue[iQueue];
		}

		// no itmes to match
		if (numItemsInQueues == 0)
			goto Proc_End;

		// Pick matching pattern
		pCurMatchingPattern = pMatchingPatternTable;
		for (uint iPattern = 0; iPattern < numPatterns; iPattern++, pCurMatchingPattern += targetMatchingMemberCount)
		{
			//pCurMatchingPattern[0];

			bool bPatternMat = true;
			for (uint iQueue = 1; iQueue < targetMatchingMemberCount; iQueue++)
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


		for (uint iQueue = 1; iQueue < targetMatchingMemberCount; iQueue++)
		{
			uint numItemsToDequeue = pMatchingPattern[iQueue];
			for (uint iItem = 0; iItem < numItemsToDequeue; iItem++)
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
		pTrans = new(GetHeap()) MatchingTransProcessMatchedItems(GetHeap(), targetMatchingMemberCount, grabbedItems);
		svrMem(pTrans);
		svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));


	Proc_End:

		return hr;
	}

	Result MatchingServiceEntity::UpdateBotMatching()
	{
		Result hr = ResultCode::SUCCESS;
		TransactionPtr pTrans;
		uint targetMatchingMemberCount = m_TargetMatchingMemberCount;
		StaticArray<ReservedMatchingItem, MAX_PREPARED_PLAYER> grabbedItems(GetHeap());
		uint grabbedPlayerCount = 0;

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
			pTrans = new(GetHeap()) MatchingTransProcessMatchedItems(GetHeap(), targetMatchingMemberCount, grabbedItems);
			svrMem(pTrans);
			svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));
		}

	Proc_End:

		return hr;
	}

	uint MatchingServiceEntity::GetGrabbedPlayerCount(Array<ReservedMatchingItem>& grabbedItems)
	{
		uint grabbedPlayerCount = 0;
		for (uint iGrab = 0; iGrab < grabbedItems.size(); iGrab++)
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
		for (uint iQueue = 1; iQueue < m_TargetMatchingMemberCount; iQueue++)
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

	MatchingServiceQueueEntity::MatchingServiceQueueEntity(uint matchingMemberCount, uint minQueueCount, uint maxQueueCount)
		: m_MatchingMemberCount(matchingMemberCount)
		, m_MinQueueCount(minQueueCount)
		, m_MaxQueueCount(maxQueueCount)
	{
		for (auto& itQueue : m_ReservedItemQueue)
		{
			itQueue = new(GetHeap()) PageQueue<MatchingServiceEntity::ReservedMatchingItem>(GetHeap());
		}
	}

	MatchingServiceQueueEntity::~MatchingServiceQueueEntity()
	{
		for (auto& itQueue : m_ReservedItemQueue)
		{
			IHeap::Delete(itQueue);
		}
	}

	PageQueue<MatchingServiceEntity::ReservedMatchingItem>& MatchingServiceQueueEntity::GetReservedItemQueue(uint iMemberCount, PlayerRole requestRole)
	{
		if (iMemberCount == 1 && m_MatchingMemberCount > 2)
		{
			if (requestRole == PlayerRole::Werewolf)   return *m_ReservedItemQueue[(uint)m_MatchingMemberCount + 0];
			else if (requestRole == PlayerRole::Seer)  return *m_ReservedItemQueue[(uint)m_MatchingMemberCount + 1];
		}

		return *m_ReservedItemQueue[iMemberCount];
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
			uint minComponentID, maxComponentID;
			svrChk(MatchingUtil::GetQueueComponentIDMinMax(m_MatchingMemberCount, minComponentID, maxComponentID));

			for (uint iQueue = 1, compoID = minComponentID; compoID <= maxComponentID; iQueue++, compoID++)
			{
				if (GetTransactionStartedForQueue(iQueue) <= 0)
				{
					auto playerRole = MatchingUtil::GetPlayerRoleFromQueueComponentID(compoID);
					auto targetMemberCount = MatchingUtil::GetPartyMemberCountFromQueueComponentID(compoID);
					TransactionPtr pTrans = new(GetHeap()) MatchingTransGrabPlayer(GetHeap(), m_MatchingMemberCount, targetMemberCount, playerRole, m_MinQueueCount, m_MaxQueueCount);
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
}; // namespace SF {



