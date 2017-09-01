
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
#include "DB/dbTrace.h"
#include "Query.h"

#include "Types/SvrTypes.h"
#include "ServerSystem/Transaction.h"
#include "Common/SvrPolicyID.h"
#include "Net/Message.h"

#include <atlcore.h>
#include <atldbcli.h>


namespace BR {
namespace DB {

	typedef DBTIMESTAMP BRTIMESTAMP;
	
	
	enum BRDB_PARAMIO
    {
        BRDB_PARAMIO_NOTPARAM	= DBPARAMIO_NOTPARAM,
        BRDB_PARAMIO_INPUT	= DBPARAMIO_INPUT,
        BRDB_PARAMIO_OUTPUT	= DBPARAMIO_OUTPUT
    } ;

	
	// Parameter count string helper
	#define BRDB_PARAM(X)	DRDB_PARAM_##X
	#define BRDB_PARAM_0	L""
	#define BRDB_PARAM_1	BRDB_PARAM_0  L"?"
	#define BRDB_PARAM_2	BRDB_PARAM_1  L"?"
	#define BRDB_PARAM_3	BRDB_PARAM_2  L"?"
	#define BRDB_PARAM_4	BRDB_PARAM_3  L"?"
	#define BRDB_PARAM_5	BRDB_PARAM_4  L"?"
	#define BRDB_PARAM_6	BRDB_PARAM_5  L"?"
	#define BRDB_PARAM_7	BRDB_PARAM_6  L"?"
	#define BRDB_PARAM_8	BRDB_PARAM_7  L"?"
	#define BRDB_PARAM_9	BRDB_PARAM_8  L"?"
	#define BRDB_PARAM_10	BRDB_PARAM_9  L"?"
	#define BRDB_PARAM_11	BRDB_PARAM_10 L"?"
	#define BRDB_PARAM_12	BRDB_PARAM_11 L"?"
	#define BRDB_PARAM_13	BRDB_PARAM_12 L"?"
	#define BRDB_PARAM_14	BRDB_PARAM_13 L"?"
	#define BRDB_PARAM_15	BRDB_PARAM_14 L"?"
	#define BRDB_PARAM_16	BRDB_PARAM_15 L"?"
	#define BRDB_PARAM_17	BRDB_PARAM_16 L"?"
	#define BRDB_PARAM_18	BRDB_PARAM_17 L"?"
	#define BRDB_PARAM_19	BRDB_PARAM_18 L"?"
	#define BRDB_PARAM_20	BRDB_PARAM_19 L"?"
	#define BRDB_PARAM_21	BRDB_PARAM_20 L"?"
	#define BRDB_PARAM_22	BRDB_PARAM_21 L"?"
	#define BRDB_PARAM_23	BRDB_PARAM_22 L"?"
	#define BRDB_PARAM_24	BRDB_PARAM_23 L"?"
	#define BRDB_PARAM_25	BRDB_PARAM_24 L"?"
	#define BRDB_PARAM_26	BRDB_PARAM_25 L"?"
	#define BRDB_PARAM_27	BRDB_PARAM_26 L"?"
	#define BRDB_PARAM_28	BRDB_PARAM_27 L"?"
	#define BRDB_PARAM_29	BRDB_PARAM_28 L"?"
	#define BRDB_PARAM_30	BRDB_PARAM_29 L"?"
	#define BRDB_PARAM_31	BRDB_PARAM_30 L"?"

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Query string macro
	//

	// Define query string
	
	#define BRDB_QUERYSTRING(strSP,strParam) const WCHAR* GetQueryString() { return L"{ CALL dbo." strSP L"(" strParam L") }"; }


	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query definitions macros
	//

	// define query class
	#define BRDB_DEFINE_QUERYCLASS(Policy,QueryClass) \
		class QueryClass##Cmd : public CCommand<CAccessor<QueryClass>>, public Query, public BR::MemoryPoolObject<##QueryClass##Cmd>	\
		{																			\
		public :																	\
			QueryClass##Cmd() : Query( Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, false, Policy, MCODE_##QueryClass## ), false ) { }		\
			virtual void Release()	{ delete this; }								\
		};

	// define rowset query class
	#define BRDB_DEFINE_ROWSETQUERYCLASS(Policy,QueryClass,QueryClassRowset) \
		class QueryClass##Cmd : public CCommand<CAccessor<QueryClass>>, public Query, public BR::MemoryPoolObject<QueryClass##Cmd>	\
		{																			\
		public :																	\
			std::list<QueryClassRowset>	m_RowsetResult;							\
																					\
			QueryClass##Cmd() : Query( Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, false, Policy, MCODE_##QueryClass## ), true ) { }		\
			virtual void Release()	{ delete this; }								\
		};

	// execute
	#define BRDB_DEFINE_QUERY_IMPL(QueryClass)										\
				SF_MEMORYPOOL_IMPLEMENT(BR::BR::DB::##QueryClass##Cmd);					\



	#define BRDB_BEGIN_PARAM_MAP		BEGIN_PARAM_MAP
	#define BRDB_SET_PARAM_TYPE			SET_PARAM_TYPE
	#define BRDB_COLUMN_ENTRY(type)		COLUMN_ENTRY(nColumns+1,type)
	#define BRDB_END_PARAM_MAP			END_PARAM_MAP

	#define BRDB_BEGIN_RESULT_MAP		BEGIN_COLUMN_MAP
	#define BRDB_END_RESULT_MAP			END_COLUMN_MAP


} // namespace DB
} // namespace BR
