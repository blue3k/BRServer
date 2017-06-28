////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Monitoring entity transaction
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/MemoryPool.h"
#include "Common/BrBaseTypes.h"
#include "Common/GameConst.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/Policy/MonitoringIPolicy.h"


#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/MonitoringServiceTrans.h"
#include "ServerSystem/ServiceEntity/MonitoringServiceEntity.h"

#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterServer.h"


BR_MEMORYPOOL_IMPLEMENT(Svr::MonitoringTransGetInstanceList);
BR_MEMORYPOOL_IMPLEMENT(Svr::MonitoringTransRequestCounterValues);



namespace BR {
namespace Svr {



	// Start Transaction
	Result MonitoringTransGetInstanceList::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		StaticArray<SharedPointerT<PerformanceCounterInstance>, 1024> instanceList;
		m_TotalCounterInstance = 0;

		svrChk(super::StartTransaction());

		m_TotalCounterInstance = PerformanceCounterServer::GetInstanceCount();

		m_TotalCounterInstance = PerformanceCounterServer::GetInstanceList(0, instanceList);

		for (UINT iInstance = 0; iInstance < instanceList.GetSize(); iInstance++)
		{
			auto pInstance = instanceList[iInstance];
			if (pInstance == nullptr)
				break;

			PerformanceCounterInstanceInfo info;
			StrUtil::StringCpy(info.InstanceName, pInstance->GetInstanceName());
			info.InstanceUID = pInstance->GetInstanceEntityUID();
			if (!(m_InstanceList.push_back(info)))
				break;
		}

	Proc_End:

		// We need to retry until it's successed
		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result MonitoringTransRequestCounterValues::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		UINT bufferSize = (UINT)m_CounterValues.GetAllocatedSize();
		svrChk(super::StartTransaction());

		//svrChk(PerformanceCounterServer::GetCounter(GetInstanceUID(), bufferSize, m_CounterValues.data()));
		Assert(false);
		m_CounterValues.SetSize(bufferSize);

	Proc_End:

		// We need to retry until it's successed
		CloseTransaction(hr);

		return hr;
	}


};// namespace Svr 
};// namespace BR 

