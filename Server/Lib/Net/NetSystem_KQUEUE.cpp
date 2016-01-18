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
#include "Net/NetSystem_KQUEUE.h"
#include "Net/NetSystem_impl.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/TypeUtility.h"
#include "Common/SystemSynchronization.h"
#include "Common/MemoryPool.h"


#if KQUEUE

#include <sys/event.h>



namespace BR {
namespace Net {





	////////////////////////////////////////////////////////////////////////////////
	//
	//	KQUEUE thread worker
	//

	KQUEUEWorker::KQUEUEWorker(bool bHandleSend, int hKQUEUE)
		: m_hKQUEUE(hKQUEUE)
		, m_HandleSend(bHandleSend)
	{
		if (m_hKQUEUE == 0)
		{
			m_hKQUEUE = kqueue();
		}
	}

	KQUEUEWorker::~KQUEUEWorker()
	{

	}

	HRESULT KQUEUEWorker::RegisterSocket(INetIOCallBack* cbInstance)
	{
		struct kevent evSet;

		EV_SET(&evSet, cbInstance->GetIOSocket(), EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, cbInstance);
		if (kevent(m_hKQUEUE, &evSet, 1, NULL, 0, NULL) == -1)
		{
			netTrace(Trace::TRC_ERROR, "KQUEUE_ctl: RegisterSocket");
			return GetLastWSAHRESULT();
		}

		return S_OK;
	}

	HRESULT KQUEUEWorker::UnregisterSocket(INetIOCallBack* cbInstance)
	{
		struct kevent evSet;

		EV_SET(&evSet, cbInstance->GetIOSocket(), EVFILT_READ | EVFILT_WRITE, EV_DELETE, 0, 0, cbInstance);
		if (kevent(m_hKQUEUE, &evSet, 1, NULL, 0, NULL) == -1)
		{
			netTrace(Trace::TRC_ERROR, "KQUEUE_ctl: RegisterSocket");
			return GetLastWSAHRESULT();
		}

		return S_OK;
	}

	HRESULT KQUEUEWorker::HandleAccept(SOCKET sock, INetIOCallBack* pCallBack)
	{
		HRESULT hr = S_OK;
		IOBUFFER_ACCEPT* pAcceptInfo = nullptr;

		while (1)
		{
			// Accept will happened in network thread
			hr = pCallBack->Accept(pAcceptInfo);
			switch (hr)
			{
			case S_OK:
				netChk(pCallBack->OnIOAccept(hr, pAcceptInfo));
				pAcceptInfo = nullptr;
				break;
			case E_SYSTEM_NOTIMPL:
				Assert(false); // Fix it!
				break;
			case E_NET_TRY_AGAIN:
			case E_NET_WOULDBLOCK:
			case E_NET_IO_PENDING:
			case E_NET_BADF:
				goto Proc_End;
			default:
				// some failure? try again
				break;
			}

			Util::SafeDelete(pAcceptInfo);
		}

	Proc_End:

		Util::SafeDelete(pAcceptInfo);


		return hr;
	}

	HRESULT KQUEUEWorker::HandleRW(SOCKET sock, unsigned int events, INetIOCallBack* pCallBack)
	{
		HRESULT hr = S_OK, hrErr = S_OK;
		IOBUFFER_READ* pReadBuffer = nullptr;

		if (!(events & (EVFILT_READ| EVFILT_WRITE)))
		{
			netTrace(Trace::TRC_ERROR, "Error sock:{0}, event:{1}", sock, events);
			return E_SYSTEM_UNEXPECTED;
		}

		if (events & EVFILT_READ)
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
				case S_SYSTEM_FALSE:
					// These are expected return code
					hr = S_OK;
					break;
				case E_NET_IO_PENDING:
					Assert(false);
					break;
				default:
					if (FAILED(hr))
					{
						netTrace(TRC_NETSYS, "ERROR KQUEUE Recv fail events:{0:X8} hr:{1:X8}", events, hrErr);
					}
					// fallthru
				case S_OK:
					// toss data to working thread
					if (pReadBuffer != nullptr)
					{
						netChk(pCallBack->OnIORecvCompleted(hrErr, pReadBuffer));
					}
					pReadBuffer = nullptr;
					hr = hrErr;
					break;
				}

				// OnIORecvCompleted wouldn't delete anything
				Util::SafeDelete(pReadBuffer);
			}
		}

		if (m_HandleSend && (events & EVFILT_WRITE))
		{
			// This call will just poke working thread
			hr = pCallBack->OnSendReady();
			if (FAILED(hr))
			{
				//netErr(hr);
				goto Proc_End;
			}
		}

	Proc_End:

		if (FAILED(hr))
		{
			netTrace(TRC_NETSYS, "ERROR KQUEUE RW fail events:{0:X8} hr:{1:X8}", events, hr);
		}

		Util::SafeDelete(pReadBuffer);

		return hr;
	}

	void KQUEUEWorker::Run()
	{
		HRESULT hr = S_OK;
		int iNumEvents;
		struct kevent events[MAX_KQUEUE_EVENTS];

		while (1)
		{
			hr = S_OK;

			// Getting status
			iNumEvents = kevent(m_hKQUEUE, NULL, 0, events, countof(events), NULL);
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
					netTrace(TRC_NETSYS, "ERROR KQUEUE wait failed hr={0:X8}", hr);
					break;
				}
			}

			for (int iEvent = 0; iEvent < iNumEvents; iEvent++)
			{
				if (events[iEvent].udata == nullptr)
				{
					netTrace(TRC_NETSYS, "ERROR KQUEUE null handler, skipping...");
					continue;
				}

				auto& curEvent = events[iEvent];
				auto pCallback = (INetIOCallBack*)curEvent.udata;
				bool isListenSocket = pCallback->GetIOFlags().IsListenSocket != 0;
				SOCKET sock = pCallback->GetIOSocket();

				if (isListenSocket)
				{
					HandleAccept(sock, pCallback);
				}
				else
				{
					HandleRW(sock, curEvent.flags, pCallback);
				}

			}

			// Check exit event
			if (CheckKillEvent(DurationMS(0)))
				break;

		} // while(1)

	}




	// Constructor/destructor
	KQUEUESendWorker::KQUEUESendWorker()
	{
	}

	KQUEUESendWorker::~KQUEUESendWorker()
	{
		m_WriteQueue.ClearQueue();
	}

	void KQUEUESendWorker::Run()
	{
		HRESULT hr = S_OK;
		IOBUFFER_WRITE* pSendBuffer = nullptr;
		DurationMS tickInterval(0);

		while (1)
		{
			hr = S_OK;

			// Check exit event
			if (CheckKillEvent(tickInterval))
				break;


			if (pSendBuffer == nullptr) m_WriteQueue.Dequeue(pSendBuffer);

			if (pSendBuffer == nullptr)
			{
				tickInterval = DurationMS(1);
				continue;
			}
			else
			{
				tickInterval = DurationMS(0);
			}

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
					netTrace(TRC_NETSYS, "ERROR UDP send failed {0:X8}", hr);
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
			

		} // while(1)


		Util::SafeDelete(pSendBuffer);
	}




	////////////////////////////////////////////////////////////////////////////////
	//
	//	KQUEUE network system
	//



	KQUEUESystem::KQUEUESystem()
		: m_ListenWorker(nullptr)
		, m_iTCPAssignIndex(0)
		, m_UDPSendWorker(nullptr)
	{
	}

	HRESULT KQUEUESystem::Initialize(UINT netThreadCount)
	{
		if (m_ListenWorker != nullptr)
			return S_OK;

		int hKQUEUEUDP = kqueue();

		m_ListenWorker = new KQUEUEWorker(false);
		m_ListenWorker->Start();

		m_UDPSendWorker = new KQUEUESendWorker;
		m_UDPSendWorker->Start();

		m_iTCPAssignIndex = 0;

		if (netThreadCount > 1)
		{
			// 
			for (UINT iThread = 0; iThread < netThreadCount; iThread++)
			{
				auto pNewWorker = new KQUEUEWorker(true);
				m_WorkerTCP.push_back(pNewWorker);

				pNewWorker->Start();
			}

			// 
			for (UINT iThread = 0; iThread < netThreadCount; iThread++)
			{
				auto pNewWorker = new KQUEUEWorker(false, hKQUEUEUDP);
				m_WorkerUDP.push_back(pNewWorker);

				pNewWorker->Start();
			}
		}

		return S_OK;
	}

	void KQUEUESystem::Terminate()
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
		int hKQUEUE = 0;
		for (UINT iThread = 0; iThread < m_WorkerUDP.GetSize(); iThread++)
		{
			auto pThread = m_WorkerUDP[iThread];
			hKQUEUE = pThread->GetKQUEUEHandle();
			pThread->Stop(true);
			delete pThread;
		}
		m_WorkerUDP.Clear();

		if (hKQUEUE != 0)
		{
			close(hKQUEUE);
		}
	}

	HRESULT KQUEUESystem::MakeSocketNonBlocking(SOCKET sfd)
	{
		int flags, s;

		flags = fcntl(sfd, F_GETFL, 0);
		if (flags == -1)
		{
			netTrace(Trace::TRC_ERROR, "KQUEUE_ctl: fcntl F_GETFL");
			return E_SYSTEM_FAIL;
		}

		flags |= O_NONBLOCK;
		s = fcntl(sfd, F_SETFL, flags);
		if (s == -1)
		{
			netTrace(Trace::TRC_ERROR, "KQUEUE_ctl: fcntl F_SETFL");
			return E_SYSTEM_FAIL;
		}

		return S_OK;
	}

	// UDP shares the send queue
	WriteBufferQueue* KQUEUESystem::GetWriteBufferQueue()
	{
		if (m_UDPSendWorker == nullptr)
			return nullptr;

		return &m_UDPSendWorker->GetWriteQueue();
	}


	HRESULT KQUEUESystem::RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance)
	{
		Assert(sockType == SockType::DataGram);
		if (sockType != SockType::DataGram)
			return E_SYSTEM_UNEXPECTED;

		if (m_WorkerUDP.GetSize() < 1)
			return E_NET_NOTINITIALISED;

		if (cbInstance->GetWriteQueue() == nullptr)
		{
			Assert(sockType == SockType::DataGram);
			cbInstance->SetWriteQueue(&m_UDPSendWorker->GetWriteQueue());
		}

		return S_OK;
	}

	// Register the socket to EPOLL
	HRESULT KQUEUESystem::RegisterToNETIO(SockType sockType, INetIOCallBack* cbInstance)
	{
		if (m_ListenWorker == nullptr)
			return E_NET_NOTINITIALISED;

		if (sockType == SockType::Stream) // TCP
		{
			// Listen worker will do all job when there is no other thread.
			if (cbInstance->GetIOFlags().IsListenSocket != 0 || m_WorkerTCP.GetSize() == 0)
			{
				return m_ListenWorker->RegisterSocket(cbInstance);
			}
			else
			{
				auto assignIndex = m_iTCPAssignIndex.fetch_add(1, std::memory_order_relaxed) % m_WorkerTCP.GetSize();
				cbInstance->SetAssignedIOWorker(assignIndex);
				m_WorkerTCP[assignIndex]->RegisterSocket(cbInstance);
			}
		}
		else
		{
			if (cbInstance->GetWriteQueue() == nullptr)
			{
				Assert(sockType == SockType::DataGram);
				cbInstance->SetWriteQueue(&m_UDPSendWorker->GetWriteQueue());
			}

			if (m_WorkerUDP.GetSize() < 1)
			{
				HRESULT hr = m_ListenWorker->RegisterSocket(cbInstance);
				if (FAILED(hr)) return hr;
			}
			else
			{
				// UDP workers are sharing epoll, add any of them will work same.
				HRESULT hr = m_WorkerUDP[0]->RegisterSocket(cbInstance);
				if (FAILED(hr)) return hr;
			}
			cbInstance->SetAssignedIOWorker(0);
		}

		return S_OK;
	}

	HRESULT KQUEUESystem::UnregisterFromNETIO(SockType sockType, INetIOCallBack* cbInstance)
	{
		if (m_ListenWorker == nullptr)
			return E_NET_NOTINITIALISED;

		if (sockType == SockType::Stream) // TCP
		{
			if (cbInstance->GetIOFlags().IsListenSocket != 0 || m_WorkerTCP.GetSize() == 0)
			{
				return m_ListenWorker->UnregisterSocket(cbInstance);
			}
			else
			{
				auto assignIndex = cbInstance->GetAssignedIOWorker();
				if (assignIndex >= 0 && assignIndex < (int)m_WorkerTCP.GetSize())
				{
					m_WorkerTCP[assignIndex]->UnregisterSocket(cbInstance);
					cbInstance->SetAssignedIOWorker(-1);
				}
			}
		}
		else
		{
			if (cbInstance->GetWriteQueue() == nullptr)
			{
				Assert(sockType == SockType::DataGram);
				cbInstance->SetWriteQueue(&m_UDPSendWorker->GetWriteQueue());
			}

			if (m_WorkerUDP.GetSize() < 1)
			{
				HRESULT hr = m_ListenWorker->UnregisterSocket(cbInstance);
				if (FAILED(hr)) return hr;
			}
			else
			{
				HRESULT hr = m_WorkerUDP[0]->UnregisterSocket(cbInstance);
				if (FAILED(hr)) return hr;
			}
			cbInstance->SetAssignedIOWorker(-1);
		}

		return S_OK;
	}


	KQUEUESystem KQUEUESystem::stm_Instance;






} // namespace Net
} // namespace BR

#else
void Dummp_NetSystem_KQUEUE() {}
#endif

