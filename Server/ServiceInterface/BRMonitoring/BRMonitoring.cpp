// BRMonitoring.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Common/MemoryPool.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/BrLibComponents.h"
#include "Common/TraceComponent.h"
#include "ServerSystem/SvrTrace.h"
#include "BRMonitoring.h"
#include "Common/BrXML.h"
#include "Common/Utility.h"

//// This is an example of an exported variable

static BR::LibComponentCarrier g_libComponents;

using namespace BR;

BRMONITORING_API int InitializeNativeSystem(const char* serviceName)
{
	HRESULT hr = S_OK;

	xmlInitParser();

	BR::Util::SetServiceName(serviceName);

	defChk(g_libComponents.AddComponent<BR::LibComponentTrace>());
	defChk(g_libComponents.AddComponent<BR::Util::LibComponentTime>());
	defChk(g_libComponents.AddComponent<BR::MemoryPoolManager>());

	defChk(g_libComponents.InitializeComponents());

Proc_End:

	return hr;
}

BRMONITORING_API int TerminateNativeSystem(void)
{
	HRESULT hr = S_OK;

	g_libComponents.TerminateComponents();


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

