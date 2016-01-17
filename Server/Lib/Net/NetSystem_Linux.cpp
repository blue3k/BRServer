////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Net base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Common/HRESCommon.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/TypeUtility.h"
#include "Common/SystemSynchronization.h"
#include "Common/MemoryPool.h"
#include "Net/NetSystem.h"
#include "Net/NetSystem_impl.h"
#include "Net/NetSystem_Linux.h"
#include "Net/NetTrace.h"


#if LINUX

#include "Net/NetSystem_EPOLL.h"
#include "Net/NetSystem_KQUEUE.h"


BR_MEMORYPOOL_IMPLEMENT(Net::IOBUFFER_WRITE);
BR_MEMORYPOOL_IMPLEMENT(Net::IOBUFFER_READ);
BR_MEMORYPOOL_IMPLEMENT(Net::IOBUFFER_ACCEPT);


namespace BR {
namespace Net {

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
		case EWOULDBLOCK: return E_NET_TRY_AGAIN; // Linux epoll
#if EWOULDBLOCK != EAGAIN
		case EAGAIN: return E_NET_TRY_AGAIN;
#endif
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
		case EPIPE: return E_NET_SHUTDOWN;
		case ETOOMANYREFS: return E_NET_TOOMANYREFS;
		case ETIMEDOUT: return E_NET_TIMEDOUT;
		case ECONNREFUSED: return E_NET_CONNECTION_REFUSSED;
		case ELOOP: return E_NET_LOOP;
		case ENAMETOOLONG: return E_NET_NAMETOOLONG;

		case EHOSTDOWN: return E_NET_HOSTDOWN;
		case EHOSTUNREACH: return E_NET_HOSTUNREACH;
		case ENOTEMPTY: return E_NET_NOTEMPTY;
		case EPROTO: return E_NET_PROTO;
		case EPROTOTYPE: return E_NET_PROTOTYPE;
		case ENOPROTOOPT: return E_NET_NOPROTOOPT;
		case ENONET: return E_NET_NONET;
		case EOPNOTSUPP: return E_NET_OPNOTSUPP;
			//case EPROCLIM: return E_NET_PROCLIM;
			//case SYSNOTREADY: return E_NET_SYSNOTREADY;
			//case VERNOTSUPPORTED: return E_NET_VERNOTSUPPORTED;
			//case NOTINITIALISED: return E_NET_NOTINITIALISED;
			//case EDISCON: return E_NET_DISCON;
			//case SYSCALLFAILURE: return E_NET_SYSCALLFAILURE;
			//case HOST_NOT_FOUND: return E_NET_HOST_NOT_FOUND;

			//case TRY_AGAIN: return E_NET_TRY_AGAIN;
			//case _SECURE_HOST_NOT_FOUND: return E_NET_SECURE_HOST_NOT_FOUND;
			//case WSA_IO_PENDING: return E_NET_IO_PENDING;
		default:
			defTrace(Trace::TRC_WARN, "Unknown Winsock error {0}", ierr);
			return GetLastHRESULT();
		}
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Overlapped I/O structures
	//


	// Clear Buffer
	void IOBUFFER::ClearBuffer()
	{
	}



	IOBUFFER_RWBASE::IOBUFFER_RWBASE()
	{
	}




	IOBUFFER_WRITE::IOBUFFER_WRITE()
	{
		memset(this, 0, sizeof(IOBUFFER_WRITE));
	}

	IOBUFFER_WRITE::~IOBUFFER_WRITE()
	{
		ClearBuffer();

		Util::SafeRelease(pMsgs);
	}


	IOBUFFER_READ::IOBUFFER_READ()
	{
		memset(this, 0, sizeof(IOBUFFER_READ));

		iSockLen = sizeof(sockaddr_storage);
	}

	IOBUFFER_READ::~IOBUFFER_READ()
	{
		ClearBuffer();
	}

	HRESULT IOBUFFER_READ::SetPendingTrue()
	{
		bool expected = false;
		while (!bIsPending.compare_exchange_weak(expected, true, std::memory_order_seq_cst))
		{
			if (expected == true)
			{
				return E_FAIL;
			}

			expected = false;
		}
		return S_OK;
	}

	HRESULT IOBUFFER_READ::SetPendingFalse()
	{
		bool expected = true;
		while (!bIsPending.compare_exchange_weak(expected, false, std::memory_order_seq_cst))
		{
			if (expected == true)
			{
				return E_FAIL;
			}

			expected = false;
		}
		return S_OK;
	}


	IOBUFFER_ACCEPT::IOBUFFER_ACCEPT()
	{
		memset(this, 0, sizeof(IOBUFFER_ACCEPT));
	}


	IOBUFFER_ACCEPT::~IOBUFFER_ACCEPT()
	{
		ClearBuffer();
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	//	socket network system
	//

	namespace NetSystem
	{
		// winsock system open count
		static std::atomic<LONG> g_lWSOpenCount(0);
		static BR::CriticalSection g_InitLock;


		// Open network system
		HRESULT OpenSystem( UINT uiOverBufferCount, UINT numNetThread, UINT gatheringBufferSize)
		{
			MutexScopeLock scopeLock(g_InitLock);

			auto orgCounter = g_lWSOpenCount.fetch_add(1, std::memory_order_relaxed);

			if (orgCounter == 0)
			{
				Net::IOBUFFER_WRITE::MemoryPoolCache(uiOverBufferCount);
				SetGatheringBufferSize(gatheringBufferSize);

				return GetNetIOSystem().Initialize(numNetThread);
			}

			return S_OK;
		}

		// Close network system
		void CloseSystem()
		{
			MutexScopeLock scopeLock(g_InitLock);

			LONG lCount = g_lWSOpenCount;
			if (g_lWSOpenCount > 0)
			{
				lCount = g_lWSOpenCount.fetch_sub(1, std::memory_order_relaxed) - 1;
			}

			if (lCount <= 0)
			{
				GetNetIOSystem().Terminate();
			}
		}



		WriteBufferQueue* GetWriteBufferQueue()
		{
			return GetNetIOSystem().GetWriteBufferQueue();
		}


		///////////////////////////////////////////////////////////////////////////////
		// Socket handling 

		HRESULT RegisterSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			HRESULT hr = S_OK;

			netChkPtr(cbInstance);
			Assert(cbInstance->GetIOSocket() != INVALID_SOCKET);

			netChk(GetNetIOSystem().MakeSocketNonBlocking(cbInstance->GetIOSocket()));
			netChk(GetNetIOSystem().RegisterToNETIO(sockType, cbInstance));

		Proc_End:

			return hr;
		}

		HRESULT UnregisterSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			HRESULT hr = S_OK;

			netChkPtr(cbInstance);
			Assert(cbInstance->GetIOSocket() != INVALID_SOCKET);

			netChk(GetNetIOSystem().RegisterToNETIO(sockType, cbInstance));

		Proc_End:

			return hr;
		}

		HRESULT RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			HRESULT hr = S_OK;

			netChkPtr(cbInstance);
			Assert(cbInstance->GetIOSocket() != INVALID_SOCKET);

			netChk(GetNetIOSystem().RegisterSharedSocket(sockType, cbInstance));

		Proc_End:

			return hr;
		}


		SOCKET Socket(SockFamily domain, SockType type)
		{
			return socket(ToSockValue(domain), ToSockValue(type), 0);
		}

		void CloseSocket(SOCKET sock)
		{
			shutdown(sock, SHUT_RDWR);
			close(sock);
		}

		HRESULT Accept(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept)
		{
			HRESULT hr = S_OK;
			socklen_t len = sizeof(pAccept->sockAddr);

			netChkPtr(pAccept);

			pAccept->sockAccept = accept4(sockListen, (sockaddr*)&pAccept->sockAddr, &len, SOCK_NONBLOCK);

			if (len == 0)
			{
				netErr(E_UNEXPECTED);
			}

			if(pAccept->sockAccept < 0)
			{
				HRESULT err = GetLastWSAHRESULT();
				switch (err)
				{
				case E_NET_WOULDBLOCK:
				case E_NET_IO_PENDING:
					goto Proc_End;

				case E_NET_TRY_AGAIN:
					// Nothing to accept for now
					hr = err;
					goto Proc_End;

				case E_NET_NETDOWN:
				case E_NET_PROTO:
				case E_NET_NOPROTOOPT:
				case E_NET_HOSTDOWN:
				case E_NET_NONET:
				case E_NET_HOSTUNREACH:
				case E_NET_OPNOTSUPP:
				case E_NET_NETUNREACH:
				case E_NET_BADF:
					// silently handle them
					hr = err;
					goto Proc_End;
				}

				netTrace(Trace::TRC_ERROR, "TCP accept failed err={0:X8}", err);
			}


		Proc_End:

			return hr;
		}

		HRESULT HandleAcceptedSocket(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept, sockaddr_storage& remoteAddr)
		{
			socklen_t len;

			unused(sockListen);

			len = sizeof remoteAddr;
			getpeername(pAccept->sockAccept, (struct sockaddr*)&remoteAddr, &len);

			return S_OK;
		}


		HRESULT Recv(SOCKET sock, IOBUFFER_READ* pBuffer)
		{
			ssize_t recvSize = recv(sock, pBuffer->buffer, sizeof(pBuffer->buffer), MSG_DONTWAIT);
			if (recvSize < 0)
			{
				return GetLastWSAHRESULT();
			}

			pBuffer->TransferredSize = recvSize;
			if (recvSize == 0)
			{
				return S_FALSE;
			}

			return S_OK;
		}

		HRESULT RecvFrom(SOCKET sock, IOBUFFER_READ* pBuffer)
		{
			Assert(pBuffer->iSockLen == sizeof(pBuffer->NetAddr.From));

			ssize_t recvSize = recvfrom(sock, pBuffer->buffer, sizeof(pBuffer->buffer), MSG_DONTWAIT,
				(sockaddr*)&pBuffer->NetAddr.From, &pBuffer->iSockLen);
			if (recvSize < 0)
			{
				return GetLastWSAHRESULT();
			}

			pBuffer->TransferredSize = recvSize;
			if (recvSize == 0)
			{
				return S_FALSE;
			}

			return S_OK;
		}


		HRESULT Send(SOCKET sock, IOBUFFER_WRITE* pBuffer)
		{
			ssize_t sendSize = send(sock, pBuffer->pRawSendBuffer, pBuffer->RawSendSize, MSG_DONTWAIT | MSG_NOSIGNAL);
			if (sendSize < 0)
			{
				return GetLastWSAHRESULT();
			}

			pBuffer->TransferredSize = sendSize;

			return S_OK;
		}

		HRESULT SendTo(SOCKET sock, IOBUFFER_WRITE* pBuffer)
		{
			const sockaddr_storage& dstAddress = pBuffer->NetAddr.To;
			ssize_t sendSize = sendto(sock, pBuffer->pRawSendBuffer, pBuffer->RawSendSize, MSG_DONTWAIT | MSG_NOSIGNAL,
				(sockaddr*)&dstAddress, GetSockAddrSize(dstAddress));

			if (sendSize < 0)
			{
				return GetLastWSAHRESULT();
			}

			pBuffer->TransferredSize = sendSize;

			return S_OK;
		}


	}; // namespace NetSystem






} // namespace Net
} // namespace BR

#else
void Dummp_NetSystem_LINUX() {}
#endif

