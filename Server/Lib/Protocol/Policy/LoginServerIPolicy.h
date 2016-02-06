////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : LoginServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyLoginServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_LOGINSERVER };

			// Constructor
			ISvrPolicyLoginServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Notify user joind and see it's valid authticket instance
			virtual HRESULT PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult ) = 0;
			// Cmd: Kick logged in player
			virtual HRESULT KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult ) = 0;

		}; // class ISvrPolicyLoginServer : public Net::INetPolicy


		class IPolicyLoginServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_LOGINSERVER };

			// Constructor
			IPolicyLoginServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Notify user joind and see it's valid authticket instance
			virtual HRESULT PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket ) = 0;
			// Cmd: Kick logged in player
			virtual HRESULT KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID ) = 0;

		}; // class IPolicyLoginServer : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


