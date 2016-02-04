////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug definitions
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
 
		class ISvrPolicyGamePartyManager : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_GAMEPARTYMANAGER };

			// Constructor
			ISvrPolicyGamePartyManager( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Create a party instance
			virtual HRESULT CreatePartyRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult ) = 0;
			// C2S: Party instance notify of deletion

		}; // class ISvrPolicyGamePartyManager : public Net::INetPolicy


		class IPolicyGamePartyManager : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAMEPARTYMANAGER };

			// Constructor
			IPolicyGamePartyManager( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Create a party instance
			virtual HRESULT CreatePartyCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator ) = 0;
			// C2S: Party instance notify of deletion
			virtual HRESULT PartyDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount ) = 0;

		}; // class IPolicyGamePartyManager : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


