////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/PartyMatchingIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyPartyMatching : public IPolicyPartyMatching
		{
 			public:
			// Constructor
			NetPolicyPartyMatching ( Net::IConnection* pConn ) : IPolicyPartyMatching(pConn)
			{}


		}; // class NetPolicyPartyMatching : public IPolicyPartyMatching


		class NetSvrPolicyPartyMatching : public ISvrPolicyPartyMatching
		{
 			public:
			// Constructor
			NetSvrPolicyPartyMatching ( Net::IConnection* pConn ) : ISvrPolicyPartyMatching(pConn)
			{}

			// S2C: 
			HRESULT PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount );
			// S2C: 
			HRESULT PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole );

		}; // class NetSvrPolicyPartyMatching : public ISvrPolicyPartyMatching


	}; // namespace Policy
}; // namespace BR


