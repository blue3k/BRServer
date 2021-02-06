////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : RelayServer Message debug definitions
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


	class NetPolicyRelayServer 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyRelayServer (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Relay Instance
		Result CreateRelayInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore );
		// Cmd: Add a player to Relay
		Result AddPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRelayScore );
		// Cmd: Remove a player to Relay
		Result RemovePlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );

	}; // class NetPolicyRelayServer 


	class NetSvrPolicyRelayServer
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyRelayServer ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Relay Instance
		Result CreateRelayInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRelay );
		// Cmd: Add a player to Relay
		Result AddPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRelay );
		// Cmd: Remove a player to Relay
		Result RemovePlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

	}; // class NetSvrPolicyRelayServer


}; // namespace SF


