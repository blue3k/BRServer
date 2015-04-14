// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif


#include "Common/Typedefs.h"
#include "Common/HRESSvrSys.h"
#include "Common/Utility.h"
#include "Common/StrUtil.h"
#include "Common/Message.h"
#include "Common/Trace.h"
#include "Common/Synchronize.h"
#include "Common/Memory.h"
#include "Common/Argument.h"
#include "Common/ToString.h"
#include "Common/ToStringBase.h"
// TODO: reference additional headers your program requires here
