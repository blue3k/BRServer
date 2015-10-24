////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif


#ifndef NOMINMAX
#define NOMINMAX
#endif 

#define WIN32_LEAN_AND_MEAN

#include <crtdbg.h>
#include <SDKDDKVer.h>
#include <new.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <errno.h>
#include <malloc.h>
#include <crtdbg.h>
#include <tchar.h>
//#include <atltrace.h>
#include <mbstring.h>
//#include <atlcore.h>
//#include <cstringt.h>
// Combine with system default types
//#include <afxwin.h>
#include <winsock2.h>
#include <Mswsock.h>
#include <Ws2tcpip.h>
#include <BaseTsd.h>
//#include <windows.h>
#include <wmsdkidl.h>
#include <stdlib.h>
#include <stdio.h>
#include <shlwapi.h>
#include <float.h>

#include <limits>
#include <limits.h>

#include <time.h>

#include <math.h>
#include <memory>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include <time.h>
#include <vector>
#include <queue>
#include <intrin.h>
#include <emmintrin.h>
#include <assert.h>
#include <atomic>

#include <functional>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>


// See MSDN common datatypes section for base type definitions
// http://msdn.microsoft.com/en-us/library/aa505945.aspx


// Additional alise type
typedef INT8				SBYTE;
typedef SBYTE				*PSBYTE;




// Thread synchronize counter type
#ifdef _WIN64
		// System sync counter type
		typedef UINT64	CounterType;
		typedef INT64	SignedCounterType;

		// System atomic integer type
		typedef UINT64	SysUInt;
		typedef INT64	SysInt;
#else // regard as WIN32
		// System sync counter type
		typedef ULONG	CounterType;
		typedef LONG	SignedCounterType;

		// System atomic integer type
		typedef int		SysInt;
		typedef unsigned int		SysUInt;
#endif



////////////////////////////////////////////////////////////////////////////////
//
//	Debugger display types
//



struct __UTF8
{
	char ch[1];
};



////////////////////////////////////////////////////////////////////////////////
//
//	Auxiliary types
//



////////////////////////////////////////////////////////////////////////////////
//
//	Keywords
//

#define FORCEINLINE __forceinline

#ifdef DEBUG
#define DEBUG_DYNAMIC_CAST(TargetType,src) (dynamic_cast<TargetType>(src))
#else
#define DEBUG_DYNAMIC_CAST(TargetType,src) ((TargetType)(src))
#endif

// disable unreferenced label warnning (Proc_End)
#pragma warning( disable : 4102 )



