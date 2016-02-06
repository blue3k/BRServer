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
			HRESULT CreatePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator );
			// C2S: Party instance notify of deletion
			HRESULT PartyDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount );

		}; // class NetPolicyGamePartyManager : public IPolicyGamePartyManager


		class NetSvrPolicyGamePartyManager : public ISvrPolicyGamePartyManager
		{
 			public:
			// Constructor
			NetSvrPolicyGamePartyManager ( Net::IConnection* pConn ) : ISvrPolicyGamePartyManager(pConn)
			{}

			// Cmd: Create a party instance
			HRESULT CreatePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );

		}; // class NetSvrPolicyGamePartyManager : public ISvrPolicyGamePartyManager


	}; // namespace Policy
}; // namespace BR


