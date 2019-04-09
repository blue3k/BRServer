////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : Monitoring Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class MonitoringService : public ServerServiceBase
		{
 			public:
			// Constructor
			MonitoringService ( ServerServiceInformation* pService );

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

		}; // class MonitoringService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


