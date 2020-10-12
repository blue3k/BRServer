
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB Management Class
//
////////////////////////////////////////////////////////////////////////////////

#include "DBPch.h"
#include "SFTypedefs.h"
#include "Util/SFTimeUtil.h"
#include "String/SFStrUtil.h"
#include "DB/DBTrace.h"
#include "DB/Query.h"
#include "DB/QueryWorker.h"
#include "DB/DBClusterManager.h"
#include "DB/Factory.h"
#include "DB/Session.h"
#include "DB/ShardCoordinatorDBQuery.h"
#include "DB/QueryWorkerManager.h"
#include "Server/BrServer.h"
#include "Entity/EntityTable.h"
#include "Server/BrServerUtil.h"
#include "Service/ServerService.h"


namespace SF {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DBClusterManager Class 
	//
	
	DBClusterManager::DBClusterManager()
		: m_Heap("DBClusterManager", GetSystemHeap())
		, m_ShardingBucket(m_Heap)
		, m_PendingQueries(m_Heap)
		, m_ResultQueries(m_Heap)
	{
	}

	DBClusterManager::~DBClusterManager()
	{
		Clear();
	}

	// Initialize DBClusterManager
	Result DBClusterManager::InitializeDBCluster( uint partitioningCount )
	{
		Result hr = ResultCode::SUCCESS;

		m_PartitioningCount = partitioningCount;

		m_bIsDisconnected = false;


		// Clear if something is remain
		if (m_ShardingBucket.size() > 0)
		{
			m_ShardingBucket.for_each([](DataSource* dataSource) -> Result {
				if (dataSource != nullptr)
				{
					dataSource->CloseDBSource();
					IHeap::Delete(dataSource);
				}
				return ResultCode::SUCCESS;
			});
			m_ShardingBucket.Clear();
		}

		// Create DB sources
		m_ShardingBucket.reserve(m_PartitioningCount);
		for( uint part =0; part < m_PartitioningCount; part++)
		{
			DataSource *pDBSource = nullptr;
			dbChk( Factory::GetInstance().CreateDataSource(GetHeap(), pDBSource) );
			dbChk( m_ShardingBucket.push_back( pDBSource ) );
		}

		dbChk(QueryWorkerManager::InitializeDBWorkerManager());

		// put initial DB shard information query
		dbChk(RequestShardList());

	Proc_End:

		return hr;
	}

	void DBClusterManager::Clear()
	{
		while(m_PendingQueries.GetEnqueCount() > 0)
		{
			DB::Query* pQuery = nullptr;
			m_PendingQueries.Dequeue(pQuery);
			if (pQuery != nullptr)
				delete pQuery;
		}

		while (m_ResultQueries.GetEnqueCount() > 0)
		{
			DB::Query* pQuery = nullptr;
			m_ResultQueries.Dequeue(pQuery);
			if (pQuery != nullptr)
				delete pQuery;
		}

		m_ShardingBucket.for_each([](DataSource* dataSource) -> Result {
			if (dataSource != nullptr)
			{
				dataSource->CloseDBSource();
				delete dataSource;
			}
			return ResultCode::SUCCESS;
		});
		m_ShardingBucket.Clear();

	}

	void DBClusterManager::TerminateDB()
	{
		QueryWorkerManager::TerminateDBWorkerManager();
		Clear();
	}

	Result DBClusterManager::RequestShardList()
	{
		Result hr = ResultCode::SUCCESS;

		if (m_GetShardListLockTimer.IsTimerWorking()
			&& !m_GetShardListLockTimer.CheckTimer())
		{
			return ResultCode::SUCCESS_FALSE;
		}

		UniquePtr<QueryGetShardListCmd> pQuery;
		pQuery.reset(new(GetHeap()) QueryGetShardListCmd(GetHeap()));

		pQuery->SetPartitioningKey(0);
		pQuery->SetTransaction(TransactionID());
		pQuery->ShardID = 0;

		dbChk(RequestQuery(pQuery));

		m_GetShardListLockTimer.SetTimer(DurationMS(Const::DB_MANAGER_UPDATE_SHARDLIST_LIMIT));

	Proc_End:

		return hr;
	}

	Result	DBClusterManager::AddDBSource( uint partitioningID, const String& strInstanceName, const String& strConnectionString, const String& strDBName, const String& strUserID, const String& strPassword )
	{
		Result	hr = ResultCode::SUCCESS;
		DataSource *pDBSource = nullptr;
		String userID = strUserID;
		String password = strPassword;

		if (m_UserID.IsNullOrEmpty()) m_UserID = userID;
		else userID = m_UserID;

		if (m_Password.IsNullOrEmpty()) m_Password = password;
		else password = m_Password;

		if (userID.IsNullOrEmpty() || password.IsNullOrEmpty())
			dbErr(ResultCode::DB_INVALID_CONFIG);

		while (m_ShardingBucket.size() <= partitioningID)
		{
			m_ShardingBucket.push_back(nullptr);
		}

		pDBSource = m_ShardingBucket[partitioningID];
		dbAssert(pDBSource == nullptr || !pDBSource->GetOpened());

		dbTrace(Info, "Adding DBSource {0}, {1}, {2}", strInstanceName, strConnectionString, strDBName);

		if (pDBSource == nullptr)
		{
			dbChk(Factory::GetInstance().CreateDataSource(GetHeap(), pDBSource));
			m_ShardingBucket[partitioningID] = pDBSource;
		}


		dbChk(pDBSource->InitializeDBSource(strConnectionString, strDBName, userID, password));


	Proc_End:

		return hr;
	}

	void DBClusterManager::Update()
	{
		UpdateQuery();

		UpdateResultQueries();
	}


	// select db source by partitioning key
	Result DBClusterManager::SelectDBByKey(uint partitioningKey, DataSource* &pDataSource)
	{
		Result hr = ResultCode::SUCCESS;

		pDataSource = nullptr;
		
		// modulate by partitioning size
		uint partition = (uint)(partitioningKey % m_ShardingBucket.size());

		dbChkPtr( pDataSource = m_ShardingBucket[partition] );

Proc_End:


		return hr;
	}


	// Update query worker status
	Result DBClusterManager::UpdateQuery()
	{
		Result hr = ResultCode::SUCCESS;
		Query* pQuery = nullptr;
		Session * pSession = nullptr;

		// Don't try any query while disconnected
		if( m_bIsDisconnected )
			return ResultCode::SUCCESS;

		auto maxTry = m_PendingQueries.GetEnqueCount();
		for (decltype(maxTry) iQuery = 0; iQuery < maxTry; iQuery++)
		{
			DataSource *pDBSource = nullptr;

			// dequeue one item
			if (!(m_PendingQueries.Dequeue(pQuery)))
				continue;

			if (Util::TimeSince(pQuery->GetRequestedTime()) > DurationMS(DB::MAX_QUERY_TIMEOUT))
			{
				pQuery->SetResult(ResultCode::SVR_TIMEOUT);
				RouteResult(pQuery);
				continue;
			}

			// Find the data source
			if (!(SelectDBByKey(pQuery->GetPartitioningKey(), pDBSource)))
			{
				dbTrace(Error, "QueryWorker failure: The sharding bucket is empty");
				// It's not expected
				pQuery->SetResult(ResultCode::UNEXPECTED);
				RouteResult(pQuery);
				continue;
			}

			if (!pDBSource->GetOpened())
			{
				RequestShardList();
				// Put query in the back
				dbChk(m_PendingQueries.Enqueue(pQuery));
				continue;
			}

			// Assign DB session
			if (pQuery->GetSession() == nullptr)
			{
				// Get Session
				if (!(pDBSource->AssignSession(pSession)))
				{
					// It's not expected
					pQuery->SetResult(ResultCode::UNEXPECTED);
					dbTrace(Error, "Assigning query to a worker failed {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
					RouteResult(pQuery);
					continue;
				}

				if (!pSession->IsOpened())
				{
					if (!(pSession->OpenSession()))
					{
						pQuery->SetResult(ResultCode::UNEXPECTED);
						pSession->ReleaseSession();
						dbTrace(Error, "Failed to open DB session {0}, TransID:{1}", typeid(*pQuery).name(), pQuery->GetTransID());
						RouteResult(pQuery);
						continue;
					}
				}

				pQuery->SetSession(pSession);
				pSession = nullptr;
			}

			if (!m_PendingQueries.Enqueue(pQuery))
			{
				// It's not expected
				pQuery->SetResult(ResultCode::UNEXPECTED);
				dbTrace(Error, "Assigning query to a worker failed {0}, TransID:{1}", typeid(*pQuery).name(), pQuery->GetTransID());
			}
			else
			{
				pQuery = nullptr;
			}

			if (pQuery != nullptr && !(pQuery->GetResult()))
			{
				RouteResult(pQuery);
			}
		}

	Proc_End:

		if (pSession != nullptr)
			pSession->ReleaseSession();

		return hr;
	}

	Result	DBClusterManager::UpdateResultQueries()
	{
		FunctionContext hr;

		Query* pQuery = nullptr;
		auto numQueries = m_ResultQueries.GetEnqueCount();
		for (decltype(numQueries) iQuery = 0; iQuery < numQueries; iQuery++)
		{
			if (!(m_ResultQueries.Dequeue(pQuery)))
				break;

			if (pQuery->GetMsgID().GetMsgID() == QueryGetShardListCmd::MID.GetMsgID())
			{
				DataSource *pDBSource = nullptr;

				// Update shard ids
				auto pDBRes = (QueryGetShardListCmd*)pQuery;
				for (auto& rowRes : pDBRes->RowsetResults)
				{
					auto shardId = rowRes.GetValue<int>("ShardID"_crc32c);
					auto DBName = rowRes.GetValue<String>("DBName"_crc32c);
					auto ConnectionString = rowRes.GetValue<String>("ConnectionString"_crc32c);

					if ((unsigned)shardId < GetPartitioningCount() && (SelectDBByKey(shardId, pDBSource)))
					{
						if (pDBSource->GetDefaultDB() != DBName
							|| pDBSource->GetConnectionString() != ConnectionString)
						{
							dbTrace(Info, "Initializating DBSource {0}, Shard:{1} {2}, {3}", typeid(*this).name(), shardId,  ConnectionString, DBName);
							dbCheck(pDBSource->InitializeDBSource(ConnectionString, DBName, m_UserID, m_Password));
						}
					}
					else
					{
						AddDBSource(shardId, typeid(*this).name(), ConnectionString, DBName);
					}
					
				}
			}
			else
			{
				// not handled query result
				dbError(ResultCode::NOT_IMPLEMENTED);
			}
		}

		return hr;
	}

	Result	DBClusterManager::RequestQuery(UniquePtr<Query>& pQuery)
	{
		Session* pSession = nullptr;
		DataSource *pDBSource = nullptr;
		FunctionContext hr([&pSession](Result hr)
			{
				if (pSession != nullptr)
					pSession->ReleaseSession();
			});

		pQuery->UpdateRequestedTime();
		pQuery->SetQueryManager(this);

		// Find the data source
		if (!(SelectDBByKey(pQuery->GetPartitioningKey(), pDBSource)))
		{
			dbTrace(Error, "QueryWorker failure: The sharding bucket is empty");
			// It's not expected
			pQuery->SetResult(ResultCode::UNEXPECTED);
			dbError(ResultCode::UNEXPECTED);
		}

		if (!pDBSource->GetOpened())
		{
			// DB source fail, put it in the local queue and try again when DB is ready
			// This sometimes hides DB error.
			dbCheck(m_PendingQueries.Enqueue(pQuery.get()));
			pQuery.release();
			return ResultCode::SUCCESS;
		}

		// Get Session
		if (!(pDBSource->AssignSession(pSession)))
		{
			// It's not expected
			pQuery->SetResult(ResultCode::UNEXPECTED);
			dbTrace(Error, "Assigning query to a worker failed {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
			dbError(ResultCode::UNEXPECTED);
		}

		// Open session if it isn't
		if (!pSession->IsOpened())
		{
			if (!(pSession->OpenSession()))
			{
				pQuery->SetResult(ResultCode::UNEXPECTED);
				pSession->ReleaseSession();
				dbTrace(Error, "Failed to open DB session {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
				dbError(ResultCode::UNEXPECTED);
			}
		}

		pQuery->SetSession(pSession);
		pSession = nullptr;

		dbTrace(TRC_QUERY, "Query pending transID:{0} msg:{1}", pQuery->GetTransID(), pQuery->GetMsgID());

		dbCheck(QueryWorkerManager::PendingQuery(pQuery));

		return hr;
	}



	// Route query result to entity
	Result	DBClusterManager::RouteResult(Query* &pQuery)
	{
		FunctionContext hr;

		auto queryName = pQuery->GetQueryString();

		dbTrace(TRC_QUERY, "Query route result transID:{0} msg:{1}, class:{2}", pQuery->GetTransID(), pQuery->GetMsgID(), queryName);

		if(pQuery->GetTransID() != TransactionID() )
		{
			UniquePtr<Svr::TransactionResult> pRes(pQuery);
			pQuery = nullptr;

			Svr::BrServer *pMyServer = Svr::BrServer::GetInstance();
			dbCheckPtr( pMyServer );

			auto msgID = pRes->GetMsgID();
			auto entityID = pRes->GetTransID().GetEntityID();
			hr = Service::EntityTable->RouteTransactionResult(pRes);
			if (!hr)
			{
				dbTrace(TRC_ROUTING, "Failed to route a message hr:{0} msgID:{1}, target entityID:{2}, query:{3}", hr, msgID, entityID, queryName);
				hr = ResultCode::INVALID_ENTITY;
				return hr;
			}
		}
		else if (pQuery->GetMsgID().GetMsgID() == QueryGetShardListCmd::MID.GetMsgID())
		{
			dbCheck(m_ResultQueries.Enqueue(pQuery));
			pQuery = nullptr;
		}
		else
		{
			assert(pQuery->GetTransID() == TransactionID()); // otherwise it's a lost transaction result
			delete pQuery;
			pQuery = nullptr;
		}

		return hr;
	}


	Result DBClusterManager::RequestQuerySync(Query* pQuery)
	{
		Result hr = ResultCode::SUCCESS;
		Session * pSession = nullptr;
		DataSource *pDBSource = nullptr;

		pQuery->UpdateRequestedTime();
		pQuery->SetQueryManager(this);

		// Find the data source
		if (!(SelectDBByKey(pQuery->GetPartitioningKey(), pDBSource)))
		{
			dbTrace(Error, "QueryWorker failure: The sharding bucket is empty");
			// It's not expected
			pQuery->SetResult(ResultCode::UNEXPECTED);
			dbErr(ResultCode::UNEXPECTED);
		}

		if (!pDBSource->GetOpened())
		{
			dbErr(ResultCode::NOT_INITIALIZED);
		}

		// Get Session
		if (!(pDBSource->AssignSession(pSession)))
		{
			// It's not expected
			pQuery->SetResult(ResultCode::UNEXPECTED);
			dbTrace(Error, "Assigning query to a worker failed {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
			dbErr(ResultCode::UNEXPECTED);
		}

		if (!pSession->IsOpened())
		{
			if (!(pSession->OpenSession()))
			{
				pQuery->SetResult(ResultCode::UNEXPECTED);
				pSession->ReleaseSession();
				dbTrace(Error, "Failed to open DB session {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
				dbErr(ResultCode::UNEXPECTED);
			}
		}

		pQuery->SetSession(pSession);

		defChk(pSession->ProcessQuery(pQuery));


	Proc_End:

		pQuery->SetSession(nullptr);

		if (pSession)
			pSession->ReleaseSession();

		return hr;
	}


} // namespace DB
} // namespace SF
