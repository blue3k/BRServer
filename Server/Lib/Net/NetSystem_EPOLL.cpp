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
#include "Common/TypeUtility.h"
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

	EPOLLWorker::EPOLLWorker(bool bHandleSend, int hEpoll)
		: m_hEpoll(hEpoll)
		, m_HandleSend(bHandleSend)
	{
		if (m_hEpoll == 0)
		{
			m_hEpoll = epoll_create(1);
		}
	}

	EPOLLWorker::~EPOLLWorker()
	{

	}

	HRESULT EPOLLWorker::RegisterSocket(INetIOCallBack* cbInstance)
	{
		epoll_event epollEvent;

		memset(&epollEvent, 0, sizeof(epollEvent));
		epollEvent.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLET;// EPOLLERR | EPOLLHUP;
		epollEvent.data.ptr = cbInstance;
		if (epoll_ctl(m_hEpoll, EPOLL_CTL_ADD, cbInstance->GetIOSocket(), &epollEvent) == -1) {
			netTrace(Trace::TRC_ERROR, "epoll_ctl: listen_sock");
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT EPOLLWorker::HandleAccept(SOCKET sock, INetIOCallBack* pCallBack)
	{
		HRESULT hr = S_OK;
		IOBUFFER_ACCEPT* pAcceptInfo = nullptr;

		// Accept will happened in network thread
		hr = pCallBack->Accept(pAcceptInfo);
		switch (hr)
		{
		case S_OK:
			break;
		case E_NOTIMPL:
			Assert(false); // Fix it!
			break;
		case E_NET_TRY_AGAIN:
		default:
			goto Proc_End;
			break;
		}

		netChk(pCallBack->OnIOAccept(hr, pAcceptInfo));
		pAcceptInfo = nullptr;

	Proc_End:

		Util::SafeDelete(pAcceptInfo);


		return hr;
	}

	HRESULT EPOLLWorker::HandleRW(SOCKET sock, unsigned int events, INetIOCallBack* pCallBack)
	{
		HRESULT hr = S_OK, hrErr = S_OK;
		IOBUFFER_READ* pReadBuffer = nullptr;

		if (!(events & (EPOLLIN | EPOLLOUT)))
		{
			netTrace(Trace::TRC_ERROR, "Error sock:{0}, event:{1}", sock, events);
			return E_UNEXPECTED;
		}

		if (events & EPOLLIN)
		{
			while (SUCCEEDED(hrErr))
			{
				// Read
				pReadBuffer = new IOBUFFER_READ;
				hrErr = pCallBack->Recv(pReadBuffer);
				hr = hrErr;
				switch (hrErr)
				{
				case E_NET_TRY_AGAIN:
				case E_NET_WOULDBLOCK:
					// These are expected return code
					hr = S_OK;
					break;
				case E_NET_IO_PENDING:
					Assert(false);
					break;
				default:
					netTrace(Trace::TRC_ERROR, "Epoll Recv fail events:{0:X8} hr:{1:X8}", events, hrErr);
					// fallthru
				case S_OK:
					// toss data to working thread
					netChk(pCallBack->OnIORecvCompleted(hrErr, pReadBuffer));
					pReadBuffer = nullptr;
					hr = hrErr;
					break;
				}

				// OnIORecvCompleted wouldn't delete anything
				Util::SafeDelete(pReadBuffer);
			}
		}

		if (m_HandleSend && (events & EPOLLOUT))
		{
			// This call will just poke working thread
			hr = pCallBack->OnSendReady();
			if (FAILED(hr))
			{
				netErr(hr);
			}
		}

	Proc_End:

		if (FAILED(hr))
		{
			netTrace(Trace::TRC_ERROR, "Epoll RW fail events:{0:X8} hr:{1:X8}", events, hr);
		}

		Util::SafeDelete(pReadBuffer);

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
				case E_INTERRUPTED_SYSCALL:
					break;
				case E_INVALID_FILE_HANDLE:
				case E_NET_BADF:
				case E_INVALID_POINTER:
				case E_NET_FAULT:
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
				auto pCallback = (INetIOCallBack*)curEvent.data.ptr;
				bool isListenSocket = pCallback->GetIOFlags().IsListenSocket != 0;
				SOCKET sock = pCallback->GetIOSocket();

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




	// Constructor/destructor
	EPOLLSendWorker::EPOLLSendWorker()
	{
	}

	EPOLLSendWorker::~EPOLLSendWorker()
	{
		m_WriteQueue.ClearQueue();
	}

	void EPOLLSendWorker::Run()
	{
		HRESULT hr = S_OK;
		IOBUFFER_WRITE* pSendBuffer = nullptr;

		while (1)
		{
			hr = S_OK;

			do {
				

				// Check exit event
				if (CheckKillEvent(DurationMS(0)))
					break;

				if (pSendBuffer == nullptr) m_WriteQueue.Dequeue(pSendBuffer);

				if (pSendBuffer == nullptr) break;

				switch (pSendBuffer->Operation)
				{
				case IOBUFFER_OPERATION::OP_TCPWRITE:
					Assert(false); // TCP packets will be sent by RW workers
					break;
				case IOBUFFER_OPERATION::OP_UDPWRITE:
					hr = NetSystem::SendTo(pSendBuffer->SockWrite, pSendBuffer);
					switch (hr)
					{
					case E_NET_TRY_AGAIN:
						continue; // try again
						break;
					case S_OK:
						break;
					default:
						netTrace(TRC_RAW, "UDP send failed {0:X8}", hr);
						// send fail
						break;
					}
					break;
				default:
					Assert(false);// This thread isn't designed to work on other stuffs
					break;
				}


				Util::SafeDelete(pSendBuffer);
				pSendBuffer = nullptr;
			} while (1);
			

		} // while(1)


		Util::SafeDelete(pSendBuffer);
	}




	////////////////////////////////////////////////////////////////////////////////
	//
	//	EPOLL network system
	//



	class EPOLLSystem
	{
	private:

		static EPOLLSystem stm_Instance;

		EPOLLWorker* m_ListenWorker;
		// workers for TCP
		std::atomic<int> m_iTCPAssignIndex;
		DynamicArray<EPOLLWorker*> m_WorkerTCP;

		// workers for UDP
		EPOLLSendWorker* m_UDPSendWorker;
		DynamicArray<EPOLLWorker*> m_WorkerUDP;

	public:

		EPOLLSystem()
			: m_ListenWorker(nullptr)
			, m_iTCPAssignIndex(0)
		{
		}

		HRESULT Initialize(UINT netThreadCount)
		{
			if (m_ListenWorker != nullptr)
				return S_OK;


			m_ListenWorker = new EPOLLWorker(false);
			m_ListenWorker->Start();

			m_iTCPAssignIndex = 0;

			// 
			for (UINT iThread = 0; iThread < netThreadCount; iThread++)
			{
				auto pNewWorker = new EPOLLWorker(true);
				m_WorkerTCP.push_back(pNewWorker);

				pNewWorker->Start();
			}

			m_UDPSendWorker = new EPOLLSendWorker;
			m_UDPSendWorker->Start();

			// 
			int hEPollUDP = epoll_create(1);
			for (UINT iThread = 0; iThread < netThreadCount; iThread++)
			{
				auto pNewWorker = new EPOLLWorker(false, hEPollUDP);
				m_WorkerUDP.push_back(pNewWorker);

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

			// 
			for (UINT iThread = 0; iThread < m_WorkerTCP.GetSize(); iThread++)
			{
				m_WorkerTCP[iThread]->Stop(true);
				delete m_WorkerTCP[iThread];
			}
			m_WorkerTCP.Clear();


			if (m_UDPSendWorker)
			{
				m_UDPSendWorker->Stop(true);
				delete m_UDPSendWorker;
			}
			m_UDPSendWorker = nullptr;

			// 
			int hEpoll = 0;
			for (UINT iThread = 0; iThread < m_WorkerUDP.GetSize(); iThread++)
			{
				auto pThread = m_WorkerUDP[iThread];
				hEpoll = pThread->GetEpollHandle();
				pThread->Stop(true);
				delete pThread;
			}
			m_WorkerUDP.Clear();

			if (hEpoll != 0)
			{
				close(hEpoll);
			}
		}

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
		HRESULT RegisterToEPOLL(SockType sockType, INetIOCallBack* cbInstance)
		{
			if (sockType == SockType::Stream) // TCP
			{
				if (m_ListenWorker == nullptr)
					return E_NET_NOTINITIALISED;

				if (cbInstance->GetIOFlags().IsListenSocket != 0)
				{
					return m_ListenWorker->RegisterSocket(cbInstance);
				}
				else
				{
					auto assignIndex = m_iTCPAssignIndex.fetch_add(1,std::memory_order_relaxed) % m_WorkerTCP.GetSize();
					m_WorkerTCP[assignIndex]->RegisterSocket(cbInstance);
				}
			}
			else
			{
				if(m_WorkerUDP.GetSize() < 1)
					return E_NET_NOTINITIALISED;

				if (cbInstance->GetWriteQueue() == nullptr)
				{
					Assert(sockType == SockType::DataGram);
					cbInstance->SetWriteQueue( &m_UDPSendWorker->GetWriteQueue() );
				}

				// UDP workers are sharing epoll, add any of them will work same.
				return m_WorkerUDP[0]->RegisterSocket(cbInstance);
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

		HRESULT RegisterSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			HRESULT hr = S_OK;

			netChkPtr(cbInstance);
			Assert(cbInstance->GetIOSocket() != INVALID_SOCKET);

			netChk(EPOLLSystem::GetSystem().MakeSocketNonBlocking(cbInstance->GetIOSocket()));
			netChk(EPOLLSystem::GetSystem().RegisterToEPOLL(sockType, cbInstance));

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

			unused(sockListen);

			len = sizeof addr;
			getpeername(pAccept->sockAccept, (struct sockaddr*)&addr, &len);

			// deal with both IPv4 and IPv6:
			if (addr.ss_family == AF_INET)
			{
				// this shouldn't happened
				AssertRel(false);
				memset(&remoteAddr, 0, sizeof remoteAddr);
			}
			else
			{ // AF_INET6
				struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
				remoteAddr = *s;
			}

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
			const sockaddr_in6& dstAddress = pBuffer->NetAddr.To;
			ssize_t sendSize = sendto(sock, pBuffer->pRawSendBuffer, pBuffer->RawSendSize, MSG_DONTWAIT | MSG_NOSIGNAL,
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

