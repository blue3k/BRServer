// ServerInstanceLauncher.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Object/LibraryComponent.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLogComponent.h"
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Server/ParameterSetting.h"

#include "ServerInstanceLauncher.h"
#include "ServerInstanceLauncherClass.h"


using namespace SF;


#if WINDOWS
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
	Result hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	Result hRes = CoInitialize(NULL);
#endif
	_ASSERTE((hRes));

	ParameterSetting::ProcessParameter(lpCmdLine);

#elif LINUX
int main(int numArg, const char* argc[])
{
	ParameterSetting::ProcessParameter(numArg, argc);
#endif



	Result hr = ResultCode::SUCCESS;
	SharedPointerT<ServerInstanceLauncher::ServerInstanceLauncher> pServerInstance;

	svrChk(Svr::Service::ServicePrepare());

	SF::Svr::InitializeEngineForServer();


	pServerInstance = new(GetSystemMemoryManager()) ServerInstanceLauncher::ServerInstanceLauncher;


	svrChk(Svr::Service::ServiceRun((ServerInstanceLauncher::ServerInstanceLauncher*)pServerInstance));


Proc_End:

	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<ServerInstanceLauncher::ServerInstanceLauncher>();
	}

	SF::Svr::DeinitializeEngine();


	return (int)0;
}


