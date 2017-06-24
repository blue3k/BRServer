////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Login Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/LoginIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyLogin : public IPolicyLogin
		{
 			public:
			// Constructor
			NetPolicyLogin ( Net::IConnection* pConn ) : IPolicyLogin(pConn)
			{}

			// Cmd: Login request
			Result LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword );
			// Cmd: Login request with Facebook UID
			Result LoginByFacebookCmd( const GameID &InGameID, const uint64_t &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken );
			// Cmd: Login request
			Result CreateRandomUserCmd( const GameID &InGameID, const char* InCellPhone );

		}; // class NetPolicyLogin : public IPolicyLogin


		class NetSvrPolicyLogin : public ISvrPolicyLogin
		{
 			public:
			// Constructor
			NetSvrPolicyLogin ( Net::IConnection* pConn ) : ISvrPolicyLogin(pConn)
			{}

			// Cmd: Login request
			Result LoginRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID );
			// Cmd: Login request with Facebook UID
			Result LoginByFacebookRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID );
			// Cmd: Login request
			Result CreateRandomUserRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID );

		}; // class NetSvrPolicyLogin : public ISvrPolicyLogin


	}; // namespace Policy
}; // namespace BR


