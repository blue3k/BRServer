﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyGamePartyManager : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_SVR_GAMEPARTYMANAGER };

			// Constructor
			ISvrPolicyGamePartyManager( Net::Connection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Create a party instance
			virtual Result CreatePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// C2S: Party instance notify of deletion

		}; // class ISvrPolicyGamePartyManager : public Net::INetPolicy


		class IPolicyGamePartyManager : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_GAMEPARTYMANAGER };

			// Constructor
			IPolicyGamePartyManager( Net::Connection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Create a party instance
			virtual Result CreatePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerInformation &InCreator ) = 0;
			// C2S: Party instance notify of deletion
			virtual Result PartyDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount ) = 0;

		}; // class IPolicyGamePartyManager : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


