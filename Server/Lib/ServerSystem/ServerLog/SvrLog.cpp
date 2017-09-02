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
#include "SFTypedefs.h"
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Common/Trace_Internal.h"
#include "Thread/Thread.h"
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



namespace SF {
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
	std::unordered_map<std::string, uint32_t> TraceModule::stm_Masks;

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
			if (*numberStart == '\0')
				continue; // invalid line

			auto maskValue = strtol(numberStart, nullptr, 16);
			StrUtil::StringLwr(nameStart, sizeof(stringBuffer));
			stm_Masks[nameStart] = (uint)maskValue;
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
	



	TraceOutModule::TraceOutModule()
		: m_uiLineHeaderLen(0)
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
			static const uint8_t bytBOM[3] = { 0xEF, 0xBB, 0xBF };
			m_LogFile[iFile].Write(bytBOM, 3, szWritten);
		}

		// Write open header
		char szLogBuff[1024];
		snprintf(szLogBuff, MAX_PATH, "----------------------------------------------------------------------------\n");
		DWORD dwStrLen = (DWORD)strlen(szLogBuff);
		m_LogFile[iFile].Write((uint8_t*)szLogBuff, dwStrLen, szWritten);

		snprintf(szLogBuff, MAX_PATH, "Start Log: %04d-%02d-%02d-%02d:%02d \n", curtm.tm_year + 1900, curtm.tm_mon + 1, curtm.tm_mday, curtm.tm_hour, curtm.tm_min );
		dwStrLen = (DWORD)strlen(szLogBuff);
		m_LogFile[iFile].Write((uint8_t*)szLogBuff, dwStrLen, szWritten);

		snprintf(szLogBuff, MAX_PATH, "----------------------------------------------------------------------------\n");
		dwStrLen = (DWORD)strlen(szLogBuff);
		m_LogFile[iFile].Write((uint8_t*)szLogBuff, dwStrLen, szWritten);

		return ResultCode::SUCCESS;
	}



	// Thread inherit
	void TraceOutModule::Run()
	{
		DurationMS WaitDelay = DurationMS(5);

		TraceModule::LoadTraceConfig();
		UpdateOutputMask();

		while( 1 )
		{
			auto loopInterval = UpdateInterval(WaitDelay);
			if (CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				//ThisThread::SleepFor(DurationMS(9900*1000));
				break;
			}

			auto ulCurTime = Util::Time.GetTimeUTCSec();
			CheckAndUpdate(ulCurTime);


			if( m_TraceSpinBuffer.GetReadableCount() > 0 )
			{
				TraceBufferType::BLOCK *pBlock = m_TraceSpinBuffer.Read_Lock();

				if( !pBlock->Data.KillSignal )
				{
					TraceOut( pBlock->Data.InputMask, pBlock->Data.TraceBuff, pBlock->Data.TraceBuffUsedLen);
				}

				m_TraceSpinBuffer.Read_Unlock( pBlock );
			}
		}
	}

	void TraceOutModule::UpdateConsoleHandle()
	{
#if WINDOWS
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}

	void TraceOutModule::UpdateOutputMask()
	{
		auto itMask = TraceModule::stm_Masks.find("outputdebugger");
		if (itMask != TraceModule::stm_Masks.end())
			m_uiOutputMask = itMask->second;

		itMask = TraceModule::stm_Masks.find("outputconsole");
		if (itMask != TraceModule::stm_Masks.end())
			m_uiOutputMaskDebugger = itMask->second;

		itMask = TraceModule::stm_Masks.find("outputfile");
		if (itMask != TraceModule::stm_Masks.end())
			m_uiOutputMaskFile[0] = itMask->second;

		itMask = TraceModule::stm_Masks.find("outputdbgfile");
		if (itMask != TraceModule::stm_Masks.end())
			m_uiOutputMaskFile[1] = itMask->second;
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
			UpdateOutputMask();

			m_tRegCheck = tCurTime;
		}

		return ResultCode::SUCCESS;
	}

	void TraceOutModule::ValidateLogFile()
	{
		static const char* FormatByType[TRCOUT_NUMFILE] = 
		{
#if ANDROID
			"%s/%s_log.txt",
			"%s/%s_dbglog.txt"
#else
			"%s../log/%s[%d_%04d_%02d_%02d]log.txt",
			"%s../log/%s[%d_%04d_%02d_%02d]logdbg.txt"
#endif
		};

		// if file output then check log file
		uint32_t uiOutputMask = m_uiOutputMaskFile[0] | m_uiOutputMaskFile[1];
		if (uiOutputMask & TRCOUT_FILE_ALL)
		{
			char strFileName[MAX_PATH];
			for (int iFile = 0; iFile < TRCOUT_NUMFILE; iFile++)
			{
				uiOutputMask = m_uiOutputMaskFile[iFile];

				const char* strFormat = FormatByType[iFile];
				if (StrUtil::IsNullOrEmpty(strFormat))
				{
					m_LogFile[iFile].Close();
					continue;
				}

				if (uiOutputMask != 0)
				{
					// if already file created then skip
					if (m_tLogFileHour[iFile] == (UINT)m_tCurTimeTM.tm_hour)
						continue;

					// build name
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
		//if (m_tCurTime != m_tLineHeader)
		{
			m_tLineHeader = m_tCurTime;

			m_uiLineHeaderLen = snprintf(m_szLineHeader, sizeof(m_szLineHeader), "%02d:%02d:%02d/%08X ", m_tCurTimeTM.tm_hour, m_tCurTimeTM.tm_min, m_tCurTimeTM.tm_sec, Util::Time.GetRawTimeMs().time_since_epoch().count());
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
	void TraceOutModule::ConsoleOut( const WCHAR *strString, size_t strLen )
	{
		DWORD dwWriten = 0;
		// Get the standard input handle.
		if (m_hConsole != INVALID_NATIVE_HANDLE_VALUE)
		{
			if(strString)
				WriteConsoleW( m_hConsole, strString, (DWORD)strLen, &dwWriten, nullptr );
		}
#else
	void TraceOutModule::ConsoleOut(const char *strString, size_t strLen)
	{
		if (strString != nullptr)
			printf("%s", strString);

#endif
	}

	// Trace print out
	void TraceOutModule::TraceOut(uint trcOutMask, char * szOutput, size_t outputStringLen )
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

		uint32_t outputMask = m_uiOutputMask & trcOutMask;
		uint32_t outputMaskDebug = m_uiOutputMaskDebugger & trcOutMask;
		uint32_t outputMaskFile[2] = { m_uiOutputMaskFile[0] & trcOutMask, m_uiOutputMaskFile[0] & trcOutMask };

		UpdateLineHeader();

		// I don't want to have null terminate here. I just want to replace the line header part
		memcpy(szOutput, m_szLineHeader, m_uiLineHeaderLen);

		static WCHAR wszOutput[2048] = L"";
		size_t wcsOutputSize = 0;
		if(outputMask != 0 && (eventOut || wcharConsole))
		{
			StrUtil::UTF8ToWCS( szOutput, wszOutput );
			wcsOutputSize = wcslen(wszOutput);
		}

		if(outputMaskDebug != 0)
		{
#if WINDOWS
			OutputDebugStringW( wszOutput );
#endif
		}


		if(outputMask != 0)
		{
#if WINDOWS
			ConsoleOut( wszOutput, wcsOutputSize);
#elif ANDROID
			android_LogPriority logPriority = ANDROID_LOG_INFO;
			switch (trcOutMask)
			{
			case TRC_ERROR:
				logPriority = ANDROID_LOG_ERROR;
				break;
			case TRC_WARN:
				logPriority = ANDROID_LOG_WARN;
				break;
			case TRC_ASSERT:
				logPriority = ANDROID_LOG_FATAL;
				break;
			default:
				break;
			};

			__android_log_print(logPriority, "StormForge", "%s", szOutput);
			unused(wcsOutputSize);
#else
			unused(wcsOutputSize);
			ConsoleOut(szOutput, outputStringLen);
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

		// Validate log file
		ValidateLogFile();

		size_t szWritten;
		for (int iFile = 0; iFile < TRCOUT_NUMFILE; iFile++)
		{
			if (outputMaskFile[iFile] != 0 && m_LogFile[iFile].IsOpened())
			{
				m_LogFile[iFile].Write((const uint8_t*)szOutput, (DWORD)outputStringLen, szWritten);
			}
		}
	}

	void TraceOutModule::ReserveTimePrefix(char* &szDest, int& iBuffLen)
	{
		memset(szDest, ' ', m_uiLineHeaderLen); // fill with space to reserve
		szDest += m_uiLineHeaderLen;
		iBuffLen -= m_uiLineHeaderLen;
	}

	// Append Trace mask prefix
	void TraceOutModule::AppendMaskPrefix( uint32_t trcMask, char* &szDest, int& iBuffLen )
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

	// Append mask prefix + ReserveTimePrefix
	void TraceOutModule::AppendPrefix(const char* traceName, uint32_t trcMask, char* &szDest, int& iBuffLen)
	{
		ReserveTimePrefix(szDest, iBuffLen);
		StrUtil::StringCpyEx(szDest, iBuffLen, traceName);
		StrUtil::StringCpyEx(szDest, iBuffLen, ": ");
		AppendMaskPrefix(trcMask, szDest, iBuffLen);
	}


	//// Trace print data to Spin Buffer
	//void TraceOutModule::TracePush( UINT trcInputMask, const char *strTrace, const char* traceName )
	//{
	//	// if not thread mode then print directly
	//	if(!GetIsRunning())
	//	{
	//		char strTraceBuff[4096];

	//		char *pOutBuff = strTraceBuff;
	//		INT iBuffLen = sizeof(strTraceBuff);

	//		//StrUtil::StringCpyEx( pOutBuff, iBuffLen, traceName );
	//		//StrUtil::StringCpyEx( pOutBuff, iBuffLen, ": " );
	//		//AppendMaskPrefix( trcInputMask, pOutBuff, iBuffLen );
	//		AppendPrefix(traceName, trcInputMask, pOutBuff, iBuffLen);
	//		StrUtil::StringCpyEx( pOutBuff, iBuffLen, strTrace );

	//		// pending \r\n
	//		if( iBuffLen < 3 )
	//		{
	//			pOutBuff -= 3-iBuffLen;
	//			iBuffLen = 3;
	//		}

	//		StrUtil::StringCpyEx( pOutBuff, iBuffLen, "\n" );

	//		TraceOut( trcInputMask, strTraceBuff );

	//		return;
	//	}
	//	else
	//	{
	//		TraceBufferType::BLOCK *pSpinBlock = m_TraceSpinBuffer.Write_Lock();

	//		char *pOutBuff = pSpinBlock->Data.TraceBuff;
	//		INT iBuffLen = sizeof(pSpinBlock->Data.TraceBuff);


	//		//StrUtil::StringCpyEx( pOutBuff, iBuffLen, traceName );
	//		//StrUtil::StringCpyEx( pOutBuff, iBuffLen, ": " );
	//		//AppendMaskPrefix( trcInputMask, pOutBuff, iBuffLen );
	//		AppendPrefix(traceName, trcInputMask, pOutBuff, iBuffLen);
	//		StrUtil::StringCpyEx( pOutBuff, iBuffLen, strTrace );

	//		// pending \r\n
	//		if( iBuffLen < 3 )
	//		{
	//			pOutBuff -= 3-iBuffLen;
	//			iBuffLen = 3;
	//		}

	//		StrUtil::StringCpyEx( pOutBuff, iBuffLen, "\n" );

	//		pSpinBlock->Data.InputMask = trcInputMask;
	//		pSpinBlock->Data.KillSignal = false;

	//		m_TraceSpinBuffer.Write_Unlock( pSpinBlock );
	//	}
	//}

	
	TraceOutModule::TraceBufferType::BLOCK* TraceOutModule::TraceReserveWriteBuffer(uint trcInputMask, const char* traceName, char*& stringBuffer, int &buffLen )
	{
		TraceBufferType::BLOCK *pSpinBlock = m_TraceSpinBuffer.Write_Lock();

		stringBuffer = pSpinBlock->Data.TraceBuff;
		buffLen = sizeof(pSpinBlock->Data.TraceBuff);


		//StrUtil::StringCpyEx( stringBuffer, buffLen, traceName );
		//StrUtil::StringCpyEx( stringBuffer, buffLen, ": " );
		//AppendMaskPrefix( trcInputMask, stringBuffer, buffLen );
		AppendPrefix(traceName, trcInputMask, stringBuffer, buffLen);

		pSpinBlock->Data.InputMask = trcInputMask;

		return pSpinBlock;
	}

	void TraceOutModule::TraceSendWriteBuffer( TraceBufferType::BLOCK* pWriteBuffer, int buffLen )
	{
		char *pOutBuff = pWriteBuffer->Data.TraceBuff;
		int orgBuffLen = sizeof(pWriteBuffer->Data.TraceBuff);

		pOutBuff += orgBuffLen - buffLen;

		// pending \r\n
		if( buffLen < 3 )
		{
			pOutBuff -= 3-buffLen;
			buffLen = 3;
		}

		StrUtil::StringCpyEx( pOutBuff, buffLen, "\n" );

		pWriteBuffer->Data.KillSignal = false;
		pWriteBuffer->Data.TraceBuffUsedLen.store(orgBuffLen - buffLen, std::memory_order_relaxed);

		m_TraceSpinBuffer.Write_Unlock( pWriteBuffer );
	}


	// Trace flush result
	void TraceOutModule::TraceFlush()
	{
		// 10 sec timeout
		m_TraceSpinBuffer.WaitPendingWork(DurationMS(10*1000));
	}



	//////////////////////////////////////////////////////////////////////////////////////
	//
	// Trace system initialize
	//


	void Flush()
	{
		if (TraceOutModule::GetInstance() == nullptr)
			return;

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
} // namespace SF

