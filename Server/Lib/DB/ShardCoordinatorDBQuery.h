
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Shard Coordinator DB queries
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "DB/Query.h"
#include "DB/DBConst.h"
#include "DB/QueryConst.h"


namespace SF {
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
		int32_t ShardID;
		String ConnectionString;
		String DBName;
	};

	class QueryGetShardList : public QueryGetShardListSet, public QueryBase
	{
	public:
		int32_t Dummy = 0;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetShardList, "spGetShardList")
			BRDB_PARAM_ENTRY(ParamIO::Input, Dummy)
		BRDB_END_PARAM_MAP()
		
		//BRDB_BEGIN_RESULT_MAP(QueryGetShardListSet)
		//	BRDB_COLUMN_ENTRY(ShardID)
		//	BRDB_COLUMN_ENTRY(ConnectionString)
		//	BRDB_COLUMN_ENTRY(DBName)
		//BRDB_END_RESULT_MAP()
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_SHRDCOORDINATORDB, QueryGetShardList);

	
}  // namespace DB
}  // namespace SF
