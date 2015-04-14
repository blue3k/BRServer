
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Game DB query data
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
	//	Login DB Query Message Code
	//

	enum
	{
		MCODE_QueryGameTransactionNull,
		// Login
		MCODE_QueryAddGameLog,

	}; // enum MsgCode



	////////////////////////////////////////////////////////////////////////////////
	//
	// Query
	//

	class QueryAddGameLog : public QueryBase
	{
	public:
		INT64 PlayerID;
		INT32 GameTime;
		char  LogCategory[2];
		INT32 Consume;
		INT32 Gain;
		INT64 TotalValue;
		char LogMessage[MAX_TRANSACTION_LOG_MESSAGE];
		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryAddGameLog, 8)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(GameTime)
			BRDB_COLUMN_ENTRY(LogCategory)
			BRDB_COLUMN_ENTRY(Consume)
			BRDB_COLUMN_ENTRY(Gain)
			BRDB_COLUMN_ENTRY(TotalValue)
			BRDB_COLUMN_ENTRY(LogMessage)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spAddGameLog", BRDB_PARAM_8 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_GAMETRANSACTIONDB, QueryAddGameLog);




} // namespace DB
}  // namespace BR
