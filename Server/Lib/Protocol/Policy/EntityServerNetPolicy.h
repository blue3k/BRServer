////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/EntityServerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyEntityServer : public IPolicyEntityServer
		{
 			public:
			// Constructor
			NetPolicyEntityServer ( Net::Connection* pConn ) : IPolicyEntityServer(pConn)
			{}

			// Cmd: Register entity
			Result RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName );
			// Cmd: Find Entity
			Result UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID );
			// Cmd: Find Entity
			Result FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID );

		}; // class NetPolicyEntityServer : public IPolicyEntityServer


		class NetSvrPolicyEntityServer : public ISvrPolicyEntityServer
		{
 			public:
			// Constructor
			NetSvrPolicyEntityServer ( Net::Connection* pConn ) : ISvrPolicyEntityServer(pConn)
			{}

			// Cmd: Register entity
			Result RegisterEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID );
			// Cmd: Find Entity
			Result UnregisterEntityRes( const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: Find Entity
			Result FindEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID );

		}; // class NetSvrPolicyEntityServer : public ISvrPolicyEntityServer


	}; // namespace Policy
}; // namespace BR


