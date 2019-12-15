
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

		dbMem( pQuery = new(GetHeap()) QueryFacebookCreateUserCmd(GetHeap()) );

		pQuery->FBUserID = facebookUID;
		pQuery->EMail = EMail;
		pQuery->CellPhone = cellPhone;
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

		dbMem( pQuery = new(GetHeap()) QueryFacebookLoginCmd(GetHeap()) );

		pQuery->FBUserID = facebookUID;
		pQuery->AccountID = 0;
		pQuery->EMail = 0;
		pQuery->CellPhone = 0;
		pQuery->GCMKeys = 0;
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

		dbMem( pQuery = new(GetHeap()) QueryCreateUserCmd(GetHeap()));

		pQuery->UserName = UserName;
		pQuery->Password = Password;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		delete pQuery;

		return hr;
	}

	Result AccountDB::LogIn( TransactionID Sender, const char* UserName, const char* Password)
	{
		Result hr = ResultCode::SUCCESS;
		QueryLoginCmd *pQuery = nullptr;

		dbMem( pQuery = new(GetHeap()) QueryLoginCmd(GetHeap()));

		pQuery->UserName, UserName;
		pQuery->Password, Password;
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

		dbMem( pQuery = new(GetHeap()) QueryLogoutCmd(GetHeap()));

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

		dbMem(pQuery = new(GetHeap()) QueryCreateRandomUserCmd(GetHeap()));

		pQuery->UserName, userName;
		pQuery->CellPhone, cellPhone;
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

		dbMem( pQuery = new(GetHeap()) QueryUserListCmd(GetHeap()));

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

		dbMem( pQuery = new(GetHeap()) QueryUpdateGCMKeysCmd(GetHeap()));

		pQuery->SetTransaction( Sender );
		pQuery->UserUID = accountID;
		pQuery->GCMKeys, strGCMKeys;

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

		dbMem(pQuery = new(GetHeap()) QueryUpdateUserContactInfoCmd(GetHeap()));

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

		dbMem( pQuery = new(GetHeap()) QueryFindPlayerByEMailCmd(GetHeap()));

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

		dbMem(pQuery = new(GetHeap()) QueryFindPlayerByPlayerIDCmd(GetHeap()));

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

		dbMem(pQuery = new(GetHeap()) QueryGetPlayerShardIDCmd(GetHeap()));

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


