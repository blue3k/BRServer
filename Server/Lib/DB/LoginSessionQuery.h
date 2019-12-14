
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
		BRDB_BEGIN_PARAM_MAP(QueryRegisterAuthTicket, "spRegisterAuthTicket")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, AuthTicket)
			BRDB_PARAM_ENTRY(ParamIO::Input, LoginEntityUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, GameEntityUID)
			BRDB_PARAM_ENTRY(ParamIO::Input, Result)
		BRDB_END_PARAM_MAP()
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
		BRDB_BEGIN_PARAM_MAP(QueryReplaceLoginSession, "spReplaceLoginSession")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, OldAuthTicket)
			BRDB_PARAM_ENTRY(ParamIO::Input, AuthTicket)
			BRDB_PARAM_ENTRY(ParamIO::Input, LoginEntityUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryReplaceLoginSession);

	
	class QueryDeleteLoginSession : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t AuthTicket;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryDeleteLoginSession, "spDeleteLoginSession")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, AuthTicket)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
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
		BRDB_BEGIN_PARAM_MAP(QueryConnectedToGameServer, "spConnectedToGameServer")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, AuthTicket)
			BRDB_PARAM_ENTRY(ParamIO::Input, LoginEntityUID)
			BRDB_PARAM_ENTRY(ParamIO::Input, GameEntityUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
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
		BRDB_BEGIN_PARAM_MAP(QueryValidateGameServerSession, "spValidateGameServerSession")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, AuthTicket)
			BRDB_PARAM_ENTRY(ParamIO::Input, GameEntityUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
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
		BRDB_BEGIN_PARAM_MAP(QueryGameServerHeartBit, "spGameServerHeartBit")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Input, AuthTicket)
			BRDB_PARAM_ENTRY(ParamIO::Input, GameEntityUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

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
		BRDB_BEGIN_PARAM_MAP(QueryFindPlayer, "spFindPlayer")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, LoginEntityID)
			BRDB_PARAM_ENTRY(ParamIO::Output, GameEntityUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_LOGINSESSIONDB, QueryFindPlayer);



} // namespace DB
}  // namespace SF
