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

#include "SFTypedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Protocol.h"
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
			enum { ID_POLICY = PROTOCOLID_SVR_PARTYMATCHING };

			// Constructor
			ISvrPolicyPartyMatching( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// S2C: 
			virtual Result PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount ) = 0;
			// S2C: 
			virtual Result PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole ) = 0;

		}; // class ISvrPolicyPartyMatching : public Net::INetPolicy


		class IPolicyPartyMatching : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_PARTYMATCHING };

			// Constructor
			IPolicyPartyMatching( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// S2C: 
			// S2C: 

		}; // class IPolicyPartyMatching : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


