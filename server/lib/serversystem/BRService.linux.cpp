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
#include <conio.h>
#include "Common/HRESEvent.h"
#include "Common/HRESSvrSys.h"
#include "Serversystem/SvrTrace.h"
#include "Serversystem/BRService.h"
#include "Common/TimeUtil.h"

#if LINUX

namespace BR {
namespace Svr {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Service control
	//

	namespace Service
	{

		// Service Main handler
		void ServiceMain( DWORD dwArgc, LPWSTR *lpszArgv );


		// Install Service
		HRESULT ServiceInstall( const WCHAR *strCfgPath, const WCHAR *strUser, const WCHAR *strPWD )
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
		HRESULT ServiceRun( int argc, WCHAR* argv[], BrServer *pSvrInstance )
		{
			HRESULT hr = S_OK;
			bool bIsDebugRun = false;
			std::wstring strCfgPath = Util::GetModulePath();
			bool bIsInstall = false;
			WCHAR *strUser = nullptr; WCHAR *strPWD = nullptr;
			WCHAR *strServiceName = nullptr;

#if WINDOWS
			SetCurrentDirectoryW( Util::GetModulePath() );
#else
			chdir(Util::GetModulePathA());
#endif

			strCfgPath.append(L"..\\..\\Config\\ServerConfig.xml");


			g_pSvrInstance = pSvrInstance;
			Assert( BrServer::GetInstance() == pSvrInstance );


			for( int iArg = 0; iArg < argc; iArg++ )
			{
				WCHAR* pCurParam = argv[iArg];

				switch( pCurParam[0] )
				{
				case L'-':
					pCurParam++;
					if( StrUtil::StringCmpLwr( pCurParam, (INT)wcslen(pCurParam), L"install", (INT)wcslen(L"install") ) == 0 )
					{
						bIsInstall = true;
					}
					else if( pCurParam[0] == 'n' || pCurParam[0] == 'N' )
					{
						pCurParam += 2;
						strServiceName = pCurParam;
					}
					else if( pCurParam[0] == 'u' || pCurParam[0] == 'U'  )
					{
						pCurParam += 2;
						strUser = pCurParam;
					}
					else if( pCurParam[0] == 'p' || pCurParam[0] == 'P' )
					{
						pCurParam += 2;
						strPWD = pCurParam;
					}
					else if( StrUtil::StringCmpLwr( pCurParam, (INT)wcslen(pCurParam), L"uninstall", (INT)wcslen(L"uninstall") ) == 0 )
					{
						svrChk( Service::ServiceUninstall() );
						goto Proc_End;
					}
					else if( StrUtil::StringCmpLwr( pCurParam, (INT)wcslen(pCurParam), L"debug", (INT)wcslen(L"debug") ) == 0 )
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
				svrChk( Service::ServiceInstall( strCfgPath.c_str(), strUser, strPWD ) );
				goto Proc_End;
			}

			if( bIsDebugRun )
			{
				Trace::AllocScreenConsole();
			}

			svrTrace( Trace::TRC_TRACE, "Loading configuration" );

			svrChk( Svr::Config::LoadConfig( strCfgPath.c_str() ) );



			svrTrace( Trace::TRC_TRACE, "<%0%> Start with Mode %1% ", Util::GetServiceName(), bIsDebugRun ? "Debug" : "Service" );


			// if not service mode
			if( bIsDebugRun )
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

					int iCh = 0;
					Util::PeekKey( iCh );
					if( iCh == 'q' || iCh == 'Q' )
						bRun = false;

					if( g_pSvrInstance->GetServerState() == ServerState::STOPED )
					{
						bRun = false;
					}

					SleepEx( 1000, FALSE );
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
				SERVICE_TABLE_ENTRY DispatchTable[] = 
				{ 
					{ (WCHAR*)Util::GetServiceName(), (LPSERVICE_MAIN_FUNCTION)ServiceMain }, 
					{ NULL, NULL } 
				};

				// This call returns when the service has stopped. 
				// The process should simply terminate when the call returns.

				if (!StartServiceCtrlDispatcher( DispatchTable )) 
				{
					int iError = GetLastError();
					svrTrace( Trace::TRC_ERROR, "StartServiceCtrlDispatcher failed err:%0%", iError );
					return HRESULT_FROM_WIN32( iError );
				}
			}


		Proc_End:

			svrTrace( Trace::TRC_TRACE, "<%0%> Closed", Util::GetServiceName() );

			g_pSvrInstance = nullptr;

			Trace::Uninitialize();

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
		void WINAPI ServiceMain( DWORD dwArgc, LPWSTR *lpszArgv )
		{
			HRESULT hr = S_OK;

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
					svrTrace( Trace::TRC_ERROR, "CreateEvent failed err:%0%", iError );
					trcErr( HRESULT_FROM_WIN32( iError ) );
				}
			}



			// Register the handler function for the service.
			g_SvcStatusHandle = RegisterServiceCtrlHandler( 
				Util::GetServiceName(), 
				ServiceCtrlHandler);

			if( !g_SvcStatusHandle )
			{ 
				int iError = GetLastError();
				svrTrace( Trace::TRC_ERROR, "RegisterServiceCtrlHandler failed err:%0%", iError );
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
				svrTrace( Trace::TRC_ERROR, "Invalid ServerInstance" );
				ReportServiceStatus( SERVICE_STOPPED, E_SVR_SERVICE_FAILED, 0 );
				trcErr( E_UNEXPECTED );
			}


			svrChk( g_pSvrInstance->StartServer() );
			// Report running status when initialization is complete.
			ReportServiceStatus( SERVICE_RUNNING, NO_ERROR, 0 );


			// Check whether to stop the service.
			while( 1 )
			{
				DWORD dwWaitRes = WaitForMultipleObjects( NUM_SVCCTRLEVT, g_hCtrlEvents, FALSE, 3000 );

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
							trcErr( E_UNEXPECTED );
						}
					}
					else
					{
						ReportServiceStatus( SERVICE_STOPPED, NO_ERROR, 0 );
					}
					goto Proc_End;
					break;
				case WAIT_OBJECT_0+SVCCTRL_PAUSE:
					//break;
				case WAIT_OBJECT_0+SVCCTRL_RESUME:
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
					trcErr( E_UNEXPECTED );
					break;
				}
			}


		Proc_End:



			if( FAILED(hr) )
			{
				ReportServiceStatus( SERVICE_STOPPED, E_SVR_SERVICE_FAILED, 0 );
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


}; // namespace BR {
}; // namespace Svr {


#else
void Dummy_BRService() {}
#endif

