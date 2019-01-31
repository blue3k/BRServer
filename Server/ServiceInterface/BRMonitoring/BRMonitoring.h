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


#ifdef BRMONITORING_EXPORTS
#define BRMONITORING_API __declspec(dllexport)
#else
#define BRMONITORING_API __declspec(dllimport)
#endif


BRMONITORING_API int InitializeNativeSystem(const char* serviceName, const char* modulePath, const char* logCfgPath);
BRMONITORING_API int TerminateNativeSystem();

BRMONITORING_API BR::Array<BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>>* CreatePerformanceCounterInstanceArray();

BRMONITORING_API BR::SharedPointerT<BR::Svr::PerformanceCounterInstance>* CreatePerformanceCounterInstance();
