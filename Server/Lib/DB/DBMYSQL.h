
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
#include "Net/SFMessage.h"
#include "DB/DBTrace.h"


#include <mysqlx/xdevapi.h>



namespace SF {
namespace DB {

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Types for db
	//

	typedef uint64_t BRTIMESTAMP;
	
	typedef class QueryMYSQL QueryBase;


	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Type BIND
	//

	#define MYSQL_TYPE_FUNCTION(ctype, oledbtype) \
			inline mysqlx::Type _GetMYSQLDBType(ctype&) throw () { return oledbtype; }

	inline mysqlx::Type _GetMYSQLDBType(uint8_t[]) throw ()
	{
		return mysqlx::Type::BYTES;
	}
	inline mysqlx::Type _GetMYSQLDBType(char[]) throw ()
	{
		return mysqlx::Type::STRING;
	}

	MYSQL_TYPE_FUNCTION(const char*		 , mysqlx::Type::STRING)
	MYSQL_TYPE_FUNCTION(int64_t			 , mysqlx::Type::BIGINT)
	MYSQL_TYPE_FUNCTION(uint64_t			 , mysqlx::Type::BIGINT)
	MYSQL_TYPE_FUNCTION(int8_t			 , mysqlx::Type::TINYINT)
	MYSQL_TYPE_FUNCTION(uint8_t             , mysqlx::Type::TINYINT)
	MYSQL_TYPE_FUNCTION(int16_t			 , mysqlx::Type::SMALLINT)
	MYSQL_TYPE_FUNCTION(uint16_t			 , mysqlx::Type::SMALLINT)
	MYSQL_TYPE_FUNCTION(int32_t            , mysqlx::Type::INT)
	MYSQL_TYPE_FUNCTION(uint32_t			 , mysqlx::Type::INT)
	MYSQL_TYPE_FUNCTION(float            , mysqlx::Type::FLOAT)
	MYSQL_TYPE_FUNCTION(double           , mysqlx::Type::DOUBLE)
	MYSQL_TYPE_FUNCTION(char			 , mysqlx::Type::TINYINT)


	// Check weather this two type can be convertable or not
	inline bool MYSQL_IsCompatibleType( mysqlx::Type type1, mysqlx::Type type2 )
	{
		return type1 == type2;
	}

	
	// Check weather this two type can be convertable or not
	inline bool MYSQL_IsArrayType( mysqlx::Type type )
	{
		switch( type )
		{
		case mysqlx::Type::STRING:
		case mysqlx::Type::BYTES:
			return true;
			break;
		default:
			return false;
			break;
		};
	}
	
	// 
	template<class cType>
	inline unsigned long MYSQL_GetDataSize( cType &data )
	{
		return (unsigned long)sizeof(data);
	}

	template<int size>
	inline unsigned long MYSQL_GetDataSize( char (&data)[size] )
	{
		return (unsigned long)strlen(data);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Error handling
	//

	Result MYSQL_ToResult( int errorValue );


	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query definitions macros
	//


	// define query class
	#define BRDB_DEFINE_QUERYCLASS(Policy,QueryClass) \
		class QueryClass##Cmd : public QueryClass	\
		{																			\
		public :																	\
			enum { MESSAGE_POLICY = Policy };										\
			static const Message::MessageID MID;												\
			QueryClass##Cmd(IHeap& heap) : QueryClass(heap, MID ) { }		\
		};


	// define rowset query class
	#define BRDB_DEFINE_ROWSETQUERYCLASS(Policy,QueryClass,QueryClassRowset) \
		class QueryClass##Cmd : public QueryClass	\
		{																			\
		public :																	\
			enum { MESSAGE_POLICY = Policy };										\
			using RowsetList = DynamicArray<QueryClassRowset>;						\
			static const Message::MessageID MID;									\
			QueryClass##Cmd(IHeap& heap) : QueryClass(heap, MID ), RowsetResults(heap) { }		\
			DynamicArray<QueryClassRowset> RowsetResults;						\
			virtual void AddRowset() override { RowsetResults.push_back(std::forward<QueryClassRowset>(*static_cast<QueryClassRowset*>(this))); } \
		};


	// execute
	#define BRDB_DEFINE_QUERY_IMPL(QueryClass)										\
				const SF::Message::MessageID SF::DB::QueryClass##Cmd::MID = Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, false, DB::QueryClass##Cmd::MESSAGE_POLICY, MCODE_##QueryClass ); \



	//////////////////////////////////////////////////////////////
	//
	//	Mapping
	//

	#define BRDB_RAW_QUERY(classType, spName)										\
			classType(IHeap& heap, Message::MessageID msgID):QueryMYSQL(heap, msgID){	\
			BuildParameters(); BuildQueryString(spName, false); \
			}	\
			virtual void BuildParameters() override {}								\


	#define BRDB_BEGIN_PARAM_MAP(classType, spName)										\
			classType(IHeap& heap, Message::MessageID msgID):QueryMYSQL(heap, msgID){	\
			BuildParameters(); BuildRowset(); BuildQueryString(spName); \
			}	\
			virtual void BuildParameters() override {									\


	#define BRDB_PARAM_ENTRY(ioType, member)												\
			AddParameterBinding(ParameterInfo(#member,ioType,BoxingByReference(member)));	



	#define BRDB_END_PARAM_MAP()													\
			}


	#define BRDB_BEGIN_RESULT_MAP(classType)							\
			virtual void BuildRowset() override {										\


	#define BRDB_COLUMN_ENTRY(member)												\
			AddRowsetBinding(ParameterInfo(#member,ParamIO::Output,BoxingByReference(member)));


	#define BRDB_END_RESULT_MAP()		}



} // namespace DB
} // namespace SF
