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
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Trace_Internal.h"
#include "Common/Thread.h"
#include "Common/SharedObj.h"


#if WINDOWS
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
			UPDATE_LINEHEADER_TIME = 1,
			UPDATE_REGISTERY_TIME = 5,
			UPDATE_LOG_BUFFER = 5,
	};




	////////////////////////////////////////////////////////////////////////////////
	//
	// Trace Log Module
	//

	const char* TraceModule::CONFIG_FILENAME = "../../Config/traceConfig.cfg";

	TraceModule* TraceModule::stm_ModuleList[MAX_TRACEMODULE] = {0,};
	std::unordered_map<std::string, UINT32> TraceModule::stm_Masks;

	// Module state registry key
	TimeStampMS TraceModule::m_MaskUpdated;

	TraceModule::TraceModule( const char *szName, const char *szNameTag )
		:m_uiTraceMask(TRC_DEFAULT),
		m_szName(nullptr),m_szNameTag(nullptr)
	{
		StrUtil::StringDup( m_szName, szName );
		StrUtil::StringDup( m_szNameTag, szNameTag );

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
		strCfgPath.append(CONFIG_FILENAME);

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
			StrUtil::StringLwr(nameStart, sizeof(stringBuffer));
			stm_Masks[nameStart] = maskValue;
		}

		fclose(file);
		return true;
	}

	// check and update trace module
	Result TraceModule::CheckAndUpdate()
	{
		Result hr = ResultCode::SUCCESS;

		// Update output mask
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
	TraceOutModule* TraceOutModule::stm_pInstance = nullptr;
	static const UINT _g_uiFileMask[] = {TRCOUT_FILE1, TRCOUT_FILE2};



	TraceOutModule::TraceOutModule()
		: m_uiOutputMask(TRCOUT_DEFAULT)
		, m_uiDbgOutputMask(TRCOUT_NONE)
		, m_uiLineHeaderLen(0)
#if WINDOWS
		, m_hEventLog(nullptr)
		, m_hConsole(INVALID_NATIVE_HANDLE_VALUE)
#endif
	{
		for( int iFile = 0; iFile < TRCOUT_NUMFILE; iFile++ )
		{
			m_tLogFileHour[iFile] = -1;
		}

#if WINDOWS
		// create event log handle
		m_hEventLog = RegisterEventSourceW( nullptr, Util::GetServiceName() );
#endif
		UpdateLineHeader();
	}

	TraceOutModule::~TraceOutModule()
	{
		Stop(true);

		for( int iFile = 0; iFile < TRCOUT_NUMFILE; iFile++ )
		{
			m_LogFile[iFile].Close();
		}

#if WINDOWS
		if( m_hEventLog )
			DeregisterEventSource(m_hEventLog);
		m_hEventLog = nullptr;
#endif
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

		Thread::Stop(bSendKillEvt);
	}

	// Open Log file
	Result TraceOutModule::OpenLogFile( int iFile, const struct tm &curtm, const char *strFileName )
	{
		// close previous opened file
		m_LogFile[iFile].Close();

		// Open file
		Result hr = m_LogFile[iFile].Open(strFileName, IO::File::OpenMode::Append, IO::File::SharingMode::ReadShared);
		if (!(hr))
		{
			return hr;
		}

		m_LogFile[iFile].Seek(IO::File::SeekMode::End);

		m_tLogFileHour[iFile] = curtm.tm_hour;

		size_t szWritten;

		// if created file then write BOM
		if (m_LogFile[iFile].GetFileSize() == 0)
		{
			// UTF8 BOM
			static const BYTE bytBOM[3] = { 0xEF, 0xBB, 0xBF };
			m_LogFile[iFile].Write(bytBOM, 3, szWritten);
		}

		// Write open header
		char szLogBuff[1024];
		snprintf(szLogBuff, MAX_PATH, "----------------------------------------------------------------------------\n");
		DWORD dwStrLen = (DWORD)strlen(szLogBuff);
		m_LogFile[iFile].Write((BYTE*)szLogBuff, dwStrLen, szWritten);

		snprintf(szLogBuff, MAX_PATH, "Start Log: %04d-%02d-%02d-%02d:%02d \n", curtm.tm_year + 1900, curtm.tm_mon + 1, curtm.tm_mday, curtm.tm_hour, curtm.tm_min );
		dwStrLen = (DWORD)strlen(szLogBuff);
		m_LogFile[iFile].Write((BYTE*)szLogBuff, dwStrLen, szWritten);

		snprintf(szLogBuff, MAX_PATH, "----------------------------------------------------------------------------\n");
		dwStrLen = (DWORD)strlen(szLogBuff);
		m_LogFile[iFile].Write((BYTE*)szLogBuff, dwStrLen, szWritten);

		return ResultCode::SUCCESS;
	}



	// Thread inherit
	void TraceOutModule::Run()
	{
		DurationMS WaitDelay = DurationMS(5);
		
		while( 1 )
		{
			auto loopInterval = UpdateInterval(WaitDelay);
			if (CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				//ThisThread::SleepFor(DurationMS(9900*1000));
				break;
			}

			if( m_TraceSpinBuffer.GetReadableCount() > 0 )
			{
				TraceBufferType::BLOCK *pBlock = m_TraceSpinBuffer.Read_Lock();

				if( !pBlock->Data.KillSignal )
				{
					auto ulCurTime = Util::Time.GetTimeUTCSec();

					CheckAndUpdate( ulCurTime );

					TraceOut( pBlock->Data.InputMask, pBlock->Data.TraceBuff );
				}

				m_TraceSpinBuffer.Read_Unlock( pBlock );
			}
			else
			{
				auto ulCurTime = Util::Time.GetTimeUTCSec();

				CheckAndUpdate( ulCurTime );
			}
		}
	}

	void TraceOutModule::UpdateConsoleHandle()
	{
#if WINDOWS
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}

	// Check file system and update
	Result TraceOutModule::CheckAndUpdate(TimeStampSec tCurTime )
	{
		// Update curtime
		m_tCurTime = Util::Time.GetRawUTCSec();
		time_t time = m_tCurTime.time_since_epoch().count() + Util::Time.GetUTCSecOffset().count();
		m_tCurTimeTM = *gmtime(&time);

		if( (m_tLineHdrCheck - tCurTime) > DurationSec(Trace::UPDATE_LINEHEADER_TIME) )
		{
			m_tLineHdrCheck = tCurTime;
			UpdateLineHeader();

			UpdateConsoleHandle();
		}

		if( (m_tRegCheck - tCurTime) > DurationSec(Trace::UPDATE_REGISTERY_TIME) )// check every 3sec
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

		return ResultCode::SUCCESS;
	}

	void TraceOutModule::ValidateLogFile()
	{
		// if file output then check log file
		UINT uiOutputMask = m_uiOutputMask | m_uiDbgOutputMask;
		if (uiOutputMask & TRCOUT_FILE_ALL)
		{
			char strFileName[MAX_PATH];
			for (int iFile = 0; iFile < TRCOUT_NUMFILE; iFile++)
			{
				if (uiOutputMask&_g_uiFileMask[iFile])
				{
					// if already file created then skip
					if (m_tLogFileHour[iFile] == (UINT)m_tCurTimeTM.tm_hour)
						continue;

					// build name
					const char* strFormat = nullptr;
					if (iFile == TRCOUT_FILE_LOG)
						strFormat = "%s../log/%s[%d_%04d_%02d_%02d]log.txt";
					else
						strFormat = "%s../log/%s[%d_%04d_%02d_%02d]logdbg.txt";

					snprintf(strFileName, MAX_PATH, strFormat, Util::GetModulePathA(), Util::GetServiceNameA(), m_tCurTimeTM.tm_year + 1900, m_tCurTimeTM.tm_mon + 1, m_tCurTimeTM.tm_mday, m_tCurTimeTM.tm_hour);

					OpenLogFile(iFile, m_tCurTimeTM, strFileName);
				}
				else
				{
					m_LogFile[iFile].Close();
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


	void TraceOutModule::EventLog(DWORD dwEventId, const WCHAR *string1, const WCHAR *string2)
	{
#if WINDOWS
		if (m_hEventLog == nullptr)
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
#endif
	}

	// Console output
#if WINDOWS
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
#else
	void TraceOutModule::ConsoleOut(const char *strString1, const char *strString2)
	{

		if (strString1 != nullptr)
			printf("%s", strString1);

		if (strString2 != nullptr)
			printf("%s", strString2);

#endif
	//Proc_End:
		return;
	}

	// Trace print out
	void TraceOutModule::TraceOut( UINT trcOutMask, const char * szOutput )
	{
#if WINDOWS
		const bool wcharConsole = true;
		const bool eventOut = true;
#else
		const bool wcharConsole = false;
		const bool eventOut = false;
#endif
		if( trcOutMask == 0 )
			return;


		UINT uiOutputMask = trcOutMask&TRC_GENERIC_ALL ? m_uiOutputMask|m_uiDbgOutputMask : m_uiDbgOutputMask;


		static WCHAR wszOutput[2048] = L"";
		if( ((uiOutputMask&(TRCOUT_DEBUG|TRCOUT_EVENT)) && eventOut)
			|| ((uiOutputMask&TRCOUT_CONSOLE) && wcharConsole) )
		{
			StrUtil::UTF8ToWCS( szOutput, wszOutput );
		}

		if( uiOutputMask&TRCOUT_DEBUG )
		{
#if WINDOWS
			OutputDebugStringW( m_wszLineHeader );
			OutputDebugStringW( wszOutput );
#endif
		}

		if( uiOutputMask&TRCOUT_CONSOLE )
		{
#if WINDOWS
			ConsoleOut( m_wszLineHeader, wszOutput );
#else
			//ConsoleOut(m_szLineHeader, szOutput);
#endif
		}

		// Disable event log
		//if( uiOutputMask&TRCOUT_EVENT )
		//{
		//	if( trcOutMask&(TRC_ERROR) )
		//		EventLog( BR_EVENT_ERROR, m_wszLineHeader, wszOutput );
		//	else if( trcOutMask&(TRC_WARN) )
		//		EventLog( BR_EVENT_WARNING, m_wszLineHeader, wszOutput );
		//	else if( trcOutMask&(TRC_ASSERT) )
		//		EventLog( BR_EVENT_ASSERT, m_wszLineHeader, wszOutput );
		//}

		DWORD dwszLineHeader = (DWORD)strlen(m_szLineHeader);
		DWORD dwszOutput = (DWORD)strlen(szOutput);

		// Validate log file
		ValidateLogFile();

		size_t szWritten;
		if( (m_uiOutputMask&_g_uiFileMask[TRCOUT_FILE_LOG])
			&& m_LogFile[TRCOUT_FILE_LOG].IsOpened() )
		{
			m_LogFile[TRCOUT_FILE_LOG].Write((const BYTE*)m_szLineHeader, dwszLineHeader, szWritten );
			m_LogFile[TRCOUT_FILE_LOG].Write((const BYTE*)szOutput, dwszOutput, szWritten );
		}
		
		if( (m_uiDbgOutputMask&_g_uiFileMask[TRCOUT_FILE_DBGLOG])
			&& m_LogFile[TRCOUT_FILE_DBGLOG].IsOpened())
		{
			m_LogFile[TRCOUT_FILE_DBGLOG].Write((const BYTE*)m_szLineHeader, dwszLineHeader, szWritten );
			m_LogFile[TRCOUT_FILE_DBGLOG].Write((const BYTE*)szOutput, dwszOutput, szWritten );
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
		default:
			return;
		};
	}

	// Trace print data to Spin Buffer
	void TraceOutModule::TracePush( UINT trcInputMask, const char *strTrace, const char* traceName )
	{
		// if not thread mode then print directly
		if(!GetIsRunning())
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

			StrUtil::StringCpyEx( pOutBuff, iBuffLen, "\n" );

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

			StrUtil::StringCpyEx( pOutBuff, iBuffLen, "\n" );

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

		StrUtil::StringCpyEx( pOutBuff, buffLen, "\n" );

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


	void Flush()
	{
		TraceOutModule::GetInstance()->TraceFlush();
	}

	// Allocate console if not exist
	void AllocScreenConsole()
	{
#if WINDOWS
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		if( hOutput == 0 
			|| hOutput == INVALID_NATIVE_HANDLE_VALUE )
		{
			AllocConsole();
			if( TraceOutModule::GetInstance() )
				TraceOutModule::GetInstance()->UpdateConsoleHandle();

			SetConsoleTitle(Util::GetServiceName());
		}
#endif
	}




};	// namespace Trace
} // namespace BR

