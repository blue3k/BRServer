////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/EntityServerIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class EntityServerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyEntityServer* GetPolicyEntityServer() { return GetPolicy<Policy::IPolicyEntityServer>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyEntityServer::ID_POLICY };
			// Constructor
			EntityServerService ( ServerServiceInformation* pService );

			// Cmd: Register entity
			HRESULT RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName );
			// Cmd: Find Entity
			HRESULT UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID );
			// Cmd: Find Entity
			HRESULT FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID );

		}; // class EntityServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


