////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Performance counter Instance
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "String/SFStrFormat.h"
#include "SvrTrace.h"
#include "PerformanceCounter/PerformanceCounterInstance.h"
#include "PerformanceCounter/PerformanceCounterClient.h"





namespace SF {

	template class SharedPointerT < Svr::PerformanceCounterInstance >;
	template class WeakPointerT < Svr::PerformanceCounterInstance >;

namespace Svr {




	PerformanceCounterInstance::PerformanceCounterInstance(const char *strInstanceName, EntityUID instanceEntityUID)
		: m_InstanceEntityUID(instanceEntityUID)
		, m_SyncSerial(0)
		, m_Counters(GetSystemHeap())
		, m_pManager(nullptr)
	{
		AssertRel(strInstanceName);
		AssertRel(m_InstanceEntityUID.UID != 0);

		StrUtil::StringCopy(m_pInstanceName, strInstanceName);

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
};	// namespace SF


