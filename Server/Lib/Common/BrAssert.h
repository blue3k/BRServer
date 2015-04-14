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

#include "Typedefs.h"



#ifdef _DEBUG

// Base Assert
#define Assert(expr) assert(expr)

// Assert even release mode
#define AssertRel(expr) assert(expr)


#else // #ifdef _DEBUG

#define Assert(condi)

// Assert even release mode
#define AssertRel(condi) \
				do{ \
					if( !(condi) ) {\
						__debugbreak(); \
					}else{}\
				}while(0); \


#endif




