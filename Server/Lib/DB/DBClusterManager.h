
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB Management Class
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	
#include "Types/SFEngineTypedefs.h"
#include "Multithread/SFThread.h"
#include "Util/SFTimeUtil.h"
#include "Container/SFPageQueue.h"
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
	
	class DBClusterManager : public SharedObject
	{
	public:

		Heap m_Heap;

		// Shard partitioning count
		uint m_PartitioningCount = 0;


	private:

		// DB Connection information
		StaticArray<DataSource*,sizeof(DataSource*)*DB::Const::DEFAULT_SHARDING_BUCKETS>		m_ShardingBucket;

		// same cluster will share same IP/Password
		String m_UserID = "";
		String m_Password = "";

		// Pending Query Queue
		PageQueue<Query*>	m_PendingQueries;

		PageQueue<Query*>	m_ResultQueries;

		Util::TimeStampTimer m_GetShardListLockTimer;

		// flag for disconnected check
		bool				m_bIsDisconnected = true;

	public:
		// constructor / destructor
		DBClusterManager();
		virtual ~DBClusterManager();

		IHeap& GetHeap() { return *m_Heap.get(); }

		uint GetPartitioningCount() const { return m_PartitioningCount; }

		// Initialize DB
		Result InitializeDBCluster( uint partitioningCount );

		// Terminate DB module and close connections
		void TerminateDB();

		// Add DB source
		Result	AddDBSource( uint partitioningID, const String& strInstanceName, const String& strConnectionString, const String& strDBName, const String& strUserID = "", const String& strPassword = "" ); 

	protected:

		void Clear();

	public:

		template<typename QueryType,
			typename = std::enable_if_t<std::is_base_of_v<Query, QueryType>>
		>
		Result RequestQuery(SFUniquePtr<QueryType>& pQuery)
		{
			SFUniquePtr<Query> temp(pQuery.get());
			auto hr = RequestQuery(temp);
			if (hr)
				pQuery.release();
			temp.release();
			return hr;
		}

		// Request a DB Query
		Result	RequestQuery(SFUniquePtr<Query>& pQuery);

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
