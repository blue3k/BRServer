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
#include "ServerSystem/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Types/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Net/Message.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/Policy/MonitoringIPolicy.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServiceEntity/MonitoringServiceEntity.h"
#include "ServerSystem/ServerTransaction.h"


namespace BR {
namespace Svr {

	

	class MonitoringTransGetInstanceList : public ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::GetInstanceListCmd, MonitoringTransGetInstanceList, 1>
	{
	public:
		typedef ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::GetInstanceListCmd, MonitoringTransGetInstanceList, 1> super;

	private:
		StaticArray<PerformanceCounterInstanceInfo, 1024> m_InstanceList;
		uint32_t m_TotalCounterInstance;

	public:
		MonitoringTransGetInstanceList(Message::MessageData* &pIMsg) : ServerEntityMessageTransaction(pIMsg) {}
		virtual ~MonitoringTransGetInstanceList() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyMonitoring* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyMonitoring>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(GetInstanceListRes, m_InstanceList, m_TotalCounterInstance);
	};



	class MonitoringTransRequestCounterValues : public ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::RequestCounterValuesCmd, MonitoringTransRequestCounterValues, 1>
	{
	public:
		typedef ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::RequestCounterValuesCmd, MonitoringTransRequestCounterValues, 1> super;

	private:
		StaticArray<uint64_t, 1024> m_CounterValues;

	public:
		MonitoringTransRequestCounterValues(Message::MessageData* &pIMsg) : ServerEntityMessageTransaction(pIMsg) {}
		virtual ~MonitoringTransRequestCounterValues() {}

		// Start Transaction
		virtual Result StartTransaction();

		Policy::ISvrPolicyMonitoring* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyMonitoring>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(RequestCounterValuesRes, GetInstanceUID(), m_CounterValues);
	};





} // namespace GameServer 
} // namespace BR 

