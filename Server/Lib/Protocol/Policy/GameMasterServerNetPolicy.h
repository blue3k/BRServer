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

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
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
			NetPolicyGameMasterServer ( Net::IConnection* pConn ) : IPolicyGameMasterServer(pConn)
			{}

			// C2S: Player entered
			HRESULT PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName );
			// C2S: Player leaved
			HRESULT PlayerLeavedC2SEvt( const AccountID &InPlayerID );

		}; // class NetPolicyGameMasterServer : public IPolicyGameMasterServer


		class NetSvrPolicyGameMasterServer : public ISvrPolicyGameMasterServer
		{
 			public:
			// Constructor
			NetSvrPolicyGameMasterServer ( Net::IConnection* pConn ) : ISvrPolicyGameMasterServer(pConn)
			{}


		}; // class NetSvrPolicyGameMasterServer : public ISvrPolicyGameMasterServer


	}; // namespace Policy
}; // namespace BR


