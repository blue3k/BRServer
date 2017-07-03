
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : MadK
//
// Description : Shard Coordinator DB queries
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "DB/Query.h"
#include "DB/DBConst.h"
#include "DB/QueryConst.h"


namespace BR {
namespace DB {
	
	
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query Message Code
	//

	enum MsgShardCoordinatorDBCode
	{
		MCODE_QueryGetShardList = 1,
	}; // enum MsgCode



	struct QueryGetShardListSet
	{
		INT32 ShardID;
		char ConnectionString[512];
		char DBName[128];
	};

	class QueryGetShardList : public QueryGetShardListSet, public QueryBase
	{
	public:
		INT32 Dummy;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetShardList, 1)
			BRDB_COLUMN_ENTRY(Dummy)
		BRDB_END_PARAM_MAP()
		
		BRDB_BEGIN_RESULT_MAP(QueryGetShardListSet, 3)
			BRDB_COLUMN_ENTRY(ShardID)
			BRDB_COLUMN_ENTRY(ConnectionString)
			BRDB_COLUMN_ENTRY(DBName)
		BRDB_END_RESULT_MAP()

		BRDB_QUERYSTRING( "spGetShardList", BRDB_PARAM_1 )
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(POLICY_SHRDCOORDINATORDB, QueryGetShardList, QueryGetShardListSet);

	
}  // namespace DB
}  // namespace BR
