// EntityServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Object/LibraryComponent.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLogComponent.h"
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Server/ParameterSetting.h"

#include "EntityServer.h"
#include "EntityServerClass.h"


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

	ParameterSetting::SetSetting("config", "..\\..\\Config\\ServerConfig.xml");
	ParameterSetting::ProcessParameter(lpCmdLine);

#elif LINUX
int main(int numArg, const char* argc[])
{
	ParameterSetting::SetSetting("config", "../../Config/ServerConfig_linux.xml");
	ParameterSetting::ProcessParameter(numArg, argc);
#endif



	Result hr = ResultCode::SUCCESS;
	SharedPointerT<EntityServer::EntityServer> pServerInstance;

	svrChk(Svr::Service::ServicePrepare());

	SF::Svr::InitializeEngineForServer();


	pServerInstance = new(GetSystemMemoryManager()) EntityServer::EntityServer;


	svrChk(Svr::Service::ServiceRun((EntityServer::EntityServer*)pServerInstance));


Proc_End:

	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<EntityServer::EntityServer>();
	}

	SF::Svr::DeinitializeEngine();


	return (int)0;
}


