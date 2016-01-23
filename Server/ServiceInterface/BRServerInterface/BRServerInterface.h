// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BRMONITORING_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BRMONITORING_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "Common/MemoryPool.h"
#include "Common/ArrayUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"


#ifdef BRSERVERINTERFACE_EXPORTS
#define BRSERVERINTERFACE_API __declspec(dllexport)
#else
#define BRSERVERINTERFACE_API __declspec(dllimport)
#endif


BRSERVERINTERFACE_API int InitializeNativeSystem(const char* serviceName, const char* modulePath, const char* logCfgPath);
BRSERVERINTERFACE_API int TerminateNativeSystem();

BRSERVERINTERFACE_API BR::Array<BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>>* CreatePerformanceCounterInstanceArray();

BRSERVERINTERFACE_API BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>* CreatePerformanceCounterInstance();
