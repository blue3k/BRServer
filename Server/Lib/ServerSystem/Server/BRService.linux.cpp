////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server service implementation. 
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "SvrTrace.h"
#include "BrService.h"
#include "Util/SFTimeUtil.h"
#include "Net/SFNetUtil.h"
#include "Server/ParameterSetting.h"
#include "Service/ServerService.h"


#if SF_PLATFORM == SF_PLATFORM_LINUX
#include <signal.h>

namespace SF {
namespace Svr {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Service control
	//

	namespace Service
	{
		BrServer *g_pSvrInstance = nullptr;

		// Service Main handler
		static bool m_StopSignaled = false;
		static void signal_handler(int sig)
		{
			switch (sig)
			{
			case SIGHUP:
				break;
			case SIGTERM:
				printf("Stop Signaled");
				defTrace(Info, "Stop Signaled");
				m_StopSignaled = true;
				//exit(0);
				break;
			}
		}

		static void daemonize()
		{
			//char lockFileName[1024];
			//int lfp;
			//char str[10];
			if (getppid() == 1) return; /* already a daemon */
			int folkRes = fork();
			if (folkRes < 0)
			{
				/* fork error */
				printf("fork error %d\n", errno);
				exit(1); 
			}
			if (folkRes > 0)
			{
				/* parent exits */
				//printf("fork done parent exit\n");
				exit(0);
			}
			else
			{
				/* child (daemon) continues */
			}


			setsid(); /* obtain a new process group */
			printf("Service pid:%d, tid:%d\n", getpid(), (int)syscall(SYS_gettid));
			for (int iDescriptor = getdtablesize(); iDescriptor >= 0; --iDescriptor) close(iDescriptor); /* close all descriptors */
			int i = open("/dev/null", O_RDWR); dup(i); dup(i); /* handle standart I/O */

			/* Close out the standard file descriptors */
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			//umask(027); /* set newly created file permissions */
			//chdir(RUNNING_DIR); /* change running directory */

			// Disable lock file check
			//snprintf(lockFileName, sizeof(lockFileName), "%s.Lock", Util::GetModuleNameA());
			//lfp = open(lockFileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);// 0640);
			//if (lfp < 0)
			//{
			//	printf("Failed to get lock. pid:%d\n", getpid());
			//	exit(1); /* can not open */
			//}

			//if (lockf(lfp, F_TLOCK, 0) < 0)
			//{
			//	printf("Failed to get lock2. pid:%d\n", getpid());
			//	exit(0); /* can not lock */
			//}
												   /* first instance continues */
			//sprintf(str, "%d\n", getpid());
			//write(lfp, str, strlen(str)); /* record pid to lockfile */

			m_StopSignaled = false;

			//printf("Registering signals pid:%d\n", getpid());

			signal(SIGCHLD, SIG_IGN); /* ignore child */
			signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
			signal(SIGTTOU, SIG_IGN);
			signal(SIGTTIN, SIG_IGN);
		}




		// Install Service
		Result ServiceInstall()
		{
			Result hr = ResultCode::SUCCESS;
			return hr;
		}

		// Uninstall service
		Result ServiceUninstall()
		{
			Result hr = ResultCode::SUCCESS;
			return hr;
		}

		// prepare service running
		Result ServicePrepare()
		{
			chdir(Util::GetModulePath());

			auto serviceModeSetting = ParameterSetting::GetSetting("servicemode");
			bool bAsService = StrUtil::StringCompairIgnoreCase(serviceModeSetting, -1, "true", -1);
			if(bAsService)
				daemonize();

			return ResultCode::SUCCESS;
		}

		// Run service main function
		Result ServiceRun(BrServer *pSvrInstance )
		{
			Result hr = ResultCode::SUCCESS;
			const char *strServiceName = nullptr;
			bool bRun = false;


			g_pSvrInstance = pSvrInstance;
			Assert( BrServer::GetInstance() == pSvrInstance );


			strServiceName = ParameterSetting::GetSetting("servicename");
			if( strServiceName != nullptr && strServiceName[0] != '\0' )
			{
				Util::SetServiceName(strServiceName);
			}


			Net::RegisterConnectionDebugMessage();


			svrTrace( Info, "<{0}> Starting", Util::GetServiceNameA() );

			// register signal handlers
			signal(SIGHUP, signal_handler); /* catch hangup signal */
			signal(SIGTERM, signal_handler); /* catch kill signal */


			svrChk( g_pSvrInstance->StartServer() );

			bRun = true;
			while( bRun )
			{
				switch( g_pSvrInstance->GetServerState() )
				{
				case ServerState::STOPED:
					bRun = false;
					break;
				default:
					break;
				};

				if(m_StopSignaled || g_pSvrInstance->GetServerState() == ServerState::STOPED)
				{
					bRun = false;
				}

				ThisThread::SleepFor(DurationMS(1000));
			}


		Proc_End:

			g_pSvrInstance->StopServer();

			svrTrace( Info, "<{0}> Closed", Util::GetServiceNameA() );

			//Trace::Uninitialize();

			g_pSvrInstance = nullptr;

			return hr;
		}

	};




}; // namespace SF {
}; // namespace Svr {


#else
void Dummy_BRService() {}
#endif

