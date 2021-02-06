////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug definitions
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


	class NetPolicyGamePartyManager 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyGamePartyManager (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Create a party instance
		Result CreatePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator );
		// C2S: Party instance notify of deletion. Sent by party instance
		Result PartyDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

	}; // class NetPolicyGamePartyManager 


	class NetSvrPolicyGamePartyManager
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyGamePartyManager ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Create a party instance
		Result CreatePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

	}; // class NetSvrPolicyGamePartyManager


}; // namespace SF


