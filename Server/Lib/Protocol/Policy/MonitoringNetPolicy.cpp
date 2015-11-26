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
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/MonitoringIPolicy.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Add a player to ranking
		HRESULT NetPolicyMonitoring::GetInstanceListCmd( const Context &InContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::GetInstanceListCmd::BuildIMsg(pMsg, InContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyMonitoring::GetInstanceListCmd( const Context &InContext )
		// Cmd: Remove a player to ranking
		HRESULT NetPolicyMonitoring::RequestCounterValuesCmd( const Context &InContext, const EntityUID &InInstanceUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::RequestCounterValuesCmd::BuildIMsg(pMsg, InContext, InInstanceUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyMonitoring::RequestCounterValuesCmd( const Context &InContext, const EntityUID &InInstanceUID )
		// C2S: Counter instance is created
		HRESULT NetPolicyMonitoring::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::PerformanceCounterNewC2SEvt::BuildIMsg(pMsg, InInstanceName, InInstanceUID, InNewCounters));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyMonitoring::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		// C2S: Counter instance is deleted
		HRESULT NetPolicyMonitoring::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::PerformanceCounterFreeC2SEvt::BuildIMsg(pMsg, InFreeInstances));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyMonitoring::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		// C2S: Counter update broadcast
		HRESULT NetPolicyMonitoring::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::PerformanceCounterUpdateC2SEvt::BuildIMsg(pMsg, InInstanceUID, InCounterValues));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyMonitoring::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )


		// Cmd: Add a player to ranking
		HRESULT NetSvrPolicyMonitoring::GetInstanceListRes( const Context &InContext, const HRESULT &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const UINT32 &InTotalInstanceCount )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::GetInstanceListRes::BuildIMsg(pMsg, InContext, InResult, InCounterInstances, InTotalInstanceCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyMonitoring::GetInstanceListRes( const Context &InContext, const HRESULT &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const UINT32 &InTotalInstanceCount )
		// Cmd: Remove a player to ranking
		HRESULT NetSvrPolicyMonitoring::RequestCounterValuesRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::RequestCounterValuesRes::BuildIMsg(pMsg, InContext, InResult, InInstanceUID, InCounterValues));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyMonitoring::RequestCounterValuesRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )
		// S2C: Request from server
		HRESULT NetSvrPolicyMonitoring::PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::BuildIMsg(pMsg, InInstanceUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyMonitoring::PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID )


	}; // namespace Policy
}; // namespace BR


