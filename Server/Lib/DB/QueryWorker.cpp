////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "DBPch.h"
//#include <atldbcli.h>
#include "SFTypedefs.h"
#include "String/SFStrUtil.h"
#include "ResultCode/SFResultCodeDB.h"

#include "QueryWorker.h"
#include "DBTrace.h"
#include "DBClusterManager.h"
#include "QueryWorkerManager.h"
#include "Query.h"
#include "Session.h"
#include "Factory.h"


namespace SF {
namespace DB {

	// Constructor
	QueryWorker::QueryWorker(QueryWorkerManager *pWorkerManager)
		: m_pWorkerManager(pWorkerManager)
	{
	}

	QueryWorker::~QueryWorker()
	{
	}


	// Send query
	Result QueryWorker::ExecuteQuery(Query* &pQuery)
	{
		Result hr = ResultCode::SUCCESS;
		DBClusterManager *pQueryManager = nullptr;
		Session* pSession = nullptr;

		dbChkPtr( pQuery );

		pSession = pQuery->GetSession();
		dbChkPtr(pSession);

		pQueryManager = pQuery->GetQueryManager();
		dbChkPtr(pQueryManager);

		hr = pSession->SendQuery(pQuery);
		if( hr == ResultCode::DB_CONNECTION_LOST )
		{
			// Give one more chance, because the session will try to reconnect
			hr = pSession->SendQuery(pQuery);
		}
		dbChk(hr);

		// This will be run only if the query is succeeded
		// We need to handle exceptions so that transaction can handle errors
		hr = pQueryManager->RouteResult(pQuery);
		if (!hr && hr != ResultCode::INVALID_ENTITY) // Ignore entity not found error
		{
			dbErr(hr);
		}

	Proc_End:

		if( !(hr) )
		{
			if (pSession != nullptr)
				Factory::ErrorLog(pSession->GetContext(), hr, pQuery ? typeid(*pQuery).name() : "");

			// when some DB error happenend, try to route result directly
			if (pQuery && pQueryManager)
			{
				pQueryManager->RouteResult(pQuery);
			}
		}

		// return back to Data source
		if (pSession != nullptr)
			pSession->ReleaseSession();
		pSession = nullptr;
		if (pQuery)
			pQuery->SetSession(nullptr);

		Util::SafeDelete(pQuery);

		return hr;
	}

	
	void QueryWorker::Run()
	{
		//Result	hr = ResultCode::SUCCESS;
		DurationMS expectedTickInterval(5);

		while(1)
		{
			Query *pQuery = nullptr;

			// 50ms will be the precision of our timer
			auto loopInterval = UpdateInterval(expectedTickInterval);

			if (CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				break;
			}

			if (!(m_pWorkerManager->TryGetQuery(pQuery)))
				continue;

			if (pQuery != nullptr)
			{
				ExecuteQuery(pQuery);
			}
		}


		dbTrace(TRC_DBINFO, "Closing DB Worker");
	}


	

} // namespace DB
} // namespace SF
