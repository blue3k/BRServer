
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
	

#include "SFTypedefs.h"
#include "Types/BrSvrTypes.h"
#include "DB/DBClusterManager.h"
#include "SvrPolicyID.h"
#include "ServerSystem/ServerComponent.h"


namespace SF {
namespace DB {



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	AccountDBServer Class 
	//

	class AccountDB : protected DBClusterManager, public Svr::IServerComponent
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

		virtual void TerminateComponent() override;

	public:

		/////////////////////////////////////////////////////////////////////////////////
		//
		//	Account DB interface
		//

		Result FacebookCreateUser(TransactionID Sender, uint64_t facebookUID, const char* EMail, const char* cellPhone);
		Result FacebookLogIn( TransactionID Sender, uint64_t facebookUID );

		Result CreateUser( TransactionID Sender, const char* UserName, const char* Password );
		Result LogIn( TransactionID Sender, const char* UserName, const char* Password );
		Result LogOut( TransactionID Sender);

		Result CreateRandomUser(TransactionID Sender, const char* userName, const char* cellPhone);

		Result UserList( TransactionID Sender);

		Result UpdateGCMKeys( TransactionID Sender, AccountID accountID, const char* strGCMKeys );
		Result UpdateUserContactInfo(TransactionID Sender, AccountID accountID, const char* strEMail, const char* strCellPhone);

		// Find player
		Result FindPlayerByEMail( TransactionID Sender, const char* email );
		Result FindPlayerByPlayerID(TransactionID Sender, AccountID accountID);

		// Player shard id
		Result GetPlayerShardID(TransactionID Sender, AccountID accountID);
	};


} // namespace DB
} // namespace SF
