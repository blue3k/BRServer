
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
		int64_t PlayerID;
		int32_t GameTime;
		char  LogCategory[2];
		int32_t Consume;
		int32_t Gain;
		int64_t TotalValue;
		char LogMessage[MAX_TRANSACTION_LOG_MESSAGE];
		int32_t Result;

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

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMETRANSACTIONDB, QueryAddGameLog);




} // namespace DB
}  // namespace BR
