////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Debug definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/StackWalker.h"


#if LINUX
#define __debugbreak() raise(SIGTRAP)
#else
#endif

#define ForceAssert(condi) \
				do{ \
					if( !(condi) ) {\
						ReportAssert();\
						__debugbreak(); \
					}else{}\
				}while(0); \


inline void ReportAssert() { BR::CallStackTrace trace; trace.PrintStackTrace(1, 0); }


#if defined(_DEBUG) || defined(DEBUG)

// Base Assert
#define Assert(expr) ForceAssert(expr)

// Assert even release mode
#define AssertRel(expr) ForceAssert(expr)


#else // #ifdef _DEBUG

#define Assert(condi)

// Assert in release mode
#define AssertRel(condi) ForceAssert(condi);


#endif

