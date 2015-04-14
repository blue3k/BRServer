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

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
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
			NetPolicyServer ( Net::IConnection* pConn ) : IPolicyServer(pConn)
			{}

			// Cmd: 
			HRESULT GenericFailureCmd( const Context &InContext, const RouteContext &InRouteContext );
			// C2S: Server Started or Connected
			HRESULT ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT64 &InStartUpTime, const NetAddress &InPublicAddress, const NetAddress &InPrivateAddress );

		}; // class NetPolicyServer : public IPolicyServer


		class NetSvrPolicyServer : public ISvrPolicyServer
		{
 			public:
			// Constructor
			NetSvrPolicyServer ( Net::IConnection* pConn ) : ISvrPolicyServer(pConn)
			{}

			// Cmd: 
			HRESULT GenericFailureRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

		}; // class NetSvrPolicyServer : public ISvrPolicyServer


	}; // namespace Policy
}; // namespace BR


