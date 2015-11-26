
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : MadK
//
// Description : DB Query worker manager Class
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	
#include "Common/BrBaseTypes.h"
#include "Common/Thread.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"
#include "Common/PageQueue.h"
#include "Common/StaticHashTable.h"
#include "Common/SystemSynchronization.h"
#include "DBConst.h"
#include "DB/DataSource.h"

namespace BR {
namespace DB {

	class Query;
	class QueryWorker;


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryManager Class 
	//
	
	class QueryWorkerManager
	{
	public:

		// Working thread list
		typedef std::vector<QueryWorker*> WorkerList;

	private:

		static std::atomic<LONG>	stm_InitializationCount;

		BR::CriticalSection		m_QueryQueueLock;
		BR::Semaphore			m_QueryCounter;

		// DB Query Worker class
		WorkerList			m_QueryWorker;

		// Pending Query Queue
		PageQueue<Query*>	m_PendingQueries;

		static QueryWorkerManager* stm_pInstance;

	public:
		// constructor / destructor
		QueryWorkerManager();
		virtual ~QueryWorkerManager();

		// Initialize DB
		static HRESULT InitializeDBWorkerManager();

		// Terminate DB module and close connections
		static void TerminateDBWorkerManager();

		// Request a DB Qurey
		static HRESULT	PendingQuery(Query* &pQuery);

		// 
		HRESULT	TryGetQuery(Query* &pQuery);

	private:
	};

}  // namespace DB
}  // namespace BR
