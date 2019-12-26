////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : RelayServer
//
////////////////////////////////////////////////////////////////////////////////

#include "RelayServerPCH.h"
#include "Object/SFLibraryComponent.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLogComponent.h"
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Server/ParameterSetting.h"

#include "RelayServer.h"
#include "RelayServerClass.h"

#include "gsdk.h"

using namespace SF;


#if SF_PLATFORM == SF_PLATFORM_WINDOWS
int main(int numArg, const char* argc[])
{
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
	Result hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	Result hRes = CoInitialize(NULL);
#endif
	_ASSERTE((hRes));

	ParameterSetting::ProcessParameter(numArg, argc);

#elif SF_PLATFORM == SF_PLATFORM_LINUX
int main(int numArg, const char* argc[])
{
	ParameterSetting::ProcessParameter(numArg, argc);
#endif


	Result hr = ResultCode::SUCCESS;
	SharedPointerT<RelayServer> pServerInstance;

	svrChk(Svr::Service::ServicePrepare());

	uint32_t workerThreadCount = 1;
	uint32_t netIOThreadCount = 2;
	SF::Svr::InitializeEngineForPlayFabServer(workerThreadCount, netIOThreadCount);

	pServerInstance = SharedPointerT<RelayServer>(new(GetSystemHeap()) RelayServer);

	svrChk(Svr::Service::ServiceRun(*pServerInstance));


Proc_End:


	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<RelayServer>();
	}

	SF::Svr::DeinitializeEngine();

	return 0;
}
