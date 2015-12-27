
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : MadK
//
// Description : DB Query worker manager Class
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/TimeUtil.h"
#include "Common/StrUtil.h"
#include "DB/DBTrace.h"
#include "DB/Query.h"
#include "DB/QueryWorker.h"
#include "DB/QueryWorkerManager.h"
#include "DB/Factory.h"
#include "DB/Session.h"
#include "DB/ShardCoordinatorDBQuery.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/EntityTable.h"

namespace BR {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryWorkerManager Class 
	//

	QueryWorkerManager* QueryWorkerManager::stm_pInstance = nullptr;
	std::atomic<LONG> QueryWorkerManager::stm_InitializationCount(0);
	
	QueryWorkerManager::QueryWorkerManager()
	{
		// create QueryWorkers
		for (int i = 0; i< Const::QUERYWORKER_MAX; i++)
		{
			QueryWorker* pQueryWorker = new QueryWorker(this);
			pQueryWorker->Start();
			m_QueryWorker.push_back(pQueryWorker);
		}
	}

	QueryWorkerManager::~QueryWorkerManager()
	{
		StopWorkers();
	}

	void QueryWorkerManager::StopWorkers()
	{
		for (auto itWorker = m_QueryWorker.begin(); itWorker != m_QueryWorker.end(); itWorker++)
		{
			QueryWorker* pWorker = *itWorker;
			pWorker->Stop(true);
			delete pWorker;
		}
		m_QueryWorker.clear();
	}

	// Initialize QueryWorkerManager
	HRESULT QueryWorkerManager::InitializeDBWorkerManager()
	{
		HRESULT hr = S_OK;

		auto initCount = stm_InitializationCount.fetch_add(1, std::memory_order_relaxed);
		if (initCount == 0 && stm_pInstance == nullptr)
		{
			stm_pInstance = new QueryWorkerManager;
		}


	//Proc_End:

		return hr;
	}

	void QueryWorkerManager::TerminateDBWorkerManager()
	{
		auto initCount = stm_InitializationCount.fetch_sub(1, std::memory_order_relaxed) - 1;
		if (initCount <= 0 && stm_pInstance != nullptr)
		{
			stm_pInstance->StopWorkers();
			delete stm_pInstance;
			stm_pInstance = nullptr;
		}
	}

	HRESULT	QueryWorkerManager::PendingQuery(Query* &pQuery)
	{
		HRESULT hr = S_OK;

		if (stm_pInstance == nullptr)
			dbErr(E_FAIL);

		dbChk(stm_pInstance->m_PendingQueries.Enqueue(pQuery));
		pQuery = nullptr;

		//stm_pInstance->m_QueryCounter.Release();

	Proc_End:

		return hr;
	}

	HRESULT	QueryWorkerManager::TryGetQuery(Query* &pQuery)
	{
		HRESULT hr = S_OK;

		MutexScopeLock lockScope(m_QueryQueueLock);

		//if (!m_QueryCounter.Acquire(Const::DB_WORKER_JOB_WAITING_MAX))
		//	return E_FAIL;

		if (FAILED(m_PendingQueries.Dequeue(pQuery)))
		{
			// if this faild we need to retry
			// Return back the query counter
			//m_QueryCounter.Release();
			return E_FAIL;
		}

	//Proc_End:

		return hr;
	}

} // namespace DB
} // namespace BR
