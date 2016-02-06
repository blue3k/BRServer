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

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
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
			NetPolicyLoginServer ( Net::IConnection* pConn ) : IPolicyLoginServer(pConn)
			{}

			// Cmd: Notify user joind and see it's valid authticket instance
			HRESULT PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );
			// Cmd: Kick logged in player
			HRESULT KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID );

		}; // class NetPolicyLoginServer : public IPolicyLoginServer


		class NetSvrPolicyLoginServer : public ISvrPolicyLoginServer
		{
 			public:
			// Constructor
			NetSvrPolicyLoginServer ( Net::IConnection* pConn ) : ISvrPolicyLoginServer(pConn)
			{}

			// Cmd: Notify user joind and see it's valid authticket instance
			HRESULT PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );
			// Cmd: Kick logged in player
			HRESULT KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );

		}; // class NetSvrPolicyLoginServer : public ISvrPolicyLoginServer


	}; // namespace Policy
}; // namespace BR


