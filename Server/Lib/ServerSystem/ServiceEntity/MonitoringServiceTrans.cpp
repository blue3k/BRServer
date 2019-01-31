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
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/Policy/MonitoringNetPolicy.h"


#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/MonitoringServiceTrans.h"
#include "ServiceEntity/MonitoringServiceEntity.h"

#include "PerformanceCounter/PerformanceCounterInstance.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterServer.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::MonitoringTransGetInstanceList);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::MonitoringTransRequestCounterValues);



namespace SF {
namespace Svr {



	// Start Transaction
	Result MonitoringTransGetInstanceList::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		StaticArray<SharedPointerT<PerformanceCounterInstance>, 1024> instanceList(GetHeap());
		m_TotalCounterInstance = 0;

		svrChk(super::StartTransaction());

		m_TotalCounterInstance = PerformanceCounterServer::GetInstanceCount();

		m_TotalCounterInstance = PerformanceCounterServer::GetInstanceList(0, instanceList);

		for (uint iInstance = 0; iInstance < instanceList.size(); iInstance++)
		{
			auto pInstance = instanceList[iInstance];
			if (pInstance == nullptr)
				break;

			PerformanceCounterInstanceInfo info;
			StrUtil::StringCopy(info.InstanceName, pInstance->GetInstanceName());
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
		uint bufferSize = (uint)m_CounterValues.GetAllocatedSize();
		svrChk(super::StartTransaction());

		//svrChk(PerformanceCounterServer::GetCounter(GetInstanceUID(), bufferSize, m_CounterValues.data()));
		Assert(false);
		m_CounterValues.resize(bufferSize);

	Proc_End:

		// We need to retry until it's successed
		CloseTransaction(hr);

		return hr;
	}


};// namespace Svr 
};// namespace SF 

