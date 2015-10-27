////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 Madk
// 
// Author : KyungKun Ko
//
// Description : Crash reporter win32
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Trace_Internal.h"
#include "Common/HRESEvent.h"

#if WINDOWS

namespace BR {
namespace Trace {

	//////////////////////////////////////////////////////////////////////////////////////
	//
	//  System Exception handler
	//

	static char DumpFilePath[1024];
	void WriteCrashDump(EXCEPTION_POINTERS* ipExPtrs, MINIDUMP_TYPE dumpType, const char* strMode)
	{
		const char szDumpFileExt[] = ".dmp";
		SYSTEMTIME kSysTime;
		GetLocalTime(&kSysTime);

		//if (GetModuleFileNameA(nullptr, strModulePath, MAX_PATH) != 0)
		//{
		//	_splitpath_s(strModulePath, nullptr, 0, nullptr, 0, strModuleName, sizeof(strModuleName), NULL, 0);
		//}
		sprintf_s(DumpFilePath, "..\\log\\[%04d_%02d_%02d]%2d_%02d_%02d_%s_[%s]%s",
			kSysTime.wYear, kSysTime.wMonth, kSysTime.wDay,
			kSysTime.wHour, kSysTime.wMinute, kSysTime.wSecond, strMode, Util::GetServiceNameA(), szDumpFileExt);
		//else
		//{
		//	sprintf_s(DumpFilePath, "..\\log\\[%04d_%02d_%02d]%2d_%02d_%02d_%s_[%s]%s",
		//		kSysTime.wYear, kSysTime.wMonth, kSysTime.wDay,
		//		kSysTime.wHour, kSysTime.wMinute, kSysTime.wSecond, strMode, strModuleName, szDumpFileExt);
		//}


		HANDLE hFile = CreateFileA(
			DumpFilePath,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hFile)
		{
			MINIDUMP_EXCEPTION_INFORMATION eInfo;
			eInfo.ThreadId = GetCurrentThreadId();
			eInfo.ExceptionPointers = ipExPtrs;
			eInfo.ClientPointers = FALSE;


			BOOL bResult = MiniDumpWriteDump(
				GetCurrentProcess(),
				GetCurrentProcessId(),
				hFile,
				dumpType,
				ipExPtrs ? &eInfo : NULL,
				NULL,
				NULL);
		}
		CloseHandle(hFile);
	}

	LONG __stdcall CrashHandler( EXCEPTION_POINTERS* ipExPtrs )
	{
		//char strModulePath[MAX_PATH] = "";
		//char strModuleName[MAX_PATH] = "_";
		BOOL bResult = FALSE;
		UINT uiRetCode = EXCEPTION_EXECUTE_HANDLER;
		MINIDUMP_TYPE dumpType = MiniDumpWithFullMemory;


	#ifdef _DEBUG
		uiRetCode = EXCEPTION_CONTINUE_SEARCH;
	#endif



	#ifdef _DEBUG
		const char* strMode = "Debug";
	#else
		const char* strMode = "Release";
	#endif

		WriteCrashDump(ipExPtrs, MiniDumpNormal, strMode);
		if (dumpType != MiniDumpNormal)
		{
#ifdef _DEBUG
			strMode = "DebugFull";
#else
			strMode = "ReleaseFull";
#endif

			WriteCrashDump(ipExPtrs, dumpType, strMode);
		}

		Trace::Flush();

		return uiRetCode;
	}




	//////////////////////////////////////////////////////////////////////////////////////
	//
	//  System Exception handler
	//


	void InitExceptionHandler()
	{
		::SetUnhandledExceptionFilter( &CrashHandler );
		//return Exception_Crash;
		//__debugbreak();
		//DumpSnapShot();
	}


	ExceptionMode g_expMode = Exception_Crash;//InitExceptionHandler();


	void DumpSnapShot()
	{
		g_expMode = Exception_SnapShot;
		__debugbreak();
	}

};	// namespace Trace
} // namespace BR


#endif