﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
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
			NetPolicyEntityServer ( Net::IConnection* pConn ) : IPolicyEntityServer(pConn)
			{}

			// Cmd: Register entity
			HRESULT RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName );
			// Cmd: Find Entity
			HRESULT UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID );
			// Cmd: Find Entity
			HRESULT FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID );

		}; // class NetPolicyEntityServer : public IPolicyEntityServer


		class NetSvrPolicyEntityServer : public ISvrPolicyEntityServer
		{
 			public:
			// Constructor
			NetSvrPolicyEntityServer ( Net::IConnection* pConn ) : ISvrPolicyEntityServer(pConn)
			{}

			// Cmd: Register entity
			HRESULT RegisterEntityRes( const TransactionID &InTransactionID, const HRESULT &InResult, const EntityUID &InEntUID );
			// Cmd: Find Entity
			HRESULT UnregisterEntityRes( const TransactionID &InTransactionID, const HRESULT &InResult );
			// Cmd: Find Entity
			HRESULT FindEntityRes( const TransactionID &InTransactionID, const HRESULT &InResult, const EntityUID &InEntUID );

		}; // class NetSvrPolicyEntityServer : public ISvrPolicyEntityServer


	}; // namespace Policy
}; // namespace BR


