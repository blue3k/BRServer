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

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
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
			HRESULT CreateGameCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer );
			// C2S: Game instance notify of deletion
			HRESULT GameDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount );

		}; // class NetPolicyGameInstanceManager : public IPolicyGameInstanceManager


		class NetSvrPolicyGameInstanceManager : public ISvrPolicyGameInstanceManager
		{
 			public:
			// Constructor
			NetSvrPolicyGameInstanceManager ( Net::IConnection* pConn ) : ISvrPolicyGameInstanceManager(pConn)
			{}

			// Cmd: Create a game instance
			HRESULT CreateGameRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult );

		}; // class NetSvrPolicyGameInstanceManager : public ISvrPolicyGameInstanceManager


	}; // namespace Policy
}; // namespace BR


