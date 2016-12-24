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
#include "Common/BRResult.h"






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

		size_t m_StackTraceCount;
		void* m_StackTrace[MAX_CALLSTACK_DEPTH];

		// constructor
		CallStackTrace();

		// print stack trace
		BR::Result PrintStackTrace( int channel, HANDLE hProcess );

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
		static bool Initialize();
		static void Deinitialize();


		// get current stack trace
		static void CaptureCallStack( CallStackTrace& stackTrace, UINT skipDepth = 0, UINT maxDepth = CallStackTrace::MAX_CALLSTACK_DEPTH );

		// print stack trace
		static void PrintStackTrace(int channel);
	};


}; // namespace BR



