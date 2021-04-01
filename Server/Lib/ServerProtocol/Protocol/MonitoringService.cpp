////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : Monitoring Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/MonitoringService.h"
#include "SvrTrace.h"



namespace SF
{
 	MonitoringService::MonitoringService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: [deprecated]
	Result MonitoringService::GetInstanceListCmd( const uint64_t &InTransactionID )
	{
 		ScopeContext hr;

		 svrCheck(NetPolicyMonitoring(GetTargetEndpoint()).GetInstanceListCmd( InTransactionID ) );

		return hr;

	}; // Result MonitoringService::GetInstanceListCmd( const uint64_t &InTransactionID )
	// Cmd: [deprecated]
	Result MonitoringService::RequestCounterValuesCmd( const uint64_t &InTransactionID, const uint64_t &InInstanceUID )
	{
 		ScopeContext hr;

		 svrCheck(NetPolicyMonitoring(GetTargetEndpoint()).RequestCounterValuesCmd( InTransactionID, InInstanceUID ) );

		return hr;

	}; // Result MonitoringService::RequestCounterValuesCmd( const uint64_t &InTransactionID, const uint64_t &InInstanceUID )
	// C2S: Message when new performance counter is added.
	Result MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
	{
 		ScopeContext hr;

		 svrCheck(NetPolicyMonitoring(GetTargetEndpoint()).PerformanceCounterNewC2SEvt( InInstanceName, InInstanceUID, InNewCounters ) );

		return hr;

	}; // Result MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
	// C2S: Counter instance is deleted
	Result MonitoringService::PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances )
	{
 		ScopeContext hr;

		 svrCheck(NetPolicyMonitoring(GetTargetEndpoint()).PerformanceCounterFreeC2SEvt( InFreeInstances ) );

		return hr;

	}; // Result MonitoringService::PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances )
	// C2S: Counter update broadcast
	Result MonitoringService::PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
	{
 		ScopeContext hr;

		 svrCheck(NetPolicyMonitoring(GetTargetEndpoint()).PerformanceCounterUpdateC2SEvt( InInstanceUID, InCounterValues ) );

		return hr;

	}; // Result MonitoringService::PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )


}; // namespace SF


