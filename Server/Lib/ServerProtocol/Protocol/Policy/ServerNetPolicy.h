////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Server Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyServer 
		{
 			private:
				Net::Connection* m_pConnection;
			public:
			// Constructor
			NetPolicyServer ( Net::Connection* pConn ) : m_pConnection(pConn)
			{}

			// Cmd: 
			Result GenericFailureCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID );
			// C2S: Server Started or Connected
			Result ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress );

		}; // class NetPolicyServer 


		class NetSvrPolicyServer
		{
 			private:
				Net::Connection* m_pConnection;
			public:
			// Constructor
			NetSvrPolicyServer ( Net::Connection* pConn ) : m_pConnection(pConn)
			{}

			// Cmd: 
			Result GenericFailureRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyServer


	}; // namespace Policy
}; // namespace SF


