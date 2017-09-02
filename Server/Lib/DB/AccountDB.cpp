
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SFTypedefs.h"
#include "String/StrUtil.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"



namespace SF {
namespace DB { 


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	AccountDB Class 
	//
	
	// constructor / destructor
	AccountDB::AccountDB()
		:Svr::IServerComponent(ComponentID)
	{
	}


	AccountDB::~AccountDB()
	{
	}

	void AccountDB::TerminateComponent()
	{
		TerminateDB();
		Svr::IServerComponent::TerminateComponent();
	}


	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Account DB interface
	//

	Result AccountDB::FacebookCreateUser(TransactionID Sender, uint64_t facebookUID, const char* EMail, const char* cellPhone)
	{
		Result hr = ResultCode::SUCCESS;
		QueryFacebookCreateUserCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryFacebookCreateUserCmd );

		pQuery->FBUserID = facebookUID;
		dbChk(StrUtil::StringCpy(pQuery->EMail, EMail));
		dbChk(StrUtil::StringCpy(pQuery->CellPhone, cellPhone));
		pQuery->AccountID = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	Result AccountDB::FacebookLogIn( TransactionID Sender, uint64_t facebookUID )
	{
		Result hr = ResultCode::SUCCESS;
		QueryFacebookLoginCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryFacebookLoginCmd );

		pQuery->FBUserID = facebookUID;
		pQuery->AccountID = 0;
		pQuery->EMail[0] = 0;
		pQuery->CellPhone[0] = 0;
		pQuery->GCMKeys[0] = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	
	Result AccountDB::CreateUser( TransactionID Sender, const char* UserName, const char* Password)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCreateUserCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryCreateUserCmd );

		dbChk(StrUtil::StringCpy(pQuery->UserName, UserName));
		dbChk(StrUtil::StringCpy(pQuery->Password, Password));
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	Result AccountDB::LogIn( TransactionID Sender, const char* UserName, const char* Password)
	{
		Result hr = ResultCode::SUCCESS;
		QueryLoginCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryLoginCmd );

		dbChk(StrUtil::StringCpy(pQuery->UserName, UserName));
		dbChk(StrUtil::StringCpy(pQuery->Password, Password));
		pQuery->AccountID = 0;
		pQuery->FBUserID = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	Result AccountDB::LogOut( TransactionID Sender)
	{
		Result hr = ResultCode::SUCCESS;
		QueryLogoutCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryLogoutCmd );

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}
	

	Result AccountDB::CreateRandomUser(TransactionID Sender, const char* userName, const char* cellPhone)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCreateRandomUserCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryCreateRandomUserCmd);

		dbChk(StrUtil::StringCpy(pQuery->UserName, userName));
		dbChk(StrUtil::StringCpy(pQuery->CellPhone, cellPhone));
		pQuery->AccountID = 0;
		pQuery->FBUserID = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction(Sender);

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	Result AccountDB::UserList( TransactionID Sender)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUserListCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryUserListCmd );

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	Result AccountDB::UpdateGCMKeys( TransactionID Sender, AccountID accountID, const char* strGCMKeys )
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateGCMKeysCmd *pQuery = nullptr;

		dbChkPtr( strGCMKeys );

		dbMem( pQuery = new QueryUpdateGCMKeysCmd );

		pQuery->SetTransaction( Sender );
		pQuery->UserUID = accountID;
		dbChk(StrUtil::StringCpy(pQuery->GCMKeys, strGCMKeys));

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}


	Result AccountDB::UpdateUserContactInfo(TransactionID Sender, AccountID accountID, const char* strEMail, const char* strCellPhone)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateUserContactInfoCmd *pQuery = nullptr;

		dbChkPtr(strEMail);

		dbMem(pQuery = new QueryUpdateUserContactInfoCmd);

		pQuery->SetTransaction( Sender );
		pQuery->UserUID = accountID;
		dbChk(StrUtil::StringCpy(pQuery->EMail, strEMail));
		dbChk(StrUtil::StringCpy(pQuery->CellPhone, strCellPhone));

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	// Find player
	Result AccountDB::FindPlayerByEMail( TransactionID Sender, const char* email )
	{
		Result hr = ResultCode::SUCCESS;
		QueryFindPlayerByEMailCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryFindPlayerByEMailCmd );

		pQuery->SetTransaction( Sender );
		dbChk( StrUtil::StringCpy( pQuery->EMail, email ) );

		pQuery->UserID = 0;
		pQuery->ShardID = 0;
		pQuery->FacebookUID = 0;

		pQuery->Result = 0;

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	Result AccountDB::FindPlayerByPlayerID(TransactionID Sender, AccountID accountID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryFindPlayerByPlayerIDCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryFindPlayerByPlayerIDCmd);

		pQuery->SetTransaction(Sender);

		pQuery->PlayerID = accountID;
		pQuery->ShardID = 0;
		pQuery->FacebookUID = 0;

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		if (!(hr))
			Util::SafeRelease(pQuery);

		return hr;
	}


	// Player shard id
	Result AccountDB::GetPlayerShardID(TransactionID Sender, AccountID accountID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetPlayerShardIDCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetPlayerShardIDCmd);

		pQuery->SetTransaction( Sender );

		pQuery->UserID = accountID;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		if( !(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

} // namespace DB
} // namespace SF


