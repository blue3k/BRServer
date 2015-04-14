// BRMonitoring.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Common/MemoryPool.h"
#include "Common/TimeUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "BRMonitoring.h"


//// This is an example of an exported variable

BRMONITORING_API int InitializeNativeSystem(void)
{
	HRESULT hr = S_OK;

	BR::MemoryPoolManager::Initialize();
	BR::Util::Time.InitializeTimer();

Proc_End:

	return hr;
}

BRMONITORING_API int TerminateNativeSystem(void)
{
	HRESULT hr = S_OK;

	BR::MemoryPoolManager::Terminate();
	BR::Util::Time.TerminateTimer();

Proc_End:

	return hr;
}

BRMONITORING_API BR::Array<BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>>* CreatePerformanceCounterInstanceArray()
{
	return new BR::DynamicArray < BR::SharedPointerT<BR::Svr::PerformanceCounterInstance> >;
}

BRMONITORING_API BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>* CreatePerformanceCounterInstance()
{
	return new BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>;
}