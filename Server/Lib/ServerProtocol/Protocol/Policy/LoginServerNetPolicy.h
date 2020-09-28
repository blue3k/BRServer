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
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



class VariableTable;



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

			// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
			Result PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );
			// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
			Result KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InKickedPlayerID );

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

			// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
			Result PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
			Result KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyLoginServer


	}; // namespace Policy
}; // namespace SF


