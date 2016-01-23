// BRMonitoring.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Common/MemoryPool.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/BrLibComponents.h"
#include "Common/TraceComponent.h"
#include "ServerSystem/SvrTrace.h"
#include "BRServerInterface.h"
#include "Common/BrXML.h"
#include "Common/Utility.h"





using namespace BR;



BRSERVERINTERFACE_API int InitializeNativeSystem(const char* serviceName, const char* modulePath, const char* logCfgPath)
{
	HRESULT hr = S_OK;

	BR::Util::SetServiceName(serviceName);

	defChk(LibComponentManager::GetInstance().AddComponent<BR::LibComponentTrace>(modulePath, serviceName, logCfgPath));
	defChk(LibComponentManager::GetInstance().AddComponent<BR::Util::LibComponentTime>());
	defChk(LibComponentManager::GetInstance().AddComponent<BR::MemoryPoolManager>());

	defChk(LibComponentManager::GetInstance().InitializeComponents());

Proc_End:

	return hr;
}

BRSERVERINTERFACE_API int TerminateNativeSystem(void)
{
	HRESULT hr = S_OK;

	LibComponentManager::GetInstance().TerminateComponents();



Proc_End:

	return hr;
}

BRSERVERINTERFACE_API BR::Array<BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>>* CreatePerformanceCounterInstanceArray()
{
	return new BR::DynamicArray < BR::SharedPointerT<BR::Svr::PerformanceCounterInstance> >;
}

BRSERVERINTERFACE_API BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>* CreatePerformanceCounterInstance()
{
	return new BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>;
}

