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



#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/Memory.h"
#include "Common/GameConst.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServicebase.h"
#include "ServerSystem/ServerEntity.h"
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

	const MatchingServiceEntity::ReservedMatchingItem MatchingServiceEntity::ReservedMatchingItem::NullValue;

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


	MatchingServiceEntity::ReservedMatchingItem& MatchingServiceEntity::ReservedMatchingItem::operator = (ReservedMatchingItem&& src)
	{
		MatchingTicket = src.MatchingTicket;
		MemberCount = src.MemberCount;

		src.MatchingTicket = 0;
		src.MemberCount = 0;

		return *this;
	}

	const MatchingServiceEntity::MatchingItem MatchingServiceEntity::MatchingItem::NullValue;

	MatchingServiceEntity::MatchingItem& MatchingServiceEntity::MatchingItem::operator = (MatchingItem&& src)
	{
		MatchingTicket = src.MatchingTicket;
		MemberCount = src.MemberCount;

		src.MatchingTicket = 0;
		src.MemberCount = 0;

		return *this;
	}


	/////////////////////////////////////////////////////////////
	//
	//
	//

	MatchingServiceEntity::MatchingServiceEntity(ClusterID clusterID, ClusterMembership initialMembership, const char* parameters)
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
		case ClusterID::Matching_Game_10:
			m_TargetMatchingMemberCount = 10;
			break;
		case ClusterID::Matching_Game_12:
			m_TargetMatchingMemberCount = 12;
			break;
		default:
			AssertRel(false);
			m_TargetMatchingMemberCount = 0;
			break;
		}

		if (parameters == nullptr)
			return;

		char strParameterBuffer[1024];
		char *curParameter = strParameterBuffer;
		StrUtil::StringCpy(strParameterBuffer, parameters);
		StrUtil::StringLwr(strParameterBuffer);
		size_t parameterBufferLength = sizeof(strParameterBuffer);
		while (curParameter != nullptr)
		{
			char* pTemp = nullptr;
			auto tokParam = strtok_s(curParameter, ",", &pTemp);
			if (tokParam == nullptr) break; // end of pattern

			size_t parameterLength = 0;
			if (pTemp != nullptr)
			{
				*pTemp = '\0';
				parameterLength = pTemp - curParameter;
			}
			else
			{
				parameterLength = strlen(curParameter);
			}

			const char* parameterName = "usebot";
			auto parameterNameLen = strlen(parameterName);
			size_t separate = std::min(parameterLength, parameterNameLen);
			curParameter[separate] = '\0';
			if (StrUtil::StringCmp(curParameter, (int)separate, parameterName, (int)parameterNameLen) == 0)
			{
				m_IsUseBot = StrUtil::StringCmp(curParameter + parameterNameLen + 1, (int)(parameterLength - parameterNameLen - 1), "true", (int)strlen("true")) == 0;
			}

			curParameter = pTemp;
		}


		SetTickInterval(1);
	}

	MatchingServiceEntity::~MatchingServiceEntity()
	{
	}

	HRESULT MatchingServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;
		char strName[128];
		auto entityManager = GetServerComponent<EntityManager>();
		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();

		svrChk( __super::InitializeEntity(newEntityID) );

		m_ReservationStartFrom = 1;
		m_WaitingBotMatchingStart = 0;


		auto newQueueEntity = new MatchingServiceQueueEntity(m_TargetMatchingMemberCount, MIN_ITEM_RESERVATION, MAX_ITEM_RESERVATION);
		svrChkPtr(newQueueEntity);

		// TODO: Map queues
		m_MatchingReserevedQueues.push_back(nullptr);
		for (UINT iMemberCount = 1; iMemberCount < m_TargetMatchingMemberCount; iMemberCount++)
		{
			PageQueue<ReservedMatchingItem>& queue = newQueueEntity->GetReservedItemQueue(iMemberCount);
			svrChk(m_MatchingReserevedQueues.push_back(&queue));


			if (pInstance != nullptr)
			{
				StrUtil::Format(strName, "Matching Cache: %0%-%1%", m_TargetMatchingMemberCount, iMemberCount);
				m_ItemCounts[iMemberCount - 1].SetCounterName(strName);

				pInstance->AddCounter(&m_ItemCounts[iMemberCount - 1]);
			}
		}

		StrUtil::Format(strName, "Matching matched : %0%", m_TargetMatchingMemberCount);
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
	HRESULT MatchingServiceEntity::ClearEntity()
	{
		HRESULT hr = S_OK;

		svrChk( __super::ClearEntity() );

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

	HRESULT MatchingServiceEntity::GetMatchingPatterTable(UINT targetMatchingMemberCount, UINT& numTableEntries, const UINT* &pTable)
	{
		switch (targetMatchingMemberCount)
		{
		case 4:
			numTableEntries = _countof(g_MatchingPattern4) / targetMatchingMemberCount;
			pTable = g_MatchingPattern4;
			break;
		case 8:
			numTableEntries = _countof(g_MatchingPattern8) / targetMatchingMemberCount;
			pTable = g_MatchingPattern8;
			break;
		default:
			return E_UNEXPECTED;
		};

		// table count will be smaller one
		numTableEntries >>= 1;
		// Apply offset
		pTable += targetMatchingMemberCount * m_ReservationStartFrom;

		return S_OK;
	}

	HRESULT MatchingServiceEntity::UpdateMatching()
	{
		HRESULT hr = S_OK;
		Transaction *pTrans = nullptr;
		UINT targetMatchingMemberCount = m_TargetMatchingMemberCount;
		UINT grabbedPlayerCount = 0;
		UINT numPatterns = 0;
		const UINT* pMatchingPatternTable = nullptr;
		StaticArray<ReservedMatchingItem, MAX_PREPARED_PLAYER> grabbedItems;
		UINT itemCountPerQueue[MAX_QUEUE_COUNT];
		UINT numItemsInQueues = 0;

		memset(itemCountPerQueue, 0, sizeof(itemCountPerQueue));

		svrChk(GetMatchingPatterTable(targetMatchingMemberCount, numPatterns, pMatchingPatternTable));

		// increase pattern offset after getting pattern table
		m_ReservationStartFrom++;
		if (m_ReservationStartFrom >= numPatterns)
			m_ReservationStartFrom = 0;


		// cache queue item count
		for (UINT iQueue = 1; iQueue < targetMatchingMemberCount; iQueue++)
		{
			itemCountPerQueue[iQueue] = (UINT)m_MatchingReserevedQueues[iQueue]->GetEnqueCount();
			numItemsInQueues += itemCountPerQueue[iQueue];
		}

		// Pick matching pattern
		const UINT* pCurMatchingPattern = pMatchingPatternTable;
		const UINT* pMatchingPattern = nullptr;
		for (UINT iPattern = 0; iPattern < numPatterns; iPattern++, pCurMatchingPattern += targetMatchingMemberCount)
		{
			pCurMatchingPattern[0];

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
				ReservedMatchingItem item;
				if (SUCCEEDED(m_MatchingReserevedQueues[iQueue]->Dequeue(item)))
				{
					grabbedPlayerCount += item.MemberCount;
					svrChk(grabbedItems.push_back(item));
				}
				else
				{
					Assert(false);
				}
			}
		}

		m_WaitingBotMatchingStart = 0;

		++m_MatchedCount;
		pTrans = new MatchingTransProcessMatchedItems(targetMatchingMemberCount, grabbedItems);
		svrMem(pTrans);
		svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), pTrans));


	Proc_End:

		return hr;
	}

	HRESULT MatchingServiceEntity::UpdateBotMatching()
	{
		HRESULT hr = S_OK;
		Transaction *pTrans = nullptr;
		UINT targetMatchingMemberCount = m_TargetMatchingMemberCount;
		StaticArray<ReservedMatchingItem, MAX_PREPARED_PLAYER> grabbedItems;
		UINT grabbedPlayerCount = 0;

		if (m_WaitingBotMatchingStart == 0)
		{
			m_WaitingBotMatchingStart = Util::Time.GetTimeMs();
			return hr;
		}

		if (Util::TimeSince(m_WaitingBotMatchingStart) < TIME_BOTMATCHING_WAIT)
			return hr;

		// grab one
		INT iQueue = 1;
		for (; grabbedPlayerCount < m_TargetMatchingMemberCount && iQueue > 0; iQueue++)
		{
			ReservedMatchingItem item;
			if (SUCCEEDED(m_MatchingReserevedQueues[iQueue]->Dequeue(item)))
			{
				grabbedPlayerCount += item.MemberCount;
				svrChk(grabbedItems.push_back(item));
				break;
			}
		}


		for (iQueue = m_TargetMatchingMemberCount - grabbedPlayerCount; grabbedPlayerCount < m_TargetMatchingMemberCount && iQueue > 0;)
		{
			ReservedMatchingItem item;
			if (SUCCEEDED(m_MatchingReserevedQueues[iQueue]->Dequeue(item)))
			{
				grabbedPlayerCount += item.MemberCount;
				svrChk(grabbedItems.push_back(item));
				iQueue = m_TargetMatchingMemberCount - grabbedPlayerCount;
			}
			else
			{
				iQueue--;
			}
		}


		m_WaitingBotMatchingStart = 0;

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

	HRESULT MatchingServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;

		hr = __super::TickUpdate(pAction);
		if (hr == S_FALSE)
			return hr;
		svrChk(hr);

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			goto Proc_End;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			goto Proc_End;


		UpdateMatching();

		// update counters
		for (UINT iQueue = 1; iQueue < m_TargetMatchingMemberCount; iQueue++)
		{
			m_ItemCounts[iQueue - 1] = m_MatchingReserevedQueues[iQueue]->GetEnqueCount();
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

	HRESULT MatchingServiceQueueEntity::InitializeEntity(EntityID newEntityID)
	{
		HRESULT hr = S_OK;

		memset(m_ReservedItemQueueTransaction, 0, sizeof m_ReservedItemQueueTransaction);

		svrChk(__super::InitializeEntity(newEntityID));

	Proc_End:

		return hr;
	}

	// TickUpdate 
	HRESULT MatchingServiceQueueEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;

		hr = __super::TickUpdate(pAction);
		if (hr == S_FALSE)
			return hr;

		if (GetPendingTransactionCount() == 0 && GetActiveTransactionCount() < (m_MatchingMemberCount-1))
		{
			for (UINT iMemberCount = 1; iMemberCount < m_MatchingMemberCount; iMemberCount++)
			{
				if (GetTransactionStartedForQueue(iMemberCount) <= 0)
				{
					Transaction *pTrans = new MatchingTransGrabPlayer(m_MatchingMemberCount, iMemberCount, m_MinQueueCount, m_MaxQueueCount);
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

	HRESULT MatchingServiceHandleMatchedEntity::InitializeEntity(EntityID newEntityID)
	{
		HRESULT hr = S_OK;

		svrChk(InitializeEntity(newEntityID));

	Proc_End:

		return hr;
	}




}; // namespace Svr {
}; // namespace BR {



