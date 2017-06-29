////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GamePartyManagerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyGamePartyManager : public IPolicyGamePartyManager
		{
 			public:
			// Constructor
			NetPolicyGamePartyManager ( Net::IConnection* pConn ) : IPolicyGamePartyManager(pConn)
			{}

			// Cmd: Create a party instance
			Result CreatePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerInformation &InCreator );
			// C2S: Party instance notify of deletion
			Result PartyDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

		}; // class NetPolicyGamePartyManager : public IPolicyGamePartyManager


		class NetSvrPolicyGamePartyManager : public ISvrPolicyGamePartyManager
		{
 			public:
			// Constructor
			NetSvrPolicyGamePartyManager ( Net::IConnection* pConn ) : ISvrPolicyGamePartyManager(pConn)
			{}

			// Cmd: Create a party instance
			Result CreatePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyGamePartyManager : public ISvrPolicyGamePartyManager


	}; // namespace Policy
}; // namespace BR


