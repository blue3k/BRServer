// GameServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Object/SFLibraryComponent.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLogComponent.h"
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Server/ParameterSetting.h"


#include "GameServer.h"
#include "ConspiracyGameInstanceServerClass.h"

using namespace SF;




#if SF_PLATFORM == SF_PLATFORM_WINDOWS
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

	ParameterSetting::SetSetting("config", "..\\..\\Config\\ServerConfig.xml");
	ParameterSetting::ProcessParameter(lpCmdLine);

#elif SF_PLATFORM == SF_PLATFORM_LINUX
int main(int numArg, const char* argc[])
{
	ParameterSetting::SetSetting("config", "../../Config/ServerConfig_linux.xml");
	ParameterSetting::ProcessParameter(numArg, argc);
#endif


	Result hr = ResultCode::SUCCESS;
	SharedPointerT<ConspiracyGameInstanceServer::GameInstanceServer> pServerInstance;

	svrChk(Svr::Service::ServicePrepare());
	SF::Svr::InitializeEngineForServer();

	pServerInstance = SharedPointerT<ConspiracyGameInstanceServer::GameInstanceServer>(new(GetSystemHeap()) ConspiracyGameInstanceServer::GameInstanceServer );

	svrChk(Svr::Service::ServiceRun((ConspiracyGameInstanceServer::GameInstanceServer*)pServerInstance));


Proc_End:

	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<ConspiracyGameInstanceServer::GameInstanceServer>();
	}

	SF::Svr::DeinitializeEngine();

	return 0;
}
