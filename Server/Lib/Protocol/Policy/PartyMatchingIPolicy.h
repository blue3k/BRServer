////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyPartyMatching : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_PARTYMATCHING };

			// Constructor
			ISvrPolicyPartyMatching( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// S2C: 
			virtual HRESULT PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount ) = 0;
			// S2C: 
			virtual HRESULT PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole ) = 0;

		}; // class ISvrPolicyPartyMatching : public Net::INetPolicy


		class IPolicyPartyMatching : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_PARTYMATCHING };

			// Constructor
			IPolicyPartyMatching( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// S2C: 
			// S2C: 

		}; // class IPolicyPartyMatching : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


