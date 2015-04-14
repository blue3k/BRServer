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
#include "Net/Netdef.h"
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
			virtual HRESULT LoginRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID ) = 0;
			// Cmd: Login request with Facebook UID
			virtual HRESULT LoginByFacebookRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID ) = 0;

		}; // class ISvrPolicyLogin : public Net::INetPolicy


		class IPolicyLogin : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_LOGIN };

			// Constructor
			IPolicyLogin( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Login request
			virtual HRESULT LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword ) = 0;
			// Cmd: Login request with Facebook UID
			virtual HRESULT LoginByFacebookCmd( const GameID &InGameID, const UINT64 &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken ) = 0;

		}; // class IPolicyLogin : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


