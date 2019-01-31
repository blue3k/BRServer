
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


#include <mysql.h>
#include <mysqld_error.h>



namespace SF {
namespace DB {

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Types for db
	//

	typedef MYSQL_TIME BRTIMESTAMP;
	
	typedef class QueryMYSQL QueryBase;

	enum BRDB_PARAMIO
    {
        BRDB_PARAMIO_NOTPARAM	= 0,
        BRDB_PARAMIO_INPUT	= 1,
        BRDB_PARAMIO_OUTPUT	= 2,
        BRDB_PARAMIO_INOUT	= 3,
    } ;


	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Type BIND
	//

	#define MYSQL_TYPE_FUNCTION(ctype, oledbtype) \
			inline enum_field_types _GetMYSQLDBType(ctype&) throw () { return oledbtype; }

	inline enum_field_types _GetMYSQLDBType(uint8_t[]) throw ()
	{
		return MYSQL_TYPE_BLOB;
	}
	inline enum_field_types _GetMYSQLDBType(char[]) throw ()
	{
		return MYSQL_TYPE_STRING;
	}

	MYSQL_TYPE_FUNCTION(const char*		 , MYSQL_TYPE_VARCHAR)
	MYSQL_TYPE_FUNCTION(int64_t			 , MYSQL_TYPE_LONGLONG)
	MYSQL_TYPE_FUNCTION(uint64_t			 , MYSQL_TYPE_LONGLONG)
	MYSQL_TYPE_FUNCTION(int8_t			 , MYSQL_TYPE_TINY)
	//MYSQL_TYPE_FUNCTION(UINT8			 , MYSQL_TYPE_TINY)
	//MYSQL_TYPE_FUNCTION(signed char      , MYSQL_TYPE_TINY)
	//MYSQL_TYPE_FUNCTION(int16_t            , MYSQL_TYPE_SHORT)
	MYSQL_TYPE_FUNCTION(int16_t			 , MYSQL_TYPE_SHORT)
	MYSQL_TYPE_FUNCTION(uint16_t			 , MYSQL_TYPE_SHORT)
	MYSQL_TYPE_FUNCTION(int32_t            , MYSQL_TYPE_LONG)
	MYSQL_TYPE_FUNCTION(uint32_t			 , MYSQL_TYPE_LONG)
//	MYSQL_TYPE_FUNCTION(LONG             , MYSQL_TYPE_LONG)
//	MYSQL_TYPE_FUNCTION(LONGLONG		 , MYSQL_TYPE_LONGLONG) 
	MYSQL_TYPE_FUNCTION(uint8_t             , MYSQL_TYPE_TINY)
//	MYSQL_TYPE_FUNCTION(unsigned long    , MYSQL_TYPE_LONG)
//	MYSQL_TYPE_FUNCTION(uint64_t		 , MYSQL_TYPE_LONGLONG)
	MYSQL_TYPE_FUNCTION(float            , MYSQL_TYPE_FLOAT)
	MYSQL_TYPE_FUNCTION(double           , MYSQL_TYPE_DOUBLE)
//	MYSQL_TYPE_FUNCTION(uint64_t        , MYSQL_TYPE_LONGLONG)
	MYSQL_TYPE_FUNCTION(char			 , FIELD_TYPE_CHAR)


	// Check weather this two type can be convertable or not
	inline bool MYSQL_IsCompatibleType( enum_field_types type1, enum_field_types type2 )
	{
		switch( type1 )
		{
		case MYSQL_TYPE_VAR_STRING:
		case MYSQL_TYPE_STRING:
			if (type2 == MYSQL_TYPE_VAR_STRING || type2 == MYSQL_TYPE_STRING)
				return true;
			else
				return false;
			break;
		default:
			return type1 == type2;
			break;
		};
	}

	
	// Check weather this two type can be convertable or not
	inline bool MYSQL_IsArrayType( enum_field_types type )
	{
		switch( type )
		{
		case MYSQL_TYPE_VAR_STRING:
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VARCHAR:
		case MYSQL_TYPE_BLOB:
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

	// Parameter count string helper
	#define BRDB_PARAM(X)	DRDB_PARAM_##X
	#define BRDB_PARAM_0	""
	#define BRDB_PARAM_1	BRDB_PARAM_0  "?"
	#define BRDB_PARAM_2	BRDB_PARAM_1  ",?"
	#define BRDB_PARAM_3	BRDB_PARAM_2  ",?"
	#define BRDB_PARAM_4	BRDB_PARAM_3  ",?"
	#define BRDB_PARAM_5	BRDB_PARAM_4  ",?"
	#define BRDB_PARAM_6	BRDB_PARAM_5  ",?"
	#define BRDB_PARAM_7	BRDB_PARAM_6  ",?"
	#define BRDB_PARAM_8	BRDB_PARAM_7  ",?"
	#define BRDB_PARAM_9	BRDB_PARAM_8  ",?"
	#define BRDB_PARAM_10	BRDB_PARAM_9  ",?"
	#define BRDB_PARAM_11	BRDB_PARAM_10 ",?"
	#define BRDB_PARAM_12	BRDB_PARAM_11 ",?"
	#define BRDB_PARAM_13	BRDB_PARAM_12 ",?"
	#define BRDB_PARAM_14	BRDB_PARAM_13 ",?"
	#define BRDB_PARAM_15	BRDB_PARAM_14 ",?"
	#define BRDB_PARAM_16	BRDB_PARAM_15 ",?"
	#define BRDB_PARAM_17	BRDB_PARAM_16 ",?"
	#define BRDB_PARAM_18	BRDB_PARAM_17 ",?"
	#define BRDB_PARAM_19	BRDB_PARAM_18 ",?"
	#define BRDB_PARAM_20	BRDB_PARAM_19 ",?"
	#define BRDB_PARAM_21	BRDB_PARAM_20 ",?"
	#define BRDB_PARAM_22	BRDB_PARAM_21 ",?"
	#define BRDB_PARAM_23	BRDB_PARAM_22 ",?"
	#define BRDB_PARAM_24	BRDB_PARAM_23 ",?"
	#define BRDB_PARAM_25	BRDB_PARAM_24 ",?"
	#define BRDB_PARAM_26	BRDB_PARAM_25 ",?"
	#define BRDB_PARAM_27	BRDB_PARAM_26 ",?"
	#define BRDB_PARAM_28	BRDB_PARAM_27 ",?"
	#define BRDB_PARAM_29	BRDB_PARAM_28 ",?"
	#define BRDB_PARAM_30	BRDB_PARAM_29 ",?"
	#define BRDB_PARAM_31	BRDB_PARAM_30 ",?"


	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query definitions macros
	//

	// Define query string
	#define BRDB_QUERYSTRING(strSP,strParam) const char* GetQueryString() { return "CALL " strSP "(" strParam ")"; }

	#define BRDB_RAW_QUERY_STRING(strSP) const char* GetQueryString() { return strSP; }


	// define query class
	#define BRDB_DEFINE_QUERYCLASS(Policy,QueryClass) \
		class QueryClass##Cmd : public QueryClass	\
		{																			\
		public :																	\
			enum { MESSAGE_POLICY = Policy };										\
			static const Message::MessageID MID;												\
			QueryClass##Cmd() : QueryClass( MID ) { }		\
		};


	// define rowset query class
	#define BRDB_DEFINE_ROWSETQUERYCLASS(Policy,QueryClass,QueryClassRowset) \
		class QueryClass##Cmd : public QueryClass	\
		{																			\
		public :																	\
			enum { MESSAGE_POLICY = Policy };										\
			static const Message::MessageID MID;									\
			typedef std::list<QueryClassRowset> RowsetList;							\
			RowsetList	m_RowsetResult;												\
																					\
			QueryClass##Cmd() : QueryClass( MID ) { }		\
			virtual void PrepareResultColumn() { memset( ((QueryClassRowset*)(this)), 0, sizeof(QueryClassRowset) ); } \
			virtual void OnNewResultColumn() { m_RowsetResult.push_back( *this ); }\
		};


	// execute
	#define BRDB_DEFINE_QUERY_IMPL(QueryClass)										\
				const SF::Message::MessageID SF::DB::QueryClass##Cmd::MID = Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, false, DB::QueryClass##Cmd::MESSAGE_POLICY, MCODE_##QueryClass ); \



	//////////////////////////////////////////////////////////////
	//
	//	Mapping
	//

	#define BRDB_BEGIN_EMPTY_PARAM_MAP(classType)								\
			classType(Message::MessageID msgID):QueryMYSQL(msgID){}					\
			virtual INT GetParameterCount() override		{ return 0; }			\
			virtual INT GetInputParameterCount() override	{ return 0; }\


	#define BRDB_END_EMPTY_PARAM_MAP()													\


	#define BRDB_BEGIN_PARAM_MAP(classType,ParamCount)								\
			classType(Message::MessageID msgID):QueryMYSQL(msgID){}					\
			virtual INT GetParameterCount()		{ return ParamCount; }			\
			INT m_DBQueryInputParamCount;											\
			virtual INT GetInputParameterCount()	{ return m_DBQueryInputParamCount; }\
			MYSQL_BIND m_DBQueryParameters[ParamCount];								\
			BindVariableState m_DBQueryParameterStates[ParamCount];					\
			virtual MYSQL_BIND* BuildParameter() {									\
				BRDB_PARAMIO ioType = BRDB_PARAMIO_INPUT;							\
				INT iParam = 0;													\
				m_DBQueryInputParamCount = 0;										\
				memset( m_DBQueryParameters, 0, sizeof(m_DBQueryParameters) );		\
				memset( m_DBQueryParameterStates, 0, sizeof(m_DBQueryParameterStates) );	\
				MYSQL_BIND *pColumn = m_DBQueryParameters;							\
				BindVariableState *pColumnState = m_DBQueryParameterStates;			\


	#define BRDB_SET_PARAM_TYPE(IOType)												\
			Assert((ioType&BRDB_PARAMIO_INPUT)!=0);									\
			ioType = IOType;														\
			if( (ioType&BRDB_PARAMIO_INPUT)==0 )									\
				m_DBQueryInputParamCount = iParam;									\

	#define BRDB_COLUMN_ENTRY(member)												\
			pColumnState[iParam].isNull = false;									\
			pColumnState[iParam].OutputLength = MYSQL_GetDataSize(member);			\
			pColumn[iParam].buffer_type= _GetMYSQLDBType(member);					\
			pColumn[iParam].is_null= &pColumnState[iParam].isNull;					\
			pColumn[iParam].buffer= MYSQL_IsArrayType(pColumn[iParam].buffer_type) ? (char*)((intptr_t)member): (char*)&member;	\
			pColumn[iParam].buffer_length= sizeof(member);							\
			pColumn[iParam].length = &pColumnState[iParam].OutputLength;			\
			iParam++;\

	#define BRDB_END_PARAM_MAP()													\
			Assert(iParam == GetParameterCount());									\
			if((ioType&BRDB_PARAMIO_OUTPUT) == 0)									\
				m_DBQueryInputParamCount = iParam;									\
			return m_DBQueryParameters; }


	#define BRDB_BEGIN_RESULT_MAP(classType,resultCount)							\
			virtual INT GetResultCount() override		{ return resultCount; }				\
			MYSQL_BIND m_DBQueryResults[resultCount];								\
			BindVariableState m_DBQueryResultStates[resultCount];					\
			virtual MYSQL_BIND* BuildResult() {										\
				INT iParam = 0;													\
				memset( m_DBQueryResults, 0, sizeof(m_DBQueryResults) );			\
				memset( m_DBQueryResultStates, 0, sizeof(m_DBQueryResultStates) );	\
				MYSQL_BIND *pColumn = m_DBQueryResults;								\
				QueryMYSQL::BindVariableState *pColumnState = m_DBQueryResultStates;\


	#define BRDB_END_RESULT_MAP()								Assert(iParam == GetResultCount()); return m_DBQueryResults; }



} // namespace DB
} // namespace SF
