////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Monitoring Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/MonitoringIPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class MonitoringService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyMonitoring* GetPolicyMonitoring() { return GetPolicy<Policy::IPolicyMonitoring>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyMonitoring::ID_POLICY };
			// Constructor
			MonitoringService ( ServerServiceInformation* pService );

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

		}; // class MonitoringService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


