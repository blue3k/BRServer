////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Query worker class
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Thread.h"
#include "Common/SystemSynchronize.h"

namespace BR {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Query Worker Class 
	//

	class Query;
	class QueryWorkerManager;
	class Session;
	class DataSource;

	class QueryWorker : public BR::Thread
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
		virtual bool Run(); 


		// Send Query
		HRESULT ExecuteQuery(Query* &pQuery);  
	};

} // namespace DB
} // namespace BR
