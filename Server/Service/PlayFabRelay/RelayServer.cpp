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

	printf("test 1\n");

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

	SharedPointerT<RelayServer> pServerInstance;

	FunctionContext hr([&pServerInstance](Result result)
	{
		if (!result && pServerInstance != nullptr)
		{
			pServerInstance->TerminateEntity();
			pServerInstance->OnRemovedFromTaskManager(nullptr);
			pServerInstance = SharedPointerT<RelayServer>();
		}

		printf("test 2\n");

		SF::Svr::DeinitializeEngine();
	});

	printf("test 3\n");

	Svr::Service::ServicePrepare();

	printf("test 4\n");

	uint32_t workerThreadCount = 1;
	uint32_t netIOThreadCount = 2;
	SF::Svr::InitializeEngineForPlayFabServer(workerThreadCount, netIOThreadCount);

	LogChannelMask LogOutputGSDK = { 0 };

	if (SF::Engine::GetInstance())
		SF::Engine::GetInstance()->AddComponent<LogOutputPlayFabGSDKComponent>(LogOutputGSDK);

	printf("test 5\n");

	pServerInstance = SharedPointerT<RelayServer>(new(GetSystemHeap()) RelayServer);

	Svr::Service::ServiceRun(*pServerInstance);

	return 0;
}
