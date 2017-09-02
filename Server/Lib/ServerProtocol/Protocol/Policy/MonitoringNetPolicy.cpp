////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : Monitoring Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Add a player to ranking
		Result NetPolicyMonitoring::GetInstanceListCmd( const TransactionID &InTransactionID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::GetInstanceListCmd::Create(m_pConnection->GetMemoryManager(), InTransactionID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::GetInstanceListCmd( const TransactionID &InTransactionID )
		// Cmd: Remove a player to ranking
		Result NetPolicyMonitoring::RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::RequestCounterValuesCmd::Create(m_pConnection->GetMemoryManager(), InTransactionID, InInstanceUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID )
		// C2S: Counter instance is created
		Result NetPolicyMonitoring::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::PerformanceCounterNewC2SEvt::Create(m_pConnection->GetMemoryManager(), InInstanceName, InInstanceUID, InNewCounters);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		// C2S: Counter instance is deleted
		Result NetPolicyMonitoring::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::PerformanceCounterFreeC2SEvt::Create(m_pConnection->GetMemoryManager(), InFreeInstances);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		// C2S: Counter update broadcast
		Result NetPolicyMonitoring::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::PerformanceCounterUpdateC2SEvt::Create(m_pConnection->GetMemoryManager(), InInstanceUID, InCounterValues);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )


		// Cmd: Add a player to ranking
		Result NetSvrPolicyMonitoring::GetInstanceListRes( const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::GetInstanceListRes::Create(m_pConnection->GetMemoryManager(), InTransactionID, InResult, InCounterInstances, InTotalInstanceCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyMonitoring::GetInstanceListRes( const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
		// Cmd: Remove a player to ranking
		Result NetSvrPolicyMonitoring::RequestCounterValuesRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::RequestCounterValuesRes::Create(m_pConnection->GetMemoryManager(), InTransactionID, InResult, InInstanceUID, InCounterValues);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyMonitoring::RequestCounterValuesRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
		// S2C: Request from server
		Result NetSvrPolicyMonitoring::PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::Create(m_pConnection->GetMemoryManager(), InInstanceUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyMonitoring::PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID )


	}; // namespace Policy
}; // namespace SF


