////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : Monitoring Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
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
		Result MonitoringService::GetInstanceListCmd( const uint64_t &InTransactionID )
		{
 			FunctionContext hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).GetInstanceListCmd( InTransactionID ) );

			return hr;

		}; // Result MonitoringService::GetInstanceListCmd( const uint64_t &InTransactionID )
		// Cmd: [deprecated]
		Result MonitoringService::RequestCounterValuesCmd( const uint64_t &InTransactionID, const uint64_t &InInstanceUID )
		{
 			FunctionContext hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).RequestCounterValuesCmd( InTransactionID, InInstanceUID ) );

			return hr;

		}; // Result MonitoringService::RequestCounterValuesCmd( const uint64_t &InTransactionID, const uint64_t &InInstanceUID )
		// C2S: Message when new performance counter is added.
		Result MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		{
 			FunctionContext hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).PerformanceCounterNewC2SEvt( InInstanceName, InInstanceUID, InNewCounters ) );

			return hr;

		}; // Result MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		// C2S: Counter instance is deleted
		Result MonitoringService::PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances )
		{
 			FunctionContext hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).PerformanceCounterFreeC2SEvt( InFreeInstances ) );

			return hr;

		}; // Result MonitoringService::PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances )
		// C2S: Counter update broadcast
		Result MonitoringService::PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			FunctionContext hr;

			 svrChk(Policy::NetPolicyMonitoring(GetConnection()).PerformanceCounterUpdateC2SEvt( InInstanceUID, InCounterValues ) );

			return hr;

		}; // Result MonitoringService::PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )


	}; // namespace Svr
}; // namespace SF


