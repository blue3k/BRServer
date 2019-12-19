
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB query base
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Memory/SFMemoryPool.h"
#include "DB/DBTrace.h"
#include "DB/Query.h"

#include <mysqlx/xdevapi.h>


namespace SF {
namespace DB {



	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query class
	//

	class QueryMYSQL : public Query
	{
	public:



	private:

	public:
		QueryMYSQL(IHeap& heap, Message::MessageID MsgID);
		virtual ~QueryMYSQL();


		virtual void BindParameters(mysqlx::SqlStatement& statement);
		virtual void BuildRowset() {}

		virtual void ParseResult(mysqlx::SqlResult& queryResult);
		virtual void ParseOutput(mysqlx::SqlResult& queryResult);

		// add patched rowset value into array
		virtual void AddRowset() {}
	};




} // namespace DB
} // namespace SF
