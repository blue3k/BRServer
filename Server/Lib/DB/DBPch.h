// DBPch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "SFTypedefs.h"

#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "String/SFToString.h"
#include "String/SFToStringBase.h"
#include "String/SFToStringEngineTypes.h"


#if !ANDROID
#include <mysql.h>
#include <mysqld_error.h>

#include "DB/DBTrace.h"
#include <my_global.h>
#endif

