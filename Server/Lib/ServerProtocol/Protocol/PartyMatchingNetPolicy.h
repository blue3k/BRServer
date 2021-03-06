﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message debug definitions
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


	class NetPolicyPartyMatching 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyPartyMatching (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}


	}; // class NetPolicyPartyMatching 


	class NetSvrPolicyPartyMatching
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyPartyMatching ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// S2C: Event message will be sent from matching srvice when the pary game mathcing has done.
		Result PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );
		// S2C: Event message will be sent when a player game matching has successfully finished.
		Result PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole );

	}; // class NetSvrPolicyPartyMatching


}; // namespace SF


