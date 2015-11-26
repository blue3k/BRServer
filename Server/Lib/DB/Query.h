
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

#include "Common/Typedefs.h"
#include "Common/MemoryPool.h"
#include "DB/DBTrace.h"
#include "DB/Factory.h"

#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/Transaction.h"
#include "Common/SvrPolicyID.h"
#include "Common/Message.h"
#include "Common/ClassUtil.h"



namespace BR {
namespace DB {

	class Session;
	class QueryManager;

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

		FORCEINLINE QueryManager* GetQueryManager()				{ return m_pQueryManager; }
		FORCEINLINE void SetQueryManager(QueryManager* pMgr)	{ m_pQueryManager = pMgr; }

		FORCEINLINE Session* GetSession()						{ return m_pSession; }
		FORCEINLINE void SetSession(Session* pSes)				{ m_pSession = pSes; }

		// Sharding ID
		FORCEINLINE UINT GetPartitioningKey()					{ return m_PartitioningKey; }
		FORCEINLINE void SetPartitioningKey( UINT key )			{ m_PartitioningKey = key; }

		virtual bool GetHasRowSetResult()			{ return false; }


	private:

		QueryManager *m_pQueryManager;
		Session*	m_pSession;
		// DB shard partitioning key
		UINT		m_PartitioningKey;

		TimeStampMS	m_RequestedTime;
	};

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Query string macro
	//



	
} // namespace DB
} // namespace BR


#ifdef BRDB_USE_OLEDB
#include "DB/DBOLEDB.h"
#include "DB/QueryOLEDB.h"
#elif defined(BRDB_USE_MYSQL)
#include "DB/DBMYSQL.h"
#include "DB/QueryMYSQL.h"
#else
#error "DB type isn't specified"
#endif

