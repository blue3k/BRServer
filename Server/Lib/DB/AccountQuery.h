
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
		char UserName[Const::MAX_USERNAME];
		char Password[Const::MAX_PASSWORD];
		char EMail[Const::MAX_USERNAME];

		int32_t Result;

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

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryCreateUser);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryLogin class
	//

	class QueryLogin : public QueryBase
	{
	public:
		char UserName[Const::MAX_USERNAME];
		char Password[Const::MAX_PASSWORD];

		int64_t AccountID;
		int64_t FBUserID;
		int32_t ShardID;
		int32_t Result;

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

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryLogin);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryLogin class
	//

	class QueryLogout : public QueryBase
	{
	public:
		char UserName[Const::MAX_USERNAME];
		uint8_t LogoutTime[4];
		int32_t Result;


	public:
		BRDB_BEGIN_PARAM_MAP(QueryLogout, 2)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)	
			BRDB_COLUMN_ENTRY(UserName)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_OUTPUT)
			BRDB_COLUMN_ENTRY(Result)
		BRDB_END_PARAM_MAP()

		BRDB_QUERYSTRING( "spLogout", BRDB_PARAM_2 )
	};

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryLogout);



	struct QueryUserListResultSet
	{
		int64_t m_PlayerUID;
		char m_Name[BINSIZE_NAME];
		int64_t m_FBUID;
	};

	class QueryUserList : public QueryUserListResultSet, public QueryBase
	{
	public:
		int32_t Result;


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

	BRDB_DEFINE_ROWSETQUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryUserList, QueryUserListResultSet);

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryFacebookCreateUser class
	//

	class QueryFacebookCreateUser : public QueryBase
	{
	public:
		int64_t FBUserID;
		char EMail[Const::MAX_EMAIL];
		char CellPhone[Const::MAX_CELLPHONE];

		int64_t AccountID;
		int32_t ShardID;
		int32_t Result;


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
		char EMail[Const::MAX_EMAIL];
		char CellPhone[Const::MAX_CELLPHONE];
		char GCMKeys[Const::MAX_GCMKEY];
		int32_t ShardID;
		int32_t Result;

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

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryFacebookLogin);



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryCreateRandomUser class
	//

	class QueryCreateRandomUser : public QueryBase
	{
	public:
		char UserName[Const::MAX_USERNAME];
		char CellPhone[Const::MAX_CELLPHONE];

		int64_t AccountID;
		int64_t FBUserID;
		int32_t ShardID;
		int32_t Result;


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

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB, QueryCreateRandomUser);

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryFacebookCreateUser class
	//

	class QueryUpdateGCMKeys : public QueryBase
	{
	public:
		int64_t UserUID;
		char GCMKeys[Const::MAX_GCMKEY];

		int32_t Result;

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
		BRDB_BEGIN_PARAM_MAP(QueryUpdateUserContactInfo, 3)
			BRDB_SET_PARAM_TYPE(BRDB_PARAMIO_INPUT)
			BRDB_COLUMN_ENTRY(UserUID)
			BRDB_COLUMN_ENTRY(EMail)
			BRDB_COLUMN_ENTRY(CellPhone)
			BRDB_END_PARAM_MAP()

			BRDB_QUERYSTRING("spUpdateUserContactInfo", BRDB_PARAM_3)
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

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB,QueryFindPlayerByEMail);




	class QueryFindPlayerByPlayerID : public QueryBase
	{
	public:
		int64_t PlayerID;
		int64_t FacebookUID;
		int32_t ShardID;
		int32_t Result;

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

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB, QueryFindPlayerByPlayerID);



	class QueryGetPlayerShardID : public QueryBase
	{
	public:
		int64_t UserID;
		int32_t ShardID;
		int32_t Result;

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

	BRDB_DEFINE_QUERYCLASS(PROTOCOLID_ACCOUNTDB, QueryGetPlayerShardID);

}  // namespace DB
}  // namespace SF
