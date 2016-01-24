// BRMonitoring.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Common/MemoryPool.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/BrLibComponents.h"
#include "Common/DefaultLibComponent.h"
#include "Common/TraceComponent.h"
#include "Common/BrXML.h"
#include "Common/Utility.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ParameterSetting.h"

#include "BRServerInterface.h"




using namespace BR;



BRSERVERINTERFACE_API int InitializeNativeSystem(const char* serviceName, const char* modulePath, const char* logCfgPath)
{
	HRESULT hr = S_OK;

	ParameterSetting::SetSetting("modulepath", modulePath);
	ParameterSetting::SetSetting("servicename", serviceName);
	ParameterSetting::SetSetting("config", logCfgPath);

	BR::Util::SetServiceName(serviceName);

	defChk(LibComponentManager::GetInstance().AddComponent<BR::LibComponentDefault>());
	defChk(LibComponentManager::GetInstance().AddComponent<BR::LibComponentTrace>(modulePath, serviceName, logCfgPath));
	defChk(LibComponentManager::GetInstance().AddComponent<BR::Util::LibComponentTime>());
	defChk(LibComponentManager::GetInstance().AddComponent<BR::MemoryPoolManager>());

	defChk(LibComponentManager::GetInstance().InitializeComponents());

	svrTrace(Trace::TRC_TRACE, "Starting native system, {0}", Util::GetServiceNameA());

Proc_End:

	return hr;
}

BRSERVERINTERFACE_API int TerminateNativeSystem(void)
{
	HRESULT hr = S_OK;

	svrTrace(Trace::TRC_TRACE, "Native system terminated {0}", Util::GetServiceNameA());

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

