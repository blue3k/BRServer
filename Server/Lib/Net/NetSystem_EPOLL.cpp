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
#include "Net/NetSystem.h"
#include "Net/NetSystem_impl.h"
#include "Common/HRESNet.h"
#include "Common/HRESCommon.h"
#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/SystemSynchronization.h"
#include "Common/MemoryPool.h"


#if LINUX

#include <sys/epoll.h>


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
		case EWOULDBLOCK: return E_NET_WOULDBLOCK;
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

		iSockLen = sizeof(sockaddr_in6);
	}

	IOBUFFER_READ::~IOBUFFER_READ()
	{
		ClearBuffer();
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
	//	EPOLL thread worker
	//

	EPOLLWorker::EPOLLWorker(int hEpoll)
		: m_hEpoll(hEpoll)
	{

	}

	EPOLLWorker::~EPOLLWorker()
	{

	}

	HRESULT EPOLLWorker::HandleAccept(SOCKET sock, INetIOCallBack* pCallBack)
	{
		HRESULT hr = S_OK;
		IOBUFFER_ACCEPT* pAcceptInfo = nullptr;

		// Accept will happened in network thread
		hr = pCallBack->Accept(pAcceptInfo);

		netChk(pCallBack->OnIOAccept(hr, pAcceptInfo));

	Proc_End:

		return hr;
	}

	HRESULT EPOLLWorker::HandleRW(SOCKET sock, unsigned int events, INetIOCallBack* pCallBack)
	{
		HRESULT hr = S_OK;

		if (!(events & (EPOLLIN | EPOLLOUT)))
		{
			netTrace(Trace::TRC_ERROR, "Error event {0}, {1}", events, sock);
			//pCallBack->
			return E_UNEXPECTED;
		}

		if (events & EPOLLIN)
		{
			IOBUFFER_READ* pBuffer = nullptr;
			// Read
			hr = pCallBack->Recv(pBuffer);
			// toss data to working thread
			netChk(pCallBack->OnIORecvCompleted(hr, pBuffer));
		}

		if (events & EPOLLOUT)
		{
			// This call will just poke working thread
			netChk(pCallBack->OnIOSendCompleted(hr, nullptr));
		}

	Proc_End:

		return hr;
	}

	void EPOLLWorker::Run()
	{
		HRESULT hr = S_OK;
		int iNumEvents;
		epoll_event events[MAX_EPOLL_EVENTS];

		while (1)
		{
			hr = S_OK;

			// Getting status
			iNumEvents = epoll_wait(m_hEpoll, events, countof(events), MAX_EPOLL_WAIT);
			if (iNumEvents < 0)
			{
				hr = GetLastHRESULT();
				switch (hr)
				{
				case E_INVALID_FILE_HANDLE:
				case E_NET_BADF:
				case E_INVALID_POINTER:
				case E_NET_FAULT:
				case E_INTERRUPTED_SYSCALL:
				case E_NET_INTR:
				case E_INVALID_ARG:
				case E_NET_INVAL:
				default:
					netTrace(Trace::TRC_ERROR, "EPOLL wait failed hr={0:X8}", hr);
					break;
				}
			}

			for (int iEvent = 0; iEvent < iNumEvents; iEvent++)
			{
				if (events[iEvent].data.ptr == nullptr)
				{
					netTrace(Trace::TRC_ERROR, "EPOLL null handler, skipping...");
					continue;
				}

				auto& curEvent = events[iEvent];
				bool isListenSocket = curEvent.data.u32 != 0;
				auto pCallback = (INetIOCallBack*)curEvent.data.ptr;
				SOCKET sock = curEvent.data.fd;

				if (isListenSocket)
				{
					HandleAccept(sock, pCallback);
				}
				else
				{
					HandleRW(sock, curEvent.events, pCallback);
				}

			}

			// Check exit event
			if (CheckKillEvent(DurationMS(0)))
				break;

		} // while(1)

	}

	////////////////////////////////////////////////////////////////////////////////
	//
	//	EPOLL network system
	//



	class EPOLLSystem
	{
	private:

		static EPOLLSystem stm_Instance;

		int m_hEpollListen;
		int m_hEpoll;

		EPOLLWorker* m_ListenWorker;
		// workers for RW
		DynamicArray<EPOLLWorker*> m_Workers;

	public:

		EPOLLSystem()
			: m_hEpollListen(0)
			, m_hEpoll(0)
			, m_ListenWorker(nullptr)
		{
		}

		HRESULT Initialize(UINT netThreadCount)
		{
			if (m_hEpoll != 0)
				return S_OK;

			m_hEpoll = epoll_create1(0);
			m_hEpollListen = epoll_create1(0);

			m_ListenWorker = new EPOLLWorker(m_hEpollListen);
			m_ListenWorker->Start();

			for (UINT iThread = 0; iThread < netThreadCount; iThread++)
			{
				auto pNewWorker = new EPOLLWorker(m_hEpoll);
				m_Workers.push_back(pNewWorker);

				pNewWorker->Start();
			}

			return S_OK;
		}

		void Terminate()
		{
			if (m_ListenWorker)
			{
				m_ListenWorker->Stop(true);
				delete m_ListenWorker;
			}
			m_ListenWorker = nullptr;


			for (UINT iThread = 0; iThread < m_Workers.GetSize(); iThread++)
			{
				m_Workers[iThread]->Stop(true);
				delete m_Workers[iThread];
			}
			m_Workers.Clear();
		}

		//int create_and_bind(char *port)
		//{
		//	struct addrinfo hints;
		//	struct addrinfo *result, *rp;
		//	int s, sfd;

		//	memset(&hints, 0, sizeof(struct addrinfo));
		//	hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
		//	hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
		//	hints.ai_flags = AI_PASSIVE;     /* All interfaces */

		//	s = getaddrinfo(NULL, port, &hints, &result);
		//	if (s != 0)
		//	{
		//		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		//		return -1;
		//	}

		//	for (rp = result; rp != NULL; rp = rp->ai_next)
		//	{
		//		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		//		if (sfd == -1)
		//			continue;

		//		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		//		if (s == 0)
		//		{
		//			/* We managed to bind successfully! */
		//			break;
		//		}

		//		close(sfd);
		//	}

		//	if (rp == NULL)
		//	{
		//		fprintf(stderr, "Could not bind\n");
		//		return -1;
		//	}

		//	freeaddrinfo(result);

		//	return sfd;
		//}


		HRESULT MakeSocketNonBlocking(SOCKET sfd)
		{
			int flags, s;

			flags = fcntl(sfd, F_GETFL, 0);
			if (flags == -1)
			{
				netTrace(Trace::TRC_ERROR, "epoll_ctl: fcntl F_GETFL");
				return E_FAIL;
			}

			flags |= O_NONBLOCK;
			s = fcntl(sfd, F_SETFL, flags);
			if (s == -1)
			{
				netTrace(Trace::TRC_ERROR, "epoll_ctl: fcntl F_SETFL");
				return E_FAIL;
			}

			return S_OK;
		}


		// Register the socket to EPOLL
		HRESULT RegisterToEPOLL(SOCKET sfd, INetIOCallBack* cbInstance, bool isListenSocket)
		{
			epoll_event epollEvent;
			auto hEpoll = isListenSocket ? m_hEpollListen : m_hEpoll;

			memset(&epollEvent, 0, sizeof(epollEvent));
			epollEvent.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLET;// EPOLLERR | EPOLLHUP;
			epollEvent.data.u32 = isListenSocket ? 1 : 0;
			epollEvent.data.fd = sfd;
			epollEvent.data.ptr = cbInstance;
			if (epoll_ctl(hEpoll, EPOLL_CTL_ADD, sfd, &epollEvent) == -1) {
				netTrace(Trace::TRC_ERROR, "epoll_ctl: listen_sock");
				return E_FAIL;
			}

			return S_OK;
		}


		// Get system instance
		static EPOLLSystem& GetSystem()
		{
			return stm_Instance;
		}


	};

	EPOLLSystem EPOLLSystem::stm_Instance;


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

			if (g_lWSOpenCount == 0)
			{
				Net::IOBUFFER_WRITE::MemoryPoolCache(uiOverBufferCount);
				SetGatheringBufferSize(gatheringBufferSize);

				return EPOLLSystem::GetSystem().Initialize(numNetThread);
			}

			g_lWSOpenCount.fetch_add(1, std::memory_order_relaxed);

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
				EPOLLSystem::GetSystem().Terminate();
			}
		}



		///////////////////////////////////////////////////////////////////////////////
		// Socket handling 


		HRESULT RegisterSocket(SOCKET sock, INetIOCallBack* cbInstance, bool isListenSocket)
		{
			HRESULT hr = S_OK;

			netChk(EPOLLSystem::GetSystem().MakeSocketNonBlocking(sock));
			netChk(EPOLLSystem::GetSystem().RegisterToEPOLL(sock, cbInstance, isListenSocket));

		Proc_End:

			return hr;
		}


		SOCKET Socket(SockFamily domain, SockType type)
		{
			return socket((int)domain, (int)type, 0);
			if ((int)type == SOCK_STREAM)
			{
				return socket((int)domain, (int)type, 0);
			}
			else
			{
				return socket((int)domain, (int)type, 0);
			}
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

			if (len != sizeof(pAccept->sockAddr))
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
					// silently handle them
					hr = err;
					goto Proc_End;
				}

				netTrace(Trace::TRC_ERROR, "TCP accept failed err={0:X8}", err);
				netErr(err);
			}


		Proc_End:

			return hr;
		}

		HRESULT HandleAcceptedSocket(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept, sockaddr_in6& remoteAddr)
		{
			socklen_t len;
			struct sockaddr_storage addr;
			//char ipstr[INET6_ADDRSTRLEN];
			//int port;

			unused(sockListen);

			len = sizeof addr;
			getpeername(pAccept->sockAccept, (struct sockaddr*)&addr, &len);

			// deal with both IPv4 and IPv6:
			if (addr.ss_family == AF_INET)
			{
				// this shouldn't happened
				AssertRel(false);
				memset(&remoteAddr, 0, sizeof remoteAddr);
				//struct sockaddr_in *s = (struct sockaddr_in *)&addr;
				//port = ntohs(s->sin_port);
				//inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
			}
			else
			{ // AF_INET6
				struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
				remoteAddr = *s;
				//port = ntohs(s->sin6_port);
				//inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
			}

			return S_OK;
		}


		HRESULT Recv(SOCKET sock, IOBUFFER_READ* pBuffer)
		{
			ssize_t recvSize = recv(sock, &pBuffer->buffer, sizeof(pBuffer->buffer), MSG_DONTWAIT);
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
			Assert(pBuffer->iSockLen == sizeof(pBuffer->From));

			ssize_t recvSize = recvfrom(sock, &pBuffer->buffer, sizeof(pBuffer->buffer), MSG_DONTWAIT,
				(sockaddr*)&pBuffer->From, &pBuffer->iSockLen);
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
			ssize_t sendSize = send(sock, &pBuffer->RawSendSize, pBuffer->RawSendSize, MSG_DONTWAIT | MSG_NOSIGNAL);
			if (sendSize < 0)
			{
				return GetLastWSAHRESULT();
			}

			pBuffer->TransferredSize = sendSize;

			return S_OK;
		}

		HRESULT SendTo(SOCKET sock, const sockaddr_in6& dstAddress, IOBUFFER_WRITE* pBuffer)
		{
			ssize_t sendSize = sendto(sock, &pBuffer->RawSendSize, pBuffer->RawSendSize, MSG_DONTWAIT | MSG_NOSIGNAL,
				(sockaddr*)&dstAddress, sizeof(sockaddr_in6));

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
void Dummp_NetSystem_EPOLL() {}
#endif

