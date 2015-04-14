// GameServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GameServer.h"
#include "Shellapi.h"
#include "ServerSystem/BRService.h"
#include "ServerSystem/SvrTrace.h"
#include "SharedModuleServerClass.h"

#include "Common/MemoryPool.h"


using namespace BR;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
		HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	#else
		HRESULT hRes = CoInitialize(NULL);
	#endif
		_ASSERTE(SUCCEEDED(hRes));


	HRESULT hr = S_OK;
	SharedPointerT<BR::SharedModuleServer::SharedModuleServer> pServerInstance;

	BR::MemoryPoolManager::Initialize();

	pServerInstance = SharedPointerT<BR::SharedModuleServer::SharedModuleServer>(new BR::SharedModuleServer::SharedModuleServer );

	int nArgs;
	LPWSTR *pCmdArgs = CommandLineToArgvW( lpCmdLine, &nArgs );

	svrChk(BR::Svr::Service::ServiceRun(nArgs, pCmdArgs, (BR::SharedModuleServer::SharedModuleServer*)pServerInstance));

	LocalFree(pCmdArgs);

Proc_End:


	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<BR::SharedModuleServer::SharedModuleServer>();
	}

	BR::MemoryPoolManager::Terminate();

	return 0;
}
