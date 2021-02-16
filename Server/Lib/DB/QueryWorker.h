////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Query worker class
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Multithread/SFThread.h"
#include "Multithread/SFSystemSynchronization.h"

namespace SF {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Query Worker Class 
	//

	class Query;
	class QueryWorkerManager;
	class Session;
	class DataSource;

	class QueryWorker : public Thread
	{
	public:

	private:
		// Query now working on
		//Query*			m_pQuery;

		// DB session
		//Session			*m_pSession;

		QueryWorkerManager *m_pWorkerManager;

	public:
		// constructor / destructor
		QueryWorker(QueryWorkerManager *pWorkerManager);
		virtual ~QueryWorker();

		
		// get assigned query
		//Query* GetWorkingQuery()				{ return m_pQuery; }

		// clean up query
		//void CleanUpQuery()						{ m_pQuery = nullptr; }

		// Check weather it's working or not
		//bool IsWorking()						{ return m_pQuery != nullptr; }

	private:
		// Override run in Thread
		virtual void Run() override;


		// Send Query
		Result ExecuteQuery(Query* &pQuery);  
	};

} // namespace DB
} // namespace SF
