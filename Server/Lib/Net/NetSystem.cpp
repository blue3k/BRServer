////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Net base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Net/NetSystem.h"
#include "Net/NetSystem_impl.h"
#include "Common/HRESNet.h"
#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/SpinSharedBuffer.h"
#include "Common/MemoryPool.h"



namespace BR {
namespace Net {


#if LINUX

	HRESULT GetLastWSAHRESULT()
	{
		int ierr = errno;
		switch (ierr)
		{
		case 0: return S_OK;
		case EINTR: return E_NET_INTR;
		case EBADF: return E_NET_BADF;
		case EACCES: return E_NET_ACCES;
		case EFAULT: return E_NET_FAULT;
		case EINVAL: return E_NET_INVAL;
		case EMFILE: return E_NET_MFILE;
		case EWOULDBLOCK: return E_NET_WOULDBLOCK;
		case EINPROGRESS: return E_NET_INPROGRESS;
		case EALREADY: return E_NET_ALREADY;
		case ENOTSOCK: return E_NET_FAULT;
		case EDESTADDRREQ: return E_NET_DESTADDRREQ;
		case ENETDOWN: return E_NET_NETDOWN;
		case ENETUNREACH: return E_NET_NETUNREACH;
		case ENETRESET: return E_NET_NETRESET;
		case ECONNABORTED: return E_NET_CONNABORTED;
		case ECONNRESET: return E_NET_CONNRESET;
		case ENOBUFS: return E_NET_NOBUFS;
		case EISCONN: return E_NET_ISCONN;
		case ENOTCONN: return E_NET_NOTCONN;
		case ESHUTDOWN: return E_NET_SHUTDOWN;
		case ETOOMANYREFS: return E_NET_TOOMANYREFS;
		case ETIMEDOUT: return E_NET_TIMEDOUT;
		case ECONNREFUSED: return E_NET_CONNECTION_REFUSSED;
		case ELOOP: return E_NET_LOOP;
		case ENAMETOOLONG: return E_NET_NAMETOOLONG;


		case EHOSTDOWN: return E_NET_HOSTDOWN;
		case EHOSTUNREACH: return E_NET_HOSTUNREACH;
		case ENOTEMPTY: return E_NET_NOTEMPTY;
			//case EPROCLIM: return E_NET_PROCLIM;
			//case SYSNOTREADY: return E_NET_SYSNOTREADY;
			//case VERNOTSUPPORTED: return E_NET_VERNOTSUPPORTED;
			//case NOTINITIALISED: return E_NET_NOTINITIALISED;
			//case EDISCON: return E_NET_DISCON;
			//case SYSCALLFAILURE: return E_NET_SYSCALLFAILURE;
			//case HOST_NOT_FOUND: return E_NET_HOST_NOT_FOUND;

			//case TRY_AGAIN: return E_NET_TRY_AGAIN;
			//case _SECURE_HOST_NOT_FOUND: return E_NET_SECURE_HOST_NOT_FOUND;
		default:
			defTrace(Trace::TRC_WARN, "Unknown Winsock error {0}", ierr);
			return GetLastHRESULT();
		}
	}


#else

	HRESULT GetLastWSAHRESULT()
	{
		int ierr = WSAGetLastError();
		switch (ierr)
		{
		case 0: return S_OK;
		case WSAEINTR: return E_NET_INTR;
		case WSAEBADF: return E_NET_BADF;
		case WSAEACCES: return E_NET_ACCES;
		case WSAEFAULT: return E_NET_FAULT;
		case WSAEINVAL: return E_NET_INVAL;
		case WSAEMFILE: return E_NET_MFILE;
		case WSAEWOULDBLOCK: return E_NET_WOULDBLOCK;
		case WSAEINPROGRESS: return E_NET_INPROGRESS;
		case WSAEALREADY: return E_NET_ALREADY;
		case WSAENOTSOCK: return E_NET_FAULT;
		case WSAEDESTADDRREQ: return E_NET_DESTADDRREQ;
		case WSAENETDOWN: return E_NET_NETDOWN;
		case WSAENETUNREACH: return E_NET_NETUNREACH;
		case WSAENETRESET: return E_NET_NETRESET;
		case WSAECONNABORTED: return E_NET_CONNABORTED;
		case WSAECONNRESET: return E_NET_CONNRESET;
		case WSAENOBUFS: return E_NET_NOBUFS;
		case WSAEISCONN: return E_NET_ISCONN;
		case WSAENOTCONN: return E_NET_NOTCONN;
		case WSAESHUTDOWN: return E_NET_SHUTDOWN;
		case WSAETOOMANYREFS: return E_NET_TOOMANYREFS;
		case WSAETIMEDOUT: return E_NET_TIMEDOUT;
		case WSAECONNREFUSED: return E_NET_CONNECTION_REFUSSED;
		case WSAELOOP: return E_NET_LOOP;
		case WSAENAMETOOLONG: return E_NET_NAMETOOLONG;


		case WSAEHOSTDOWN: return E_NET_HOSTDOWN;
		case WSAEHOSTUNREACH: return E_NET_HOSTUNREACH;
		case WSAENOTEMPTY: return E_NET_NOTEMPTY;
		case WSAEPROCLIM: return E_NET_PROCLIM;
		case WSASYSNOTREADY: return E_NET_SYSNOTREADY;
		case WSAVERNOTSUPPORTED: return E_NET_VERNOTSUPPORTED;
		case WSANOTINITIALISED: return E_NET_NOTINITIALISED;
		case WSAEDISCON: return E_NET_DISCON;
		case WSASYSCALLFAILURE: return E_NET_SYSCALLFAILURE;
		case WSAHOST_NOT_FOUND: return E_NET_HOST_NOT_FOUND;

		case WSATRY_AGAIN: return E_NET_TRY_AGAIN;
		case WSA_SECURE_HOST_NOT_FOUND: return E_NET_SECURE_HOST_NOT_FOUND;
		default:
			defTrace(Trace::TRC_WARN, "Unknown Winsock error {0}", ierr);
			return GetLastHRESULT();
		}
	}

#endif





	namespace NetSystem
	{
		static UINT g_GatheringSize = 1024;
		static MemoryPool* g_pGatheringBufferPool = nullptr;


		HRESULT AllocBuffer(IOBUFFER_WRITE* &pIOBuffer)
		{
			pIOBuffer = new IOBUFFER_WRITE;

			return pIOBuffer == nullptr ? E_FAIL : S_OK;
		}

		HRESULT FreeBuffer(IOBUFFER_WRITE *pIOBuffer)
		{
			Util::SafeDelete(pIOBuffer);

			return S_OK;
		}

		HRESULT SetGatheringBufferSize(UINT bufferSize)
		{
			g_GatheringSize = bufferSize;
			MemoryPoolManager::GetMemoryPoolBySize(g_GatheringSize, g_pGatheringBufferPool);
			if (g_pGatheringBufferPool == nullptr)
				return E_FAIL;

			return S_OK;
		}

		HRESULT AllocGatheringBuffer(BYTE* &pBuffer, UINT& bufferSize)
		{
			if (g_pGatheringBufferPool == nullptr)
				return E_UNEXPECTED;

			void* pPtr = nullptr;
			bufferSize = g_GatheringSize;
			if(FAILED(g_pGatheringBufferPool->Alloc(pPtr, "AllocGatheringBuffer")))
				return E_OUTOFMEMORY;

			pBuffer = (BYTE*)pPtr;

			return S_OK;
		}

		HRESULT FreeGatheringBuffer(BYTE *pBuffer)
		{
			if (g_pGatheringBufferPool == nullptr)
				return E_FAIL;

			return g_pGatheringBufferPool->Free(pBuffer, "AllocGatheringBuffer");
		}
	}


} // namespace Net
} // namespace BR

