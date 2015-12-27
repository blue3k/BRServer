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



#if WINDOWS
#define STACKWALKER_CALL STDCALL
#else
#define STACKWALKER_CALL
#endif



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

		size_t StackTraceCount;
		void* StackTrace[MAX_CALLSTACK_DEPTH];

		// constructor
		CallStackTrace();

		// print stack trace
		HRESULT PrintStackTrace( int channel, NativeHandle hProcess );
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
		static bool STACKWALKER_CALL Initialize();
		static void STACKWALKER_CALL Deinitialize();


		// get current stack trace
		static void STACKWALKER_CALL CaptureCallStack( CallStackTrace& stackTrace, UINT skipDepth = 0, UINT maxDepth = CallStackTrace::MAX_CALLSTACK_DEPTH );

		// print stack trace
		static void PrintStackTrace(int channel);
	};


}; // namespace BR



