

#include "stdafx.h"


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_MYSQL


#include "SFTypedefs.h"
#include "String/StrUtil.h"
#include "DBTrace.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"
#include "Query.h"
#include "QueryMYSQL.h"
#include "ResultCode/SFResultCodeSvr.h"



namespace BR {
namespace DB {


	QueryMYSQL::QueryMYSQL(Message::MessageID MsgID)
		:Query( MsgID)
	{
	}

	QueryMYSQL::~QueryMYSQL()
	{
	}
	

} // namespace DB
} // namespace BR

#endif // BRDB_USE_MYSQL
