
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

		HRESULT FacebookCreateUser(TransactionID Sender, UINT64 facebookUID, const char* EMail, const char* cellPhone);
		HRESULT FacebookLogIn( TransactionID Sender, UINT64 facebookUID );

		HRESULT CreateUser( TransactionID Sender, const char* UserName, const char* Password );
		HRESULT LogIn( TransactionID Sender, const char* UserName, const char* Password );
		HRESULT LogOut( TransactionID Sender);

		HRESULT CreateRandomUser(TransactionID Sender, const char* userName, const char* cellPhone);

		HRESULT UserList( TransactionID Sender);

		HRESULT UpdateGCMKeys( TransactionID Sender, AccountID accountID, const char* strGCMKeys );
		HRESULT UpdateUserContactInfo(TransactionID Sender, AccountID accountID, const char* strEMail, const char* strCellPhone);

		// Find player
		HRESULT FindPlayerByEMail( TransactionID Sender, const char* email );
		HRESULT FindPlayerByPlayerID(TransactionID Sender, AccountID accountID);

		// Player shard id
		HRESULT GetPlayerShardID(TransactionID Sender, AccountID accountID);
	};


} // namespace DB
} // namespace BR
