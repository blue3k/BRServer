////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
//#include <atldbcli.h>
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "Common/ResultCode/BRResultCodeDB.h"

#include "QueryWorker.h"
#include "DBTrace.h"
#include "QueryManager.h"
#include "QueryWorkerManager.h"
#include "Query.h"
#include "Session.h"
#include "Factory.h"


namespace BR {
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
	HRESULT QueryWorker::ExecuteQuery(Query* &pQuery)
	{
		HRESULT hr = S_SYSTEM_OK;
		QueryManager *pQueryManager = nullptr;
		Session* pSession = nullptr;

		dbChkPtr( pQuery );

		pSession = pQuery->GetSession();
		dbChkPtr(pSession);

		pQueryManager = pQuery->GetQueryManager();
		dbChkPtr(pQueryManager);

		hr = pSession->SendQuery(pQuery);
		if( hr == ((HRESULT)E_DB_CONNECTION_LOST) )
		{
			// Give one more chance, because the session will try to reconnect
			hr = pSession->SendQuery(pQuery);
		}
		dbChk(hr);

		// This will be run only if the query is succeeded
		// We need to handle exceptions so that transaction can handle errors
		dbChk(pQueryManager->RouteResult(pQuery));

	Proc_End:

		if( FAILED(hr) )
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
		//HRESULT	hr = S_SYSTEM_OK;
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

			if (FAILED(m_pWorkerManager->TryGetQuery(pQuery)))
				continue;

			if (pQuery != nullptr)
			{
				ExecuteQuery(pQuery);
			}
		}


		dbTrace(TRC_INFO, "Closing DB Worker");
	}


	

} // namespace DB
} // namespace BR
