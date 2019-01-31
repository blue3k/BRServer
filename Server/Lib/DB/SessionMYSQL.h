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

#include <my_global.h>
#include <mysql.h>
#include <mysqld_error.h>

#include "Container/SFStackPool.h"



namespace SF {
namespace DB {


	class QueryMYSQL;
	class StatementMYSQL;
	


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB statement pool class
	//

	class StatementPoolMYSQL
	{
	public:
		typedef std::unordered_map<intptr_t,StackPool*> StatementPoolMap;

		// free stack item
		class PoolItem : public StackPool::Item
		{
		public:
			PoolItem()
			{
				memset(this, 0, sizeof(PoolItem));
			}
			virtual ~PoolItem(){}
		};

	private:

		IHeap& m_Heap;

		// statement pool map
		StatementPoolMap	m_StatementPoolMap;

	public:
		// statement pool MYSQL
		StatementPoolMYSQL(IHeap& memoryManager);
		~StatementPoolMYSQL();

		IHeap& GetHeap() { return m_Heap; }

		// Cleanup the pool
		Result ClearStatementPools();

		// Get free statement
		StatementMYSQL* GetFreeStatement( const char* queryString );

		// free statement
		Result FreeStatement( StatementMYSQL* &pStatement );
		
	};

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB statement class
	//

	class StatementMYSQL : public StatementPoolMYSQL::PoolItem
	{
	private:

		// This will be going to link query string of query class
		const char* m_QueryString = nullptr;

		MYSQL* m_Context = nullptr;
		MYSQL_STMT* m_Stmt = nullptr;

		CounterType m_StateInitSync;

		// binding temporary values
		MYSQL_BIND *m_pParameter = nullptr;


	public:
		StatementMYSQL( const char* queryString );

		const char* GetQueryString() { return m_QueryString; }
		MYSQL* GetContext() { return m_Context; }
		MYSQL_STMT* GetStmt() { return m_Stmt; }
		CounterType GetStateInitSync() { return m_StateInitSync; }

		// prepared
		bool IsPrepared();

		// Prepare statement
		Result PrepareState( MYSQL *pContext, CounterType syncInit );

		// Bind query instance
		Result Bind( QueryMYSQL *pMyQuery );

		// Execute
		Result Execute();

		// Patch result
		Result PatcResults( QueryMYSQL *pMyQuery );

		// Clear prepared statement
		void Clear();
	};
	
	


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB session class
	//

	class SessionMYSQL : public Session
	{
	private:

		// data source reference
		DataSourceMYSQL *m_pMyDataSource;

		// My sql context
		MYSQL* m_mySQL;

		// true if thread initialization is required
		CounterType m_Synced;
		SyncCounter m_InitSync;

		// statememnt pool
		StatementPoolMYSQL m_StatementPool;

	public:
		// constructor / destructor
		SessionMYSQL(IHeap& memMgr, DataSourceMYSQL *pDataSource);
		virtual ~SessionMYSQL();

		// return context value
		virtual void* GetContext();

		// Send a query
		virtual Result SendQuery( Query *pQuery );

		// Open session
		virtual Result OpenSession();

		// Close session
		virtual Result CloseSession();

		virtual Result Ping() override;
	};


} // namespace DB
} // namespace SF
