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
#include "Common/Thread.h"
//#include "Common/SpinBuffer.h"
#include "Common/SharedObj.h"
#include "Common/StrUtil.h"
#include "Common/StrFormat.h"
#include "Common/SpinBufferMT.h"


#ifndef SHIPPING
#define SVRTRC_DBG
#endif



#define TRCCALL 


namespace BR {
namespace Trace {

	//Struct for Spin Buffer
	typedef struct tag_TraceBufferItem
	{
		bool	KillSignal;
		UINT	InputMask;
		char	TraceBuff[4096];

		tag_TraceBufferItem()
			:KillSignal(false),
			InputMask(0)
		{
			TraceBuff[0] = 0;
		}
	} TraceBufferItem;


	////////////////////////////////////////////////////////////////////////////////
	//
	// Trace Output Module
	//

	class TraceOutModule : public BR::Thread, public BR::SharedObj
	{
	public:
		typedef BR::SpinBufferMT<TraceBufferItem,30> TraceBufferType;

	private:

		/////////////////////////////////////////////////////
		// Static for shared use

		// Trace output mode Mask 
		HANDLE		m_hLogFile[TRCOUT_NUMFILE];

		// Latest log file creation time
		UINT		m_tLogFileHour[TRCOUT_NUMFILE];

		time_t      m_tCurTime;
		struct tm	m_tCurTimeTM;

		// Trace output mode Mask 
		UINT		m_uiOutputMask;

		// Trace output mode Mask 
		UINT		m_uiDbgOutputMask;

		// System event log handle
		HANDLE		m_hEventLog;

		// System console handle
		HANDLE		m_hConsole;

		// Latest checked time
		ULONGLONG	m_tTraceCheck;

		// Line header timestamp
		ULONGLONG	m_tLineHeader;

		// Pre updated registry time
		TimeStampMS		m_tRegCheck;
		TimeStampMS		m_tLineHdrCheck;

		// Line header buffer
		char		m_szLineHeader[512];
		UINT		m_uiLineHeaderLen;
		WCHAR		m_wszLineHeader[512];

		//// SpinBuffer for TraceOut
		TraceBufferType m_TraceSpinBuffer;
		//CRITICAL_SECTION m_OutputLock;

		// Static instance for singleton
		static BR::SharedPtr<TraceOutModule> stm_pInstance;

		// Private for singleton
		TraceOutModule();
		~TraceOutModule();


		// Open Log file
		HRESULT OpenLogFile( int iFile, const struct tm &curtm, wchar_t *strFileName );


		// Append Trace mask prefix
		void AppendMaskPrefix( ULONG trcMask, char* &szDest, INT& iBuffLen );

	public:

		static inline void NewInstance();
		static inline void SetInstance( TraceOutModule *pInstance );
		static inline TraceOutModule* GetInstance();


		// Override stop to handle kill method
		virtual void Stop( bool bSendKillEvt );

		// Thread inherit
		virtual void Run() override;

		// Check file system and update
		HRESULT CheckAndUpdate( TimeStampMS tCurTime );

		void ValidateLogFile();

		// Update line header
		void UpdateLineHeader();

		// update console handle
		void UpdateConsoleHandle();

		// Event log
		void EventLog( DWORD dwEventId, const WCHAR *string1 = L"", const WCHAR *string2 = L"" );

		// Console output
		void ConsoleOut( const WCHAR *strString1, const WCHAR *strString2 );


		// Trace print out
		void TraceOut( UINT trcOutMask, const char * szOutput );

		// Trace print data to Spin Buffer
		void TracePush( UINT trcInputMask, const char *strTrace, const char* traceName);

		// two-way version
		TraceBufferType::BLOCK* TraceReserveWriteBuffer( UINT trcInputMask, const char* traceName, char*& stringBuffer, INT &buffLen );
		void TraceSendWriteBuffer( TraceBufferType::BLOCK* pWriteBuffer, INT buffLen );


		// Trace flush result
		void TraceFlush();
	};


#include "Trace_Internal.inl"



}; // namespace Trace
} //namespace BR

