
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
#include "DB/dbTrace.h"
#include "DB/Query.h"
#include "Common/ClassUtil.h"

#include <my_global.h>
#include <mysql.h>
#include <mysqld_error.h>


namespace BR {
namespace DB {



	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query class
	//

	class QueryMYSQL : public Query
	{
	public:

		struct BindVariableState
		{
			my_bool isNull;
			unsigned long OutputLength;
		};


	private:

	public:
		QueryMYSQL(Message::MessageID MsgID);
		virtual ~QueryMYSQL();

		// total parameter count
		virtual UINT GetParameterCount()		{ return 0; }
		// input only parameter count
		virtual UINT GetInputParameterCount()	{ return 0; }

		// result colomn count
		virtual UINT GetResultCount()			{ return 0; }

		virtual MYSQL_BIND* BuildParameter()	{ return nullptr; }

		virtual MYSQL_BIND* BuildResult()		{ return nullptr; }


		// Query message
		virtual const char* GetQueryString() = 0;

		// handle new result column
		virtual void PrepareResultColumn()		{}
		virtual void OnNewResultColumn()		{}
	};




} // namespace DB
} // namespace BR