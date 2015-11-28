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
#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/SystemSynchronization.h"
#include "Common/MemoryPool.h"


#if LINUX

#include <sys/epoll.h>


BR_MEMORYPOOL_IMPLEMENT(Net::IOBUFFER_WRITE);


namespace BR {
namespace Net {



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

	EPOLLWorker::EPOLLWorker()
	{

	}

	EPOLLWorker::~EPOLLWorker()
	{

	}

	HRESULT HandleAccept()
	{

	}

	HRESULT HandleRW()
	{

	}

	void EPOLLWorker::Run()
	{
		HRESULT hr = S_OK;
		int iNumEvents;
		DWORD dwTransferred = 0;
		IOBUFFER *pOverlapped = NULL;
		epoll_event events[MAX_EPOLL_EVENTS];
		int iErr = 0;
		int iLastError;

		while (1)
		{
			dwTransferred = 0;
			iErr = 0;
			iLastError = 0;
			hr = S_OK;

			// Getting status
			iNumEvents = epoll_wait(m_hEpoll, events, countof(events), -1);
			for (int iEvent = 0; iEvent < iNumEvents; iEvent++)
			{
			}


			// Operation
			switch (pOverlapped->Operation)
			{
			case IOBUFFER_OPERATION::OP_TCPACCEPT:
			{
				IOBUFFER_ACCEPT *pAcceptOver = (IOBUFFER_ACCEPT*)pOverlapped;
				INetIOCallBack *pCallback = (INetIOCallBack*)ulKey;
				hr = pCallback->OnIOAccept(hr, pAcceptOver);
			}
			break;
			case IOBUFFER_OPERATION::OP_TCPWRITE:
			case IOBUFFER_OPERATION::OP_UDPWRITE:
			case IOBUFFER_OPERATION::OP_PEERUDPWRITE:
			{
				IOBUFFER_WRITE *pIOBuffer = (IOBUFFER_WRITE*)pOverlapped;
				INetIOCallBack *pCallback = (INetIOCallBack*)ulKey;
				hr = pCallback->OnIOSendCompleted(hr, pIOBuffer, dwTransferred);
			}
			break;
			case IOBUFFER_OPERATION::OP_TCPREAD:
			case IOBUFFER_OPERATION::OP_UDPREAD:
			case IOBUFFER_OPERATION::OP_PEERUDPREAD:
				if (ulKey) // TCP operation
				{
					IOBUFFER_READ *pIOBuffer = (IOBUFFER_READ*)pOverlapped;
					INetIOCallBack *pCallback = (INetIOCallBack*)ulKey;
					hr = pCallback->OnIORecvCompleted(hr, pIOBuffer, dwTransferred);
					pOverlapped = NULL;
				}
				else
				{
					AssertRel(!"Invalid Key at IOCP");
				}
				break;
			default:
				netTrace(Trace::TRC_ERROR, "IOCP Invalid Overlapped Operation");
				break;
			};


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

		int m_hEpoll;

	public:

		EPOLLSystem()
			: m_hEpoll(0)
		{
		}

		HRESULT Initialize(UINT netThreadCount)
		{
			if (m_hEpoll != 0)
				return S_OK;

			m_hEpoll = epoll_create1(0);
		}

		void Terminate()
		{
			//m_hEpoll = 0;
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
		HRESULT RegisterToEPOLL(SOCKET sfd)
		{
			epoll_event epollEvent;

			memset(&epollEvent, 0, sizeof(epollEvent));
			epollEvent.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLET;// EPOLLERR | EPOLLHUP;
			epollEvent.data.fd = sfd;
			if (epoll_ctl(m_hEpoll, EPOLL_CTL_ADD, sfd, &epollEvent) == -1) {
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
		static std::atomic<LONG> g_lWSOpenCount = 0;
		static CriticalSection g_InitLock;


		// Open network system
		HRESULT OpenSystem( UINT uiOverBufferCount, UINT numNetThread )
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

			int iErr = 0;
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



		HRESULT RegisterSocket(SOCKET sock, INetIOCallBack* cbInstance)
		{
			HRESULT hr = S_OK;

			netChk(EPOLLSystem::GetSystem().MakeSocketNonBlocking(sock));
			netChk(EPOLLSystem::GetSystem().RegisterToEPOLL(sock));

		Proc_End:

			return hr;
		}


	}; // namespace NetSystem






} // namespace Net
} // namespace BR

#else
void Dummp_NetSystem_EPOLL() {}
#endif

