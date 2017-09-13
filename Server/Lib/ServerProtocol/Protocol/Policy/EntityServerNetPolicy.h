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



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyEntityServer 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyEntityServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyEntityServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Register entity
			Result RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName );
			// Cmd: Find Entity
			Result UnregisterEntityCmd( const TransactionID &InTransactionID, const uint64_t &InEntUID );
			// Cmd: Find Entity
			Result FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID );

		}; // class NetPolicyEntityServer 


		class NetSvrPolicyEntityServer
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyEntityServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyEntityServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Register entity
			Result RegisterEntityRes( const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InEntUID );
			// Cmd: Find Entity
			Result UnregisterEntityRes( const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: Find Entity
			Result FindEntityRes( const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InEntUID );

		}; // class NetSvrPolicyEntityServer


	}; // namespace Policy
}; // namespace SF


