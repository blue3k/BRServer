////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter Instance
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/StrFormat.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterClient.h"





namespace BR {
namespace Svr {

	template SharedPointerT < PerformanceCounterInstance >;
	template WeakPointerT < PerformanceCounterInstance >;



	PerformanceCounterInstance::PerformanceCounterInstance(const char *strInstanceName, EntityUID instanceEntityUID)
		: m_InstanceEntityUID(instanceEntityUID)
		, m_pManager(nullptr)
		, m_SyncSerial(0)
	{
		HRESULT hr = S_OK;

		AssertRel(strInstanceName);
		AssertRel(m_InstanceEntityUID != 0);

		StrUtil::StringCpy(m_pInstanceName, strInstanceName);

		m_LatestUpdated = Util::Time.GetTimeMs();

	Proc_End:

		hr;
	}

	void PerformanceCounterInstance::SetUpdatedTime(ULONG updatedTime)
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

	HRESULT PerformanceCounterInstance::AddCounter(PerformanceCounter* pCounter)
	{
		HRESULT hr = S_OK;

		svrChkPtr(pCounter);

		svrChk(m_Counters.push_back(pCounter));

	Proc_End:

		return hr;
	}



};	// Svr
};	// namespace BR


