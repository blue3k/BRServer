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
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyMonitoring 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyMonitoring ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyMonitoring ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: [deprecated]
			Result GetInstanceListCmd( const uint64_t &InTransactionID );
			// Cmd: [deprecated]
			Result RequestCounterValuesCmd( const uint64_t &InTransactionID, const uint64_t &InInstanceUID );
			// C2S: Message when new performance counter is added.
			Result PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters );
			// C2S: Counter instance is deleted
			Result PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances );
			// C2S: Counter update broadcast
			Result PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues );

		}; // class NetPolicyMonitoring 


		class NetSvrPolicyMonitoring
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyMonitoring ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyMonitoring ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: [deprecated]
			Result GetInstanceListRes( const uint64_t &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount );
			// Cmd: [deprecated]
			Result RequestCounterValuesRes( const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues );
			// S2C: Server will send this message to an instance to get performance counters.
			Result PerformanceCounterUpdateCounterInfoS2CEvt( const uint64_t &InInstanceUID );

		}; // class NetSvrPolicyMonitoring


	}; // namespace Policy
}; // namespace SF


