////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
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
 	namespace Policy
	{
 		class NetPolicyPartyMatching 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyPartyMatching ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyPartyMatching ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}


		}; // class NetPolicyPartyMatching 


		class NetSvrPolicyPartyMatching
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyPartyMatching ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyPartyMatching ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// S2C: Event message will be sent from matching srvice when the pary game mathcing has done.
			Result PartyGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );
			// S2C: Event message will be sent when a player game matching has successfully finished.
			Result PlayerGameMatchedS2CEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole );

		}; // class NetSvrPolicyPartyMatching


	}; // namespace Policy
}; // namespace SF


