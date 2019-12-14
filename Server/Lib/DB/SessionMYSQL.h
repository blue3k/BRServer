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
		mysqlx::Session* m_pXSession = nullptr;

		// true if thread initialization is required
		CounterType m_Synced;
		SyncCounter m_InitSync;

	public:
		// constructor / destructor
		SessionMYSQL(IHeap& memMgr, DataSourceMYSQL *pDataSource);
		virtual ~SessionMYSQL();

		// return context value
		virtual void* GetContext();

		// Process a query
		virtual Result ProcessQuery( Query *pQuery );

		// Open session
		virtual Result OpenSession();

		// Close session
		virtual Result CloseSession();

		virtual Result Ping() override;
	};


} // namespace DB
} // namespace SF
