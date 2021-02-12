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


#include "DB/Session.h"
#include "Container/SFStackPool.h"

#include <mysqlx/xdevapi.h>

namespace SF {
namespace DB {


	class QueryMYSQL;
	class StatementMYSQL;
	

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB session class
	//

	class SessionMYSQL : public Session
	{
	private:

		// data source reference
		DataSourceMYSQL *m_pMyDataSource = nullptr;

		// My sql context
		SFUniquePtr<mysqlx::Session> m_pXSession;

		// true if thread initialization is required
		CounterType m_Synced;
		SyncCounter m_InitSync;

	public:
		// constructor / destructor
		SessionMYSQL(IHeap& memMgr, DataSourceMYSQL *pDataSource);
		virtual ~SessionMYSQL();

		// return context value
		virtual void* GetContext() override;

		// Process a query
		virtual Result ProcessQuery( Query *pQuery ) override;

		// Open session
		virtual Result OpenSession() override;

		// Close session
		virtual Result CloseSession() override;

		virtual Result Ping() override;
	};


} // namespace DB
} // namespace SF
