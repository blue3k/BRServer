////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Crash reporter linux
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#if LINUX || ANDROID

#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Trace_Internal.h"
#include "Common/File/BRFile.h"




namespace BR {
namespace Trace {


	void InitExceptionHandler()
	{
	}


};	// namespace Trace
} // namespace BR

#else

int CrashDump_Linux_Dummy = 0;

#endif // Linux

