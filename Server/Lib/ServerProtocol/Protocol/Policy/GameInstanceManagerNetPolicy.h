////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug definitions
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
 		class NetPolicyGameInstanceManager 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyGameInstanceManager ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Create a game instance
			Result CreateGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer );
			// C2S: Game instance notify of deletion
			Result GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

		}; // class NetPolicyGameInstanceManager 


		class NetSvrPolicyGameInstanceManager
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyGameInstanceManager ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Create a game instance
			Result CreateGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyGameInstanceManager


	}; // namespace Policy
}; // namespace SF


