
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
		INT64 PlayerID;
		INT64 AuthTicket;
		INT64 LoginEntityUID;
		INT64 GameEntityUID;
		//ULONGLONG HeartbitTime;
		INT32 Result;

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

	BRDB_DEFINE_QUERYCLASS(POLICY_LOGINSESSIONDB, QueryRegisterAuthTicket);


	class QueryReplaceLoginSession : public QueryBase
	{
	public:
		INT64 PlayerID;
		INT64 OldAuthTicket;
		INT64 AuthTicket;
		INT64 LoginEntityUID;
		INT32 Result;

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

	BRDB_DEFINE_QUERYCLASS(POLICY_LOGINSESSIONDB, QueryReplaceLoginSession);

	
	class QueryDeleteLoginSession : public QueryBase
	{
	public:
		INT64 PlayerID;
		INT64 AuthTicket;
		INT32 Result;

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

	BRDB_DEFINE_QUERYCLASS(POLICY_LOGINSESSIONDB, QueryDeleteLoginSession);


	class QueryConnectedToGameServer : public QueryBase
	{
	public:
		INT64 PlayerID;
		INT64 AuthTicket;
		INT64 LoginEntityUID;
		INT64 GameEntityUID;
		INT32 Result;

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

	BRDB_DEFINE_QUERYCLASS(POLICY_LOGINSESSIONDB, QueryConnectedToGameServer);



	class QueryValidateGameServerSession : public QueryBase
	{
	public:
		INT64 PlayerID;
		INT64 AuthTicket;
		INT64 GameEntityUID;
		INT32 Result;

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

	BRDB_DEFINE_QUERYCLASS(POLICY_LOGINSESSIONDB, QueryValidateGameServerSession);


	class QueryGameServerHeartBit : public QueryBase
	{
	public:
		INT64 PlayerID;
		INT64 AuthTicket;
		INT64 GameEntityUID;
		INT32 Result;

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

	BRDB_DEFINE_QUERYCLASS(POLICY_LOGINSESSIONDB, QueryGameServerHeartBit);
	


	class QueryFindPlayer : public QueryBase
	{
	public:
		INT64 PlayerID;
		INT64 LoginEntityID;
		INT64 GameEntityUID;
		INT32 Result;

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

	BRDB_DEFINE_QUERYCLASS(POLICY_LOGINSESSIONDB, QueryFindPlayer);



} // namespace DB
}  // namespace BR
