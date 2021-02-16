
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
#include "Util/SFStrUtil.h"

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
			SFUniquePtr<QueryFacebookCreateUserCmd> pQuery;
			ScopeContext hr;

			pQuery.reset(new(GetHeap()) QueryFacebookCreateUserCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->FBUserID = facebookUID;
			pQuery->EMail = EMail;
			pQuery->CellPhone = cellPhone;
			pQuery->AccountID = 0;
			pQuery->ShardID = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}

		Result AccountDB::FacebookLogIn(TransactionID Sender, uint64_t facebookUID)
		{
			SFUniquePtr<QueryFacebookLoginCmd> pQuery;
			ScopeContext hr;

			pQuery.reset(new(GetHeap()) QueryFacebookLoginCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->FBUserID = facebookUID;
			pQuery->AccountID = 0;
			pQuery->EMail = 0;
			pQuery->CellPhone = 0;
			pQuery->GCMKeys = 0;
			pQuery->ShardID = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}


		Result AccountDB::CreateUser(TransactionID Sender, const char* UserName, const char* Password)
		{
			ScopeContext hr;

			SFUniquePtr<QueryCreateUserCmd> pQuery(new(GetHeap()) QueryCreateUserCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->UserName = UserName;
			pQuery->Password = Password;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}

		Result AccountDB::LogIn(TransactionID Sender, const char* UserName, const char* Password)
		{
			ScopeContext hr;

			SFUniquePtr<QueryLoginCmd> pQuery(new(GetHeap()) QueryLoginCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->UserName = UserName;
			pQuery->Password = Password;
			pQuery->AccountID = 0;
			pQuery->FBUserID = 0;
			pQuery->ShardID = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}

		Result AccountDB::LogOut(TransactionID Sender)
		{
			ScopeContext hr;

			SFUniquePtr<QueryLogoutCmd> pQuery(new(GetHeap()) QueryLogoutCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}


		Result AccountDB::CreateRandomUser(TransactionID Sender, const char* userName, const char* cellPhone)
		{
			ScopeContext hr;

			SFUniquePtr<QueryCreateRandomUserCmd> pQuery(new(GetHeap()) QueryCreateRandomUserCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->UserName = userName;
			pQuery->CellPhone = cellPhone;
			pQuery->AccountID = 0;
			pQuery->FBUserID = 0;
			pQuery->ShardID = 0;
			pQuery->Result = 0;

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}

		Result AccountDB::UserList(TransactionID Sender)
		{
			ScopeContext hr;

			SFUniquePtr<QueryUserListCmd> pQuery(new(GetHeap()) QueryUserListCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetTransaction(Sender);

			dbCheck(RequestQuery(pQuery));
			pQuery = nullptr;

			return hr;
		}

		Result AccountDB::UpdateGCMKeys(TransactionID Sender, AccountID accountID, const char* strGCMKeys)
		{
			ScopeContext hr;

			SFUniquePtr<QueryUpdateGCMKeysCmd> pQuery(new(GetHeap()) QueryUpdateGCMKeysCmd(GetHeap()));
			dbCheckMem(pQuery);

			dbCheckPtr(strGCMKeys);

			pQuery->SetTransaction(Sender);
			pQuery->UserUID = accountID;
			pQuery->GCMKeys = strGCMKeys;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}


		Result AccountDB::UpdateUserContactInfo(TransactionID Sender, AccountID accountID, const char* strEMail, const char* strCellPhone)
		{
			ScopeContext hr;

			SFUniquePtr<QueryUpdateUserContactInfoCmd> pQuery(new(GetHeap()) QueryUpdateUserContactInfoCmd(GetHeap()));
			dbCheckMem(pQuery);

			dbCheckPtr(strEMail);

			pQuery->SetTransaction(Sender);
			pQuery->UserUID = accountID;
			dbCheck(StrUtil::StringCopy(pQuery->EMail, strEMail));
			dbCheck(StrUtil::StringCopy(pQuery->CellPhone, strCellPhone));

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		// Find player
		Result AccountDB::FindPlayerByEMail(TransactionID Sender, const char* email)
		{
			ScopeContext hr;

			SFUniquePtr<QueryFindPlayerByEMailCmd> pQuery(new(GetHeap()) QueryFindPlayerByEMailCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetTransaction(Sender);
			dbCheck(StrUtil::StringCopy(pQuery->EMail, email));

			pQuery->UserID = 0;
			pQuery->ShardID = 0;
			pQuery->FacebookUID = 0;

			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

		Result AccountDB::FindPlayerByPlayerID(TransactionID Sender, AccountID accountID)
		{
			ScopeContext hr;

			SFUniquePtr<QueryFindPlayerByPlayerIDCmd> pQuery(new(GetHeap()) QueryFindPlayerByPlayerIDCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetTransaction(Sender);

			pQuery->PlayerID = accountID;
			pQuery->ShardID = 0;
			pQuery->FacebookUID = 0;

			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}


		// Player shard id
		Result AccountDB::GetPlayerShardID(TransactionID Sender, AccountID accountID)
		{
			ScopeContext hr;

			SFUniquePtr<QueryGetPlayerShardIDCmd> pQuery(new(GetHeap()) QueryGetPlayerShardIDCmd(GetHeap()));
			dbCheckMem(pQuery);

			pQuery->SetTransaction(Sender);

			pQuery->UserID = accountID;
			pQuery->ShardID = 0;
			pQuery->Result = 0;

			dbCheck(RequestQuery(pQuery));

			pQuery = nullptr;

			return hr;
		}

	} // namespace DB
} // namespace SF


