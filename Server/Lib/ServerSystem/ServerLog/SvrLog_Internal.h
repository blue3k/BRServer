////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : debug trace message definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

//#include "Common/SvrTypes.h"
#include "Thread/Thread.h"
//#include "Common/SpinBuffer.h"
#include "Common/SharedObject.h"
#include "String/StrUtil.h"
#include "String/StrFormat.h"
#include "Common/SpinBufferMT.h"
#include "Common/File/BRFile.h"


#ifndef SHIPPING
#define SVRTRC_DBG
#endif



#define TRCCALL 


namespace SF {
namespace Trace {

	//Struct for Spin Buffer
	struct TraceBufferItem
	{
		bool	KillSignal = false;
		uint	InputMask = 0;
		char	TraceBuff[4096];
		// as far as this structure is part of SpinBuffer data, whole structure should be fine with multicore cache. This atomic will provide a little bit safety
		std::atomic<uint>	TraceBuffUsedLen;

		TraceBufferItem()
			: KillSignal(false)
			, InputMask(0)
			, TraceBuffUsedLen(0)
		{
			TraceBuff[0] = 0;
		}

	};


	////////////////////////////////////////////////////////////////////////////////
	//
	// Trace Output Module
	//

	class TraceOutModule : public BR::Thread, public SharedObject
	{
	public:
		typedef BR::SpinBufferMT<TraceBufferItem,512> TraceBufferType;

	private:

		/////////////////////////////////////////////////////
		// Static for shared use

		// Trace output mode Mask 
		IO::File	m_LogFile[TRCOUT_NUMFILE];

		// Latest log file creation time
		uint		m_tLogFileHour[TRCOUT_NUMFILE];

		TimeStampSec     m_tCurTime;
		struct tm	m_tCurTimeTM;

		// Trace output mode Mask 
		uint32_t		m_uiOutputMask = std::numeric_limits<uint32_t>::max();
		uint32_t		m_uiOutputMaskDebugger = std::numeric_limits<uint32_t>::max();
		uint32_t		m_uiOutputMaskFile[TRCOUT_NUMFILE] = { std::numeric_limits<uint32_t>::max(), };

#if WINDOWS
		// System event log handle
		HANDLE		m_hEventLog;

		// System console handle
		HANDLE		m_hConsole;
#endif

		// Latest checked time
		TimeStampSec	m_tTraceCheck;

		// Line header timestamp
		TimeStampSec	m_tLineHeader;

		// Pre updated registry time
		TimeStampSec	m_tRegCheck;
		TimeStampSec	m_tLineHdrCheck;

		// Line header buffer
		char		m_szLineHeader[512];
		uint		m_uiLineHeaderLen;

		//// SpinBuffer for TraceOut
		TraceBufferType m_TraceSpinBuffer;

		// Static instance for singleton
		static TraceOutModule *stm_pInstance;


		// Open Log file
		Result OpenLogFile( int iFile, const struct tm &curtm, const char *strFileName );



		void ReserveTimePrefix(char* &szDest, int& iBuffLen);

		// Append Trace mask prefix
		void AppendMaskPrefix( uint32_t trcMask, char* &szDest, int& iBuffLen );

		// Append mask prefix + ReserveTimePrefix
		void AppendPrefix(const char* traceName, uint32_t trcMask, char* &szDest, int& iBuffLen);

	public:

		// Private for singleton
		TraceOutModule();
		~TraceOutModule();


		static inline void SetInstance( TraceOutModule *pInstance );
		static inline TraceOutModule* GetInstance();


		// Override stop to handle kill method
		virtual void Stop( bool bSendKillEvt ) override;

		// Thread inherit
		virtual void Run() override;

		// Check file system and update
		Result CheckAndUpdate( TimeStampSec tCurTime );

		void ValidateLogFile();

		// Update line header
		void UpdateLineHeader();

		// update console handle
		void UpdateConsoleHandle();

		void UpdateOutputMask();

		// Event log
		void EventLog( DWORD dwEventId, const WCHAR *string1 = L"", const WCHAR *string2 = L"" );

		// Console output
#if WINDOWS
		void ConsoleOut(const WCHAR *strString, size_t strLen);
#else
		void ConsoleOut(const char *strString, size_t strLen);
#endif


		// Trace print out
		void TraceOut( uint trcOutMask, char * szOutput, size_t outputStringLen);

		//// Trace print data to Spin Buffer
		//void TracePush( UINT trcInputMask, const char *strTrace, const char* traceName);

		// two-way version
		TraceBufferType::BLOCK* TraceReserveWriteBuffer(uint trcInputMask, const char* traceName, char*& stringBuffer, int &buffLen );
		void TraceSendWriteBuffer( TraceBufferType::BLOCK* pWriteBuffer, INT buffLen );


		// Trace flush result
		void TraceFlush();
	};


#include "Trace_Internal.inl"



}; // namespace Trace
} //namespace SF

