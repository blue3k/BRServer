////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Monitoring entity transaction
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/SFMessage.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/MonitoringServiceEntity.h"
#include "Transaction/ServerTransaction.h"


namespace SF {
	namespace Svr {



		class MonitoringTransGetInstanceList : public ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::GetInstanceListCmd>
		{
		public:
			typedef ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::GetInstanceListCmd> super;

		private:
			StaticArray<PerformanceCounterInstanceInfo, 1024> m_InstanceList;
			uint32_t m_TotalCounterInstance;

		public:
			MonitoringTransGetInstanceList(IHeap& memMgr, MessageDataPtr& pIMsg)
				: ServerEntityMessageTransaction(memMgr, pIMsg)
				, m_InstanceList(memMgr)
			{}
			virtual ~MonitoringTransGetInstanceList() {}

			// Start Transaction
			virtual Result StartTransaction() override;

			BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyMonitoring, GetInstanceListRes, m_InstanceList, m_TotalCounterInstance);
		};



		class MonitoringTransRequestCounterValues : public ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::RequestCounterValuesCmd>
		{
		public:
			typedef ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::RequestCounterValuesCmd> super;

		private:
			StaticArray<uint64_t, 1024> m_CounterValues;

		public:
			MonitoringTransRequestCounterValues(IHeap& memMgr, MessageDataPtr& pIMsg)
				: ServerEntityMessageTransaction(memMgr, pIMsg)
				, m_CounterValues(memMgr)
			{}
			virtual ~MonitoringTransRequestCounterValues() {}

			// Start Transaction
			virtual Result StartTransaction() override;


			BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(Policy::NetSvrPolicyMonitoring, RequestCounterValuesRes, GetInstanceUID(), m_CounterValues);
		};





	} // namespace GameServer 
} // namespace SF 

