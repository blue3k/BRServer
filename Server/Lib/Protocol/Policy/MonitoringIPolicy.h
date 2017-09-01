////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyMonitoring : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_SVR_MONITORING };

			// Constructor
			ISvrPolicyMonitoring( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Add a player to ranking
			virtual Result GetInstanceListRes( const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result RequestCounterValuesRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues ) = 0;
			// C2S: Counter instance is created
			// C2S: Counter instance is deleted
			// C2S: Counter update broadcast
			// S2C: Request from server
			virtual Result PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID ) = 0;

		}; // class ISvrPolicyMonitoring : public Net::INetPolicy


		class IPolicyMonitoring : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_MONITORING };

			// Constructor
			IPolicyMonitoring( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Add a player to ranking
			virtual Result GetInstanceListCmd( const TransactionID &InTransactionID ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result RequestCounterValuesCmd( const TransactionID &InTransactionID, const EntityUID &InInstanceUID ) = 0;
			// C2S: Counter instance is created
			virtual Result PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters ) = 0;
			// C2S: Counter instance is deleted
			virtual Result PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances ) = 0;
			// C2S: Counter update broadcast
			virtual Result PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues ) = 0;
			// S2C: Request from server

		}; // class IPolicyMonitoring : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


