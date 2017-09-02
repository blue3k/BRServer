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
				Net::Connection* m_pConnection;
			public:
			// Constructor
			NetPolicyMonitoring ( Net::Connection* pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Add a player to ranking
			Result GetInstanceListCmd( const TransactionID &InTransactionID );
			// Cmd: Remove a player to ranking
			Result RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID );
			// C2S: Counter instance is created
			Result PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters );
			// C2S: Counter instance is deleted
			Result PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances );
			// C2S: Counter update broadcast
			Result PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues );

		}; // class NetPolicyMonitoring 


		class NetSvrPolicyMonitoring
		{
 			private:
				Net::Connection* m_pConnection;
			public:
			// Constructor
			NetSvrPolicyMonitoring ( Net::Connection* pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Add a player to ranking
			Result GetInstanceListRes( const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount );
			// Cmd: Remove a player to ranking
			Result RequestCounterValuesRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues );
			// S2C: Request from server
			Result PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID );

		}; // class NetSvrPolicyMonitoring


	}; // namespace Policy
}; // namespace SF


