// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "Common/Typedefs.h"

#if !ANDROID
#include <mysql.h>
#include <mysqld_error.h>

#include "DB/DBTrace.h"
#include <my_global.h>
#endif

// TODO: reference additional headers your program requires here
