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
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
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

	Result GetLastWSAResult()
	{
		int ierr = errno;
		switch (ierr)
		{
		case 0: return ResultCode::SUCCESS;
		case EINTR: return ResultCode::E_NET_INTR;
		case EBADF: return ResultCode::E_NET_BADF;
		case EACCES: return ResultCode::E_NET_ACCES;
		case EFAULT: return ResultCode::E_NET_FAULT;
		case EINVAL: return ResultCode::E_NET_INVAL;
		case EMFILE: return ResultCode::E_NET_MFILE;
		case EWOULDBLOCK: return ResultCode::E_NET_TRY_AGAIN; // Linux epoll
#if EWOULDBLOCK != EAGAIN
		case EAGAIN: return ResultCode::E_NET_TRY_AGAIN;
#endif
		case EINPROGRESS: return ResultCode::E_NET_INPROGRESS;
		case EALREADY: return ResultCode::E_NET_ALREADY;
		case ENOTSOCK: return ResultCode::E_NET_FAULT;
		case EDESTADDRREQ: return ResultCode::E_NET_DESTADDRREQ;
		case ENETDOWN: return ResultCode::E_NET_NETDOWN;
		case ENETUNREACH: return ResultCode::E_NET_NETUNREACH;
		case ENETRESET: return ResultCode::E_NET_NETRESET;
		case ECONNABORTED: return ResultCode::E_NET_CONNABORTED;
		case ECONNRESET: return ResultCode::E_NET_CONNRESET;
		case ENOBUFS: return ResultCode::E_NET_NOBUFS;
		case EISCONN: return ResultCode::E_NET_ISCONN;
		case ENOTCONN: return ResultCode::E_NET_NOTCONN;
		case ESHUTDOWN: return ResultCode::E_NET_SHUTDOWN;
		case EPIPE: return ResultCode::E_NET_SHUTDOWN;
		case ETOOMANYREFS: return ResultCode::E_NET_TOOMANYREFS;
		case ETIMEDOUT: return ResultCode::E_NET_TIMEDOUT;
		case ECONNREFUSED: return ResultCode::E_NET_CONNECTION_REFUSSED;
		case ELOOP: return ResultCode::E_NET_LOOP;
		case ENAMETOOLONG: return ResultCode::E_NET_NAMETOOLONG;

		case EHOSTDOWN: return ResultCode::E_NET_HOSTDOWN;
		case EHOSTUNREACH: return ResultCode::E_NET_HOSTUNREACH;
		case ENOTEMPTY: return ResultCode::E_NET_NOTEMPTY;
		case EPROTO: return ResultCode::E_NET_PROTO;
		case EPROTOTYPE: return ResultCode::E_NET_PROTOTYPE;
		case ENOPROTOOPT: return ResultCode::E_NET_NOPROTOOPT;
		case ENONET: return ResultCode::E_NET_NONET;
		case EOPNOTSUPP: return ResultCode::E_NET_OPNOTSUPP;
		case EADDRINUSE: return ResultCode::E_NET_ADDRINUSE;
			//case EPROCLIM: return ResultCode::E_NET_PROCLIM;
			//case SYSNOTREADY: return ResultCode::E_NET_SYSNOTREADY;
			//case VERNOTSUPPORTED: return ResultCode::E_NET_VERNOTSUPPORTED;
			//case NOTINITIALISED: return ResultCode::E_NET_NOTINITIALISED;
			//case EDISCON: return ResultCode::E_NET_DISCON;
			//case SYSCALLFAILURE: return ResultCode::E_NET_SYSCALLFAILURE;
			//case HOST_NOT_FOUND: return ResultCode::E_NET_HOST_NOT_FOUND;

			//case TRY_AGAIN: return ResultCode::E_NET_TRY_AGAIN;
			//case _SECURE_HOST_NOT_FOUND: return ResultCode::E_NET_SECURE_HOST_NOT_FOUND;
			//case WSA_IO_PENDING: return ResultCode::E_NET_IO_PENDING;
		default:
			defTrace(Trace::TRC_WARN, "Unknown sock error {0}", ierr);
			return GetLastResult();
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

	Result IOBUFFER_READ::SetPendingTrue()
	{
		bool expected = false;
		while (!bIsPending.compare_exchange_weak(expected, true, std::memory_order_seq_cst))
		{
			if (expected == true)
			{
				return ResultCode::FAIL;
			}

			expected = false;
		}
		return ResultCode::SUCCESS;
	}

	Result IOBUFFER_READ::SetPendingFalse()
	{
		bool expected = true;
		while (!bIsPending.compare_exchange_weak(expected, false, std::memory_order_seq_cst))
		{
			if (expected == true)
			{
				return ResultCode::FAIL;
			}

			expected = false;
		}
		return ResultCode::SUCCESS;
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
		Result OpenSystem( UINT uiOverBufferCount, UINT numNetThread, UINT gatheringBufferSize)
		{
			MutexScopeLock scopeLock(g_InitLock);

			auto orgCounter = g_lWSOpenCount.fetch_add(1, std::memory_order_relaxed);

			if (orgCounter == 0)
			{
				Net::IOBUFFER_WRITE::MemoryPoolCache(uiOverBufferCount);
				SetGatheringBufferSize(gatheringBufferSize);

				return GetNetIOSystem().Initialize(numNetThread);
			}

			return ResultCode::SUCCESS;
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

		Result RegisterSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			Result hr = ResultCode::SUCCESS;

			netChkPtr(cbInstance);
			Assert(cbInstance->GetIOSocket() != INVALID_SOCKET);

			netChk(GetNetIOSystem().MakeSocketNonBlocking(cbInstance->GetIOSocket()));
			netChk(GetNetIOSystem().RegisterToNETIO(sockType, cbInstance));

		Proc_End:

			return hr;
		}

		Result UnregisterSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			Result hr = ResultCode::SUCCESS;

			netChkPtr(cbInstance);
			Assert(cbInstance->GetIOSocket() != INVALID_SOCKET);

			netChk(GetNetIOSystem().UnregisterFromNETIO(sockType, cbInstance));

		Proc_End:

			return hr;
		}

		Result RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			Result hr = ResultCode::SUCCESS;

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

		Result Accept(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept)
		{
			Result hr = ResultCode::SUCCESS;
			socklen_t len = sizeof(pAccept->sockAddr);

			netChkPtr(pAccept);

			pAccept->sockAccept = accept4(sockListen, (sockaddr*)&pAccept->sockAddr, &len, SOCK_NONBLOCK);

			if (len == 0)
			{
				netErr(ResultCode::UNEXPECTED);
			}

			if(pAccept->sockAccept < 0)
			{
				Result err = GetLastWSAResult();
				switch (err)
				{
				case ResultCode::E_NET_WOULDBLOCK:
				case ResultCode::E_NET_IO_PENDING:
					goto Proc_End;

				case ResultCode::E_NET_TRY_AGAIN:
					// Nothing to accept for now
					hr = err;
					goto Proc_End;

				case ResultCode::E_NET_NETDOWN:
				case ResultCode::E_NET_PROTO:
				case ResultCode::E_NET_NOPROTOOPT:
				case ResultCode::E_NET_HOSTDOWN:
				case ResultCode::E_NET_NONET:
				case ResultCode::E_NET_HOSTUNREACH:
				case ResultCode::E_NET_OPNOTSUPP:
				case ResultCode::E_NET_NETUNREACH:
				case ResultCode::E_NET_BADF:
					// silently handle them
					hr = err;
					goto Proc_End;
				}

				netTrace(Trace::TRC_ERROR, "TCP accept failed err={0:X8}", err);
			}


		Proc_End:

			return hr;
		}

		Result HandleAcceptedSocket(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept, sockaddr_storage& remoteAddr)
		{
			socklen_t len;

			unused(sockListen);

			len = sizeof remoteAddr;
			getpeername(pAccept->sockAccept, (struct sockaddr*)&remoteAddr, &len);

			return ResultCode::SUCCESS;
		}


		Result Recv(SOCKET sock, IOBUFFER_READ* pBuffer)
		{
			ssize_t recvSize = recv(sock, pBuffer->buffer, sizeof(pBuffer->buffer), MSG_DONTWAIT);
			if (recvSize < 0)
			{
				return GetLastWSAResult();
			}

			pBuffer->TransferredSize = recvSize;
			if (recvSize == 0)
			{
				return ResultCode::SUCCESS_FALSE;
			}

			return ResultCode::SUCCESS;
		}

		Result RecvFrom(SOCKET sock, IOBUFFER_READ* pBuffer)
		{
			Assert(pBuffer->iSockLen == sizeof(pBuffer->NetAddr.From));

			ssize_t recvSize = recvfrom(sock, pBuffer->buffer, sizeof(pBuffer->buffer), MSG_DONTWAIT,
				(sockaddr*)&pBuffer->NetAddr.From, &pBuffer->iSockLen);
			if (recvSize < 0)
			{
				return GetLastWSAResult();
			}

			pBuffer->TransferredSize = recvSize;
			if (recvSize == 0)
			{
				return ResultCode::SUCCESS_FALSE;
			}

			return ResultCode::SUCCESS;
		}


		Result Send(SOCKET sock, IOBUFFER_WRITE* pBuffer)
		{
			ssize_t sendSize = send(sock, pBuffer->pRawSendBuffer, pBuffer->RawSendSize, MSG_DONTWAIT | MSG_NOSIGNAL);
			if (sendSize < 0)
			{
				return GetLastWSAResult();
			}

			pBuffer->TransferredSize = sendSize;

			return ResultCode::SUCCESS;
		}

		Result SendTo(SOCKET sock, IOBUFFER_WRITE* pBuffer)
		{
			const sockaddr_storage& dstAddress = pBuffer->NetAddr.To;
			ssize_t sendSize = sendto(sock, pBuffer->pRawSendBuffer, pBuffer->RawSendSize, MSG_DONTWAIT | MSG_NOSIGNAL,
				(sockaddr*)&dstAddress, GetSockAddrSize(dstAddress));

			if (sendSize < 0)
			{
				return GetLastWSAResult();
			}

			pBuffer->TransferredSize = sendSize;

			return ResultCode::SUCCESS;
		}


	}; // namespace NetSystem






} // namespace Net
} // namespace BR

#else
void Dummp_NetSystem_LINUX() {}
#endif

