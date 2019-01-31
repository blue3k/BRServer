////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Monitoring Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/MonitoringService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		MonitoringService::MonitoringService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: [deprecated]
		Result MonitoringService::GetInstanceListCmd( const TransactionID &InTransactionID )
		{
 			Result hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).GetInstanceListCmd( InTransactionID ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::GetInstanceListCmd( const TransactionID &InTransactionID )
		// Cmd: [deprecated]
		Result MonitoringService::RequestCounterValuesCmd( const TransactionID &InTransactionID, const uint64_t &InInstanceUID )
		{
 			Result hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).RequestCounterValuesCmd( InTransactionID, InInstanceUID ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::RequestCounterValuesCmd( const TransactionID &InTransactionID, const uint64_t &InInstanceUID )
		// C2S: Message when new performance counter is added.
		Result MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		{
 			Result hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).PerformanceCounterNewC2SEvt( InInstanceName, InInstanceUID, InNewCounters ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		// C2S: Counter instance is deleted
		Result MonitoringService::PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances )
		{
 			Result hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).PerformanceCounterFreeC2SEvt( InFreeInstances ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances )
		// C2S: Counter update broadcast
		Result MonitoringService::PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			Result hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).PerformanceCounterUpdateC2SEvt( InInstanceUID, InCounterValues ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )


	}; // namespace Svr
}; // namespace SF


