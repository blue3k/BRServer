// BRMonitoring.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Common/MemoryPool.h"
#include "Common/TimeUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "BRMonitoring.h"
#include "Common/BrXML.h"
#include "Common/Utility.h"

//// This is an example of an exported variable

BRMONITORING_API int InitializeNativeSystem(const char* serviceName)
{
	HRESULT hr = S_OK;

	xmlInitParser();

	BR::Util::SetServiceNameA(serviceName);

	BR::MemoryPoolManager::Initialize();
	BR::Util::Time.InitializeTimer();
	BR::Trace::InitExceptionHandler();
	BR::Trace::Initialize();

Proc_End:

	return hr;
}

BRMONITORING_API int TerminateNativeSystem(void)
{
	HRESULT hr = S_OK;

	BR::Trace::Uninitialize();
	BR::MemoryPoolManager::Terminate();
	BR::Util::Time.TerminateTimer();


	/*
	* Cleanup function for the XML library.
	*/
	xmlCleanupParser();
	/*
	* this is to debug memory for regression tests
	*/
	xmlMemoryDump();

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