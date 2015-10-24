
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : AccountDB Interface
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrCommonTypes.h"
#include "DB/QueryManager.h"
#include "Common/SvrPolicyID.h"
#include "ServerSystem/ServerComponent.h"


namespace BR {
namespace DB {



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	AccountDBServer Class 
	//

	class AccountDB : protected QueryManager, public Svr::IServerComponent
	{
	public:

		enum {
			ComponentID = Svr::ServerComponentID_AccountDB,
		};

	private:

	public:
		// constructor / destructor
		AccountDB();
		virtual ~AccountDB();

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Account DB interface
		//

		HRESULT FacebookCreateUser(BR::TransactionID Sender, UINT64 facebookUID, const char* EMail, const char* cellPhone);
		HRESULT FacebookLogIn( BR::TransactionID Sender, UINT64 facebookUID );

		HRESULT CreateUser( BR::TransactionID Sender, const char* UserName, const char* Password );
		HRESULT LogIn( BR::TransactionID Sender, const char* UserName, const char* Password );
		HRESULT LogOut( BR::TransactionID Sender);

		HRESULT CreateRandomUser(BR::TransactionID Sender, const char* userName, const char* cellPhone);

		HRESULT UserList( BR::TransactionID Sender);

		HRESULT UpdateGCMKeys( BR::TransactionID Sender, AccountID accountID, const char* strGCMKeys );
		HRESULT UpdateUserContactInfo(BR::TransactionID Sender, AccountID accountID, const char* strEMail, const char* strCellPhone);

		// Find player
		HRESULT FindPlayerByEMail( BR::TransactionID Sender, const char* email );
		HRESULT FindPlayerByPlayerID(BR::TransactionID Sender, AccountID accountID);

		// Player shard id
		HRESULT GetPlayerShardID(BR::TransactionID Sender, AccountID accountID);
	};


} // namespace DB
} // namespace BR
