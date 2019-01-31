////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
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
 	namespace Policy
	{
 		class NetPolicyGamePartyManager 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyGamePartyManager ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyGamePartyManager ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Create a party instance
			Result CreatePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InGameID, const PlayerInformation &InCreator );
			// C2S: Party instance notify of deletion. Sent by party instance
			Result PartyDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

		}; // class NetPolicyGamePartyManager 


		class NetSvrPolicyGamePartyManager
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyGamePartyManager ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyGamePartyManager ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Create a party instance
			Result CreatePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyGamePartyManager


	}; // namespace Policy
}; // namespace SF


