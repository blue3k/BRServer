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



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyGameMasterServer 
		{
 			private:
				Net::Connection* m_pConnection;
			public:
			// Constructor
			NetPolicyGameMasterServer ( Net::Connection* pConn ) : m_pConnection(pConn)
			{}

			// C2S: Player entered
			Result PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName );
			// C2S: Player leaved
			Result PlayerLeavedC2SEvt( const AccountID &InPlayerID );

		}; // class NetPolicyGameMasterServer 


		class NetSvrPolicyGameMasterServer
		{
 			private:
				Net::Connection* m_pConnection;
			public:
			// Constructor
			NetSvrPolicyGameMasterServer ( Net::Connection* pConn ) : m_pConnection(pConn)
			{}


		}; // class NetSvrPolicyGameMasterServer


	}; // namespace Policy
}; // namespace SF


