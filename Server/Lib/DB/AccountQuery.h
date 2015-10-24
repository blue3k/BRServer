
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Account DB queries
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
		char UserName[Const::MAX_USERNAME];
		char Password[Const::MAX_PASSWORD];
		char EMail[Const::MAX_USERNAME];

		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryCreateUser,4)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserName)
			BRDB_COLUMN_ENTRY(Password)
			BRDB_COLUMN_ENTRY(EMail)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spCreateUser", BRDB_PARAM_4 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB,QueryCreateUser);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryLogin class
	//

	class QueryLogin : public QueryBase
	{
	public:
		char UserName[Const::MAX_USERNAME];
		char Password[Const::MAX_PASSWORD];

		INT64 AccountID;
		INT64 FBUserID;
		INT32 ShardID;
		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryLogin,6)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserName)
			BRDB_COLUMN_ENTRY(Password)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(AccountID)
			BRDB_COLUMN_ENTRY(FBUserID)
			BRDB_COLUMN_ENTRY(ShardID)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spLogin", BRDB_PARAM_6 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB,QueryLogin);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryLogin class
	//

	class QueryLogout : public QueryBase
	{
	public:
		char UserName[Const::MAX_USERNAME];
		BYTE LogoutTime[4];
		INT32 Result;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryLogout, 2)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)	
			BRDB_COLUMN_ENTRY(UserName)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spLogout", BRDB_PARAM_2 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB,QueryLogout);



	struct QueryUserListResultSet
	{
		INT64 m_PlayerUID;
		char m_Name[BINSIZE_NAME];
		INT64 m_FBUID;
	};

	class QueryUserList : public QueryUserListResultSet, public QueryBase
	{
	public:
		INT32 Result;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryUserList,1)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()
		
		BRDB_BEGIN_RESULT_MAP(QueryUserList,3)
			BRDB_COLUMN_ENTRY(m_PlayerUID)
			BRDB_COLUMN_ENTRY(m_Name)
			BRDB_COLUMN_ENTRY(m_FBUID)
		BRDB_END_RESULT_MAP()


		BRDB_QUERYSTRING( "spUserList", BRDB_PARAM_1 )
	};

	BRDB_DEFINE_ROWSETQUERYCLASS(POLICY_ACCOUNTDB,QueryUserList, QueryUserListResultSet);

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryFacebookCreateUser class
	//

	class QueryFacebookCreateUser : public QueryBase
	{
	public:
		INT64 FBUserID;
		char EMail[Const::MAX_EMAIL];
		char CellPhone[Const::MAX_CELLPHONE];

		INT64 AccountID;
		INT32 ShardID;
		INT32 Result;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryFacebookCreateUser,6)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(FBUserID)
			BRDB_COLUMN_ENTRY(EMail)
			BRDB_COLUMN_ENTRY(CellPhone)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(AccountID)
			BRDB_COLUMN_ENTRY(ShardID)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spFacebookCreateUser", BRDB_PARAM_6 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB, QueryFacebookCreateUser);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryFacebookLogin class
	//

	class QueryFacebookLogin : public QueryBase
	{
	public:
		INT64 FBUserID;

		INT64 AccountID;
		char EMail[Const::MAX_EMAIL];
		char CellPhone[Const::MAX_CELLPHONE];
		char GCMKeys[Const::MAX_GCMKEY];
		INT32 ShardID;
		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryFacebookLogin,7)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(FBUserID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(AccountID)
			BRDB_COLUMN_ENTRY(EMail)
			BRDB_COLUMN_ENTRY(CellPhone)
			BRDB_COLUMN_ENTRY(GCMKeys)
			BRDB_COLUMN_ENTRY(ShardID)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spFacebookLogin", BRDB_PARAM_7 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB,QueryFacebookLogin);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryCreateRandomUser class
	//

	class QueryCreateRandomUser : public QueryBase
	{
	public:
		char UserName[Const::MAX_USERNAME];
		char CellPhone[Const::MAX_CELLPHONE];

		INT64 AccountID;
		INT64 FBUserID;
		INT32 ShardID;
		INT32 Result;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryCreateRandomUser, 6)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserName)
			BRDB_COLUMN_ENTRY(CellPhone)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(AccountID)
			BRDB_COLUMN_ENTRY(FBUserID)
			BRDB_COLUMN_ENTRY(ShardID)
			BRDB_COLUMN_ENTRY(Result)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spCreateRandomUser", BRDB_PARAM_6)
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB, QueryCreateRandomUser);

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryFacebookCreateUser class
	//

	class QueryUpdateGCMKeys : public QueryBase
	{
	public:
		INT64 UserUID;
		char GCMKeys[Const::MAX_GCMKEY];

		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateGCMKeys,3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserUID)
			BRDB_COLUMN_ENTRY(GCMKeys)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spUpdateGCMKeys", BRDB_PARAM_3 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB,QueryUpdateGCMKeys);
	


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryUpdateUserContactInfo class
	//

	class QueryUpdateUserContactInfo : public QueryBase
	{
	public:
		INT64 UserUID;
		char EMail[Const::MAX_EMAIL];
		char CellPhone[Const::MAX_CELLPHONE];

		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryUpdateUserContactInfo, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserUID)
			BRDB_COLUMN_ENTRY(EMail)
			BRDB_COLUMN_ENTRY(CellPhone)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spUpdateUserContactInfo", BRDB_PARAM_3)
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB, QueryUpdateUserContactInfo);


	

	class QueryFindPlayerByEMail : public QueryBase
	{
	public:
		char EMail[Const::MAX_EMAIL];

		INT64 UserID;
		INT64 FacebookUID;
		INT32 ShardID;
		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryFindPlayerByEMail,5)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(EMail)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_COLUMN_ENTRY(ShardID)
			BRDB_COLUMN_ENTRY(FacebookUID)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spFindPlayerByEMail", BRDB_PARAM_5 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB,QueryFindPlayerByEMail);




	class QueryFindPlayerByPlayerID : public QueryBase
	{
	public:
		INT64 PlayerID;
		INT64 FacebookUID;
		INT32 ShardID;
		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryFindPlayerByPlayerID,4)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(PlayerID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(ShardID)
			BRDB_COLUMN_ENTRY(FacebookUID)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spFindPlayerByPlayerID", BRDB_PARAM_4 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB, QueryFindPlayerByPlayerID);



	class QueryGetPlayerShardID : public QueryBase
	{
	public:
		INT64 UserID;
		INT32 ShardID;
		INT32 Result;

	public:
		BRDB_BEGIN_PARAM_MAP(QueryGetPlayerShardID, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserID)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(ShardID)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spGetPlayerShardID", BRDB_PARAM_3 )
	};

	BRDB_DEFINE_QUERYCLASS(POLICY_ACCOUNTDB, QueryGetPlayerShardID);

}  // namespace DB
}  // namespace BR
