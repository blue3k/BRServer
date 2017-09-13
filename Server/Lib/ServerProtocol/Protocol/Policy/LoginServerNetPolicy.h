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



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyLoginServer 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyLoginServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyLoginServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Notify user joind and see it's valid authticket instance
			Result PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );
			// Cmd: Kick logged in player
			Result KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID );

		}; // class NetPolicyLoginServer 


		class NetSvrPolicyLoginServer
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyLoginServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyLoginServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Notify user joind and see it's valid authticket instance
			Result PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: Kick logged in player
			Result KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyLoginServer


	}; // namespace Policy
}; // namespace SF


