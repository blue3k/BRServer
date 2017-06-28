////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter Instance
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrFormat.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"





namespace BR {

	template class SharedPointerT < Svr::PerformanceCounterInstance >;
	template class WeakPointerT < Svr::PerformanceCounterInstance >;

namespace Svr {




	PerformanceCounterInstance::PerformanceCounterInstance(const char *strInstanceName, EntityUID instanceEntityUID)
		: m_InstanceEntityUID(instanceEntityUID)
		, m_SyncSerial(0)
		, m_pManager(nullptr)
	{
		AssertRel(strInstanceName);
		AssertRel(m_InstanceEntityUID != 0);

		StrUtil::StringCpy(m_pInstanceName, strInstanceName);

		m_LatestUpdated = Util::Time.GetTimeMs();
	}

	void PerformanceCounterInstance::SetUpdatedTime(TimeStampMS updatedTime)
	{
		m_LatestUpdated = updatedTime;
	}

	void PerformanceCounterInstance::AssignManager(PerformanceCounterClient* pManager)
	{
		m_pManager = pManager;
	}


	void PerformanceCounterInstance::Dispose()
	{
		if (m_pManager != nullptr)
			m_pManager->RemovePerformanceCounterInstance(this);
	}

	// Register this instance to client
	void PerformanceCounterInstance::RegisterToClient()
	{
		PerformanceCounterClient::RegisterPerformanceCounterInstance(this);
	}

	Result PerformanceCounterInstance::AddCounter(PerformanceCounter* pCounter)
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr(pCounter);

		svrChk(m_Counters.push_back(pCounter));

	Proc_End:

		return hr;
	}



};	// Svr
};	// namespace BR


