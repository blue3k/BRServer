

#include "StdAfx.h"


#include "StdAfx.h"
#include "Common/Typedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_MYSQL


#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DBTrace.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"
#include "Query.h"
#include "QueryMYSQL.h"
#include "Common/HRESSvrSys.h"



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
