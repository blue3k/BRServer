////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : LoginServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/LoginServerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyLoginServer : public IPolicyLoginServer
		{
 			public:
			// Constructor
			NetPolicyLoginServer ( Net::Connection* pConn ) : IPolicyLoginServer(pConn)
			{}

			// Cmd: Notify user joind and see it's valid authticket instance
			Result PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );
			// Cmd: Kick logged in player
			Result KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID );

		}; // class NetPolicyLoginServer : public IPolicyLoginServer


		class NetSvrPolicyLoginServer : public ISvrPolicyLoginServer
		{
 			public:
			// Constructor
			NetSvrPolicyLoginServer ( Net::Connection* pConn ) : ISvrPolicyLoginServer(pConn)
			{}

			// Cmd: Notify user joind and see it's valid authticket instance
			Result PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: Kick logged in player
			Result KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyLoginServer : public ISvrPolicyLoginServer


	}; // namespace Policy
}; // namespace BR


