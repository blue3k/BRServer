
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB Query worker manager Class
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	
#include "Types/SFEngineTypedefs.h"
#include "Multithread/SFThread.h"
#include "Util/SFTimeUtil.h"
#include "Container/SFPageQueue.h"
#include "Multithread/SFSystemSynchronization.h"
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

		Heap m_Heap;

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

		IHeap& GetHeap() { return *m_Heap.get(); }

		// Initialize DB
		static Result InitializeDBWorkerManager();

		// Terminate DB module and close connections
		static void TerminateDBWorkerManager();

		// Request a DB Query
		static Result	PendingQuery(SFUniquePtr<Query>& pQuery);

		// 
		Result	TryGetQuery(Query* &pQuery);

	private:
	};

}  // namespace DB
}  // namespace SF
