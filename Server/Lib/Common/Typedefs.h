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


// Decide target
#if __GNUC__
	#if __x86_64__ || __ppc64__
		#define X64	true
	#else
		#define X32	true
	#endif
#elif _WIN64 || defined(_WIN64)
#define X64	true
#elif _WIN32
#define X32	true
#endif



// Decide platform
#if __GNUC__
#if __ANDROID__
#define ANDROID true
#else
#define LINUX true
#endif
#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS true
#endif

#if WINDOWS

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifdef _USE_32BIT_TIME_T
#error "Do not use 32bit time_t definition"
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif 

#define WIN32_LEAN_AND_MEAN

#include <SDKDDKVer.h>
#include <crtdbg.h>
#include <SDKDDKVer.h>
//#include <Winnt.h>
#include <windows.h>
#include <stdint.h>

#include <psapi.h>
#include <DbgHelp.h>

#include <crtdbg.h>
#include <tchar.h>
#include <atltrace.h>
#include <mbstring.h>
#include <atlcore.h>
#include <cstringt.h>
#include <winsock2.h>
#include <Mswsock.h>
#include <Ws2tcpip.h>
#include <BaseTsd.h>
#include <wmsdkidl.h>
#include <shlwapi.h>
#include <intrin.h>
#include <emmintrin.h>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <stdint.h>


#define timegm _mkgmtime

#define INVALID_NATIVE_HANDLE_VALUE INVALID_HANDLE_VALUE



#elif LINUX

#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <execinfo.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdint.h>
#include <semaphore.h>
#include <my_global.h>

typedef uint8_t UINT8;
typedef int8_t INT8;
typedef uint16_t UINT16;
typedef int16_t INT16;
typedef uint32_t UINT32;
typedef int32_t INT32;
typedef uint64_t UINT64;
typedef int64_t INT64;
typedef unsigned long ULONG;
typedef long LONG;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef const wchar_t* LPCWSTR;
typedef wchar_t* LPWSTR;
typedef void* PVOID;
typedef wchar_t WCHAR;

typedef unsigned int UINT;
typedef int INT;

typedef unsigned short USHORT;
typedef short SHORT;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef UINT8 BYTE;


// Additional alise type
typedef INT8				SBYTE;
typedef SBYTE				*PSBYTE;


typedef void* HANDLE;
typedef HANDLE HMODULE;

#define INVALID_NATIVE_HANDLE_VALUE NativeHandle(-1)



#define TRUE (1)
#define FALSE (0)



#define IN
#define OUT

#define SOCKET int



#define MAX_PATH 512

#elif ANDROID


#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <assert.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdint.h>
#include <unwind.h>
#include <dlfcn.h>

typedef uint8_t UINT8;
typedef int8_t INT8;
typedef uint16_t UINT16;
typedef int16_t INT16;
typedef uint32_t UINT32;
typedef int32_t INT32;
typedef uint64_t UINT64;
typedef int64_t INT64;
typedef unsigned long ULONG;
typedef long LONG;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef const wchar_t* LPCWSTR;
typedef wchar_t* LPWSTR;
typedef void* PVOID;
typedef wchar_t WCHAR;

typedef unsigned int UINT;
typedef int INT;

typedef unsigned short USHORT;
typedef short SHORT;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef UINT8 BYTE;


// Additional alise type
typedef INT8				SBYTE;
typedef SBYTE				*PSBYTE;


typedef void* HANDLE;
typedef HANDLE HMODULE;

#define INVALID_NATIVE_HANDLE_VALUE NativeHandle(-1)



#define TRUE (1)
#define FALSE (0)



#define IN
#define OUT

#define SOCKET int
#define INVALID_SOCKET (-1)


#define MAX_PATH 512

#else

#ifndef SWIG
	#error "Not supported platform"
#endif

#endif



#include <thread>
#include <mutex>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <errno.h>
#include <malloc.h>
#include <float.h>
#include <iostream>
#include <string>
#include <string.h>

#include <limits>
#include <limits.h>

#include <time.h>
#include <memory.h>
#include <math.h>
#include <memory>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include <vector>
#include <list>
#include <queue>
#include <assert.h>
#include <atomic>

#include <functional>


// See MSDN common datatypes section for base type definitions
// http://msdn.microsoft.com/en-us/library/aa505945.aspx


// Additional alise type
typedef INT8				SBYTE;
typedef SBYTE				*PSBYTE;

typedef HANDLE				NativeHandle;


// Thread synchronize counter type
#if defined(X64)

// System sync counter type
typedef UINT64	CounterType;
typedef INT64	SignedCounterType;

// System atomic integer type
typedef UINT64	SysUInt;
typedef INT64	SysInt;

#else

// System sync counter type
typedef ULONG	CounterType;
typedef LONG	SignedCounterType;

// System atomic integer type
typedef int		SysInt;
typedef unsigned int		SysUInt;



#endif



typedef std::chrono::steady_clock ClockType;
typedef std::chrono::duration<UINT> DurationSec;
typedef std::chrono::time_point<ClockType, DurationSec> TimeStampSec;

typedef std::chrono::duration<UINT, std::milli> DurationMS;
typedef std::chrono::time_point<ClockType, DurationMS> TimeStampMS;

typedef std::chrono::duration<UINT64, std::micro> DurationMicro;
typedef std::chrono::time_point<ClockType, DurationMicro> TimeStampMicro;



////////////////////////////////////////////////////////////////////////////////
//
//	Auxiliary types
//


template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
	return N;
}


#ifndef SWIG

template<class ... ArgTypes>
inline void unused(ArgTypes... ) {}

#endif


////////////////////////////////////////////////////////////////////////////////
//
//	Keywords
//



#if __GNUC__

#define FORCEINLINE __attribute__((always_inline))
//#define STDCALL __attribute__((stdcall))
#define STDCALL 
#define FASTCALL __attribute__((fastcall))
#define TEMPLATE_EXTERN

// disable unreferenced label warnning (Proc_End)
#define DO_PRAGMA(x) _Pragma (#x)
#define COMPILETIME_TODO(x) DO_PRAGMA(message ("TODO - " #x))
#define COMPILETIME_MESSAGE(x) DO_PRAGMA(message ("Message - " #x))
#define COMPILETIME_WARNING(x) DO_PRAGMA(message ("Warning - " #x))

#define DLL_IMPORT 
#define SYSTEMAPI 

#else

#define FORCEINLINE __forceinline
#define STDCALL __stdcall
#define FASTCALL __fastcall
#define TEMPLATE_EXTERN extern

// disable unreferenced label warnning (Proc_End)
#pragma warning( disable : 4102 )
#pragma warning( disable : 4996 )

#define DO_PRAGMA(x) __pragma(#x)
#define COMPILETIME_TODO(x) DO_PRAGMA(comment ("TODO - " #x))
#define COMPILETIME_MESSAGE(x) DO_PRAGMA(comment ("Message - " #x))
#define COMPILETIME_WARNING(x) DO_PRAGMA(comment ("Warning - " #x))

#define DLL_IMPORT __declspec(dllimport)
#define SYSTEMAPI APIENTRY




#endif



#include "Common/BRResult.h"

#include "Common/ResultCode/BRResultCodeSystem.h"



