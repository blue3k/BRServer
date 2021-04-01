////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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



namespace SF
{
    class MessageEndpoint;
   class VariableTable;
   struct ActorMovement;


	class NetPolicyLoginServer 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyLoginServer (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
		Result PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );
		// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
		Result KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InKickedPlayerID );

	}; // class NetPolicyLoginServer 


	class NetSvrPolicyLoginServer
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyLoginServer ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
		Result PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
		Result KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

	}; // class NetSvrPolicyLoginServer


}; // namespace SF


