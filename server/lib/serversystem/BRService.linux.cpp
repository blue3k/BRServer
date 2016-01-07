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

#include "stdafx.h"
#include "Common/HRESEvent.h"
#include "Common/HRESSvrSys.h"
#include "SvrTrace.h"
#include "BrService.h"
#include "Common/TimeUtil.h"
#include "Net/NetUtil.h"


#if LINUX
#include <signal.h>

namespace BR {
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
			switch (sig) {
			case SIGHUP:
				break;
			case SIGTERM:
				printf("Stop Signaled");
				defTrace(Trace::TRC_TRACE, "Stop Signaled");
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
			//for (int iDescriptor = getdtablesize(); iDescriptor >= 0; --iDescriptor) close(iDescriptor); /* close all descriptors */
			int i = open("/dev/null", O_RDWR); dup(i); dup(i); /* handle standart I/O */
			//umask(027); /* set newly created file permissions */
			//chdir(RUNNING_DIR); /* change running directory */

			// Disable lock file check
			//snprintf(lockFileName, sizeof(lockFileName), "%s.Lock", Util::GetModuleNameA());
			//lfp = open(lockFileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);// 0640);
			//if (lfp < 0)
			//{
			//	printf("Faild to get lock. pid:%d\n", getpid());
			//	exit(1); /* can not open */
			//}

			//if (lockf(lfp, F_TLOCK, 0) < 0)
			//{
			//	printf("Faild to get lock2. pid:%d\n", getpid());
			//	exit(0); /* can not lock */
			//}
												   /* first instance continues */
			//sprintf(str, "%d\n", getpid());
			//write(lfp, str, strlen(str)); /* record pid to lockfile */

			m_StopSignaled = false;

			//printf("Registring signals pid:%d\n", getpid());

			signal(SIGCHLD, SIG_IGN); /* ignore child */
			signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
			signal(SIGTTOU, SIG_IGN);
			signal(SIGTTIN, SIG_IGN);
		}




		// Install Service
		HRESULT ServiceInstall( const char *strCfgPath, const char *strUser, const char *strPWD )
		{
			HRESULT hr = S_OK;
			return hr;
		}

		// Uninstall service
		HRESULT ServiceUninstall()
		{
			HRESULT hr = S_OK;
			return hr;
		}

		// prepare service running
		HRESULT ServicePrepare()
		{
			//daemonize();
			return S_OK;
		}

		// Run service main function
		HRESULT ServiceRun(std::vector<std::string>& cmdArgs, BrServer *pSvrInstance )
		{
			HRESULT hr = S_OK;
			std::string strCfgPath = Util::GetModulePathA();
			bool bIsInstall = false;
			const char *strServiceName = nullptr;
			bool bRun = false;

			chdir(Util::GetModulePathA());

			strCfgPath.append("../../Config/ServerConfig_linux.xml");


			g_pSvrInstance = pSvrInstance;
			Assert( BrServer::GetInstance() == pSvrInstance );


			for (auto itArg : cmdArgs)
			{
				const char* pCurParam = itArg.c_str();

				switch( pCurParam[0] )
				{
				case L'-':
					pCurParam++;
					if( StrUtil::StringCmpLwr( pCurParam, (INT)strlen(pCurParam), "install", (INT)strlen("install") ) == 0 )
					{
						bIsInstall = true;
					}
					else if( pCurParam[0] == 'n' || pCurParam[0] == 'N' )
					{
						pCurParam += 2;
						strServiceName = pCurParam;
					}
					else if( StrUtil::StringCmpLwr( pCurParam, (INT)strlen(pCurParam), "uninstall", (INT)strlen("uninstall") ) == 0 )
					{
						svrChk( Service::ServiceUninstall() );
						goto Proc_End;
					}
					break;
				default:
					break;
				};
			}

			if( strServiceName != nullptr )
			{
				Util::SetServiceName(strServiceName);
			}


			//Trace::Initialize();

			Net::RegisterConnectionDebugMessage();

			if( bIsInstall )
			{
				// Nothing to do
				goto Proc_End;
			}

			svrTrace( Trace::TRC_TRACE, "Loading configuration" );

			svrChk( Svr::Config::LoadConfig( strCfgPath.c_str() ) );

			svrTrace( Trace::TRC_TRACE, "<{0}> Start with Mode {1} ", Util::GetServiceName(), "Service" );

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

			svrTrace( Trace::TRC_TRACE, "<{0}> Closed", Util::GetServiceName() );

			//Trace::Uninitialize();

			g_pSvrInstance = nullptr;

			return hr;
		}

	};




}; // namespace BR {
}; // namespace Svr {


#else
void Dummy_BRService() {}
#endif

