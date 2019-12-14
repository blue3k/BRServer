
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Game DB query data
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
		String LogMessage;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryAddGameLog, "spAddGameLog")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, GameTime)
			BRDB_PARAM_ENTRY(ParamIO::Input, LogCategory)
			BRDB_PARAM_ENTRY(ParamIO::Input, Consume)
			BRDB_PARAM_ENTRY(ParamIO::Input, Gain)
			BRDB_PARAM_ENTRY(ParamIO::Input, TotalValue)
			BRDB_PARAM_ENTRY(ParamIO::Input, LogMessage)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_GAMETRANSACTIONDB, QueryAddGameLog);




} // namespace DB
}  // namespace SF
