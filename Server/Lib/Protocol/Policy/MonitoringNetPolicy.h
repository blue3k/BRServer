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

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/MonitoringIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyMonitoring : public IPolicyMonitoring
		{
 			public:
			// Constructor
			NetPolicyMonitoring ( Net::IConnection* pConn ) : IPolicyMonitoring(pConn)
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

		}; // class NetPolicyMonitoring : public IPolicyMonitoring


		class NetSvrPolicyMonitoring : public ISvrPolicyMonitoring
		{
 			public:
			// Constructor
			NetSvrPolicyMonitoring ( Net::IConnection* pConn ) : ISvrPolicyMonitoring(pConn)
			{}

			// Cmd: Add a player to ranking
			Result GetInstanceListRes( const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount );
			// Cmd: Remove a player to ranking
			Result RequestCounterValuesRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues );
			// S2C: Request from server
			Result PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID );

		}; // class NetSvrPolicyMonitoring : public ISvrPolicyMonitoring


	}; // namespace Policy
}; // namespace BR


