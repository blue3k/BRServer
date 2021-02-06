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


	class NetPolicyGameInstanceManager 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyGameInstanceManager (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Create a game instance
		Result CreateGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const VariableTable &InAttributes );
		// Cmd: Search game instance
		Result SearchGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InSearchKeyword );
		// C2S: Game instance notification of deletion
		Result GameInstanceDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

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
		// Cmd: Search game instance
		Result SearchGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<GameInstanceInfo>& InGameInstances );

	}; // class NetSvrPolicyGameInstanceManager


}; // namespace SF


