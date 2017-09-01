// GameServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Common/BrLibComponents.h"
#include "Util/TimeUtil.h"
#include "Common/TraceComponent.h"
#include "Common/DefaultLibComponent.h"
#include "Memory/MemoryPool.h"
#include "ServerSystem/BrService.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ParameterSetting.h"


#include "GameServer.h"
#include "GameServerClass.h"

using namespace BR;


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
	SharedPointerT<BR::GameServer::GameServer> pServerInstance;

	svrChk(BR::Svr::Service::ServicePrepare());

	svrChk(LibComponentManager::GetInstance().AddComponent<LibComponentDefault>());
	svrChk(LibComponentManager::GetInstance().AddComponent<LibComponentTrace>());
	svrChk(LibComponentManager::GetInstance().AddComponent<Util::LibComponentTime>());
	svrChk(LibComponentManager::GetInstance().AddComponent<MemoryPoolManager>());

	svrChk(LibComponentManager::GetInstance().InitializeComponents());


	pServerInstance = SharedPointerT<BR::GameServer::GameServer>(new BR::GameServer::GameServer);

	svrChk(BR::Svr::Service::ServiceRun((BR::GameServer::GameServer*)pServerInstance));


Proc_End:

	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<BR::GameServer::GameServer>();
	}


	LibComponentManager::GetInstance().TerminateComponents();

	return 0;
}
