////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : Server Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_SERVER };

			// Constructor
			ISvrPolicyServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: 
			virtual HRESULT GenericFailureRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// C2S: Server Started or Connected

		}; // class ISvrPolicyServer : public Net::INetPolicy


		class IPolicyServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SERVER };

			// Constructor
			IPolicyServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: 
			virtual HRESULT GenericFailureCmd( const Context &InContext, const RouteContext &InRouteContext ) = 0;
			// C2S: Server Started or Connected
			virtual HRESULT ServerConnectedC2SEvt( const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPublicAddress, const NetAddress &InPrivateAddress ) = 0;

		}; // class IPolicyServer : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


