////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
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



class VariableTable;



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyGameInstanceManager 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyGameInstanceManager ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyGameInstanceManager ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
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
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyGameInstanceManager ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyGameInstanceManager ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Create a game instance
			Result CreateGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// Cmd: Search game instance
			Result SearchGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<GameInstanceInfo>& InGameInstances );

		}; // class NetSvrPolicyGameInstanceManager


	}; // namespace Policy
}; // namespace SF


