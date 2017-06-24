////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BrBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/MonitoringService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		MonitoringService::MonitoringService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyMonitoring::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyMonitoring::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyMonitoring());
		}


		// Cmd: Add a player to ranking
		Result MonitoringService::GetInstanceListCmd( const TransactionID &InTransactionID )
		{
 			Result hr;

			 svrChk(GetPolicyMonitoring()->GetInstanceListCmd( InTransactionID ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::GetInstanceListCmd( const TransactionID &InTransactionID )
		// Cmd: Remove a player to ranking
		Result MonitoringService::RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID )
		{
 			Result hr;

			 svrChk(GetPolicyMonitoring()->RequestCounterValuesCmd( InTransactionID, InInstanceUID ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID )
		// C2S: Counter instance is created
		Result MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		{
 			Result hr;

			 svrChk(GetPolicyMonitoring()->PerformanceCounterNewC2SEvt( InInstanceName, InInstanceUID, InNewCounters ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		// C2S: Counter instance is deleted
		Result MonitoringService::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		{
 			Result hr;

			 svrChk(GetPolicyMonitoring()->PerformanceCounterFreeC2SEvt( InFreeInstances ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		// C2S: Counter update broadcast
		Result MonitoringService::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			Result hr;

			 svrChk(GetPolicyMonitoring()->PerformanceCounterUpdateC2SEvt( InInstanceUID, InCounterValues ) );

		Proc_End:

			return hr;

		}; // Result MonitoringService::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )


	}; // namespace Svr
}; // namespace BR


