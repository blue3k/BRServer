////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug definitions
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


	class NetPolicyGameInstanceManager 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyGameInstanceManager (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Create a game instance
		Result CreateGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const VariableTable &InAttributes );

	}; // class NetPolicyGameInstanceManager 


	class NetSvrPolicyGameInstanceManager
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyGameInstanceManager ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Create a game instance
		Result CreateGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

	}; // class NetSvrPolicyGameInstanceManager


}; // namespace SF


