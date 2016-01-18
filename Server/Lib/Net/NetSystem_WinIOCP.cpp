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
#include "Net/NetSystem_WinIOCP.h"
#include "Net/NetSystem_impl.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/SpinSharedBuffer.h"
#include "Common/MemoryPool.h"


#if WINDOWS

BR_MEMORYPOOL_IMPLEMENT(Net::IOBUFFER_WRITE);
BR_MEMORYPOOL_IMPLEMENT(Net::IOBUFFER_READ);
BR_MEMORYPOOL_IMPLEMENT(Net::IOBUFFER_ACCEPT);


namespace BR {
namespace Net {


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

		case WSAEAFNOSUPPORT: return E_NET_INVALID_ADDRESS;
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
		case WSA_IO_PENDING: return E_NET_IO_PENDING;
		case WSAEPROTOTYPE: return E_NET_PROTOTYPE;
		case WSAENOPROTOOPT: return E_NET_NOPROTOOPT;
		case WSAEOPNOTSUPP: return E_NET_OPNOTSUPP;
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
		if( hEvent != WSA_INVALID_EVENT )
			WSACloseEvent( hEvent );
		hEvent = WSA_INVALID_EVENT;
	}



	IOBUFFER_RWBASE::IOBUFFER_RWBASE()
	{
	}
	



	IOBUFFER_WRITE::IOBUFFER_WRITE()
	{
		memset( this, 0, sizeof(IOBUFFER_WRITE) );
		hEvent = WSA_INVALID_EVENT;
	}

	IOBUFFER_WRITE::~IOBUFFER_WRITE()
	{
		ClearBuffer();

		Util::SafeRelease( pMsgs );
	}


	IOBUFFER_READ::IOBUFFER_READ()
	{
		memset( this, 0, sizeof(IOBUFFER_READ) );
		hEvent = WSA_INVALID_EVENT;

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
				return E_SYSTEM_FAIL;
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
				Assert(false);
				return E_SYSTEM_FAIL;
			}

			expected = false;
		}
		return S_OK;
	}


	IOBUFFER_ACCEPT::IOBUFFER_ACCEPT()
		//:pConnection(nullptr)
	{
		memset( this, 0, sizeof(IOBUFFER_ACCEPT) );
		hEvent = WSA_INVALID_EVENT;
	}


	IOBUFFER_ACCEPT::~IOBUFFER_ACCEPT()
	{
		ClearBuffer();
	}







	////////////////////////////////////////////////////////////////////////////////
	//
	//	IOCP System
	//

	namespace IOCPSystem
	{


		////////////////////////////////////////////////////////////////////////////////
		//
		//	IOCP thread worker
		//


		void IOCPWorker::Run()
		{
			HRESULT hr = S_OK;
			BOOL bResult;
			DWORD dwTransferred = 0;
			IOBUFFER *pOverlapped = nullptr;
			OVERLAPPED *pOverlappedSys = nullptr;
			ULONG_PTR ulKey = 0;
			int iErr = 0;
			int iLastError;

			while( 1 )
			{
				dwTransferred = 0;
				pOverlapped = nullptr;
				pOverlappedSys = nullptr;
				ulKey = 0;
				iErr = 0;
				iLastError = 0;
				hr = S_OK;


				// Getting IOCP status
				bResult = GetQueuedCompletionStatus( m_hIOCP, &dwTransferred, (PULONG_PTR)&ulKey, &pOverlappedSys, INFINITE );
				pOverlapped = (IOBUFFER*)pOverlappedSys;

				// If End of IOCP signaled
				if( pOverlappedSys == nullptr || pOverlapped == nullptr )
				{
					// Release?
					if (ulKey != 0)
					{
						//INetIOCallBack *pCallback = (INetIOCallBack*)ulKey;
					}
					// chain call to end all IOCP worker
					//if( !PostQueuedCompletionStatus( m_hIOCP, 0, 0, NULL ) )
					//{
					//	netTrace( Trace::TRC_ERROR, "PostQueuedCompletionStatus failed hr={0:X8}", GetLastHRESULT() );
					//}
					break;
				}


				// Error mapping
				if( !bResult )
				{
					iLastError = GetLastError();
					hr = HRESULT_FROM_WIN32(iLastError);

					switch( iLastError )
					{
					case WSAECONNRESET:
					case WSAENOTSOCK:
					case ERROR_CONNECTION_ABORTED:
					case ERROR_HOST_UNREACHABLE:
					case ERROR_PROTOCOL_UNREACHABLE:
					case ERROR_PORT_UNREACHABLE:
					case ERROR_NETNAME_DELETED:
						hr = E_NET_CONNECTION_CLOSED;
						break;
					case ERROR_OPERATION_ABORTED:
						netTrace( Trace::TRC_TRACE, "IOCP Operation aborted" );
						hr = E_NET_IO_ABORTED;
						break;
					default:
						netTrace( Trace::TRC_ERROR, "IOCP Operation failed iErr={0}, hr={1:X8}", iLastError, hr );
						break;
					};
				}

				// Operation
				switch( pOverlapped->Operation )
				{
				case IOBUFFER_OPERATION::OP_TCPACCEPT:
					{
						IOBUFFER_ACCEPT *pAcceptOver = (IOBUFFER_ACCEPT*)pOverlapped;
						INetIOCallBack *pCallback = (INetIOCallBack*)ulKey;
						hr = pCallback->OnIOAccept( hr, pAcceptOver );
					}
					break;
				case IOBUFFER_OPERATION::OP_TCPWRITE:
				case IOBUFFER_OPERATION::OP_UDPWRITE:
					{
						IOBUFFER_WRITE *pIOBuffer = (IOBUFFER_WRITE*)pOverlapped;
						INetIOCallBack *pCallback = (INetIOCallBack*)ulKey;
						hr = pCallback->OnIOSendCompleted( hr, pIOBuffer );
					}
					break;
				case IOBUFFER_OPERATION::OP_TCPREAD:
				case IOBUFFER_OPERATION::OP_UDPREAD:
					if( ulKey ) // TCP operation
					{
						IOBUFFER_READ *pIOBuffer = (IOBUFFER_READ*)pOverlapped;
						INetIOCallBack *pCallback = (INetIOCallBack*)ulKey;
						pIOBuffer->TransferredSize = dwTransferred;
						hr = pCallback->OnIORecvCompleted( hr, pIOBuffer );
						pIOBuffer = nullptr;
						pOverlapped = nullptr;
					}
					else
					{
						AssertRel(!"Invalid Key at IOCP");
					}
					break;
				default:
					netTrace( Trace::TRC_ERROR, "IOCP Invalid Overlapped Operation" );
					break;
				};


			} // while(1)

		}








		//////////////////////////////////////////////////////////////////
		//
		//	network IOCP System
		//

		IOCPSystem IOCPSystem::stm_Instance;
		


		IOCPSystem::IOCPSystem()
			:m_hIOCP(INVALID_HANDLE_VALUE)
		{
		}

		IOCPSystem::~IOCPSystem()
		{
		}

		// Initialize IOCP
		HRESULT IOCPSystem::InitIOCP( UINT uiNumIOCPThread )
		{
			HRESULT hr = S_OK;


			m_RefCount.fetch_add(1, std::memory_order_relaxed);

			if( m_hIOCP != INVALID_HANDLE_VALUE )
			{
				return S_OK;// already initialized
			}


			SYSTEM_INFO sysInfo;
			memset( &sysInfo, 0, sizeof(sysInfo) );
			GetSystemInfo( &sysInfo );

			// Correct if Incorrect thread count specified
			if( uiNumIOCPThread < 1 ) uiNumIOCPThread = 1;
			if( uiNumIOCPThread > (sysInfo.dwNumberOfProcessors*2) ) uiNumIOCPThread = sysInfo.dwNumberOfProcessors*2;


			// Create IOCP port
			m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
			if( m_hIOCP == INVALID_HANDLE_VALUE )
				netErr( GetLastHRESULT() );


			m_pWorkers.resize( uiNumIOCPThread );
			for( UINT iThread = 0; iThread < uiNumIOCPThread; iThread++ )
			{
				m_pWorkers[iThread] = new IOCPWorker;
				m_pWorkers[iThread]->SetIOCPHandle( m_hIOCP );
				m_pWorkers[iThread]->Start();
			}

			netTrace( TRC_INFO, "IOCP system started" );

		Proc_End:

			if( FAILED(hr) )
			{
				CloseIOCP();
			}
			return hr;
		}

		// Close IOCP
		HRESULT IOCPSystem::CloseIOCP()
		{
			HRESULT hr = S_OK;

			CounterType lCount = m_RefCount.fetch_sub(1, std::memory_order_relaxed) - 1;

			if( lCount > 0 )
				return S_OK;

			if( m_hIOCP != INVALID_HANDLE_VALUE )
			{
				// Send IOCP close signal
				if( !PostQueuedCompletionStatus( m_hIOCP, 0, 0, NULL ) )
					netErr( GetLastHRESULT() );
			}


			if( m_pWorkers.size() )
			{
				std::for_each( m_pWorkers.begin(), m_pWorkers.end(), []( IOCPWorker* pThread )
				{
					pThread->Stop( true );
					delete pThread;
				});


				m_pWorkers.clear();
				m_pWorkers.shrink_to_fit();
			}

		Proc_End:

			if( m_hIOCP != INVALID_HANDLE_VALUE )
			{
				// Send IOCP close signal
				CloseHandle( m_hIOCP );
				m_hIOCP = INVALID_HANDLE_VALUE;
			}

			netTrace( TRC_INFO, "IOCP system closed" );

			return hr;
		}



		// Get IOCP system instance
		IOCPSystem& GetSystem()
		{
			return IOCPSystem::GetInstance();
		}
		




	}; // namespace IOCPSystem







	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network system
	//

	namespace NetSystem
	{
		// winsock system open count
		static std::atomic<LONG> g_lWSOpenCount = 0;
		static CriticalSection g_InitLock;

		// Open network system
		HRESULT OpenSystem(UINT uiOverBufferCount, UINT numRecvThread, UINT gatheringBufferSize)
		{
			MutexScopeLock scopeLock(g_InitLock);

			if (g_lWSOpenCount == 0)
			{
				WSADATA wsaData;
				int iErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
				if (iErr != 0)
				{
					netTrace(Trace::TRC_ERROR, "Can't Initialize Winsock DLL err={0}", iErr);
					return iErr;
				}

				Net::IOBUFFER_WRITE::MemoryPoolCache(uiOverBufferCount);
				SetGatheringBufferSize(gatheringBufferSize);

				return IOCPSystem::GetSystem().InitIOCP(numRecvThread);
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
				int iErr = WSACleanup();

				IOCPSystem::GetSystem().CloseIOCP();
			}
		}



		WriteBufferQueue* GetWriteBufferQueue()
		{
			return nullptr;
		}


		///////////////////////////////////////////////////////////////////////////////
		// Socket handling 

		HRESULT RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			HRESULT hr = S_OK;

			netChkPtr(cbInstance);
			Assert(cbInstance->GetIOSocket() != INVALID_SOCKET);

			//netChk(EPOLLSystem::GetSystem().RegisterSharedSocket(sockType, cbInstance));

		Proc_End:

			return hr;
		}


		HRESULT RegisterSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			HRESULT hr = S_OK;

			if (!CreateIoCompletionPort((HANDLE)cbInstance->GetIOSocket(), IOCPSystem::GetSystem().GetIOCP(), (ULONG_PTR)cbInstance, 0))
			{
				hr = GetLastHRESULT();
				netTrace(Trace::TRC_ERROR, "Registering socket to IOCP is Failed, hr = {0:X8}", hr);
				netErr(E_SYSTEM_UNEXPECTED);
			}

		Proc_End:

			return hr;
		}

		HRESULT UnregisterSocket(SockType sockType, INetIOCallBack* cbInstance)
		{
			HRESULT hr = S_OK;

			if (!PostQueuedCompletionStatus(IOCPSystem::GetSystem().GetIOCP(), 0, (ULONG_PTR)cbInstance, 0))
			{
				hr = GetLastHRESULT();
				netTrace(Trace::TRC_ERROR, "Registering socket to IOCP is Failed, hr = {0:X8}", hr);
				netErr(E_SYSTEM_UNEXPECTED);
			}

		Proc_End:

			return hr;
		}

		SOCKET Socket(SockFamily domain, SockType type)
		{
			if (type == SockType::Stream)
			{
				return WSASocket(ToSockValue(domain), ToSockValue(type), IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
			}
			else
			{
				return WSASocket(ToSockValue(domain), ToSockValue(type), IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);
			}
		}

		void CloseSocket(SOCKET sock)
		{
			//shutdown(sock, SHUT_RDWR);
			closesocket(sock);
		}


		HRESULT Accept(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept)
		{
			HRESULT hr = S_OK;

			netChkPtr(pAccept);

			if (!AcceptEx(sockListen, pAccept->sockAccept,
				pAccept->pAcceptInfo, 0,
				sizeof(sockaddr_storage), sizeof(sockaddr_storage),
				&pAccept->dwByteReceived, pAccept))
			{
				HRESULT iErr = GetLastWSAHRESULT();
				switch (iErr)
				{
				case E_NET_NOTSOCK:// server closing or not initialized
					netTrace(Trace::TRC_ERROR, "TCP Abnormal accept, Not socked {0:X8}", iErr);
					netErr(iErr);
					break;
				case E_NET_IO_PENDING:
					//netTrace(TRC_NET, "TCP accept pending {0} queued", m_pAcceptBuffer->GetUsedBufferCount());
					break;
				default:
					netTrace(Trace::TRC_ERROR, "TCP Abnormal accept, err:{0:X8}", iErr);
					netErr(iErr);
					break;
				};
			}
			else
			{
				netTrace(Trace::TRC_ERROR, "TCP Abnormal accept err={0:X8}", GetLastWSAHRESULT());
				netErr(E_SYSTEM_UNEXPECTED);
			}


		Proc_End:

			return hr;
		}

		HRESULT HandleAcceptedSocket(SOCKET sockListen, IOBUFFER_ACCEPT* pAccept, sockaddr_storage& remoteAddr)
		{
			int iLenLocalAddr = 0, iLenRemoteAddr = 0;
			sockaddr_storage *pLocalAddr = nullptr, *pRemoteAddr = nullptr;

			if (setsockopt(pAccept->sockAccept, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&sockListen, sizeof(SOCKET)) == SOCKET_ERROR)
			{
				netTrace(TRC_CONNECTION, "Failed set socket option SO_UPDATE_ACCEPT_CONTEXT err:{0:X8}", GetLastWSAHRESULT());
				return E_SYSTEM_FAIL;
			}


			GetAcceptExSockaddrs( (void*)pAccept->pAcceptInfo,
								  0,
								  sizeof(sockaddr_storage),
								  sizeof(sockaddr_storage),
								  (SOCKADDR**)&pLocalAddr, 
								  &iLenLocalAddr, 
								  (SOCKADDR**)&pRemoteAddr, 
								  &iLenRemoteAddr );

			remoteAddr = *pRemoteAddr;

			return S_OK;
		}


		HRESULT Recv(SOCKET sock, IOBUFFER_READ* pBuffer)
		{
			INT iErr = 0;
			iErr = WSARecv(sock, &pBuffer->wsaBuff, 1, &pBuffer->dwNumberOfByte, &pBuffer->dwFlags, pBuffer, NULL);
			if (iErr == SOCKET_ERROR)
			{
				return GetLastWSAHRESULT();
			}

			return S_OK;
		}

		HRESULT RecvFrom(SOCKET sock, IOBUFFER_READ* pBuffer)
		{
			INT iErr = 0;
			iErr = WSARecvFrom(sock, &pBuffer->wsaBuff, 1, NULL, &pBuffer->dwFlags, (sockaddr*)&pBuffer->NetAddr.From, &pBuffer->iSockLen, pBuffer, NULL);
			if (iErr == SOCKET_ERROR)
			{
				return GetLastWSAHRESULT();
			}

			return S_OK;
		}

		HRESULT Send(SOCKET sock, IOBUFFER_WRITE* pBuffer)
		{
			INT iErr = WSASend(sock, &pBuffer->wsaBuff, 1, nullptr, 0, pBuffer, NULL);
			if (iErr == SOCKET_ERROR)
			{
				return GetLastWSAHRESULT();
			}

			return S_OK;
		}

		HRESULT SendTo(SOCKET sock, IOBUFFER_WRITE* pBuffer)
		{
			const sockaddr_storage& dstAddress = pBuffer->NetAddr.To;
			INT iErr = WSASendTo(sock, &pBuffer->wsaBuff, 1, nullptr, 0, 
				(sockaddr*)&dstAddress, GetSockAddrSize(dstAddress),
				pBuffer, NULL);

			if (iErr == SOCKET_ERROR)
			{
				return GetLastWSAHRESULT();
			}

			return S_OK;
		}

	}; // namespace NetSystem




} // namespace Net
} // namespace BR


#else
void Dummp_NetSystem_WinIOCP() {}
#endif // WINDOWS
