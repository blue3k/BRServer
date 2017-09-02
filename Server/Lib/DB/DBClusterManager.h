
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
	
#include "Types/BrBaseTypes.h"
#include "Thread/Thread.h"
#include "Util/TimeUtil.h"
#include "Container/PageQueue.h"
#include "DBConst.h"
#include "DB/DataSource.h"

namespace SF {
namespace DB {

	class Query;
	class QueryWorker;


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DBClusterManager Class 
	//
	
	class DBClusterManager
	{
	public:

		// Shard partitioning count
		uint m_PartitioningCount = 0;


	private:

		// DB Connection information
		StaticArray<DataSource*,sizeof(DataSource*)*DB::Const::DEFAULT_SHARDING_BUCKETS>		m_ShardingBucket;

		// same cluster will share same IP/Password
		std::string m_UserID;
		std::string m_Password;

		// Pending Query Queue
		PageQueue<Query*>	m_PendingQueries;

		PageQueue<Query*>	m_ResultQueries;

		Util::TimeStampTimer m_GetShardListLockTimer;

		// flag for disconnected check
		bool				m_bIsDisconnected;

	public:
		// constructor / destructor
		DBClusterManager();
		virtual ~DBClusterManager();

		uint GetPartitioningCount() const { return m_PartitioningCount; }

		// Initialize DB
		Result InitializeDBCluster( uint partitioningCount );

		// Terminate DB module and close connections
		void TerminateDB();

		// Add DB source
		Result	AddDBSource( uint partitioningID, const std::string& strInstanceName, const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID = "", const std::string& strPassword = "" ); 

	protected:

		void Clear();

	public:

		// Request a DB Query
		Result	RequestQuery(Query* pQuery);

		// Route query result to entity
		virtual Result RouteResult(Query* &pQuery);

		// Request DB query synchronously
		Result RequestQuerySync(Query* pQuery);


		void Update();




	private:

		// select db source by partitioning key
		Result SelectDBByKey(uint partitioningKey, DataSource* &pDataSource);
		Result RequestShardList();

		// Update query worker status
		Result UpdateQuery();
		Result UpdateResultQueries();

		////////////////////////////////////////////////////////////////
		// Override thread run
		//virtual bool Run();	

	};

}  // namespace DB
}  // namespace SF
