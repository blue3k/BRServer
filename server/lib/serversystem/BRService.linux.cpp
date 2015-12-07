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
				m_StopSignaled = true;
				//exit(0);
				break;
			}
		}

		static void daemonize()
		{
			char lockFileName[1024];
			int lfp;
			char str[10];
			if (getppid() == 1) return; /* already a daemon */
			int folkRes = fork();
			if (folkRes < 0) exit(1); /* fork error */
			if (folkRes > 0) exit(0); /* parent exits */
							  /* child (daemon) continues */
			setsid(); /* obtain a new process group */
			for (int iDescriptor = getdtablesize(); iDescriptor >= 0; --iDescriptor) close(iDescriptor); /* close all descriptors */
			int i = open("/dev/null", O_RDWR); dup(i); dup(i); /* handle standart I/O */
			umask(027); /* set newly created file permissions */
			//chdir(RUNNING_DIR); /* change running directory */
			snprintf(lockFileName, sizeof(lockFileName), "%s.Lock", Util::GetModuleNameA());
			lfp = open(lockFileName, O_RDWR | O_CREAT, 0640);
			if (lfp<0) exit(1); /* can not open */
			if (lockf(lfp, F_TLOCK, 0)<0) exit(0); /* can not lock */
												   /* first instance continues */
			sprintf(str, "%d\n", getpid());
			write(lfp, str, strlen(str)); /* record pid to lockfile */

			m_StopSignaled = false;

			signal(SIGCHLD, SIG_IGN); /* ignore child */
			signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
			signal(SIGTTOU, SIG_IGN);
			signal(SIGTTIN, SIG_IGN);
			signal(SIGHUP, signal_handler); /* catch hangup signal */
			signal(SIGTERM, signal_handler); /* catch kill signal */
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

		// Run service main function
		HRESULT ServiceRun(std::vector<std::string>& cmdArgs, BrServer *pSvrInstance )
		{
			HRESULT hr = S_OK;
			bool bIsDebugRun = false;
			std::string strCfgPath = Util::GetModulePathA();
			bool bIsInstall = false;
			//wchar_t *strUser = nullptr; wchar_t *strPWD = nullptr;
			const char *strServiceName = nullptr;

			chdir(Util::GetModulePathA());

			strCfgPath.append("../../Config/ServerConfig.xml");


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
					else if( StrUtil::StringCmpLwr( pCurParam, (INT)strlen(pCurParam), "debug", (INT)strlen("debug") ) == 0 )
					{
						bIsDebugRun = true;
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


			Trace::Initialize();

			Net::RegisterConnectionDebugMessage();

			if( bIsInstall )
			{
				// Nothing to do
				goto Proc_End;
			}

			if( bIsDebugRun )
			{
				Trace::AllocScreenConsole();
			}

			svrTrace( Trace::TRC_TRACE, "Loading configuration" );

			svrChk( Svr::Config::LoadConfig( strCfgPath.c_str() ) );


			svrTrace( Trace::TRC_TRACE, "<{0}> Start with Mode {1} ", Util::GetServiceName(), bIsDebugRun ? "Debug" : "Service" );

			if (!bIsDebugRun)
			{
				daemonize();
			}

			// if not service mode
			//if( bIsDebugRun )
			{
				svrChk( g_pSvrInstance->StartServer() );

				bool bRun = true;
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

				switch( g_pSvrInstance->GetServerState() )
				{
				case ServerState::STARTING:
				case ServerState::RUNNING:
					svrChk( g_pSvrInstance->StopServer() );
				case ServerState::STOPED:
				case ServerState::STOPING:
				default:
					break;
				};
			}


		Proc_End:

			svrTrace( Trace::TRC_TRACE, "<%0%> Closed", Util::GetServiceName() );

			g_pSvrInstance = nullptr;

			Trace::Uninitialize();

			return hr;
		}

	};
	/*
	UNIX Daemon Server Programming Sample Program
	Levent Karakas <levent at mektup dot at> May 2001

	To compile:	cc -o exampled examped.c
	To run:		./exampled
	To test daemon:	ps -ef|grep exampled (or ps -aux on BSD systems)
	To test log:	tail -f /tmp/exampled.log
	To test signal:	kill -HUP `cat /tmp/exampled.lock`
	To terminate:	kill `cat /tmp/exampled.lock`
	*/




}; // namespace BR {
}; // namespace Svr {


#else
void Dummy_BRService() {}
#endif

