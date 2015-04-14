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
#include "Common/BRBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
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
			static_assert(Policy::IPolicyMonitoring::ID_POLICY == ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyMonitoring());
		}


		// Cmd: Add a player to ranking
		HRESULT MonitoringService::GetInstanceListCmd( const Context &InContext )
		{
 			HRESULT hr = S_OK;

			 svrChk(GetPolicyMonitoring()->GetInstanceListCmd( InContext ) );

		Proc_End:

			return hr;

		}; // HRESULT MonitoringService::GetInstanceListCmd( const Context &InContext )
		// Cmd: Remove a player to ranking
		HRESULT MonitoringService::RequestCounterValuesCmd( const Context &InContext, const EntityUID &InInstanceUID )
		{
 			HRESULT hr = S_OK;

			 svrChk(GetPolicyMonitoring()->RequestCounterValuesCmd( InContext, InInstanceUID ) );

		Proc_End:

			return hr;

		}; // HRESULT MonitoringService::RequestCounterValuesCmd( const Context &InContext, const EntityUID &InInstanceUID )
		// C2S: Counter instance is created
		HRESULT MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		{
 			HRESULT hr = S_OK;

			 svrChk(GetPolicyMonitoring()->PerformanceCounterNewC2SEvt( InInstanceName, InInstanceUID, InNewCounters ) );

		Proc_End:

			return hr;

		}; // HRESULT MonitoringService::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		// C2S: Counter instance is deleted
		HRESULT MonitoringService::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		{
 			HRESULT hr = S_OK;

			 svrChk(GetPolicyMonitoring()->PerformanceCounterFreeC2SEvt( InFreeInstances ) );

		Proc_End:

			return hr;

		}; // HRESULT MonitoringService::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		// C2S: Counter update broadcast
		HRESULT MonitoringService::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )
		{
 			HRESULT hr = S_OK;

			 svrChk(GetPolicyMonitoring()->PerformanceCounterUpdateC2SEvt( InInstanceUID, InCounterValues ) );

		Proc_End:

			return hr;

		}; // HRESULT MonitoringService::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )


	}; // namespace Svr
}; // namespace BR


