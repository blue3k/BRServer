
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : Account DB queries
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
	//	Query Message Code
	//

	enum MsgCode
	{
		MCODE_QueryNull,
		MCODE_QueryCreateUser,
		MCODE_QueryLogin,
		MCODE_QueryLogout,
		MCODE_QueryUserList,
		MCODE_QueryFacebookCreateUser,
		MCODE_QueryFacebookLogin,
		MCODE_QueryCreateRandomUser,
		MCODE_QueryUpdateGCMKeys,
		MCODE_QueryUpdateUserContactInfo,
		MCODE_QueryFindPlayerByEMail,
		MCODE_QueryFindPlayerByPlayerID,
		MCODE_QueryGetPlayerShardID,
	}; // enum MsgCode


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryCreateUser class
	//

	class QueryCreateUser : public QueryBase
	{
	public:
		String UserName;
		String Password;
		String EMail;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryCreateUser, "spCreateUser")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserName)
			BRDB_PARAM_ENTRY(ParamIO::Input, Password)
			BRDB_PARAM_ENTRY(ParamIO::Input, EMail)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryCreateUser);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryLogin class
	//

	class QueryLogin : public QueryBase
	{
	public:
		String UserName;
		String Password;

		int64_t AccountID;
		int64_t FBUserID;
		int32_t ShardID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryLogin, "spLogin")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserName)
			BRDB_PARAM_ENTRY(ParamIO::Input, Password)
			BRDB_PARAM_ENTRY(ParamIO::Output, AccountID)
			BRDB_PARAM_ENTRY(ParamIO::Output, FBUserID)
			BRDB_PARAM_ENTRY(ParamIO::Output, ShardID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryLogin);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryLogin class
	//

	class QueryLogout : public QueryBase
	{
	public:
		String UserName;
		uint8_t LogoutTime[4];
		int32_t Result;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryLogout, "spLogout")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserName)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryLogout);



	struct QueryUserListResultSet
	{
		int64_t m_PlayerUID;
		String m_Name;
		int64_t m_FBUID;
	};

	class QueryUserList : public QueryBase
	{
	public:
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUserList,"spUserList")
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
		

	};

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryUserList);

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryFacebookCreateUser class
	//

	class QueryFacebookCreateUser : public QueryBase
	{
	public:
		int64_t FBUserID;
		String EMail;
		String CellPhone;

		int64_t AccountID;
		int32_t ShardID;
		int32_t Result;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryFacebookCreateUser, "spFacebookCreateUser")
			BRDB_PARAM_ENTRY(ParamIO::Input, FBUserID)
			BRDB_PARAM_ENTRY(ParamIO::Input, EMail)
			BRDB_PARAM_ENTRY(ParamIO::Input, CellPhone)
			BRDB_PARAM_ENTRY(ParamIO::Output, AccountID)
			BRDB_PARAM_ENTRY(ParamIO::Output, ShardID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB, QueryFacebookCreateUser);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryFacebookLogin class
	//

	class QueryFacebookLogin : public QueryBase
	{
	public:
		int64_t FBUserID;

		int64_t AccountID;
		String EMail;
		String CellPhone;
		String GCMKeys;
		int32_t ShardID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryFacebookLogin, "spFacebookLogin")
			BRDB_PARAM_ENTRY(ParamIO::Input, FBUserID)
			BRDB_PARAM_ENTRY(ParamIO::Output, AccountID)
			BRDB_PARAM_ENTRY(ParamIO::Output, EMail)
			BRDB_PARAM_ENTRY(ParamIO::Output, CellPhone)
			BRDB_PARAM_ENTRY(ParamIO::Output, GCMKeys)
			BRDB_PARAM_ENTRY(ParamIO::Output, ShardID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryFacebookLogin);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryCreateRandomUser class
	//

	class QueryCreateRandomUser : public QueryBase
	{
	public:
		String UserName;
		String CellPhone;

		int64_t AccountID;
		int64_t FBUserID;
		int32_t ShardID;
		int32_t Result;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryCreateRandomUser, "spCreateRandomUser")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserName)
			BRDB_PARAM_ENTRY(ParamIO::Input, CellPhone)
			BRDB_PARAM_ENTRY(ParamIO::Output, AccountID)
			BRDB_PARAM_ENTRY(ParamIO::Output, FBUserID)
			BRDB_PARAM_ENTRY(ParamIO::Output, ShardID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
			BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB, QueryCreateRandomUser);

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryFacebookCreateUser class
	//

	class QueryUpdateGCMKeys : public QueryBase
	{
	public:
		int64_t UserUID;
		String GCMKeys;

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateGCMKeys, "spUpdateGCMKeys")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserUID)
			BRDB_PARAM_ENTRY(ParamIO::Input, GCMKeys)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()

	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryUpdateGCMKeys);
	


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryUpdateUserContactInfo class
	//

	class QueryUpdateUserContactInfo : public QueryBase
	{
	public:
		int64_t UserUID;
		char EMail[Const::MAX_EMAIL];
		char CellPhone[Const::MAX_CELLPHONE];

		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateUserContactInfo, "spUpdateUserContactInfo")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserUID)
			BRDB_PARAM_ENTRY(ParamIO::Input, EMail)
			BRDB_PARAM_ENTRY(ParamIO::Input, CellPhone)
			BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB, QueryUpdateUserContactInfo);


	

	class QueryFindPlayerByEMail : public QueryBase
	{
	public:
		char EMail[Const::MAX_EMAIL];

		int64_t UserID;
		int64_t FacebookUID;
		int32_t ShardID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryFindPlayerByEMail, "spFindPlayerByEMail")
			BRDB_PARAM_ENTRY(ParamIO::Input, EMail)
			BRDB_PARAM_ENTRY(ParamIO::Output, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Output, ShardID)
			BRDB_PARAM_ENTRY(ParamIO::Output, FacebookUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryFindPlayerByEMail);




	class QueryFindPlayerByPlayerID : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t FacebookUID;
		int32_t ShardID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryFindPlayerByPlayerID, "spFindPlayerByPlayerID")
			BRDB_PARAM_ENTRY(ParamIO::Input, PlayerID)
			BRDB_PARAM_ENTRY(ParamIO::Output, ShardID)
			BRDB_PARAM_ENTRY(ParamIO::Output, FacebookUID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB, QueryFindPlayerByPlayerID);



	class QueryGetPlayerShardID : public QueryBase
	{
	public:
		int64_t UserID;
		int32_t ShardID;
		int32_t Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerShardID, "spGetPlayerShardID")
			BRDB_PARAM_ENTRY(ParamIO::Input, UserID)
			BRDB_PARAM_ENTRY(ParamIO::Output, ShardID)
			BRDB_PARAM_ENTRY(ParamIO::Output, Result)
		BRDB_END_PARAM_MAP()
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB, QueryGetPlayerShardID);

}  // namespace DB
}  // namespace SF
