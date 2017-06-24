////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : Login Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrBaseTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyLogin : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_LOGIN };

			// Constructor
			ISvrPolicyLogin( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Login request
			virtual Result LoginRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID ) = 0;
			// Cmd: Login request with Facebook UID
			virtual Result LoginByFacebookRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID ) = 0;
			// Cmd: Login request
			virtual Result CreateRandomUserRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID ) = 0;

		}; // class ISvrPolicyLogin : public Net::INetPolicy


		class IPolicyLogin : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_LOGIN };

			// Constructor
			IPolicyLogin( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Login request
			virtual Result LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword ) = 0;
			// Cmd: Login request with Facebook UID
			virtual Result LoginByFacebookCmd( const GameID &InGameID, const uint64_t &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken ) = 0;
			// Cmd: Login request
			virtual Result CreateRandomUserCmd( const GameID &InGameID, const char* InCellPhone ) = 0;

		}; // class IPolicyLogin : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


