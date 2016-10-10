////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug definitions
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
 
		class ISvrPolicyGameInstanceManager : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_GAMEINSTANCEMANAGER };

			// Constructor
			ISvrPolicyGameInstanceManager( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Create a game instance
			virtual Result CreateGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// C2S: Game instance notify of deletion

		}; // class ISvrPolicyGameInstanceManager : public Net::INetPolicy


		class IPolicyGameInstanceManager : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAMEINSTANCEMANAGER };

			// Constructor
			IPolicyGameInstanceManager( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Create a game instance
			virtual Result CreateGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer ) = 0;
			// C2S: Game instance notify of deletion
			virtual Result GameDeletedC2SEvt( const RouteContext &InRouteContext, const UINT16 &InRouteHopCount ) = 0;

		}; // class IPolicyGameInstanceManager : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


