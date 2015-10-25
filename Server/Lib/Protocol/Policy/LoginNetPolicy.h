﻿////////////////////////////////////////////////////////////////////////////////
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
#include "Net/Netdef.h"
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
			HRESULT LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword );
			// Cmd: Login request with Facebook UID
			HRESULT LoginByFacebookCmd( const GameID &InGameID, const UINT64 &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken );
			// Cmd: Login request
			HRESULT CreateRandomUserCmd( const GameID &InGameID, const char* InCellPhone );

		}; // class NetPolicyLogin : public IPolicyLogin


		class NetSvrPolicyLogin : public ISvrPolicyLogin
		{
 			public:
			// Constructor
			NetSvrPolicyLogin ( Net::IConnection* pConn ) : ISvrPolicyLogin(pConn)
			{}

			// Cmd: Login request
			HRESULT LoginRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );
			// Cmd: Login request with Facebook UID
			HRESULT LoginByFacebookRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );
			// Cmd: Login request
			HRESULT CreateRandomUserRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );

		}; // class NetSvrPolicyLogin : public ISvrPolicyLogin


	}; // namespace Policy
}; // namespace BR

