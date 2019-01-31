
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
		MCODE_QueryLoginSessionNull,
		// Login
		MCODE_QueryRegisterAuthTicket,
		MCODE_QueryReplaceLoginSession,
		MCODE_QueryDeleteLoginSession,
		MCODE_QueryConnectedToGameServer,
		MCODE_QueryValidateGameServerSession,
		MCODE_QueryGameServerHeartBit,
		MCODE_QueryFindPlayer,

	}; // enum MsgCode



	////////////////////////////////////////////////////////////////////////////////
	//
	// Query
	//

	class QueryRegisterAuthTicket : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t AuthTicket;
		int64_t LoginEntityUID;
		int64_t GameEntityUID;
		//uint64_t HeartbitTime;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryRegisterAuthTicket,5)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(AuthTicket)
			BRDB_COLUMN_ENTRY(LoginEntityUID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(GameEntityUID)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spRegisterAuthTicket", BRDB_PARAM_5 )
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryRegisterAuthTicket);


	class QueryReplaceLoginSession : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t OldAuthTicket;
		int64_t AuthTicket;
		int64_t LoginEntityUID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryReplaceLoginSession, 5)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(OldAuthTicket)
			BRDB_COLUMN_ENTRY(AuthTicket)
			BRDB_COLUMN_ENTRY(LoginEntityUID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spReplaceLoginSession", BRDB_PARAM_5)
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryReplaceLoginSession);

	
	class QueryDeleteLoginSession : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t AuthTicket;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryDeleteLoginSession,3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(AuthTicket)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spDeleteLoginSession", BRDB_PARAM_3 )
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryDeleteLoginSession);


	class QueryConnectedToGameServer : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t AuthTicket;
		int64_t LoginEntityUID;
		int64_t GameEntityUID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryConnectedToGameServer,5)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(AuthTicket)
			BRDB_COLUMN_ENTRY(LoginEntityUID)
			BRDB_COLUMN_ENTRY(GameEntityUID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spConnectedToGameServer", BRDB_PARAM_5 )
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryConnectedToGameServer);



	class QueryValidateGameServerSession : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t AuthTicket;
		int64_t GameEntityUID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryValidateGameServerSession, 4)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(AuthTicket)
			BRDB_COLUMN_ENTRY(GameEntityUID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spValidateGameServerSession", BRDB_PARAM_4 )
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryValidateGameServerSession);


	class QueryGameServerHeartBit : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t AuthTicket;
		int64_t GameEntityUID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGameServerHeartBit,4)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_COLUMN_ENTRY(AuthTicket)
			BRDB_COLUMN_ENTRY(GameEntityUID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spGameServerHeartBit", BRDB_PARAM_4 )
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryGameServerHeartBit);
	


	class QueryFindPlayer : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t LoginEntityID;
		int64_t GameEntityUID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryFindPlayer,4)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(LoginEntityID)
			BRDB_COLUMN_ENTRY(GameEntityUID)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spFindPlayer", BRDB_PARAM_4 )
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryFindPlayer);



} // namespace DB
}  // namespace SF
