////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : Stack walker
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Synchronize.h"
#include "Common/StackPool.h"


namespace BR
{

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Stack trace
	//

	struct CallStackTrace
	{
		enum {
			MAX_CALLSTACK_DEPTH		= 15,
			MIN_EXEOFFSET			= 0x100,
		};

		UINT StackTraceCount;
		void* StackTrace[MAX_CALLSTACK_DEPTH];

		// constructor
		CallStackTrace();

		// print stack trace
		HRESULT PrintStackTrace( Trace::TraceChannels channel, HANDLE hProcess );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Stack walker
	//

	class StackWalker
	{
	private:

		static class StackWalkerImpl *stm_pInstance;

	public:
		// initialize stace walker
		static bool __stdcall Initialize();
		static void __stdcall Deinitialize();

		// get current stack trace
		static void __stdcall CaptureCallStack( CallStackTrace& stackTrace, UINT skipDepth = 0, UINT maxDepth = CallStackTrace::MAX_CALLSTACK_DEPTH );

		// print stack trace
		static void PrintStackTrace(Trace::TraceChannels channel);
	};


}; // namespace BR



