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
#include "Net/NetPolicy.h"
#include "Protocol/Policy/ServerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyServer : public IPolicyServer
		{
 			public:
			// Constructor
			NetPolicyServer ( Net::Connection* pConn ) : IPolicyServer(pConn)
			{}

			// Cmd: 
			Result GenericFailureCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID );
			// C2S: Server Started or Connected
			Result ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress );

		}; // class NetPolicyServer : public IPolicyServer


		class NetSvrPolicyServer : public ISvrPolicyServer
		{
 			public:
			// Constructor
			NetSvrPolicyServer ( Net::Connection* pConn ) : ISvrPolicyServer(pConn)
			{}

			// Cmd: 
			Result GenericFailureRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyServer : public ISvrPolicyServer


	}; // namespace Policy
}; // namespace BR


