
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB query base
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Memory/MemoryPool.h"
#include "DB/Factory.h"

#include "Types/BrSvrTypes.h"
#include "Transaction/Transaction.h"
#include "SvrPolicyID.h"
#include "Net/Message.h"





namespace SF {
namespace DB {

	class Session;
	class DBClusterManager;

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query Base class
	//

	class Query: public Svr::TransactionResult
	{
	public:
		Query(Message::MessageID MsgID)
			: Svr::TransactionResult(MsgID)
			, m_pQueryManager(nullptr)
			, m_pSession(nullptr)
			, m_PartitioningKey(0)
		{
		}

		virtual ~Query() {}

		FORCEINLINE TimeStampMS GetRequestedTime()				{ return m_RequestedTime; }
		FORCEINLINE void UpdateRequestedTime()					{ m_RequestedTime = Util::Time.GetTimeMs(); }

		FORCEINLINE DBClusterManager* GetQueryManager()				{ return m_pQueryManager; }
		FORCEINLINE void SetQueryManager(DBClusterManager* pMgr)	{ m_pQueryManager = pMgr; }

		FORCEINLINE Session* GetSession()						{ return m_pSession; }
		FORCEINLINE void SetSession(Session* pSes)				{ m_pSession = pSes; }

		// Sharding ID
		FORCEINLINE uint GetPartitioningKey()					{ return m_PartitioningKey; }
		FORCEINLINE void SetPartitioningKey( uint key )			{ m_PartitioningKey = key; }

		virtual bool GetHasRowSetResult()			{ return false; }


	private:

		DBClusterManager *m_pQueryManager;
		Session*	m_pSession;
		// DB shard partitioning key
		uint		m_PartitioningKey;

		TimeStampMS	m_RequestedTime;
	};

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Query string macro
	//



	
} // namespace DB
} // namespace SF


#ifdef BRDB_USE_OLEDB
#include "DB/DBOLEDB.h"
#include "DB/QueryOLEDB.h"
#elif defined(BRDB_USE_MYSQL)
#include "DB/DBMYSQL.h"
#include "DB/QueryMYSQL.h"
#else
#error "DB type isn't specified"
#endif

