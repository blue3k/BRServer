// GameServer.cpp : Defines the entry point for the application.
//

#include "ModuleServerPCH.h"
#include "Component/SFLibraryComponent.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLogComponent.h"
#include "Server/BrService.h"
#include "SvrTrace.h"
#include "Server/ParameterSetting.h"

#include "GameServer.h"
#include "SharedModuleServerClass.h"


using namespace SF;


#if SF_PLATFORM == SF_PLATFORM_WINDOWS
int main(int numArg, const char* argc[])
{

#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
	Result hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	Result hr = CoInitialize(NULL);
#endif

	//ParameterSetting::SetSetting("config", "..\\..\\Config\\ServerConfig.xml");
	ParameterSetting::ProcessParameter(numArg, argc);

#elif SF_PLATFORM == SF_PLATFORM_LINUX
int main(int numArg, const char* argc[])
{
	Result hr;
	//ParameterSetting::SetSetting("config", "../../Config/ServerConfig_linux.xml");
	ParameterSetting::ProcessParameter(numArg, argc);
#endif


	SharedPointerT<SharedModuleServer::SharedModuleServer> pServerInstance;

	svrChk(Svr::Service::ServicePrepare());

	SF::InitializeEngineForServer();


	pServerInstance = SharedPointerT<SharedModuleServer::SharedModuleServer>(new(GetSystemHeap()) SharedModuleServer::SharedModuleServer );


	svrChk(Svr::Service::ServiceRun((SharedModuleServer::SharedModuleServer*)pServerInstance));


Proc_End:


	if (pServerInstance != nullptr)
	{
		pServerInstance->TerminateEntity();
		pServerInstance->OnRemovedFromTaskManager(nullptr);
		pServerInstance = SharedPointerT<SharedModuleServer::SharedModuleServer>();
	}

	SF::DeinitializeEngine();

	return 0;
}
