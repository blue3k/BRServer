
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "DB/AccountDB.h"

#include "DB/AccountQuery.h"



namespace BR {
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


	/////////////////////////////////////////////////////////////////////////////////
	//
	//	Account DB interface
	//

	HRESULT AccountDB::FacebookCreateUser( BR::TransactionID Sender, UINT64 facebookUID, const char* EMail )
	{
		HRESULT hr = S_OK;
		QueryFacebookCreateUserCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryFacebookCreateUserCmd );

		pQuery->FBUserID = facebookUID;
		StrUtil::StringCpy( pQuery->EMail, EMail );
		pQuery->AccountID = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	HRESULT AccountDB::FacebookLogIn( BR::TransactionID Sender, UINT64 facebookUID )
	{
		HRESULT hr = S_OK;
		QueryFacebookLoginCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryFacebookLoginCmd );

		pQuery->FBUserID = facebookUID;
		pQuery->AccountID = 0;
		pQuery->EMail[0] = 0;
		pQuery->GCMKeys[0] = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	
	HRESULT AccountDB::CreateUser( BR::TransactionID Sender, const char* UserName, const char* Password)
	{
		HRESULT hr = S_OK;
		QueryCreateUserCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryCreateUserCmd );

		StrUtil::StringCpy( pQuery->UserName, UserName );
		StrUtil::StringCpy( pQuery->Password, Password );
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	HRESULT AccountDB::LogIn( BR::TransactionID Sender, const char* UserName, const char* Password)
	{
		HRESULT hr = S_OK;
		QueryLoginCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryLoginCmd );

		StrUtil::StringCpy( pQuery->UserName, UserName );
		StrUtil::StringCpy( pQuery->Password, Password );
		pQuery->AccountID = 0;
		pQuery->FBUserID = 0;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	HRESULT AccountDB::LogOut( BR::TransactionID Sender)
	{
		HRESULT hr = S_OK;
		QueryLogoutCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryLogoutCmd );

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}
	
	HRESULT AccountDB::UserList( BR::TransactionID Sender)
	{
		HRESULT hr = S_OK;
		QueryUserListCmd *pQuery = nullptr;

		dbMem( pQuery = new QueryUserListCmd );

		pQuery->SetTransaction( Sender );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	HRESULT AccountDB::UpdateGCMKeys( BR::TransactionID Sender, AccountID accountID, const char* strGCMKeys )
	{
		HRESULT hr = S_OK;
		QueryUpdateGCMKeysCmd *pQuery = nullptr;

		dbChkPtr( strGCMKeys );

		dbMem( pQuery = new QueryUpdateGCMKeysCmd );

		pQuery->SetTransaction( Sender );
		pQuery->UserUID = accountID;
		StrUtil::StringCpy( pQuery->GCMKeys, strGCMKeys );

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}


	HRESULT AccountDB::UpdateUserEMail(BR::TransactionID Sender, AccountID accountID, const char* strEMail)
	{
		HRESULT hr = S_OK;
		QueryUpdateUserEMailCmd *pQuery = nullptr;

		dbChkPtr(strEMail);

		dbMem(pQuery = new QueryUpdateUserEMailCmd);

		pQuery->SetTransaction( Sender );
		pQuery->UserUID = accountID;
		StrUtil::StringCpy(pQuery->EMail, strEMail);

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

	// Find player
	HRESULT AccountDB::FindPlayerByEMail( BR::TransactionID Sender, const char* email )
	{
		HRESULT hr = S_OK;
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

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}


	// Player shard id
	HRESULT AccountDB::GetPlayerShardID(BR::TransactionID Sender, AccountID accountID)
	{
		HRESULT hr = S_OK;
		QueryGetPlayerShardIDCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryGetPlayerShardIDCmd);

		pQuery->SetTransaction( Sender );

		pQuery->UserID = accountID;
		pQuery->ShardID = 0;
		pQuery->Result = 0;

		dbChk( RequestQuery( pQuery ) );

		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}

} // namespace DB
} // namespace BR


