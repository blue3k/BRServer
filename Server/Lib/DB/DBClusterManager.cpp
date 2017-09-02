
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB Management Class
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SFTypedefs.h"
#include "Util/TimeUtil.h"
#include "String/StrUtil.h"
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

namespace SF {
namespace DB {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DBClusterManager Class 
	//
	
	DBClusterManager::DBClusterManager()
		: m_MemoryManager("DBClusterManager", GetSystemMemoryManager())
		, m_ShardingBucket(m_MemoryManager)
		, m_PendingQueries(m_MemoryManager)
		, m_ResultQueries(m_MemoryManager)
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
			m_ShardingBucket.Foreach([](DataSource* dataSource) -> Result {
				if (dataSource != nullptr)
				{
					dataSource->CloseDBSource();
					delete dataSource;
				}
				return ResultCode::SUCCESS;
			});
			m_ShardingBucket.Clear();
		}

		// Create DB sources
		m_ShardingBucket.Reserve(m_PartitioningCount);
		for( uint part =0; part < m_PartitioningCount; part++)
		{
			DataSource *pDBSource = nullptr;
			dbChk( Factory::GetInstance().CreateDataSource(pDBSource) );
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
		m_ShardingBucket.Foreach([](DataSource* dataSource) -> Result {
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

		auto pQuery = new QueryGetShardListCmd();

		pQuery->SetPartitioningKey(0);
		pQuery->SetTransaction(TransactionID());
		pQuery->ShardID = 0;

		dbChk(RequestQuery(pQuery));

		m_GetShardListLockTimer.SetTimer(DurationMS(Const::DB_MANAGER_UPDATE_SHARDLIST_LIMIT));

	Proc_End:

		return hr;
	}

	Result	DBClusterManager::AddDBSource( uint partitioningID, const std::string& strInstanceName, const std::string& strConnectionString, const std::string& strDBName, const std::string& strUserID, const std::string& strPassword )
	{
		Result	hr = ResultCode::SUCCESS;
		DataSource *pDBSource = nullptr;
		std::string userID = strUserID;
		std::string password = strPassword;

		if (m_UserID.length() == 0) m_UserID = userID;
		else userID = m_UserID;

		if (m_Password.length() == 0) m_Password = password;
		else password = m_Password;

		if (userID.length() == 0 || password.length() == 0)
			dbErr(ResultCode::DB_INVALID_CONFIG);

		while (m_ShardingBucket.size() <= partitioningID)
		{
			m_ShardingBucket.push_back(nullptr);
		}

		pDBSource = m_ShardingBucket[partitioningID];
		dbAssert(pDBSource == nullptr || !pDBSource->GetOpened());

		dbTrace(Trace::TRC_INFO, "Adding DBSource {0}, {1}, {2}", strInstanceName, strConnectionString, strDBName);

		if (pDBSource == nullptr)
		{
			dbChk(Factory::GetInstance().CreateDataSource(pDBSource));
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

		// Don't try any query while disconnected
		if( m_bIsDisconnected )
			return ResultCode::SUCCESS;

		auto maxTry = m_PendingQueries.GetEnqueCount();
		for (decltype(maxTry) iQuery = 0; iQuery < maxTry; iQuery++)
		{
			Session * pSession = nullptr;
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
				dbTrace(Trace::TRC_ERROR, "QueryWorker failure: The sharding bucket is empty");
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

			// Get Session
			if (!(pDBSource->AssignSession(pSession)))
			{
				// It's not expected
				pQuery->SetResult(ResultCode::UNEXPECTED);
				dbTrace(Trace::TRC_ERROR, "Assigning query to a worker failed {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
				RouteResult(pQuery);
				continue;
			}

			if (!pSession->IsOpened())
			{
				if (!(pSession->OpenSession()))
				{
					pQuery->SetResult(ResultCode::UNEXPECTED);
					pSession->ReleaseSession();
					dbTrace(Trace::TRC_ERROR, "Failed to open DB session {0}, TransID:{1}", typeid(*pQuery).name(), pQuery->GetTransID());
					RouteResult(pQuery);
					continue;
				}
			}

			pQuery->SetSession(pSession);

			if (!(QueryWorkerManager::PendingQuery(pQuery)))
			{
				// It's not expected
				pQuery->SetResult(ResultCode::UNEXPECTED);
				dbTrace(Trace::TRC_ERROR, "Assigning query to a worker failed {0}, TransID:{1}", typeid(*pQuery).name(), pQuery->GetTransID());
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

		return hr;
	}

	Result	DBClusterManager::UpdateResultQueries()
	{
		Result hr = ResultCode::SUCCESS;

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
				for (auto& rowRes : pDBRes->m_RowsetResult)
				{
					if ((unsigned)rowRes.ShardID < GetPartitioningCount() && (SelectDBByKey(rowRes.ShardID, pDBSource)))
					{
						if (pDBSource->GetDefaultDB() != rowRes.DBName
							|| pDBSource->GetConnectionString() != rowRes.ConnectionString)
						{
							dbTrace(Trace::TRC_INFO, "Initializating DBSource {0}, Shard:{1} {2}, {3}", typeid(*this).name(), rowRes.ShardID,  rowRes.ConnectionString, rowRes.DBName);
							dbChk(pDBSource->InitializeDBSource(rowRes.ConnectionString, rowRes.DBName, m_UserID, m_Password));
						}
					}
					else
					{
						AddDBSource(rowRes.ShardID, typeid(*this).name(), rowRes.ConnectionString, rowRes.DBName);
					}
					
				}
			}
			else
			{
				// not handled query result
				dbErr(ResultCode::NOT_IMPLEMENTED);
			}
		}

	Proc_End:

		return hr;
	}

	Result	DBClusterManager::RequestQuery(Query* pQuery)
	{
		Result hr = ResultCode::SUCCESS;
		Session * pSession = nullptr;
		DataSource *pDBSource = nullptr;

		pQuery->UpdateRequestedTime();
		pQuery->SetQueryManager(this);

		// Find the data source
		if (!(SelectDBByKey(pQuery->GetPartitioningKey(), pDBSource)))
		{
			dbTrace(Trace::TRC_ERROR, "QueryWorker failure: The sharding bucket is empty");
			// It's not expected
			pQuery->SetResult(ResultCode::UNEXPECTED);
			dbErr(ResultCode::UNEXPECTED);
		}

		if (!pDBSource->GetOpened())
		{
			// DB source fail, put it in the local queue and try again when DB is ready
			// This sometimes hides DB error.
			dbChk(m_PendingQueries.Enqueue(pQuery));
			return ResultCode::SUCCESS;
		}

		// Get Session
		if (!(pDBSource->AssignSession(pSession)))
		{
			// It's not expected
			pQuery->SetResult(ResultCode::UNEXPECTED);
			dbTrace(Trace::TRC_ERROR, "Assigning query to a worker failed {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
			dbErr(ResultCode::UNEXPECTED);
		}

		if (!pSession->IsOpened())
		{
			if (!(pSession->OpenSession()))
			{
				pQuery->SetResult(ResultCode::UNEXPECTED);
				pSession->ReleaseSession();
				dbTrace(Trace::TRC_ERROR, "Failed to open DB session {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
				dbErr(ResultCode::UNEXPECTED);
			}
		}

		pQuery->SetSession(pSession);

		dbTrace(DB::TRC_QUERY, "Query pending transID:{0} msg:{1}, class:{2}", pQuery->GetTransID(), pQuery->GetMsgID(), typeid(pQuery).name());

		dbChk(QueryWorkerManager::PendingQuery(pQuery));

	Proc_End:

		return hr;
	}



	// Route query result to entity
	Result	DBClusterManager::RouteResult(Query* &pQuery)
	{
		Result hr = ResultCode::SUCCESS;
		Svr::TransactionResult *pRes = pQuery;

		dbTrace(DB::TRC_QUERY, "Query route result transID:{0} msg:{1}, class:{2}", pQuery->GetTransID(), pQuery->GetMsgID(), typeid(pQuery).name());

		if( pRes->GetTransID() != TransactionID() )
		{
			Svr::BrServer *pMyServer = Svr::BrServer::GetInstance();
			dbChkPtr( pMyServer );

			const char* queryName = typeid(*pRes).name();
			auto msgID = pRes->GetMsgID();
			auto entityID = pRes->GetTransID().GetEntityID();
			hr = Svr::GetEntityTable().RouteTransactionResult(pRes);
			if (!(hr))
			{
				dbTrace(TRC_ROUTING, "Failed to route a message hr:{0} msgID:{1}, target entityID:{2}, query:{3}", hr, msgID, entityID, queryName);
				hr = ResultCode::INVALID_ENTITY;
				goto Proc_End;
			}
		}
		else if (pQuery->GetMsgID().GetMsgID() == QueryGetShardListCmd::MID.GetMsgID())
		{
			m_ResultQueries.Enqueue(pQuery);
			pQuery = nullptr;
			pRes = nullptr;
		}

	Proc_End:

		pQuery = nullptr;
		Util::SafeDelete( pRes );

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
			dbTrace(Trace::TRC_ERROR, "QueryWorker failure: The sharding bucket is empty");
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
			dbTrace(Trace::TRC_ERROR, "Assigning query to a worker failed {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
			dbErr(ResultCode::UNEXPECTED);
		}

		if (!pSession->IsOpened())
		{
			if (!(pSession->OpenSession()))
			{
				pQuery->SetResult(ResultCode::UNEXPECTED);
				pSession->ReleaseSession();
				dbTrace(Trace::TRC_ERROR, "Failed to open DB session {0}, TransID:{1}", typeid(pQuery).name(), pQuery->GetTransID());
				dbErr(ResultCode::UNEXPECTED);
			}
		}

		pQuery->SetSession(pSession);

		defChk(pSession->SendQuery(pQuery));

	Proc_End:

		if (pSession)
			pSession->ReleaseSession();

		return hr;
	}


} // namespace DB
} // namespace SF
