////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMasterServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyGameMasterServer : public IPolicyGameMasterServer
		{
 			public:
			// Constructor
			NetPolicyGameMasterServer ( Net::Connection* pConn ) : IPolicyGameMasterServer(pConn)
			{}

			// C2S: Player entered
			Result PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName );
			// C2S: Player leaved
			Result PlayerLeavedC2SEvt( const AccountID &InPlayerID );

		}; // class NetPolicyGameMasterServer : public IPolicyGameMasterServer


		class NetSvrPolicyGameMasterServer : public ISvrPolicyGameMasterServer
		{
 			public:
			// Constructor
			NetSvrPolicyGameMasterServer ( Net::Connection* pConn ) : ISvrPolicyGameMasterServer(pConn)
			{}


		}; // class NetSvrPolicyGameMasterServer : public ISvrPolicyGameMasterServer


	}; // namespace Policy
}; // namespace BR


