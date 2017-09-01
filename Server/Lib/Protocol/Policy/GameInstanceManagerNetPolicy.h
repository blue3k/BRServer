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
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameInstanceManagerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyGameInstanceManager : public IPolicyGameInstanceManager
		{
 			public:
			// Constructor
			NetPolicyGameInstanceManager ( Net::IConnection* pConn ) : IPolicyGameInstanceManager(pConn)
			{}

			// Cmd: Create a game instance
			Result CreateGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer );
			// C2S: Game instance notify of deletion
			Result GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

		}; // class NetPolicyGameInstanceManager : public IPolicyGameInstanceManager


		class NetSvrPolicyGameInstanceManager : public ISvrPolicyGameInstanceManager
		{
 			public:
			// Constructor
			NetSvrPolicyGameInstanceManager ( Net::IConnection* pConn ) : ISvrPolicyGameInstanceManager(pConn)
			{}

			// Cmd: Create a game instance
			Result CreateGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyGameInstanceManager : public ISvrPolicyGameInstanceManager


	}; // namespace Policy
}; // namespace BR


