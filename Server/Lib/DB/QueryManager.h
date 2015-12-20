
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB Management Class
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	
#include "Common/BrBaseTypes.h"
#include "Common/Thread.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"
#include "Common/PageQueue.h"
#include "Common/StaticHashTable.h"
#include "DBConst.h"
#include "DB/DataSource.h"

namespace BR {
namespace DB {

	class Query;
	class QueryWorker;


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryManager Class 
	//
	
	class QueryManager
	{
	public:

		// Shard partitioning count
		BRCLASS_ATTRIBUTE_READONLY(UINT,PartitioningCount);


	private:

		// DB Connection information
		StaticArray<DataSource*,sizeof(DataSource*)*DB::Const::DEFAULT_SHARDING_BUCKETS>		m_ShardingBucket;

		// Pending Query Queue
		PageQueue<Query*>	m_PendingQueries;

		PageQueue<Query*>	m_ResultQueries;

		Util::TimeStampTimer m_GetShardListLockTimer;

		// flag for disconnected check
		bool				m_bIsDisconnected;

	public:
		// constructor / destructor
		QueryManager();
		virtual ~QueryManager();

		// Initialize DB
		HRESULT InitializeDB( UINT partitioningCount );

		// Terminate DB module and close connections
		void TerminateDB();

		// Add DB source
		HRESULT	AddDBSource( UINT partitioningID, const std::string& strInstanceName, const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword ); 

	protected:

		void Clear();

	public:

		// Request a DB Qurey
		HRESULT	RequestQuery(Query* pQuery);

		// Route query result to entity
		virtual HRESULT RouteResult(Query* &pQuery);

		void Update();

	private:

		// select db source by partitioning key
		HRESULT SelectDBByKey(UINT partitioningKey, DataSource* &pDataSource);
		HRESULT RequestShardList();

		// Update query worker status
		HRESULT UpdateQuery();
		HRESULT UpdateResultQueries();

		////////////////////////////////////////////////////////////////
		// Override thread run
		//virtual bool Run();	

	};

}  // namespace DB
}  // namespace BR
