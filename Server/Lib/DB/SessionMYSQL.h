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


#include "DB/Session.h"
#include "Common/ClassUtil.h"

#include <my_global.h>
#include <mysql.h>
#include <mysqld_error.h>

#include "Common/StackPool.h"



namespace BR {
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
			PoolItem(){}
			virtual ~PoolItem(){}
		};

	private:

		// statement pool map
		StatementPoolMap	m_StatementPoolMap;

	public:
		// statement pool MYSQL
		StatementPoolMYSQL();
		~StatementPoolMYSQL();

		// Cleanup the pool
		HRESULT ClearStatementPools();

		// Get free statement
		StatementMYSQL* GetFreeStatement( const char* queryString );

		// free statement
		HRESULT FreeStatement( StatementMYSQL* &pStatement );
		
	};

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB statement class
	//

	class StatementMYSQL : public StatementPoolMYSQL::PoolItem
	{
	private:

		// This will be going to link querystring of query class
		BRCLASS_ATTRIBUTE_READONLY(const char*,QueryString);

		BRCLASS_ATTRIBUTE_READONLY(MYSQL*,Context);
		BRCLASS_ATTRIBUTE_READONLY(MYSQL_STMT*,Stmt);

		BRCLASS_ATTRIBUTE_READONLY(CounterType,StateInitSync);

		// binding temporary values
		MYSQL_BIND *m_pParameter;


	public:
		StatementMYSQL( const char* queryString );

		// prepared
		bool IsPrepared();

		// Prepare statement
		HRESULT PrepareState( MYSQL *pContext, CounterType syncInit );

		// Bind query instance
		HRESULT Bind( QueryMYSQL *pMyQuery );

		// Execute
		HRESULT Execute();

		// Patch result
		HRESULT PatchResults( QueryMYSQL *pMyQuery );

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
		SessionMYSQL(DataSourceMYSQL *pDataSource);
		virtual ~SessionMYSQL();

		// return context value
		virtual void* GetContext();

		// Send a query
		virtual HRESULT SendQuery( Query *pQuery );

		// Open session
		virtual HRESULT OpenSession();

		// Close session
		virtual HRESULT CloseSession();
	};


} // namespace DB
} // namespace BR