// EntityServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Common/BrLibComponents.h"
#include "Common/TimeUtil.h"
#include "Common/TraceComponent.h"
#include "EntityServer.h"
#include "ServerSystem/BrService.h"
#include "ServerSystem/SvrTrace.h"
#include "EntityServerClass.h"
#include "Common/MemoryPool.h"



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
		HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	#else
		HRESULT hRes = CoInitialize(NULL);
	#endif
		_ASSERTE(SUCCEEDED(hRes));

	std::string cmdLine = lpCmdLine;
	std::istringstream iss(cmdLine);
	std::vector<std::string> cmdArgs{ 
		std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{} };

#elif LINUX
int main(int numArg, const char* argc[])
{
	std::vector<std::string> cmdArgs;
	for (int iArg = 0; iArg < numArg; iArg++) cmdArgs.push_back(argc[iArg]);
#endif


	HRESULT hr = S_OK;
	SharedPointerT<BR::EntityServer::EntityServer> pServerInstance;
	LibComponentCarrier libComponents;

	svrChk(BR::Svr::Service::ServicePrepare());

	svrChk(libComponents.AddComponent<LibComponentTrace>());
	svrChk(libComponents.AddComponent<Util::LibComponentTime>());
	svrChk(libComponents.AddComponent<MemoryPoolManager>());

	svrChk(libComponents.InitializeComponents());


	pServerInstance = SharedPointerT<BR::EntityServer::EntityServer>(new BR::EntityServer::EntityServer);


	svrChk(BR::Svr::Service::ServiceRun(cmdArgs, (BR::EntityServer::EntityServer*)pServerInstance));


Proc_End:

	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<BR::EntityServer::EntityServer>();
	}

	libComponents.TerminateComponents();


	return (int)0;
}


