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
#include "Common/HRESCommon.h"
#include "Common/HRESNet.h"
#include "Common/Trace.h"




namespace BR {

	/////////////////////////////////////////////////////////////////////
	//
	//	Type utility
	//


#if WINDOWS

	HRESULT GetLastHRESULT()
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

#elif LINUX

	HRESULT GetLastHRESULT()
	{
		int ierr = errno;
		switch (ierr)
		{
		case 0: return S_OK;
		case EPERM: return E_NO_PERMITION;
		case ENOENT: return E_NO_FILE_OR_DIR;
		case ESRCH: return E_NO_SUCH_PROCESS;
		case EINTR: return E_INTERRUPTED_SYSCALL;
		case EIO: return E_IO_ERROR;
		case ENXIO: return E_NOT_DEVICE;
		case E2BIG: return E_TOO_MANY_ARGLIST;
		case ENOEXEC: return E_INVALID_EXEC_FROMAT;
		case EBADF: return E_INVALID_FILE_HANDLE;
		case ECHILD: return E_NO_CHILD_PROCESS;
		case EAGAIN: return E_TRY_AGAIN;
		case ENOMEM: return E_OUTOFMEMORY;
		case EACCES: return E_NO_PERMITION;
		case EFAULT: return E_INVALID_POINTER;
		case ENOTBLK: return E_NOT_BLOCK;
		case EBUSY: return E_BUSY;
		case EEXIST: return E_FILE_EXISTS;
		case EXDEV: return E_CROSS_LINK;
		case ENODEV: return E_INVALID_DEVICE;
		case ENOTDIR: return E_INVALID_DIRECTORY;
		case EISDIR: return E_DIRECTORY;
		case EINVAL: return E_INVALID_ARG;
		case ENFILE: return E_NOT_ENOUGH_FILE_TABLE;
		case EMFILE: return E_TOO_MANY_OPENED_FILE;
		case ENOTTY: return E_INVALID_TYPEWRITER;
		case ETXTBSY: return E_BUSY_FILE;
		case EFBIG: return E_TOO_BIG_FILE;
		case ENOSPC: return E_NOT_ENOUGH_SPACE;
		case ESPIPE: return E_INVALID_SEEK;
		case EROFS: return E_READ_ONLY;
		case EMLINK: return E_TOO_MANY_LINK;
		case EPIPE: return E_INVALID_PIPE;
		case EDOM: return E_INVALID_MATH_DOMAIN;
		case ERANGE: return E_INVALID_NUMERIC;
		default: return E_UNEXPECTED;
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


