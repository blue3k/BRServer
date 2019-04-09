
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "DBPch.h"
#include "SFTypedefs.h"
#include "String/SFStrUtil.h"

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
	{
	}


	AccountDB::~AccountDB()
	{
	}

	void AccountDB::TerminateComponent()
	{
		TerminateDB();
	}


	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Account DB interface
	//

	Result AccountDB::FacebookCreateUser(TransactionID Sender, uint64_t facebookUID, const char* EMail, const char* cellPhone)
	{
		Result hr = ResultCode::SUCCESS;
		QueryFacebookCreateUserCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryFacebookCreateUserCmd );

		pQuery->FBUserID = facebookUID;
		dbChk(StrUtil::StringCopy(pQuery->EMail, EMail));
		dbChk(StrUtil::StringCopy(pQuery->CellPhone, cellPhone));
		pQuery->AccountID = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result AccountDB::FacebookLogIn( TransactionID Sender, uint64_t facebookUID )
	{
		Result hr = ResultCode::SUCCESS;
		QueryFacebookLoginCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryFacebookLoginCmd );

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

		IHeap::Delete(pQuery);

		return hr;
	}

	
	Result AccountDB::CreateUser( TransactionID Sender, const char* UserName, const char* Password)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCreateUserCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryCreateUserCmd );

		dbChk(StrUtil::StringCopy(pQuery->UserName, UserName));
		dbChk(StrUtil::StringCopy(pQuery->Password, Password));
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result AccountDB::LogIn( TransactionID Sender, const char* UserName, const char* Password)
	{
		Result hr = ResultCode::SUCCESS;
		QueryLoginCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryLoginCmd );

		dbChk(StrUtil::StringCopy(pQuery->UserName, UserName));
		dbChk(StrUtil::StringCopy(pQuery->Password, Password));
		pQuery->AccountID = 0;
		pQuery->FBUserID = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result AccountDB::LogOut( TransactionID Sender)
	{
		Result hr = ResultCode::SUCCESS;
		QueryLogoutCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryLogoutCmd );

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}
	

	Result AccountDB::CreateRandomUser(TransactionID Sender, const char* userName, const char* cellPhone)
	{
		Result hr = ResultCode::SUCCESS;
		QueryCreateRandomUserCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryCreateRandomUserCmd);

		dbChk(StrUtil::StringCopy(pQuery->UserName, userName));
		dbChk(StrUtil::StringCopy(pQuery->CellPhone, cellPhone));
		pQuery->AccountID = 0;
		pQuery->FBUserID = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction(Sender);

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result AccountDB::UserList( TransactionID Sender)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUserListCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryUserListCmd );

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result AccountDB::UpdateGCMKeys( TransactionID Sender, AccountID accountID, const char* strGCMKeys )
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateGCMKeysCmd *pQuery = nullptr;

		dbChkPtr( strGCMKeys );

		dbMem( pQuery = new(GetHeap()) QueryUpdateGCMKeysCmd );

		pQuery->SetTransaction( Sender );
		pQuery->UserUID = accountID;
		dbChk(StrUtil::StringCopy(pQuery->GCMKeys, strGCMKeys));

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	Result AccountDB::UpdateUserContactInfo(TransactionID Sender, AccountID accountID, const char* strEMail, const char* strCellPhone)
	{
		Result hr = ResultCode::SUCCESS;
		QueryUpdateUserContactInfoCmd *pQuery = nullptr;

		dbChkPtr(strEMail);

		dbMem(pQuery = new(GetHeap()) QueryUpdateUserContactInfoCmd);

		pQuery->SetTransaction( Sender );
		pQuery->UserUID = accountID;
		dbChk(StrUtil::StringCopy(pQuery->EMail, strEMail));
		dbChk(StrUtil::StringCopy(pQuery->CellPhone, strCellPhone));

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	// Find player
	Result AccountDB::FindPlayerByEMail( TransactionID Sender, const char* email )
	{
		Result hr = ResultCode::SUCCESS;
		QueryFindPlayerByEMailCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryFindPlayerByEMailCmd );

		pQuery->SetTransaction( Sender );
		dbChk( StrUtil::StringCopy( pQuery->EMail, email ) );

		pQuery->UserID = 0;
		pQuery->ShardID = 0;
		pQuery->FacebookUID = 0;

		pQuery->Result = 0;

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

	Result AccountDB::FindPlayerByPlayerID(TransactionID Sender, AccountID accountID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryFindPlayerByPlayerIDCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryFindPlayerByPlayerIDCmd);

		pQuery->SetTransaction(Sender);

		pQuery->PlayerID = accountID;
		pQuery->ShardID = 0;
		pQuery->FacebookUID = 0;

		pQuery->Result = 0;

		dbChk(RequestQuery(pQuery));

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}


	// Player shard id
	Result AccountDB::GetPlayerShardID(TransactionID Sender, AccountID accountID)
	{
		Result hr = ResultCode::SUCCESS;
		QueryGetPlayerShardIDCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetHeap()) QueryGetPlayerShardIDCmd);

		pQuery->SetTransaction( Sender );

		pQuery->UserID = accountID;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		IHeap::Delete(pQuery);

		return hr;
	}

} // namespace DB
} // namespace SF


