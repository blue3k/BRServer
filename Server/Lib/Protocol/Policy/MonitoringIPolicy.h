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

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
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
			enum { ID_POLICY = POLICY_SVR_MONITORING };

			// Constructor
			ISvrPolicyMonitoring( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Add a player to ranking
			virtual HRESULT GetInstanceListRes( const Context &InContext, const HRESULT &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const UINT32 &InTotalInstanceCount ) = 0;
			// Cmd: Remove a player to ranking
			virtual HRESULT RequestCounterValuesRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues ) = 0;
			// C2S: Counter instance is created
			// C2S: Counter instance is deleted
			// C2S: Counter update broadcast
			// S2C: Request from server
			virtual HRESULT PerformanceCounterUpdateCounterInfoS2CEvt( const EntityUID &InInstanceUID ) = 0;

		}; // class ISvrPolicyMonitoring : public Net::INetPolicy


		class IPolicyMonitoring : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_MONITORING };

			// Constructor
			IPolicyMonitoring( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Add a player to ranking
			virtual HRESULT GetInstanceListCmd( const Context &InContext ) = 0;
			// Cmd: Remove a player to ranking
			virtual HRESULT RequestCounterValuesCmd( const Context &InContext, const EntityUID &InInstanceUID ) = 0;
			// C2S: Counter instance is created
			virtual HRESULT PerformanceCounterNewC2SEvt( const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters ) = 0;
			// C2S: Counter instance is deleted
			virtual HRESULT PerformanceCounterFreeC2SEvt( const Array<EntityUID>& InFreeInstances ) = 0;
			// C2S: Counter update broadcast
			virtual HRESULT PerformanceCounterUpdateC2SEvt( const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues ) = 0;
			// S2C: Request from server

		}; // class IPolicyMonitoring : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


