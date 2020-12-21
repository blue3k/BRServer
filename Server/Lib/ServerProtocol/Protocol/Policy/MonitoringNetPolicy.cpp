////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: [deprecated]
		Result NetPolicyMonitoring::GetInstanceListCmd( const uint64_t &InTransactionID )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::GetInstanceListCmd::Create(m_pConnection->GetIOHeap(), InTransactionID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyMonitoring::GetInstanceListCmd( const uint64_t &InTransactionID )
		// Cmd: [deprecated]
		Result NetPolicyMonitoring::RequestCounterValuesCmd( const uint64_t &InTransactionID, const uint64_t &InInstanceUID )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::RequestCounterValuesCmd::Create(m_pConnection->GetIOHeap(), InTransactionID, InInstanceUID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyMonitoring::RequestCounterValuesCmd( const uint64_t &InTransactionID, const uint64_t &InInstanceUID )
		// C2S: Message when new performance counter is added.
		Result NetPolicyMonitoring::PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::PerformanceCounterNewC2SEvt::Create(m_pConnection->GetIOHeap(), InInstanceName, InInstanceUID, InNewCounters);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterNewC2SEvt( const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		// C2S: Counter instance is deleted
		Result NetPolicyMonitoring::PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::PerformanceCounterFreeC2SEvt::Create(m_pConnection->GetIOHeap(), InFreeInstances);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterFreeC2SEvt( const Array<uint64_t>& InFreeInstances )
		// C2S: Counter update broadcast
		Result NetPolicyMonitoring::PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::PerformanceCounterUpdateC2SEvt::Create(m_pConnection->GetIOHeap(), InInstanceUID, InCounterValues);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterUpdateC2SEvt( const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )


		// Cmd: [deprecated]
		Result NetSvrPolicyMonitoring::GetInstanceListRes( const uint64_t &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::GetInstanceListRes::Create(m_pConnection->GetIOHeap(), InTransactionID, InResult, InCounterInstances, InTotalInstanceCount);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyMonitoring::GetInstanceListRes( const uint64_t &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
		// Cmd: [deprecated]
		Result NetSvrPolicyMonitoring::RequestCounterValuesRes( const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::RequestCounterValuesRes::Create(m_pConnection->GetIOHeap(), InTransactionID, InResult, InInstanceUID, InCounterValues);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyMonitoring::RequestCounterValuesRes( const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
		// S2C: Server will send this message to an instance to get performance counters.
		Result NetSvrPolicyMonitoring::PerformanceCounterUpdateCounterInfoS2CEvt( const uint64_t &InInstanceUID )
		{
 			ScopeContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::Create(m_pConnection->GetIOHeap(), InInstanceUID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyMonitoring::PerformanceCounterUpdateCounterInfoS2CEvt( const uint64_t &InInstanceUID )


	}; // namespace Policy
}; // namespace SF


