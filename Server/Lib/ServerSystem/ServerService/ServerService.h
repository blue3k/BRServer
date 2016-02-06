////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Server Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/ServerIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class ServerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyServer* GetPolicyServer() { return GetPolicy<Policy::IPolicyServer>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyServer::ID_POLICY };
			// Constructor
			ServerService ( ServerServiceInformation* pService );

			// Cmd: 
			HRESULT GenericFailureCmd( const TransactionID &InTransactionID );
			// C2S: Server Started or Connected
			HRESULT ServerConnectedC2SEvt( const EntityID &InSenderEntityID, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPrivateAddress );

		}; // class ServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


