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
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "Protocol/Policy/MonitoringIPolicy.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Add a player to ranking
		Result NetPolicyMonitoring::GetInstanceListCmd( const TransactionID &InTransactionID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::GetInstanceListCmd::BuildIMsg(pMsg, InTransactionID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::GetInstanceListCmd( const TransactionID &InTransactionID )
		// Cmd: Remove a player to ranking
		Result NetPolicyMonitoring::RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::RequestCounterValuesCmd::BuildIMsg(pMsg, InTransactionID, InInstanceUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID )
		// C2S: Counter instance is created
		Result NetPolicyMonitoring::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::PerformanceCounterNewC2SEvt::BuildIMsg(pMsg, InInstanceName, InInstanceUID, InNewCounters));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
		// C2S: Counter instance is deleted
		Result NetPolicyMonitoring::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::PerformanceCounterFreeC2SEvt::BuildIMsg(pMsg, InFreeInstances));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances )
		// C2S: Counter update broadcast
		Result NetPolicyMonitoring::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::PerformanceCounterUpdateC2SEvt::BuildIMsg(pMsg, InInstanceUID, InCounterValues));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyMonitoring::PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )


		// Cmd: Add a player to ranking
		Result NetSvrPolicyMonitoring::GetInstanceListRes( const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::GetInstanceListRes::BuildIMsg(pMsg, InTransactionID, InResult, InCounterInstances, InTotalInstanceCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyMonitoring::GetInstanceListRes( const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
		// Cmd: Remove a player to ranking
		Result NetSvrPolicyMonitoring::RequestCounterValuesRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::RequestCounterValuesRes::BuildIMsg(pMsg, InTransactionID, InResult, InInstanceUID, InCounterValues));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyMonitoring::RequestCounterValuesRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
		// S2C: Request from server
		Result NetSvrPolicyMonitoring::PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::BuildIMsg(pMsg, InInstanceUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyMonitoring::PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID )


	}; // namespace Policy
}; // namespace BR


