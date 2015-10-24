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


#include "StdAfx.h"
//#include <atldbcli.h>
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
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
		HRESULT hr = S_OK;
		QueryManager *pQueryManager = nullptr;
		Session* pSession = nullptr;

		dbChkPtr( pQuery );

		pSession = pQuery->GetSession();
		dbChkPtr(pSession);

		pQueryManager = pQuery->GetQueryManager();
		dbChkPtr(pQueryManager);

		hr = pSession->SendQuery(pQuery);
		if( hr == E_DB_CONNECTION_LOST )
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

	
	bool QueryWorker::Run()
	{
		HRESULT	hr = S_OK;

		while(1)
		{
			Query *pQuery = nullptr;

			if (CheckKillEvent(0))
				break;

			if (FAILED(m_pWorkerManager->TryGetQuery(pQuery)))
				continue;

			if (pQuery != nullptr)
			{
				ExecuteQuery(pQuery);
			}
		}


		dbTrace(TRC_INFO, "Closing DB Worker");

		return true;
	}


	

} // namespace DB
} // namespace BR
