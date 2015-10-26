#pragma once

#include "Common/Typedefs.h"

#ifndef MEMLOG_FUNC
#define MEMLOG_FUNC DLL_IMPORT
#endif

namespace MemLog
{
	// Logging place
	enum Logging {
		LOG_DebugOut = 1,
		LOG_Console = 2,
		LOG_ALL = LOG_DebugOut | LOG_Console,
	};

	class IMemLogger
	{
	public:
		// Release interface
		virtual void Release() = 0;

		// Initialize & terminate
		virtual void Initialize( Logging logPlace, UINT32 uiLogMask ) = 0;
		virtual void Terminate() = 0;

		// Controlling Log mask
		virtual UINT32 GetLogMask() = 0;
		virtual void SetLogMask( UINT32 uiLogMask ) = 0;
		virtual void EnableLog( UINT32 uiLogMask, bool bIsEnable = true ) = 0;

		// Add to log or remove from log list
		virtual void AddToLog( UINT32 uiLogMask, void* pObj, size_t szSize, UINT32 uiUser1 = 0, UINT32 uiUser2 = 0, UINT32 uiUser3 = 0 ) = 0;
		virtual void RemoveFromLog( void* pObj ) = 0;

		// Trace address of given memory
		virtual void TrackAddress( void* pTrackAddress ) = 0;

		// Report memory leak
		virtual void ReportLeak() = 0;

		// Difference check
		virtual void StartDifferenceCheck() = 0;

		virtual void ReportDifference() = 0;

		virtual void StopDifferenceCheck() = 0;
	};


};


MEMLOG_FUNC HRESULT SYSTEMAPI QueryMemLogger( MemLog::IMemLogger* &pLogger );
typedef HRESULT (SYSTEMAPI *T_QueryMemLogger)( MemLog::IMemLogger* &pLogger );

