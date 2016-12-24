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
#include "Net/NetSystem_EPOLL.h"
#include "Net/NetSystem_impl.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/ResultCode/BRResultCodeCommon.h"

#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/TypeUtility.h"
#include "Common/SystemSynchronization.h"
#include "Common/MemoryPool.h"


#if EPOLL

#include <sys/epoll.h>




namespace BR {
namespace Net {





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

	Result EPOLLWorker::RegisterSocket(INetIOCallBack* cbInstance)
	{
		epoll_event epollEvent;

		memset(&epollEvent, 0, sizeof(epollEvent));
		epollEvent.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLET | EPOLLHUP | EPOLLRDHUP;
		epollEvent.data.ptr = cbInstance;
		if (epoll_ctl(m_hEpoll, EPOLL_CTL_ADD, cbInstance->GetIOSocket(), &epollEvent) == -1) {
			netTrace(Trace::TRC_ERROR, "epoll_ctl: RegisterSocket sock:{0}", cbInstance->GetIOSocket());
			return ResultCode::FAIL;
		}

		netTrace(Trace::TRC_TRACE, "epoll register sock:{0}", cbInstance->GetIOSocket());

		return ResultCode::SUCCESS;
	}

	Result EPOLLWorker::UnregisterSocket(INetIOCallBack* cbInstance)
	{
		epoll_event epollEvent;

		memset(&epollEvent, 0, sizeof(epollEvent));
		epollEvent.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLET | EPOLLHUP | EPOLLRDHUP;
		epollEvent.data.ptr = cbInstance;
		if (epoll_ctl(m_hEpoll, EPOLL_CTL_DEL, cbInstance->GetIOSocket(), &epollEvent) == -1) {
			netTrace(Trace::TRC_ERROR, "epoll_ctl: UnregisterSocket sock:{0}", cbInstance->GetIOSocket());
			return ResultCode::FAIL;
		}

		netTrace(Trace::TRC_TRACE, "epoll unregister sock:{0}", cbInstance->GetIOSocket());

		return ResultCode::SUCCESS;
	}

	Result EPOLLWorker::HandleAccept(SOCKET sock, INetIOCallBack* pCallBack)
	{
		Result hr = ResultCode::SUCCESS;
		IOBUFFER_ACCEPT* pAcceptInfo = nullptr;

		while (1)
		{
			// Accept will happened in network thread
			hr = pCallBack->Accept(pAcceptInfo);
			switch ((int32_t)hr)
			{
			case ResultCode::SUCCESS:
				netChk(pCallBack->OnIOAccept(hr, pAcceptInfo));
				pAcceptInfo = nullptr;
				break;
			case ResultCode::NOT_IMPLEMENTED:
				Assert(false); // Fix it!
				break;
			case ResultCode::E_NET_TRY_AGAIN:
			case ResultCode::E_NET_WOULDBLOCK:
			case ResultCode::E_NET_IO_PENDING:
			case ResultCode::E_NET_BADF:
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

	Result EPOLLWorker::HandleRW(SOCKET sock, unsigned int events, INetIOCallBack* pCallBack)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;
		IOBUFFER_READ* pReadBuffer = nullptr;

		//  skip when the socket value is different
		if (pCallBack->GetIOSocket() != sock) return ResultCode::SUCCESS_FALSE;

		//if (!(events & (EPOLLIN | EPOLLOUT)))
		//{
		//	netTrace(Trace::TRC_ERROR, "Error sock:{0}, event:{1}", sock, events);
		//	return ResultCode::UNEXPECTED;
		//}

		if (events & EPOLLIN)
		{
			while ((hrErr))
			{
				// Read
				pReadBuffer = new IOBUFFER_READ;
				hrErr = pCallBack->Recv(pReadBuffer);
				hr = hrErr;
				switch ((uint32_t)hrErr)
				{
				case ResultCode::E_NET_TRY_AGAIN:
				case ResultCode::E_NET_WOULDBLOCK:
				case ResultCode::SUCCESS_FALSE:
					// These are expected return code
					hr = ResultCode::SUCCESS;
					break;
				case ResultCode::E_NET_IO_PENDING:
					Assert(false);
					break;
				default:
					if (!(hr))
					{
						char stringBuffer[512];
						netTrace(TRC_NETSYS, "ERROR Epoll Recv fail sock:{0} events:{1} hr:{2}", sock, GetNetIOSystem().EventFlagToString(sizeof(stringBuffer), stringBuffer, events), hrErr);
					}
					// fallthru
				case ResultCode::SUCCESS:
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

		if (m_HandleSend && (events & EPOLLOUT))
		{
			// This call will just poke working thread
			hr = pCallBack->OnSendReady();
			if (!(hr))
			{
				netErr(hr);
				//goto Proc_End;
			}
		}

	Proc_End:

		if (!(hr))
		{
			char stringBuffer[512];
			netTrace(TRC_NETSYS, "ERROR Epoll RW fail sock:{0}, events:{1} hr:{2}", sock, GetNetIOSystem().EventFlagToString(sizeof(stringBuffer), stringBuffer, events), hr);
		}

		Util::SafeDelete(pReadBuffer);

		return hr;
	}

	void EPOLLWorker::Run()
	{
		Result hr = ResultCode::SUCCESS;
		int iNumEvents;
		epoll_event events[MAX_EPOLL_EVENTS];

		while (1)
		{
			hr = ResultCode::SUCCESS;

			// Getting status
			iNumEvents = epoll_wait(m_hEpoll, events, countof(events), MAX_EPOLL_WAIT);
			if (iNumEvents < 0)
			{
				hr = GetLastResult();
				switch ((int32_t)hr)
				{
				case ResultCode::E_INTERRUPTED_SYSCALL:
					break;
				case ResultCode::E_INVALID_FILE_HANDLE:
				case ResultCode::E_NET_BADF:
				case ResultCode::E_INVALID_POINTER:
				case ResultCode::E_NET_FAULT:
				case ResultCode::E_NET_INTR:
				case ResultCode::E_INVALID_ARG:
				case ResultCode::E_NET_INVAL:
				default:
					netTrace(TRC_NETSYS, "ERROR EPOLL wait failed hr={0:X8}", hr);
					break;
				}
			}

			for (int iEvent = 0; iEvent < iNumEvents; iEvent++)
			{
				if (events[iEvent].data.ptr == nullptr)
				{
					netTrace(TRC_NETSYS, "ERROR EPOLL null handler, skipping...");
					continue;
				}

				auto& curEvent = events[iEvent];
				auto pCallback = (INetIOCallBack*)curEvent.data.ptr;
				bool isListenSocket = pCallback->GetIOFlags().IsListenSocket != 0;
				SOCKET sock = pCallback->GetIOSocket();

				// skip invalid handlers
				if (pCallback == nullptr || !pCallback->GetIsIORegistered())
					continue;

				if ((curEvent.events&(EPOLLERR | EPOLLRDHUP)) != 0)
				{
					char stringBuffer[512];
					netTrace(Trace::TRC_TRACE, "Closing epoll worker sock:{0}, event:{1}", sock, GetNetIOSystem().EventFlagToString(sizeof(stringBuffer), stringBuffer, curEvent.events));
					GetNetIOSystem().UnregisterFromNETIO(pCallback);
					pCallback->OnIOUnregistered();
					continue;
				}

				if (isListenSocket)
				{
					HandleAccept(sock, pCallback);
				}
				else
				{
					//bool hangup = false;
					//if ((curEvent.events&(EPOLLHUP | EPOLLRDHUP)) != 0)
					//{
					//	hangup = true;
					//	//curEvent.events |= EPOLLOUT;
					//}

					HandleRW(sock, curEvent.events, pCallback);

					//if (hangup && pCallback->GetIOSocket() == INVALID_SOCKET)
					//{
					//	GetNetIOSystem().UnregisterFromNETIO(pCallback);
					//	pCallback->OnIOUnregistered();
					//}
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
		Result hr = ResultCode::SUCCESS;
		IOBUFFER_WRITE* pSendBuffer = nullptr;
		DurationMS tickInterval(0);

		while (1)
		{
			hr = ResultCode::SUCCESS;

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
				switch ((int32_t)hr)
				{
				case ResultCode::E_NET_TRY_AGAIN:
					continue; // try again
					break;
				case ResultCode::SUCCESS:
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
	//	EPOLL network system
	//



	EPOLLSystem::EPOLLSystem()
		: m_ListenWorker(nullptr)
		, m_iTCPAssignIndex(0)
		, m_UDPSendWorker(nullptr)
	{
	}

	Result EPOLLSystem::Initialize(UINT netThreadCount)
	{
		if (m_ListenWorker != nullptr)
			return ResultCode::SUCCESS;


		int hEPollUDP = epoll_create(1);

		m_ListenWorker = new EPOLLWorker(false);
		m_ListenWorker->Start();

		m_UDPSendWorker = new EPOLLSendWorker;
		m_UDPSendWorker->Start();

		m_iTCPAssignIndex = 0;

		// client will use only 1 thread
		if (netThreadCount > 1)
		{
			// divide by 2
			netThreadCount >>= 1;


			for (UINT iThread = 0; iThread < netThreadCount; iThread++)
			{
				auto pNewWorker = new EPOLLWorker(true);
				m_WorkerTCP.push_back(pNewWorker);

				pNewWorker->Start();
			}

			// 
			for (UINT iThread = 0; iThread < netThreadCount; iThread++)
			{
				auto pNewWorker = new EPOLLWorker(false, hEPollUDP);
				m_WorkerUDP.push_back(pNewWorker);

				pNewWorker->Start();
			}

		}

		return ResultCode::SUCCESS;
	}

	void EPOLLSystem::Terminate()
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

	Result EPOLLSystem::MakeSocketNonBlocking(SOCKET sfd)
	{
		int flags, s;

		flags = fcntl(sfd, F_GETFL, 0);
		if (flags == -1)
		{
			netTrace(Trace::TRC_ERROR, "epoll_ctl: fcntl F_GETFL");
			return ResultCode::FAIL;
		}

		flags |= O_NONBLOCK;
		s = fcntl(sfd, F_SETFL, flags);
		if (s == -1)
		{
			netTrace(Trace::TRC_ERROR, "epoll_ctl: fcntl F_SETFL");
			return ResultCode::FAIL;
		}

		return ResultCode::SUCCESS;
	}


	WriteBufferQueue* EPOLLSystem::GetWriteBufferQueue()
	{
		if (m_UDPSendWorker == nullptr)
			return nullptr;

		return &m_UDPSendWorker->GetWriteQueue();
	}


	//Result EPOLLSystem::RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance)
	//{
	//	Assert(sockType == SockType::DataGram);
	//	if (sockType != SockType::DataGram)
	//		return ResultCode::UNEXPECTED;

	//	if (m_WorkerUDP.GetSize() < 1)
	//		return ResultCode::E_NET_NOTINITIALISED;

	//	if (cbInstance->GetWriteQueue() == nullptr)
	//	{
	//		Assert(sockType == SockType::DataGram);
	//		cbInstance->SetWriteQueue(&m_UDPSendWorker->GetWriteQueue());
	//	}

	//	return ResultCode::SUCCESS;
	//}

	// Register the socket to EPOLL
	Result EPOLLSystem::RegisterToNETIO(SockType sockType, INetIOCallBack* cbInstance)
	{
		if (m_ListenWorker == nullptr)
			return ResultCode::E_NET_NOTINITIALISED;

		if (sockType == SockType::Stream) // TCP
		{
			// Listen worker will do all job when there is no other thread.
			if (cbInstance->GetIOFlags().IsListenSocket != 0 || m_WorkerTCP.GetSize() == 0)
			{
				return m_ListenWorker->RegisterSocket(cbInstance);
			}
			else
			{
				auto assignIndex = m_iTCPAssignIndex.fetch_add(1,std::memory_order_relaxed) % m_WorkerTCP.GetSize();
				cbInstance->SetAssignedIOWorker(assignIndex);
				m_WorkerTCP[assignIndex]->RegisterSocket(cbInstance);
			}
		}
		else
		{
			if (cbInstance->GetWriteQueue() == nullptr)
			{
				Assert(sockType == SockType::DataGram);
				cbInstance->SetWriteQueue( &m_UDPSendWorker->GetWriteQueue() );
			}

			if (m_WorkerUDP.GetSize() < 1)
			{
				Result hr = m_ListenWorker->RegisterSocket(cbInstance);
				if (!(hr)) return hr;
			}
			else
			{
				// UDP workers are sharing epoll, add any of them will work same.
				Result hr = m_WorkerUDP[0]->RegisterSocket(cbInstance);
				if (!(hr)) return hr;
			}
			cbInstance->SetAssignedIOWorker(0);
		}

		return ResultCode::SUCCESS;
	}

	Result EPOLLSystem::UnregisterFromNETIO(INetIOCallBack* cbInstance)
	{
		if (m_ListenWorker == nullptr)
			return ResultCode::E_NET_NOTINITIALISED;

		SockType sockType = cbInstance->GetIOSockType();

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
				Result hr = m_ListenWorker->UnregisterSocket(cbInstance);
				if (!(hr)) return hr;
			}
			else
			{
				Result hr = m_WorkerUDP[0]->UnregisterSocket(cbInstance);
				if (!(hr)) return hr;
			}
			cbInstance->SetAssignedIOWorker(-1);
		}

		return ResultCode::SUCCESS;
	}


	const char* EPOLLSystem::EventFlagToString(int32_t bufferSize, char* stringBuffer, uint32_t eventFlags)
	{
		const char* orgStart = stringBuffer;
		stringBuffer[0] = '\0';
		if ((eventFlags&EPOLLIN) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLIN ");
		if ((eventFlags&EPOLLOUT) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLOUT ");
		if ((eventFlags&EPOLLRDHUP) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLRDHUP  ");
		if ((eventFlags&EPOLLPRI) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLPRI ");
		if ((eventFlags&EPOLLERR) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLERR ");
		if ((eventFlags&EPOLLHUP) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLHUP ");
		if ((eventFlags&EPOLLET) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLET ");
		if ((eventFlags&EPOLLONESHOT) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLONESHOT ");
		if ((eventFlags&EPOLLWAKEUP) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLWAKEUP ");
#ifdef EPOLLEXCLUSIVE
		if ((eventFlags&EPOLLEXCLUSIVE) != 0) StrUtil::StringCpyEx(stringBuffer, bufferSize, "EPOLLEXCLUSIVE ");
#endif

		return orgStart;
	}


	EPOLLSystem EPOLLSystem::stm_Instance;






} // namespace Net
} // namespace BR

#else
void Dummp_NetSystem_EPOLL() {}
#endif

