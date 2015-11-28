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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"
#include "Common/Message.h"
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
		UINT32 m_TotalCounterInstance;

	public:
		MonitoringTransGetInstanceList(Message::MessageData* &pIMsg) : ServerEntityMessageTransaction(pIMsg) {}
		virtual ~MonitoringTransGetInstanceList() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyMonitoring* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyMonitoring>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(GetInstanceListRes, m_InstanceList, m_TotalCounterInstance);
	};



	class MonitoringTransRequestCounterValues : public ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::RequestCounterValuesCmd, MonitoringTransRequestCounterValues, 1>
	{
	public:
		typedef ServerEntityMessageTransaction< MonitoringServiceEntity, Message::Monitoring::RequestCounterValuesCmd, MonitoringTransRequestCounterValues, 1> super;

	private:
		StaticArray<UINT64, 1024> m_CounterValues;

	public:
		MonitoringTransRequestCounterValues(Message::MessageData* &pIMsg) : ServerEntityMessageTransaction(pIMsg) {}
		virtual ~MonitoringTransRequestCounterValues() {}

		// Start Transaction
		virtual HRESULT StartTransaction();

		Policy::ISvrPolicyMonitoring* GetPolicy()	{ return ServerEntityMessageTransaction::GetPolicy<Policy::ISvrPolicyMonitoring>(); }

		BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS(RequestCounterValuesRes, GetInstanceUID(), m_CounterValues);
	};





} // namespace GameServer 
} // namespace BR 

