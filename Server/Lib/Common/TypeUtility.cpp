////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/TypeUtility.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/Trace.h"




namespace BR {

	/////////////////////////////////////////////////////////////////////
	//
	//	Type utility
	//


#if WINDOWS

	Result GetLastResult()
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

#elif LINUX

	Result GetLastResult()
	{
		int ierr = errno;
		switch (ierr)
		{
		case 0: return ResultCode::SUCCESS;
		case EPERM: return ResultCode::E_NO_PERMITION;
		case ENOENT: return ResultCode::E_NO_FILE_OR_DIR;
		case ESRCH: return ResultCode::E_NO_SUCH_PROCESS;
		case EINTR: return ResultCode::E_INTERRUPTED_SYSCALL;
		case EIO: return ResultCode::E_IO_ERROR;
		case ENXIO: return ResultCode::E_NOT_DEVICE;
		case E2BIG: return ResultCode::E_TOO_MANY_ARGLIST;
		case ENOEXEC: return ResultCode::E_INVALID_EXEC_FROMAT;
		case EBADF: return ResultCode::E_INVALID_FILE_HANDLE;
		case ECHILD: return ResultCode::E_NO_CHILD_PROCESS;
		case EAGAIN: return ResultCode::E_TRY_AGAIN;
		case ENOMEM: return ResultCode::OUT_OF_MEMORY;
		case EACCES: return ResultCode::E_NO_PERMITION;
		case EFAULT: return ResultCode::E_INVALID_POINTER;
		case ENOTBLK: return ResultCode::E_NOT_BLOCK;
		case EBUSY: return ResultCode::E_BUSY;
		case EEXIST: return ResultCode::E_FILE_EXISTS;
		case EXDEV: return ResultCode::E_CROSS_LINK;
		case ENODEV: return ResultCode::E_INVALID_DEVICE;
		case ENOTDIR: return ResultCode::E_INVALID_DIRECTORY;
		case EISDIR: return ResultCode::E_DIRECTORY;
		case EINVAL: return ResultCode::E_INVALID_ARG;
		case ENFILE: return ResultCode::E_NOT_ENOUGH_FILE_TABLE;
		case EMFILE: return ResultCode::E_TOO_MANY_OPENED_FILE;
		case ENOTTY: return ResultCode::E_INVALID_TYPEWRITER;
		case ETXTBSY: return ResultCode::E_BUSY_FILE;
		case EFBIG: return ResultCode::E_TOO_BIG_FILE;
		case ENOSPC: return ResultCode::E_NOT_ENOUGH_SPACE;
		case ESPIPE: return ResultCode::E_INVALID_SEEK;
		case EROFS: return ResultCode::E_READ_ONLY;
		case EMLINK: return ResultCode::E_TOO_MANY_LINK;
		case EPIPE: return ResultCode::E_INVALID_PIPE;
		case EDOM: return ResultCode::E_INVALID_MATH_DOMAIN;
		case ERANGE: return ResultCode::E_INVALID_NUMERIC;
		default: return ResultCode::UNEXPECTED;
		}
	}

#endif


	/////////////////////////////////////////////////////////////////////
	//
	//  Default Value
	//

	template<>
	INT64 DefaultValue<INT64>() { return 0; }

	template<>
	UINT64 DefaultValue<UINT64>() { return 0; }

	template<>
	INT32 DefaultValue<INT32>() { return 0; }

	template<>
	UINT32 DefaultValue<UINT32>() { return 0; }


} // namespace BR


