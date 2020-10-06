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
#if SF_PLATFORM == SF_PLATFORM_WINDOWS

#include <conio.h>
#include "ResultCode/SFResultCodeSvr.h"
#include "SvrTrace.h"
#include "Server/BRService.h"
#include "Util/SFTimeUtil.h"
#include "Net/SFNetUtil.h"
#include "Server/ParameterSetting.h"
#include "Service/ServerService.h"
#include "Service/SFEngineService.h"



namespace SF {
namespace Svr {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Service control
	//

	namespace Service
	{
		enum { 
			SVCCTRL_STOP,
			SVCCTRL_PAUSE,
			SVCCTRL_RESUME,
			NUM_SVCCTRLEVT };// Start, stop, Terminate

		SERVICE_STATUS          g_SvcStatus; 
		SERVICE_STATUS_HANDLE   g_SvcStatusHandle; 

		HANDLE                  g_hCtrlEvents[NUM_SVCCTRLEVT] = {NULL,};

		// Server instance to run
		BrServer *g_pSvrInstance = nullptr;

		// state change wait time
		const uint g_uiStateWaitTime = 10; // 10 min





		// Service Main handler
		void WINAPI ServiceMain( DWORD dwArgc, LPSTR *lpszArgv );

		// Service control handler
		void WINAPI ServiceCtrlHandler( DWORD dwCtrl );



		//
		// Purpose: 
		//   Sets the current service status and reports it to the SCM
		//
		// Parameters:
		//   dwCurrentState - The current state (see SERVICE_STATUS)
		//   dwWin32ExitCode - The system error code
		//   dwWaitHint - Estimated time for pending operation, 
		//     in milliseconds
		// 
		// Return value:
		//   None
		//
		void ReportServiceStatus( DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint )
		{
			static DWORD dwCheckPoint = 1;

			// Fill in the SERVICE_STATUS structure.
			g_SvcStatus.dwCurrentState = dwCurrentState;
			g_SvcStatus.dwWin32ExitCode = dwWin32ExitCode;
			g_SvcStatus.dwWaitHint = dwWaitHint;

			if (dwCurrentState == SERVICE_START_PENDING)
				g_SvcStatus.dwControlsAccepted = 0;
			else g_SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

			if ( (dwCurrentState == SERVICE_RUNNING) ||
				   (dwCurrentState == SERVICE_STOPPED) )
				g_SvcStatus.dwCheckPoint = 0;
			else g_SvcStatus.dwCheckPoint = dwCheckPoint++;

			// Report the status of the service to the SCM.
			SetServiceStatus( g_SvcStatusHandle, &g_SvcStatus );
		}


		// Install Service
		Result ServiceInstall()
		{
			Result hr = ResultCode::SUCCESS;
			const char* strServiceName = Util::GetServiceName();

			SC_HANDLE schSCManager = nullptr;
			SC_HANDLE schService = nullptr;

			char strPath[512];


			if( !GetModuleFileNameA(nullptr, strPath, MAX_PATH ) )
			{
				int iErr = GetLastError();
				svrTrace( Error, "GetModuleFileName failed err:{0}", iErr );
				trcErr( HRESULT_FROM_WIN32(iErr) );
			}

			strcat(strPath, " -n:");
			strcat(strPath, strServiceName);

			strcat(strPath, " -servicemode");

			auto logpath = ParameterSetting::GetSetting("logpath");
			if (!StrUtil::IsNullOrEmpty(logpath))
			{
				strcat(strPath, " -logpath:");
				strcat(strPath, logpath);
			}

			auto modulePath = ParameterSetting::GetSetting("modulepath");
			if (!StrUtil::IsNullOrEmpty(modulePath))
			{
				strcat(strPath, " -modulepath:");
				strcat(strPath, modulePath);
			}

			auto zkaddress = ParameterSetting::GetSetting("zkaddress");
			if (!StrUtil::IsNullOrEmpty(zkaddress))
			{
				strcat(strPath, " -zkaddress:");
				strcat(strPath, zkaddress);
			}

			auto zkconfig = ParameterSetting::GetSetting("zkconfig");
			if (!StrUtil::IsNullOrEmpty(zkconfig))
			{
				strcat(strPath, " -zkconfig:");
				strcat(strPath, zkconfig);
			}

			// Get a handle to the SCM database. 
			schSCManager = OpenSCManager( 
				NULL,                    // local computer
				NULL,                    // ServicesActive database 
				SC_MANAGER_ALL_ACCESS);  // full access rights 
			if( schSCManager == nullptr) 
			{
				int iErr = GetLastError();
				svrTrace( Error, "OpenSCManager failed err:{0}", iErr );
				trcErr( HRESULT_FROM_WIN32(iErr) );
			}

			// check service existence
			schService = OpenServiceA(schSCManager, strServiceName, SERVICE_ALL_ACCESS);
			if( schService == NULL) 
			{
				int iErr = GetLastError();
				if( iErr != ERROR_SERVICE_DOES_NOT_EXIST )
				{
					// Service not accessible
					svrTrace( Error, "OpenService failed err:{0}", iErr );
					trcErr( HRESULT_FROM_WIN32(iErr) );
				}

				// Service not exist so create new one

				// Create the service
				schService = CreateServiceA( 
					schSCManager,				// SCM database 
					strServiceName,				// name of service 
					strServiceName,				// service name to display 
					SERVICE_ALL_ACCESS,			// desired access 
					SERVICE_WIN32_OWN_PROCESS,	// service type 
					SERVICE_DEMAND_START,		// start type 
					SERVICE_ERROR_NORMAL,		// error control type 
					strPath,					// path to service's binary 
					NULL,						// no load ordering group 
					NULL,						// no tag identifier 
					NULL,						// no dependencies 
					NULL,						// LocalSystem account 
					NULL);						// no password 
				if( schService == NULL )
				{
					int iErr = GetLastError();
					svrTrace( Error, "CreateService failed err:{0}", iErr );
					trcErr( HRESULT_FROM_WIN32(iErr) );
				}
			}
			else // service exist and opened successfully
			{
				// try to stop the service
				if ( ControlService( schService, SERVICE_CONTROL_STOP, &g_SvcStatus ) )
				{
					ThisThread::SleepFor(DurationMS(500));

					while( QueryServiceStatus( schService, &g_SvcStatus ) )
					{
						if ( g_SvcStatus.dwCurrentState == SERVICE_STOP_PENDING )
						{
							ThisThread::SleepFor(DurationMS(500));
						}
						else
							break;
					}

					if ( g_SvcStatus.dwCurrentState != SERVICE_STOPPED )
					{
						svrTrace( Error, "Service Stop Failed" );
						trcErr( ResultCode::UNEXPECTED );
					}
				}

				// 
				if( !ChangeServiceConfigA( 
						schService, 
						SERVICE_WIN32_OWN_PROCESS,	// service type 
						SERVICE_AUTO_START,		// start type 
						SERVICE_ERROR_NORMAL,		// error control type 
						strPath,					// path to service's binary 
						NULL,						// no load ordering group 
						NULL,						// no tag identifier 
						NULL,						// no dependencies 
						NULL,					// LocalSystem account 
						NULL, 					// password 
						strServiceName))
				{
					int iErr = GetLastError();
					svrTrace( Error, "ChangeServiceConfig failed err:{0}", iErr );
					trcErr( HRESULT_FROM_WIN32(iErr) );
				}
			}


			svrTrace( Info, "Service Installed" );


		Proc_End:

			if( schService )
				CloseServiceHandle( schService );

			if( schSCManager )
				CloseServiceHandle( schSCManager );

			return hr;
		}

		// Uninstall service
		Result ServiceUninstall()
		{
			Result hr = ResultCode::SUCCESS;
			SC_HANDLE   schService = NULL;
			SC_HANDLE   schSCManager = NULL;


			schSCManager = OpenSCManager(
								NULL,                   // machine (NULL == local)
								NULL,                   // database (NULL == default)
								SC_MANAGER_ALL_ACCESS   // access required
								);
			if( schSCManager == NULL) 
			{
				int iErr = GetLastError();
				svrTrace( Error, "OpenSCManager failed err:{0}", iErr );
				trcErr( HRESULT_FROM_WIN32(iErr) );
			}


			schService = OpenServiceA( schSCManager, Util::GetModuleName(), SERVICE_ALL_ACCESS );
			if( schSCManager == NULL) 
			{
				int iErr = GetLastError();
				svrTrace( Error, "OpenService failed err:{0}", iErr );
				trcErr( HRESULT_FROM_WIN32(iErr) );
			}

			// try to stop the service
			if ( ControlService( schService, SERVICE_CONTROL_STOP, &g_SvcStatus ) )
			{
				ThisThread::SleepFor(DurationMS(500));

				while( QueryServiceStatus( schService, &g_SvcStatus ) )
				{
					if ( g_SvcStatus.dwCurrentState == SERVICE_STOP_PENDING )
					{
						ThisThread::SleepFor(DurationMS(500));
					}
					else
						break;
				}

				if ( g_SvcStatus.dwCurrentState != SERVICE_STOPPED )
				{
					svrTrace( Error, "Service Stop Failed" );
					trcErr( ResultCode::UNEXPECTED );
				}
			}


			// now remove the service
			if( !DeleteService(schService) )
			{
				int iErr = GetLastError();
				if( iErr != ERROR_SERVICE_MARKED_FOR_DELETE )
				{
					svrTrace( Error, "DeleteService failed err:{0}", iErr );
					trcErr( HRESULT_FROM_WIN32(iErr) );
				}
			}

			svrTrace( Info, "DeleteService Done" );


		Proc_End:

			if( schService )
				CloseServiceHandle( schService );

			if( schSCManager )
				CloseServiceHandle( schSCManager );

			return hr;
		}

		// prepare service running
		Result ServicePrepare()
		{
			if (ParameterSetting::GetSetting("UseModulePath"))
			{
				SetCurrentDirectoryA(Util::GetModulePath());
			}

			return ResultCode::SUCCESS;
		}

		// Run service main function
		Result ServiceRun(BrServer *pSvrInstance )
		{
			Result hr = ResultCode::SUCCESS;
			bool bAsService = false;
			bool bIsInstall = false;
			bool bIsUninstall = false;
			const char *strServiceName = nullptr;

			g_pSvrInstance = pSvrInstance;
			Assert( BrServer::GetInstance() == pSvrInstance );


			bIsInstall = StrUtil::StringCompairIgnoreCase(ParameterSetting::GetSetting("install"), -1, "true", -1);
			bIsUninstall = StrUtil::StringCompairIgnoreCase(ParameterSetting::GetSetting("uninstall"), -1, "true", -1);
			bAsService = bIsInstall || StrUtil::StringCompairIgnoreCase(ParameterSetting::GetSetting("servicemode"), -1, "true", -1);

			if( bIsInstall )
			{
				svrChk( Service::ServiceInstall() );
				goto Proc_End;
			}

			if (bIsUninstall)
			{
				svrChk(Service::ServiceUninstall());
				goto Proc_End;
			}
			svrTrace( Info, "<{0}> Start with {1} Mode", Util::GetServiceName(), bAsService ? "Service" : "Normal" );


			// if not service mode
			if( !bAsService )
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
					};

					if( g_pSvrInstance->GetServerState() == ServerState::STOPED )
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
			else
			{
				SERVICE_TABLE_ENTRYA DispatchTable[] = 
				{ 
					{ (char*)Util::GetServiceName(), (LPSERVICE_MAIN_FUNCTIONA)ServiceMain },
					{ NULL, NULL } 
				};

				// This call returns when the service has stopped. 
				// The process should simply terminate when the call returns.

				if (!StartServiceCtrlDispatcherA( DispatchTable )) 
				{
					int iError = GetLastError();
					svrTrace( Error, "StartServiceCtrlDispatcher failed err:{0}", iError );
					return HRESULT_FROM_WIN32( iError );
				}
			}


		Proc_End:

			svrTrace( Info, "<{0}> Closed", Util::GetServiceName() );

			g_pSvrInstance = nullptr;

			return hr;
		}

		//
		// Purpose: 
		//   Entry point for the service
		//
		// Parameters:
		//   dwArgc - Number of arguments in the lpszArgv array
		//   lpszArgv - Array of strings. The first string is the name of
		//     the service and subsequent strings are passed by the process
		//     that called the StartService function to start the service.
		// 
		// Return value:
		//   None.
		//
		void WINAPI ServiceMain( DWORD dwArgc, LPSTR *lpszArgv )
		{
			Result hr = ResultCode::SUCCESS;

			// Create events for service control
			for( int iEvt = 0; iEvt < NUM_SVCCTRLEVT; iEvt++ )
			{
				g_hCtrlEvents[iEvt] = CreateEvent(
									 NULL,    // default security attributes
									 TRUE,    // manual reset event
									 FALSE,   // not signaled
									 NULL);   // no name

				if ( g_hCtrlEvents[iEvt] == NULL)
				{
					int iError = GetLastError();
					svrTrace( Error, "CreateEvent failed err:{0}", iError );
					trcErr( HRESULT_FROM_WIN32( iError ) );
				}
			}



			// Register the handler function for the service.
			g_SvcStatusHandle = RegisterServiceCtrlHandlerA( 
				Util::GetServiceName(), 
				ServiceCtrlHandler);

			if( !g_SvcStatusHandle )
			{ 
				int iError = GetLastError();
				svrTrace( Error, "RegisterServiceCtrlHandler failed err:{0}", iError );
				return;
			} 

			// These SERVICE_STATUS members remain as set here.
			g_SvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
			g_SvcStatus.dwServiceSpecificExitCode = 0;    


			// Report initial status to the SCM.
			ReportServiceStatus( SERVICE_START_PENDING, NO_ERROR, 10*60*1000 );



			// Run Server and initialization state check
			if( g_pSvrInstance == NULL )
			{
				svrTrace( Error, "Invalid ServerInstance" );
				ReportServiceStatus( SERVICE_STOPPED, ResultCode::SVR_SERVICE_FAILED, 0 );
				trcErr( ResultCode::UNEXPECTED );
			}


			svrChk( g_pSvrInstance->StartServer() );
			// Report running status when initialization is complete.
			ReportServiceStatus( SERVICE_RUNNING, NO_ERROR, 0 );


			// Check whether to stop the service.
			while( 1 )
			{
				DWORD dwWaitRes = WaitForMultipleObjects( NUM_SVCCTRLEVT, g_hCtrlEvents, FALSE, 3000 );

				if (dwWaitRes != WAIT_TIMEOUT)
				{
					defTrace(Info, "ServiceMain {0}, Res:{1}", __LINE__, (uint32_t)dwWaitRes);
				}

				switch( dwWaitRes )
				{
				case WAIT_OBJECT_0+SVCCTRL_STOP:
					if( g_pSvrInstance->GetServerState() == ServerState::RUNNING )
					{
						trcChk( g_pSvrInstance->StopServer() );

						if( g_pSvrInstance->GetServerState() == ServerState::STOPED )
						{
							ReportServiceStatus( SERVICE_STOPPED, NO_ERROR, 0 );
						}
						else
						{
							trcErr( ResultCode::UNEXPECTED );
						}
					}
					else
					{
						ReportServiceStatus( SERVICE_STOPPED, NO_ERROR, 0 );
					}
					goto Proc_End;
					break;
				case WAIT_OBJECT_0+SVCCTRL_PAUSE:
					//if( g_pSvrInstance->GetState() == Server::STATE_RUNNING )
					//{
					//	trcChk( g_pSvrInstance->StopServer() );
					//	WaitServerState( Server::ServerState::STOPED, SERVICE_PAUSE_PENDING, 2 );

					//	if( g_pSvrInstance->GetState() == Server::ServerState::STOPED )
					//	{
					//		ReportServiceStatus( SERVICE_PAUSED, NO_ERROR, 0 );
					//	}
					//	else
					//	{
					//		trcErr( ResultCode::UNEXPECTED );
					//	}
					//}
					//else
					//{
					//	//trcErr( ResultCode::UNEXPECTED );
					//}
					//break;
				case WAIT_OBJECT_0+SVCCTRL_RESUME:
					//if( g_pSvrInstance->GetState() == Server::STATE_NONE )
					//{
					//	trcChk( g_pSvrInstance->InitializeServer() );
					//	WaitServerState( Server::ServerState::STOPED, SERVICE_CONTINUE_PENDING, 5 );
					//}

					//if( g_pSvrInstance->GetState() == Server::ServerState::STOPED )
					//{
					//	trcChk( g_pSvrInstance->StartServer() );
					//	WaitServerState( Server::STATE_RUNNING, SERVICE_CONTINUE_PENDING, 5 );

					//	if( g_pSvrInstance->GetState() == Server::STATE_RUNNING )
					//	{
					//		ReportServiceStatus( SERVICE_RUNNING, NO_ERROR, 0 );
					//	}
					//	else
					//	{
					//		trcErr( ResultCode::UNEXPECTED );
					//	}
					//}
					//else
					//{
					//	//trcErr( ResultCode::UNEXPECTED );
					//}
					//break;
				case WAIT_TIMEOUT:
					// check server thread status and running state
					if( g_pSvrInstance->GetServerState() == ServerState::STOPED )
					{
						ReportServiceStatus( SERVICE_STOPPED, NO_ERROR, 0 );
						goto Proc_End;
					}
					break;
				default:
					trcErr( ResultCode::UNEXPECTED );
					break;
				}
			}


		Proc_End:



			if( !(hr) )
			{
				ReportServiceStatus( SERVICE_STOPPED, ResultCode::SVR_SERVICE_FAILED, 0 );
			}
		}


		//
		// Purpose: 
		//   Called by SCM whenever a control code is sent to the service
		//   using the ControlService function.
		//
		// Parameters:
		//   dwCtrl - control code
		// 
		// Return value:
		//   None
		//
		void WINAPI ServiceCtrlHandler( DWORD dwCtrl )
		{
			// Handle the requested control code. 
			switch(dwCtrl) 
			{  
			case SERVICE_CONTROL_STOP:
				if( g_pSvrInstance->GetServerState() != ServerState::STOPED )
				{
					ReportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
					SetEvent(g_hCtrlEvents[SVCCTRL_STOP]);
				}
				break;
			case SERVICE_CONTROL_PAUSE:
				//if( g_pSvrInstance->GetState() == Server::STATE_RUNNING )
				//{
				//	ReportServiceStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 0);
				//	SetEvent(g_hCtrlEvents[SVCCTRL_PAUSE]);
				//}
				//break;
			case SERVICE_CONTROL_CONTINUE:
				//if( g_pSvrInstance->GetState() == Server::ServerState::STOPED )
				//{
				//	ReportServiceStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0);
				//	SetEvent(g_hCtrlEvents[SVCCTRL_RESUME]);
				//}
				//break;
			case SERVICE_CONTROL_SHUTDOWN:
				if( g_pSvrInstance->GetServerState() != ServerState::STOPED )
				{
					ReportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
					SetEvent(g_hCtrlEvents[SVCCTRL_STOP]);
				}
				break;
			case SERVICE_CONTROL_INTERROGATE:
				// Fall through to send current status.
				//break; 
			default:
				break;
			};

			ReportServiceStatus(g_SvcStatus.dwCurrentState, NO_ERROR, 0);
		}
	};


}; // namespace SF {
}; // namespace Svr {



#endif

