
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
	
#include "Types/SFEngineTypedefs.h"
#include "Thread/Thread.h"
#include "Util/TimeUtil.h"
#include "Container/PageQueue.h"
#include "Thread/SystemSynchronization.h"
#include "DBConst.h"
#include "DB/DataSource.h"

namespace SF {
namespace DB {

	class Query;
	class QueryWorker;


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DBClusterManager Class 
	//
	
	class QueryWorkerManager
	{
	public:

		// Working thread list
		typedef std::vector<QueryWorker*> WorkerList;

	private:

		MemoryManager m_MemoryManager;

		static std::atomic<long>	stm_InitializationCount;

		CriticalSection		m_QueryQueueLock;

		// DB Query Worker class
		WorkerList			m_QueryWorker;

		// Pending Query Queue
		PageQueue<Query*>	m_PendingQueries;

		static QueryWorkerManager* stm_pInstance;

	private:

		void StopWorkers();

	public:
		// constructor / destructor
		QueryWorkerManager();
		virtual ~QueryWorkerManager();

		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }

		// Initialize DB
		static Result InitializeDBWorkerManager();

		// Terminate DB module and close connections
		static void TerminateDBWorkerManager();

		// Request a DB Qurey
		static Result	PendingQuery(Query* &pQuery);

		// 
		Result	TryGetQuery(Query* &pQuery);

	private:
	};

}  // namespace DB
}  // namespace SF
