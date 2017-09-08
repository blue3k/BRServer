////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message debug definitions
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
 		class NetPolicyMonitoring 
		{
 			private:
				SharedPointerT<Net::Connection>& m_pConnection;
			public:
			// Constructor
			NetPolicyMonitoring ( SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Add a player to ranking
			Result GetInstanceListCmd( const TransactionID &InTransactionID );
			// Cmd: Remove a player to ranking
			Result RequestCounterValuesCmd( const TransactionID &InTransactionID, const uint64_t &InInstanceUID );
			// C2S: Counter instance is created
			Result PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters );
			// C2S: Counter instance is deleted
			Result PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances );
			// C2S: Counter update broadcast
			Result PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues );

		}; // class NetPolicyMonitoring 


		class NetSvrPolicyMonitoring
		{
 			private:
				SharedPointerT<Net::Connection>& m_pConnection;
			public:
			// Constructor
			NetSvrPolicyMonitoring ( SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Add a player to ranking
			Result GetInstanceListRes( const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount );
			// Cmd: Remove a player to ranking
			Result RequestCounterValuesRes( const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues );
			// S2C: Request from server
			Result PerformanceCounterUpdateCounterInfoS2CEvt( const uint64_t &InInstanceUID );

		}; // class NetSvrPolicyMonitoring


	}; // namespace Policy
}; // namespace SF


