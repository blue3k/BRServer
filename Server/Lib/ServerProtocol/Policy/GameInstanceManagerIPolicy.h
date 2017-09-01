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
 
		class ISvrPolicyGameInstanceManager : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_SVR_GAMEINSTANCEMANAGER };

			// Constructor
			ISvrPolicyGameInstanceManager( Net::Connection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Create a game instance
			virtual Result CreateGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// C2S: Game instance notify of deletion

		}; // class ISvrPolicyGameInstanceManager : public Net::INetPolicy


		class IPolicyGameInstanceManager : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_GAMEINSTANCEMANAGER };

			// Constructor
			IPolicyGameInstanceManager( Net::Connection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Create a game instance
			virtual Result CreateGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint16_t &InNumberOfBotPlayer, const uint16_t &InMaxPlayer ) = 0;
			// C2S: Game instance notify of deletion
			virtual Result GameDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount ) = 0;

		}; // class IPolicyGameInstanceManager : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


