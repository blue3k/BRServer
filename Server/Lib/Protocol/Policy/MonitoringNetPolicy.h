////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/MonitoringIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyMonitoring : public IPolicyMonitoring
		{
 			public:
			// Constructor
			NetPolicyMonitoring ( Net::IConnection* pConn ) : IPolicyMonitoring(pConn)
			{}

			// Cmd: Add a player to ranking
			HRESULT GetInstanceListCmd( const TransactionID &InTransactionID );
			// Cmd: Remove a player to ranking
			HRESULT RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID );
			// C2S: Counter instance is created
			HRESULT PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters );
			// C2S: Counter instance is deleted
			HRESULT PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances );
			// C2S: Counter update broadcast
			HRESULT PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues );

		}; // class NetPolicyMonitoring : public IPolicyMonitoring


		class NetSvrPolicyMonitoring : public ISvrPolicyMonitoring
		{
 			public:
			// Constructor
			NetSvrPolicyMonitoring ( Net::IConnection* pConn ) : ISvrPolicyMonitoring(pConn)
			{}

			// Cmd: Add a player to ranking
			HRESULT GetInstanceListRes( const TransactionID &InTransactionID, const HRESULT &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const UINT32 &InTotalInstanceCount );
			// Cmd: Remove a player to ranking
			HRESULT RequestCounterValuesRes( const TransactionID &InTransactionID, const HRESULT &InResult, const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues );
			// S2C: Request from server
			HRESULT PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID );

		}; // class NetSvrPolicyMonitoring : public ISvrPolicyMonitoring


	}; // namespace Policy
}; // namespace BR


