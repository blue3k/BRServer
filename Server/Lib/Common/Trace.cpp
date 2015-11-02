////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Trace library
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Trace_Internal.h"
#include "Common/HRESEvent.h"
#include "Common/Thread.h"
#include "Common/SharedObj.h"



#if WINDOWS
#define _ERRTRACE_KEY_ L"SoftWare\\TheBraves\\Svr"


// For crash dump
#pragma comment(lib, "Dbghelp.lib")
#endif

///////////////////////////////////////////////////////////////////////////////////
//
// Default trace module
//

DECLARE_TRACE_MODULE(def)



namespace BR {
namespace Trace {

	enum {
			UPDATE_LINEHEADER_TIME = 100,
			UPDATE_REGISTERY_TIME = 3000,
			UPDATE_LOG_BUFFER = 5,
	};




	////////////////////////////////////////////////////////////////////////////////
	//
	// Trace Log Module
	//

	const char* TraceModule::CONFIG_FILENAME = "traceConfig.cfg";

	TraceModule* TraceModule::stm_ModuleList[MAX_TRACEMODULE] = {0,};
	std::unordered_map<std::string, UINT32> TraceModule::stm_Masks;

	// Module state registry key
	//HKEY TraceModule::stm_hRegKey = nullptr;
	TimeStampMS TraceModule::m_MaskUpdated;

	TraceModule::TraceModule( const char *szName, const char *szNameTag )
		:m_uiTraceMask(TRC_DEFAULT),
		m_szName(nullptr),m_szNameTag(nullptr)
	{
		char strErrString[MAX_PATH] = "";
		StrUtil::StringDup( m_szName, szName );
		StrUtil::StringDup( m_szNameTag, szNameTag );

		//// if registry key is not opened then open it
		//if( stm_hRegKey == nullptr )
		//{
		//	LONG lRes = RegOpenKeyExW( HKEY_LOCAL_MACHINE,
		//					_ERRTRACE_KEY_, 0,
		//					KEY_READ,
		//					&stm_hRegKey );
		//	if( lRes != ERROR_SUCCESS )
		//	{
		//		stm_hRegKey = nullptr;
		//		
		//		FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, nullptr, lRes, 
		//			0, strErrString, MAX_PATH, nullptr );
		//		printf( "%s", strErrString );
		//	}
		//}

		// Register modulelist if not in list yet
		for( int iMod = 0; iMod < MAX_TRACEMODULE; iMod++ )
		{
			if( stm_ModuleList[iMod] == nullptr )
			{
				stm_ModuleList[iMod] = this;
				break;
			}
		}
	}

	TraceModule::~TraceModule()
	{
		StrUtil::SafeDelete( m_szName );
		StrUtil::SafeDelete( m_szNameTag );


		//if( stm_hRegKey )
		//{
		//	RegCloseKey( stm_hRegKey );
		//	stm_hRegKey = nullptr;
		//}
	}

	static char* SkipSpace(char* curChar)
	{
		while ((*curChar) == ' ')
		{
			curChar++;
		}
		return curChar;
	}

	static char* FindDelimiter(char* curChar)
	{
		char curCharValue = *curChar;
		while (curCharValue != ' ' && curCharValue != '=')
		{
			curChar++;
			curCharValue = *curChar;
		}
		return curChar;
	}

	bool TraceModule::LoadTraceConfig()
	{
		if (Util::TimeSince(m_MaskUpdated) < DurationMS(60 * 1000)) return true;
		m_MaskUpdated = Util::Time.GetTimeMs();

		std::string strCfgPath = Util::GetModulePathA();
		strCfgPath.append("..\\..\\Config\\traceConfig.cfg");

		FILE *file = fopen(strCfgPath.c_str(), "r");
		if (file == nullptr)
		{
			return false;
		}

		char stringBuffer[4096];
		while (fgets(stringBuffer, sizeof(stringBuffer), file) != nullptr)
		{
			char *curChar = stringBuffer;
			auto nameStart = SkipSpace(curChar);
			
			auto endChar = FindDelimiter(nameStart);
			if (endChar == nullptr)
				continue; // invalid line
			char endCharValue = *endChar;
			*endChar = '\0';

			if (endCharValue == '\0')
				continue; // invalud line

			if (endCharValue != '=')
			{
				endChar = SkipSpace(endChar);
				if (*endChar != '=')
					continue; // invalud line
			}

			auto numberStart = SkipSpace(endChar+1);
			if (numberStart == '\0')
				continue; // invalud line

			auto maskValue = strtol(numberStart, nullptr, 16);
			strlwr(nameStart);
			stm_Masks[nameStart] = maskValue;
		}

		fclose(file);
		return true;
	}

	// check and update trace module
	HRESULT TraceModule::CheckAndUpdate()
	{
		HRESULT hr = S_OK;

		// Update output mask
		//if( stm_hRegKey )
		if(Util::TimeSince(m_MaskUpdated) > DurationMS(60*1000))
		{
			LoadTraceConfig();
			// Update trace modules
			for( int iMod = 0; iMod < MAX_TRACEMODULE; iMod++ )
			{
				if( stm_ModuleList[iMod] )
					stm_ModuleList[iMod]->UpdateTrace();
				else
					break;
			}
		}

		return hr;
	}

	// Update trace
	void TraceModule::UpdateTrace()
	{
		auto itFound = stm_Masks.find(GetName());
		if (itFound == stm_Masks.end())
		{
			itFound = stm_Masks.find("default");
			if (itFound == stm_Masks.end()) return;
		}
		m_uiTraceMask = itFound->second;
	}

	TraceModule** TraceModule::GetModules()
	{
		return stm_ModuleList;
	}


	// Trace print out
	void TraceModule::TraceOut( UINT trcMask, const char *strTrace )
	{
		// if trace system not initialized then return it
		if( TraceOutModule::GetInstance() == 0 )
			return;

		TraceOutModule::GetInstance()->TracePush( m_uiTraceMask&trcMask, strTrace, GetName() );
	}

	void* TraceModule::TraceReserveWriteBuffer( UINT trcInputMask, char*& stringBuffer, INT &buffLen )
	{
		if( TraceOutModule::GetInstance() == 0 )
			return nullptr;

		return TraceOutModule::GetInstance()->TraceReserveWriteBuffer( m_uiTraceMask&trcInputMask, GetName(), stringBuffer, buffLen );
	}

	void TraceModule::TraceSendWriteBuffer( void* pWriteBuffer, INT buffLen )
	{
		if( TraceOutModule::GetInstance() == 0 )
			return;

		TraceOutModule::GetInstance()->TraceSendWriteBuffer( (TraceOutModule::TraceBufferType::BLOCK*)pWriteBuffer, buffLen );
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	// Trace Output Module
	//

	// Static instance for singleton
	BR::SharedPtr<TraceOutModule> TraceOutModule::stm_pInstance;
	static const UINT _g_uiFileMask[] = {TRCOUT_FILE1, TRCOUT_FILE2};



	TraceOutModule::TraceOutModule()
		: m_uiOutputMask(TRCOUT_NONE)
		, m_uiDbgOutputMask(TRCOUT_NONE)
		, m_uiLineHeaderLen(0)
		, m_hEventLog(nullptr)
		, m_hConsole(INVALID_NATIVE_HANDLE_VALUE)
	{
		for( int iFile = 0; iFile < TRCOUT_NUMFILE; iFile++ )
		{
			m_tLogFileHour[iFile] = -1;
			m_hLogFile[iFile] = INVALID_NATIVE_HANDLE_VALUE;
		}


		// create event log handle
		m_hEventLog = RegisterEventSourceW( nullptr, Util::GetServiceName() );

		UpdateLineHeader();
	}

	TraceOutModule::~TraceOutModule()
	{
		Stop(true);

		for( int iFile = 0; iFile < TRCOUT_NUMFILE; iFile++ )
		{
			if( m_hLogFile[iFile] != INVALID_NATIVE_HANDLE_VALUE )
				CloseHandle( m_hLogFile[iFile] );
		}

		if( m_hEventLog )
			DeregisterEventSource(m_hEventLog);
		m_hEventLog = nullptr;


		//if( trace_bfr )
		//	delete trace_bfr;
		//trace_bfr = nullptr;
	}

	// Override stop to handle kill method
	void TraceOutModule::Stop( bool bSendKillEvt )
	{
		// Make take stop signal
		TraceBufferType::BLOCK *pSpinBlock = m_TraceSpinBuffer.Write_Lock();

		pSpinBlock->Data.TraceBuff[0] = 0;
		pSpinBlock->Data.InputMask = 0;
		pSpinBlock->Data.KillSignal = true;

		m_TraceSpinBuffer.Write_Unlock( pSpinBlock );

		__super::Stop(bSendKillEvt);
	}

	// Open Log file
	HRESULT TraceOutModule::OpenLogFile( int iFile, const struct tm &curtm, wchar_t *strFileName )
	{
		// close previous opened file
		if( m_hLogFile[iFile] )
			CloseHandle( m_hLogFile[iFile] );

		// Open file
		m_hLogFile[iFile] = CreateFile(
			strFileName,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL 
			| FILE_FLAG_SEQUENTIAL_SCAN,
			nullptr);

		if( m_hLogFile[iFile] != INVALID_NATIVE_HANDLE_VALUE )
		{
			DWORD dwWritten = 0;

			SetFilePointer(m_hLogFile[iFile], 0, 0, FILE_END);
			m_tLogFileHour[iFile] = curtm.tm_hour;

			// if created file then write BOM
			LARGE_INTEGER szSize;
			GetFileSizeEx( m_hLogFile[iFile], &szSize );
			if( szSize.QuadPart == 0 )
			{
				static const BYTE bytBOM[3] = { 0xEF, 0xBB, 0xBF };
				WriteFile( m_hLogFile[iFile], bytBOM, 3, &dwWritten, nullptr );
			}

			// Write open header
			char szLogBuff[1024];
			snprintf(szLogBuff, MAX_PATH, "----------------------------------------------------------------------------\r\n");
			DWORD dwStrLen = (DWORD)strlen(szLogBuff);
			WriteFile( m_hLogFile[iFile], szLogBuff, dwStrLen, &dwWritten, nullptr );

			snprintf(szLogBuff, MAX_PATH, "Start Log: %04d-%02d-%02d-%02d:%02d \r\n", curtm.tm_year + 1900, curtm.tm_mon + 1, curtm.tm_mday, curtm.tm_hour, curtm.tm_min );
			dwStrLen = (DWORD)strlen(szLogBuff);
			WriteFile( m_hLogFile[iFile], szLogBuff, dwStrLen, &dwWritten, nullptr );

			snprintf(szLogBuff, MAX_PATH, "----------------------------------------------------------------------------\r\n");
			dwStrLen = (DWORD)strlen(szLogBuff);
			WriteFile( m_hLogFile[iFile], szLogBuff, dwStrLen, &dwWritten, nullptr );
		}
		else
		{
			return E_FAIL;
		}

		return S_OK;
	}



	// Thread inherit
	void TraceOutModule::Run()
	{
		INT iProcMax = 1;
		DurationMS WaitDelay = DurationMS(5);
		
		while( 1 )
		{
			auto loopInterval = UpdateInterval(WaitDelay);
			if (CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				break;
			}

			if( m_TraceSpinBuffer.GetReadableCount() > 0 )
			{
				TraceBufferType::BLOCK *pBlock = m_TraceSpinBuffer.Read_Lock();

				if( !pBlock->Data.KillSignal )
				{
					auto ulCurTime = Util::Time.GetTimeMs();

					CheckAndUpdate( ulCurTime );

					TraceOut( pBlock->Data.InputMask, pBlock->Data.TraceBuff );
				}

				m_TraceSpinBuffer.Read_Unlock( pBlock );
			}
			else
			{
				auto ulCurTime = Util::Time.GetTimeMs();

				CheckAndUpdate( ulCurTime );
			}
		}

	}

	void TraceOutModule::UpdateConsoleHandle()
	{
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	// Check file system and update
	HRESULT TraceOutModule::CheckAndUpdate(TimeStampMS tCurTime )
	{
		// Update curtime
		time(&m_tCurTime);
		localtime_s(&m_tCurTimeTM, &m_tCurTime);

		if( (m_tLineHdrCheck - tCurTime) > DurationMS(Trace::UPDATE_LINEHEADER_TIME) )
		{
			m_tLineHdrCheck = tCurTime;
			UpdateLineHeader();

			UpdateConsoleHandle();
		}

		if( (m_tRegCheck - tCurTime) > DurationMS(Trace::UPDATE_REGISTERY_TIME) )// check every 3sec
		{
			// Update output mask
			TraceModule::CheckAndUpdate();

			auto itMask = TraceModule::stm_Masks.find("ctrl");
			if(itMask != TraceModule::stm_Masks.end())
				m_uiOutputMask = itMask->second;

			itMask = TraceModule::stm_Masks.find("dbgctrl");
			if (itMask != TraceModule::stm_Masks.end())
				m_uiDbgOutputMask = itMask->second;

			m_tRegCheck = tCurTime;
		}

		return S_OK;
	}

	void TraceOutModule::ValidateLogFile()
	{
		// if file output then check log file
		UINT uiOutputMask = m_uiOutputMask | m_uiDbgOutputMask;
		if (uiOutputMask & TRCOUT_FILE_ALL)
		{
			WCHAR strFileName[MAX_PATH];
			for (int iFile = 0; iFile < TRCOUT_NUMFILE; iFile++)
			{
				if (uiOutputMask&_g_uiFileMask[iFile])
				{
					// if already file created then skip
					if (m_tLogFileHour[iFile] == m_tCurTimeTM.tm_hour)
						continue;

					if (iFile == TRCOUT_FILE_LOG)
					{
						// build name
						swprintf(strFileName, MAX_PATH, L"%s..\\log\\%s[%d_%04d_%02d_%02d]log.txt", Util::GetModulePath(), Util::GetServiceName(), m_tCurTimeTM.tm_year + 1900, m_tCurTimeTM.tm_mon + 1, m_tCurTimeTM.tm_mday, m_tCurTimeTM.tm_hour);
					}
					else
					{
						// build name
						swprintf(strFileName, MAX_PATH, L"%s..\\log\\%s[%d_%04d_%02d_%02d]logdbg.txt", Util::GetModulePath(), Util::GetServiceName(), m_tCurTimeTM.tm_year + 1900, m_tCurTimeTM.tm_mon + 1, m_tCurTimeTM.tm_mday, m_tCurTimeTM.tm_hour);
					}

					// close previous opened file
					if (m_hLogFile[iFile])
						CloseHandle(m_hLogFile[iFile]);
					m_hLogFile[iFile] = INVALID_NATIVE_HANDLE_VALUE;

					OpenLogFile(iFile, m_tCurTimeTM, strFileName);
				}
				else
				{
					if (m_hLogFile[iFile])
						CloseHandle(m_hLogFile[iFile]);
					m_hLogFile[iFile] = INVALID_NATIVE_HANDLE_VALUE;

					//return GetLastHRESULT();
				}
			}
		}
	}

	// Update line header
	void TraceOutModule::UpdateLineHeader()
	{
		if (m_tCurTime != m_tLineHeader)
		{
			m_tLineHeader = m_tCurTime;

			m_uiLineHeaderLen = snprintf(m_szLineHeader, sizeof(m_szLineHeader), "%4d-%02d-%02d/%02d:%02d:%02d:", m_tCurTimeTM.tm_year + 1900, m_tCurTimeTM.tm_mon + 1, m_tCurTimeTM.tm_mday, m_tCurTimeTM.tm_hour, m_tCurTimeTM.tm_min, m_tCurTimeTM.tm_sec);
			StrUtil::UTF8ToWCS( m_szLineHeader, m_wszLineHeader );
		}
	}


	void TraceOutModule::EventLog( DWORD dwEventId, const WCHAR *string1, const WCHAR *string2 )
	{
		if ( m_hEventLog == nullptr )
			return;

		LPCWSTR Inserts[] =
		{
			string1,
			string2
		};

		static const WORD severity_map[4] = 
		{
			EVENTLOG_SUCCESS,
			EVENTLOG_INFORMATION_TYPE,
			EVENTLOG_WARNING_TYPE,
			EVENTLOG_ERROR_TYPE
		};

		ReportEventW(
			m_hEventLog,
			severity_map[dwEventId >> 30],
			0,
			dwEventId,
			nullptr,
			2,
			0,
			Inserts,
			nullptr);
	}

	// Console output
	void TraceOutModule::ConsoleOut( const WCHAR *strString1, const WCHAR *strString2 )
	{
		DWORD dwWriten = 0;

		// Get the standard input handle.
		if (m_hConsole != INVALID_NATIVE_HANDLE_VALUE)
		{
			if( strString1 )
				WriteConsoleW( m_hConsole, strString1, (DWORD)wcslen(strString1), &dwWriten, nullptr );

			if( strString2 )
				WriteConsoleW( m_hConsole, strString2, (DWORD)wcslen(strString2), &dwWriten, nullptr );
		}
	//Proc_End:
		return;
	}

	// Trace print out
	void TraceOutModule::TraceOut( UINT trcOutMask, const char * szOutput )
	{
		if( trcOutMask == 0 )
			return;


		UINT uiOutputMask = trcOutMask&TRC_GENERIC_ALL ? m_uiOutputMask|m_uiDbgOutputMask : m_uiDbgOutputMask;


		static WCHAR wszOutput[2048] = L"";
		if( (uiOutputMask&(TRCOUT_DEBUG|TRCOUT_EVENT))
			|| ((uiOutputMask&TRCOUT_CONSOLE) && m_hConsole != INVALID_NATIVE_HANDLE_VALUE) )
		{
			StrUtil::UTF8ToWCS( szOutput, wszOutput );
		}

		if( uiOutputMask&TRCOUT_DEBUG )
		{
			OutputDebugStringW( m_wszLineHeader );
			OutputDebugStringW( wszOutput );
		}

		if( uiOutputMask&TRCOUT_CONSOLE )
		{
			ConsoleOut( m_wszLineHeader, wszOutput );
		}

		if( uiOutputMask&TRCOUT_EVENT )
		{
			if( trcOutMask&(TRC_ERROR | TRC_IERROR) )
				EventLog( BR_EVENT_ERROR, m_wszLineHeader, wszOutput );
			else if( trcOutMask&(TRC_WARN) )
				EventLog( BR_EVENT_WARNING, m_wszLineHeader, wszOutput );
			else if( trcOutMask&(TRC_ASSERT) )
				EventLog( BR_EVENT_ASSERT, m_wszLineHeader, wszOutput );
		}

		DWORD dwszLineHeader = (DWORD)strlen(m_szLineHeader);
		DWORD dwszOutput = (DWORD)strlen(szOutput);

		// Validate log file
		ValidateLogFile();

		if( (m_uiOutputMask&_g_uiFileMask[TRCOUT_FILE_LOG])
			&& m_hLogFile[TRCOUT_FILE_LOG] != INVALID_NATIVE_HANDLE_VALUE )
		{
			DWORD dwWritten = 0;
			WriteFile( m_hLogFile[TRCOUT_FILE_LOG], m_szLineHeader, dwszLineHeader, &dwWritten, nullptr );
			WriteFile( m_hLogFile[TRCOUT_FILE_LOG], szOutput, dwszOutput, &dwWritten, nullptr );
		}
		
		if( (m_uiDbgOutputMask&_g_uiFileMask[TRCOUT_FILE_DBGLOG])
			&& m_hLogFile[TRCOUT_FILE_DBGLOG] != INVALID_NATIVE_HANDLE_VALUE )
		{
			DWORD dwWritten = 0;
			WriteFile( m_hLogFile[TRCOUT_FILE_DBGLOG], m_szLineHeader, dwszLineHeader, &dwWritten, nullptr );
			WriteFile( m_hLogFile[TRCOUT_FILE_DBGLOG], szOutput, dwszOutput, &dwWritten, nullptr );
		}

		// Remain output mask
		uiOutputMask = (~m_uiOutputMask)&m_uiDbgOutputMask;
	}


	// Append Trace mask prefix
	void TraceOutModule::AppendMaskPrefix( ULONG trcMask, char* &szDest, INT& iBuffLen )
	{
		switch( trcMask )
		{
		case TRC_ERROR:
			StrUtil::StringCpyEx( szDest, iBuffLen, "ERROR: " );
			break;
		case TRC_WARN:
			StrUtil::StringCpyEx( szDest, iBuffLen, "WARN: " );
			break;
		case TRC_ASSERT:
			StrUtil::StringCpyEx( szDest, iBuffLen, "ASSERT: " );
			break;
		//case TRC_TRACE:
		//	StrUtil::StringCpyEx( szDest, iBuffLen, "ERROR: " );
		//	break;
		case TRC_IERROR:
			StrUtil::StringCpyEx( szDest, iBuffLen, "ERROR: " );
			break;
		default:
			return;
		};
	}

	// Trace print data to Spin Buffer
	void TraceOutModule::TracePush( UINT trcInputMask, const char *strTrace, const char* traceName )
	{
		// if not thread mode then print directly
		if( !joinable() )
		{
			char strTraceBuff[4096];

			char *pOutBuff = strTraceBuff;
			INT iBuffLen = sizeof(strTraceBuff);

			StrUtil::StringCpyEx( pOutBuff, iBuffLen, traceName );
			StrUtil::StringCpyEx( pOutBuff, iBuffLen, ": " );
			AppendMaskPrefix( trcInputMask, pOutBuff, iBuffLen );
			StrUtil::StringCpyEx( pOutBuff, iBuffLen, strTrace );

			// pending \r\n
			if( iBuffLen < 3 )
			{
				pOutBuff -= 3-iBuffLen;
				iBuffLen = 3;
			}

			StrUtil::StringCpyEx( pOutBuff, iBuffLen, "\r\n" );

			TraceOut( trcInputMask, strTraceBuff );

			return;
		}
		else
		{
			TraceBufferType::BLOCK *pSpinBlock = m_TraceSpinBuffer.Write_Lock();

			char *pOutBuff = pSpinBlock->Data.TraceBuff;
			INT iBuffLen = sizeof(pSpinBlock->Data.TraceBuff);


			StrUtil::StringCpyEx( pOutBuff, iBuffLen, traceName );
			StrUtil::StringCpyEx( pOutBuff, iBuffLen, ": " );
			AppendMaskPrefix( trcInputMask, pOutBuff, iBuffLen );
			StrUtil::StringCpyEx( pOutBuff, iBuffLen, strTrace );

			// pending \r\n
			if( iBuffLen < 3 )
			{
				pOutBuff -= 3-iBuffLen;
				iBuffLen = 3;
			}

			StrUtil::StringCpyEx( pOutBuff, iBuffLen, "\r\n" );

			pSpinBlock->Data.InputMask = trcInputMask;
			pSpinBlock->Data.KillSignal = false;

			m_TraceSpinBuffer.Write_Unlock( pSpinBlock );
		}
	}

	
	TraceOutModule::TraceBufferType::BLOCK* TraceOutModule::TraceReserveWriteBuffer( UINT trcInputMask, const char* traceName, char*& stringBuffer, INT &buffLen )
	{
		TraceBufferType::BLOCK *pSpinBlock = m_TraceSpinBuffer.Write_Lock();

		stringBuffer = pSpinBlock->Data.TraceBuff;
		buffLen = sizeof(pSpinBlock->Data.TraceBuff);


		StrUtil::StringCpyEx( stringBuffer, buffLen, traceName );
		StrUtil::StringCpyEx( stringBuffer, buffLen, ": " );
		AppendMaskPrefix( trcInputMask, stringBuffer, buffLen );

		pSpinBlock->Data.InputMask = trcInputMask;

		return pSpinBlock;
	}

	void TraceOutModule::TraceSendWriteBuffer( TraceBufferType::BLOCK* pWriteBuffer, INT buffLen )
	{
		char *pOutBuff = pWriteBuffer->Data.TraceBuff;
		INT orgBuffLen = sizeof(pWriteBuffer->Data.TraceBuff);

		pOutBuff += orgBuffLen - buffLen;

		// pending \r\n
		if( buffLen < 3 )
		{
			pOutBuff -= 3-buffLen;
			buffLen = 3;
		}

		StrUtil::StringCpyEx( pOutBuff, buffLen, "\r\n" );

		pWriteBuffer->Data.KillSignal = false;
		m_TraceSpinBuffer.Write_Unlock( pWriteBuffer );
	}


	// Trace flush result
	void TraceOutModule::TraceFlush()
	{
	}



	//////////////////////////////////////////////////////////////////////////////////////
	//
	// Trace system initialize
	//

	// Share initialize
	void Initialize( TraceModule** trcMods, TraceOutModule &trcShare )
	{
		int iMod = 0;

		for( int iNewMod = 0; iNewMod < Trace::TraceModule::MAX_TRACEMODULE; iNewMod++ )
		{
			if( TraceModule::GetModules()[iNewMod] == nullptr )
				break;

			for( ; iMod < Trace::TraceModule::MAX_TRACEMODULE; iMod++ )
			{
				if( trcMods[iMod] == nullptr )
				{
					trcMods[iMod] = TraceModule::GetModules()[iNewMod];
					break;
				}
			}
		}

		TraceOutModule::SetInstance( &trcShare );
	}

	void Initialize()
	{
		TraceOutModule::NewInstance();
		TraceOutModule::GetInstance()->CheckAndUpdate( Util::Time.GetRawTimeMs() );
		TraceOutModule::GetInstance()->Start();
	}

	void Uninitialize( TraceModule** trcMods )
	{

		if( trcMods )
		{
			for( int iNewMod = 0; iNewMod < Trace::TraceModule::MAX_TRACEMODULE; iNewMod++ )
			{
				if( TraceModule::GetModules()[iNewMod] == nullptr )
					break;

				for( int iMod = 0; iMod < Trace::TraceModule::MAX_TRACEMODULE; iMod++ )
				{
					if( trcMods[iMod] == TraceModule::GetModules()[iNewMod] )
					{
						trcMods[iMod] = nullptr;
						break;
					}
				}
			}
		}

		if( TraceOutModule::GetInstance() && TraceOutModule::GetInstance()->GetRefCount() == 1 )
			TraceOutModule::GetInstance()->Stop(true);

		TraceOutModule::SetInstance( nullptr );
	}


	void Flush()
	{
		TraceOutModule::GetInstance()->TraceFlush();
	}

	// Allocate console if not exist
	void AllocScreenConsole()
	{
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		if( hOutput == 0 
			|| hOutput == INVALID_NATIVE_HANDLE_VALUE )
		{
			AllocConsole();
			if( TraceOutModule::GetInstance() )
				TraceOutModule::GetInstance()->UpdateConsoleHandle();

			SetConsoleTitle(Util::GetServiceName());
		}
	}





};	// namespace Trace
} // namespace BR

