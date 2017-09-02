////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : EntityServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/EntityServerIPolicy.h"



namespace SF
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
			Result RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName );
			// Cmd: Find Entity
			Result UnregisterEntityCmd( const TransactionID &InTransactionID, const uint64_t &InEntUID );
			// Cmd: Find Entity
			Result FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID );

		}; // class EntityServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


