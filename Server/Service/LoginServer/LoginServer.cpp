// LoginServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LoginServer.h"
#include "ServerSystem/BrService.h"
#include "ServerSystem/SvrTrace.h"
#include "LoginServerClass.h"

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
	SharedPointerT<BR::LoginServer::LoginServer> pServerInstance;

	BR::MemoryPoolManager::Initialize();

	pServerInstance = SharedPointerT<BR::LoginServer::LoginServer>(new BR::LoginServer::LoginServer);

	svrChk(BR::Svr::Service::ServiceRun(cmdArgs, (BR::LoginServer::LoginServer*)pServerInstance));


Proc_End:

	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<BR::LoginServer::LoginServer>();
	}


	BR::MemoryPoolManager::Terminate();

	return 0;
}
