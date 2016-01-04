////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : Stack walker
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Synchronization.h"
#include "Common/StackPool.h"

#include "Common/StackWalker.h"

#if WINDOWS
#pragma comment(lib,"DbgHelp.lib")
#endif

//#define STACKWALKER_MIN_EXE_OFFSET ((intptr_t)&__ImageBase)
#define STACKWALKER_MIN_EXE_OFFSET ((intptr_t)0)


namespace BR
{
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Stack walker
	//

	class StackWalkerImpl
	{
	public:

		// stack frame for x86
		struct stack_frame 
		{
			struct stack_frame* pNext;
			void* pReturn;
		};


	private:

		UINT m_ModuleStackSkipDepth;
		HANDLE m_hProcess;
		wchar_t m_SymbolPath[1024];

	private:

		void UpdateSkipDepth();

#if WINDOWS
		HRESULT BuildSymbolPath();
		HRESULT LoadModuleSymbols();

		FORCEINLINE void GetStackFrame( CONTEXT& context, STACKFRAME64& stackFrame, DWORD& imageType );

		void STDCALL CaptureCallStackFast( CallStackTrace& stackTrace, DWORD imageType, CONTEXT &context, STACKFRAME64 &stackFrame, UINT skipDepth, UINT maxDepth );
		void STDCALL CaptureCallStackReliable( CallStackTrace& stackTrace, DWORD imageType, CONTEXT &context, STACKFRAME64 &stackFrame, UINT skipDepth, UINT maxDepth );
#else
#endif

	public:
		// Constructor
		StackWalkerImpl();
		virtual ~StackWalkerImpl();

		// initialize stace walker
		bool STDCALL Initialize();

		// get current stack trace
		void STDCALL CaptureCallStack( CallStackTrace& stackTrace, UINT skipDepth, UINT maxDepth );

		// print stack trace
		void PrintStackTrace(int channel);
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Page allocator
	//

	CallStackTrace::CallStackTrace()
	{
		StackTraceCount = 0;
		memset( StackTrace, 0, sizeof(StackTrace) );
	}

#if WINDOWS
	HRESULT CallStackTrace::PrintStackTrace( int channel, HANDLE hProcess )
	{
		BYTE Buffer[1024];
		memset( Buffer, 0, sizeof(Buffer) );

		SYMBOL_INFO &symbolInfo = *(SYMBOL_INFO*)Buffer;
		symbolInfo.SizeOfStruct = sizeof(SYMBOL_INFO);
		symbolInfo.MaxNameLen = sizeof(Buffer) - sizeof(SYMBOL_INFO);

		IMAGEHLP_LINE64 lineInfo;
		memset( &lineInfo, 0, sizeof(lineInfo) );
		lineInfo.SizeOfStruct = sizeof(lineInfo);
		defTrace( channel, "StackTrace:" );

		for( UINT stackDepth = 0; stackDepth < StackTraceCount && StackTrace[stackDepth] != 0; stackDepth++ )
		{
			DWORD64 offsetFromSymbol;
			BOOL symbol = SymFromAddr( hProcess, (DWORD64)StackTrace[stackDepth], &offsetFromSymbol, &symbolInfo );

			DWORD offsetFromLine;
			if( SymGetLineFromAddr64( hProcess, (DWORD64)StackTrace[stackDepth], &offsetFromLine, &lineInfo ) )
			{
				if( symbol )
				{
					defTrace( channel, "{0}:{1}", symbolInfo.Name, lineInfo.LineNumber );
				}
				else
				{
					defTrace( channel, "{0}:{1}", lineInfo.FileName, lineInfo.LineNumber );
				}
			}
			else
			{
				if( !symbol )
				{
					DWORD64 relativeAddress = (DWORD64)StackTrace[stackDepth] - ((DWORD64)STACKWALKER_MIN_EXE_OFFSET + MIN_EXEOFFSET);
					sprintf_s( symbolInfo.Name, symbolInfo.MaxNameLen, "0x%p", (void*)relativeAddress );
				}
				defTrace( channel, "{0}", symbolInfo.Name );
			}
		}

		Trace::Flush();

		return E_FAIL;
	}
#else
	HRESULT CallStackTrace::PrintStackTrace(int channel, NativeHandle hProcess)
	{
		char **strings;

		strings = backtrace_symbols(StackTrace, StackTraceCount);

		defTrace(channel, "StackTrace:" );

		for (UINT stackDepth = 0; stackDepth < StackTraceCount && strings != nullptr && strings[stackDepth] != 0; stackDepth++)
		{
			defTrace(channel, "{0}", strings[stackDepth]);
		}

		free(strings);

		return S_OK;
	}

#endif

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Memory Page allocator
	//

	StackWalkerImpl::StackWalkerImpl()
		: m_ModuleStackSkipDepth(0)
		, m_hProcess(nullptr)
	{
	}

	StackWalkerImpl::~StackWalkerImpl()
	{
	}
#if WINDOWS
	HRESULT StackWalkerImpl::BuildSymbolPath()
	{
		wchar_t curDir[1024], tempBuffer[1024];

		memset( m_SymbolPath, 0, sizeof(m_SymbolPath) );

		if( GetCurrentDirectoryW( (DWORD)countof(curDir), curDir ) > 0 )
		{
			curDir[countof(curDir)-1] = L'\0';
			wcscat_s(m_SymbolPath, curDir);
			wcscat_s(m_SymbolPath, L";");
		}
		else
		{
			curDir[0] = L'\0';
		}

		// Adding module directories
		if( GetModuleFileNameW(NULL, tempBuffer, (DWORD)countof(tempBuffer)) > 0 )
		{
			tempBuffer[countof(tempBuffer)-1] = L'\0';
			if( wcsncmp( curDir, tempBuffer, wcslen(curDir) ) != 0 )
			{
				wchar_t *found = tempBuffer + wcslen(tempBuffer) - 1;
				while( *found != L'\\' && found > tempBuffer ) found--;

				if( found >= tempBuffer ) *found = L'\0';

				wcscat_s(m_SymbolPath, tempBuffer);
				wcscat_s(m_SymbolPath, L";");
			}
		}

		return S_OK;
	}

	HRESULT StackWalkerImpl::LoadModuleSymbols()
	{
		HMODULE hModules[512];
		wchar_t imageFileName[1024];
		DWORD cbNeeded = 0;
		HRESULT result = 0;

		if( m_hProcess == NULL )
			return E_UNEXPECTED;

		memset(hModules, 0, sizeof(hModules));
		if( !EnumProcessModulesEx( m_hProcess, hModules, sizeof(hModules), &cbNeeded, LIST_MODULES_ALL ) )
		{
			AssertRel( cbNeeded <= sizeof(hModules) );
			return E_FAIL;
		}

		for( UINT iModule = 0; iModule < countof(hModules) && hModules[iModule] != 0; iModule++ )
		{
			MODULEINFO moduleInfo;
			memset( &moduleInfo, 0, sizeof(moduleInfo) );
			if( !GetModuleInformation( m_hProcess, hModules[iModule], &moduleInfo, sizeof(moduleInfo) ) )
				continue;

			if( GetModuleFileNameExW( m_hProcess, hModules[iModule], imageFileName, (DWORD)countof(imageFileName) ) == 0 )
			{
				//result = GetLatestHRESULT();
				continue;
			}

			if( SymLoadModuleExW( m_hProcess, NULL, imageFileName, NULL, (DWORD64)moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage, NULL, 0 ) == 0 )
			{
				//result = GetLatestHRESULT();
				continue;
			}
		}
		

		return S_OK;
	}
	
	void StackWalkerImpl::GetStackFrame( CONTEXT& context, STACKFRAME64& stackFrame, DWORD& imageType )
	{
		memset(&stackFrame, 0, sizeof(stackFrame));
#ifdef _M_IX86
		imageType = IMAGE_FILE_MACHINE_I386;
		stackFrame.AddrPC.Offset = context.Eip;
		stackFrame.AddrPC.Mode = AddrModeFlat;
		stackFrame.AddrFrame.Offset = context.Ebp;
		stackFrame.AddrFrame.Mode = AddrModeFlat;
		stackFrame.AddrStack.Offset = context.Esp;
		stackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
		imageType = IMAGE_FILE_MACHINE_AMD64;
		stackFrame.AddrPC.Offset = context.Rip;
		stackFrame.AddrPC.Mode = AddrModeFlat;
		stackFrame.AddrFrame.Offset = context.Rdi;
		stackFrame.AddrFrame.Mode = AddrModeFlat;
		stackFrame.AddrStack.Offset = context.Rsp;
		stackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
		imageType = IMAGE_FILE_MACHINE_IA64;
		stackFrame.AddrPC.Offset = context.StIIP;
		stackFrame.AddrPC.Mode = AddrModeFlat;
		stackFrame.AddrFrame.Offset = context.IntSp;
		stackFrame.AddrFrame.Mode = AddrModeFlat;
		stackFrame.AddrStack.Offset = context.IntRsp;
		stackFrame.AddrStack.Mode = AddrModeFlat;
#else
#error "Not supported CPU architecture"
#endif
	}

	// Find maximum skip depth
	void StackWalkerImpl::UpdateSkipDepth()
	{
		CallStackTrace stackTrace;
		UINT MaxSearchDepth = 7;
		HANDLE hThread = GetCurrentThread();
		STACKFRAME64 stackFrame;
		DWORD imageType = 0;
		CONTEXT context;

		context.ContextFlags = CONTEXT_CONTROL;
		RtlCaptureContext(&context);

		GetStackFrame( context, stackFrame, imageType );

		m_ModuleStackSkipDepth = 1;

#if defined(_M_IX86) || defined(_M_X64)
		// Use fast version stack trace
		stackTrace.StackTraceCount = CaptureStackBackTrace( 1, MaxSearchDepth, stackTrace.StackTrace, nullptr );
		//CaptureCallStackFast( stackTrace, imageType, context, stackFrame, 1, MaxSearchDepth );
#else
		CaptureCallStackReliable( stackTrace, imageType, context, stackFrame, 1, MaxSearchDepth );
#endif

		IMAGEHLP_LINE64 lineInfo;
		memset( &lineInfo, 0, sizeof(lineInfo) );
		lineInfo.SizeOfStruct = sizeof(lineInfo);
		for( UINT stackDepth = 0; stackDepth < MaxSearchDepth && stackTrace.StackTrace[stackDepth] != 0; stackDepth++ )
		{
			DWORD offsetFromLine = 0;
			if( !SymGetLineFromAddr64( m_hProcess, (DWORD64)stackTrace.StackTrace[stackDepth], &offsetFromLine, &lineInfo ) )
				continue;

			if( strstr( lineInfo.FileName, "stackwalker." ) != NULL )
			{
				// Update skippable depth
				m_ModuleStackSkipDepth = stackDepth + 1;
			}
		}

		// If we couldn't find any depth then set to 2 
		if( m_ModuleStackSkipDepth <= 1 )
		{
			m_ModuleStackSkipDepth = 2;
		}
		else
		{
			// remove one for initialize
			m_ModuleStackSkipDepth--;
		}
	}

	// This implementation tested on x86 and x64
	void STDCALL StackWalkerImpl::CaptureCallStackFast( CallStackTrace& stackTrace, DWORD imageType, CONTEXT &context, STACKFRAME64 &stackFrame, UINT skipDepth, UINT maxDepth )
	{
		ULONG_PTR lowLimit, highLimit;
		HANDLE hThread = GetCurrentThread();

		//GetCurrentThreadStackLimits( &lowLimit, &highLimit );
		PNT_TIB pTib = reinterpret_cast<PNT_TIB>(NtCurrentTeb());
		highLimit = (ULONG_PTR)pTib->StackBase;
		lowLimit = (ULONG_PTR)pTib->StackLimit;
		
		Assert(skipDepth >= 1);
		skipDepth--;
		maxDepth--;

		stack_frame *currentStackFrame = (stack_frame*)stackFrame.AddrFrame.Offset;
		stack_frame *latestStackFrame = currentStackFrame;
		UINT stackIndex = 0;
		for( UINT frameNumber = 1; 
			currentStackFrame != nullptr 
				&& ((frameNumber < m_ModuleStackSkipDepth) || (frameNumber - m_ModuleStackSkipDepth) < maxDepth)
				&& stackIndex < CallStackTrace::MAX_CALLSTACK_DEPTH;
			++frameNumber )
		{
			ULONG_PTR stackFramePtr = (ULONG_PTR)currentStackFrame;
			if( stackFramePtr == NULL || stackFramePtr < lowLimit || stackFramePtr > highLimit
				|| (intptr_t)currentStackFrame->pReturn < STACKWALKER_MIN_EXE_OFFSET)
			{
				stackTrace.StackTraceCount = stackIndex;

				//if( currentStackFrame->pReturn != nullptr && (stackFramePtr < lowLimit || stackFramePtr > highLimit) )
				//{
				//	// We steped into wrong place, get back previous stack and retrive again
				//	frameNumber--;
				//	currentStackFrame = latestStackFrame;
				//	stackFrame.AddrPC.Offset = (DWORD64)latestStackFrame->pReturn;
				//	stackFrame.AddrFrame.Offset = (DWORD64)latestStackFrame;
				//	stackFrame.AddrStack.Offset = (DWORD64)latestStackFrame;

				//	// Fast trace is failed, try reliable trace
				//	if( !StackWalk64( imageType, m_hProcess, hThread, &stackFrame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL ) )
				//		return;

				//	currentStackFrame = (stack_frame*)stackFrame.AddrFrame.Offset;
				//	
				//	if( frameNumber < skipDepth )
				//		continue;

				//	stackTrace.StackTrace[ stackIndex++ ] = (void*)stackFrame.AddrPC.Offset;
				//	continue;
				//}
				//else
				{
					return;
				}
			}

			latestStackFrame = currentStackFrame;

			if( frameNumber < skipDepth )
			{
				currentStackFrame = currentStackFrame->pNext;
				continue;
			}

			stackTrace.StackTrace[stackIndex++] = currentStackFrame->pReturn;
			currentStackFrame = currentStackFrame->pNext;
		}

		stackTrace.StackTraceCount = stackIndex;
	}

	void STDCALL StackWalkerImpl::CaptureCallStackReliable( CallStackTrace& stackTrace, DWORD imageType, CONTEXT &context, STACKFRAME64 &stackFrame, UINT skipDepth, UINT maxDepth )
	{
		HANDLE hThread = GetCurrentThread();

		UINT stackIndex = 0;
		for( UINT frameNumber = 1; (frameNumber - m_ModuleStackSkipDepth) < maxDepth && stackIndex < CallStackTrace::MAX_CALLSTACK_DEPTH; ++frameNumber )
		{
			if( !StackWalk64( imageType, m_hProcess, hThread, &stackFrame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL ) )
				break;

			// frame skipping
			if( frameNumber < skipDepth )
				continue;

			stackTrace.StackTrace[ stackIndex++ ] = (void*)stackFrame.AddrPC.Offset;

			// prevent infinite call
			if( stackFrame.AddrPC.Offset == stackFrame.AddrReturn.Offset )
				break;

			// 
			if( stackFrame.AddrReturn.Offset == 0 )
				break;
		}

		stackTrace.StackTraceCount = stackIndex;
	}

	// get current stack trace
	// #pragma optimize("y",off)
	void STDCALL StackWalkerImpl::CaptureCallStack( CallStackTrace& stackTrace, UINT skipDepth, UINT maxDepth )
	{
		STACKFRAME64 stackFrame;
		DWORD imageType = 0;
		CONTEXT context;

		context.ContextFlags = CONTEXT_CONTROL;
		RtlCaptureContext(&context);

		GetStackFrame( context, stackFrame, imageType );

#if defined(_M_IX86) || defined(_M_X64)
		// Use fast version stack trace
		maxDepth = std::min( maxDepth - skipDepth + m_ModuleStackSkipDepth, (UINT)CallStackTrace::MAX_CALLSTACK_DEPTH );
		stackTrace.StackTraceCount = CaptureStackBackTrace( skipDepth + m_ModuleStackSkipDepth, maxDepth, stackTrace.StackTrace, nullptr );
		//CaptureCallStackFast( stackTrace, imageType, context, stackFrame, skipDepth + m_ModuleStackSkipDepth, maxDepth );
#else
		CaptureCallStackReliable( stackTrace, imageType, context, stackFrame, skipDepth + m_ModuleStackSkipDepth, maxDepth );
#endif
	}


	// initialize stace walker
	// #pragma optimize("y",off)
	bool StackWalkerImpl::Initialize()
	{
		m_hProcess = GetCurrentProcess();

		if (FAILED(BuildSymbolPath()))
			return false;

		if (!SymInitializeW(m_hProcess, (PWSTR)m_SymbolPath, false))
			return false;

		// symbol load option
		DWORD symbolLoadOptions = SymGetOptions();
		symbolLoadOptions |= SYMOPT_LOAD_LINES | SYMOPT_FAIL_CRITICAL_ERRORS;
		symbolLoadOptions = SymSetOptions(symbolLoadOptions);

		if (FAILED(LoadModuleSymbols()))
			return false;

		UpdateSkipDepth();

		return true;
	}

#else


	// initialize stace walker
	bool StackWalkerImpl::Initialize()
	{
		m_ModuleStackSkipDepth = 2;

		return true;
	}

	// get current stack trace
	void StackWalkerImpl::CaptureCallStack(CallStackTrace& stackTrace, UINT skipDepth, UINT maxDepth)
	{
		stackTrace.StackTraceCount = backtrace(stackTrace.StackTrace, countof(stackTrace.StackTrace));
	}


#endif


	// print stack trace
	void StackWalkerImpl::PrintStackTrace(int channel)
	{
		CallStackTrace stackTrace;
		CaptureCallStack( stackTrace, 0, CallStackTrace::MAX_CALLSTACK_DEPTH );

		stackTrace.PrintStackTrace(channel, m_hProcess);
	}



	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Stack walker
	//

	StackWalkerImpl* StackWalker::stm_pInstance;


	// initialize stace walker
	bool StackWalker::Initialize()
	{
		if( stm_pInstance == nullptr )
		{
			stm_pInstance = new StackWalkerImpl;
			return stm_pInstance->Initialize();
		}

		return true;
	}

	void StackWalker::Deinitialize()
	{
		if( stm_pInstance == nullptr )
			return;

		delete stm_pInstance;
		stm_pInstance = nullptr;
	}

	// get current stack trace
	void StackWalker::CaptureCallStack( CallStackTrace& stackTrace, UINT skipDepth, UINT maxDepth )
	{
		if( stm_pInstance == nullptr )
			return;

		stm_pInstance->CaptureCallStack(stackTrace, skipDepth, maxDepth);
	}

	// print stack trace
	void StackWalker::PrintStackTrace(int channel)
	{
		if( stm_pInstance == nullptr )
			return;

		stm_pInstance->PrintStackTrace(channel);
	}


};	// namespace BR


