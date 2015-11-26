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
#include "Common/HRESNet.h"
#include "Net/NetTrace.h"
#include "Common/TimeUtil.h"
#include "Common/Utility.h"
#include "Common/SpinSharedBuffer.h"
#include "Common/MemoryPool.h"



BR_MEMORYPOOL_IMPLEMENT(Net::OVERLAPPED_BUFFER_WRITE);


namespace BR {
namespace Net {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Overlapped I/O structures
	//


	// Clear Buffer
	void tag_OVERLAPPED_BUFFER::ClearBuffer()
	{
		if( hEvent != WSA_INVALID_EVENT )
			WSACloseEvent( hEvent );
		hEvent = WSA_INVALID_EVENT;
	}



	tag_OVERLAPPED_BUFFER_BASE::tag_OVERLAPPED_BUFFER_BASE()
	{
	}
	



	OVERLAPPED_BUFFER_WRITE::OVERLAPPED_BUFFER_WRITE()
	{
		memset( this, 0, sizeof(OVERLAPPED_BUFFER_WRITE) );
		hEvent = WSA_INVALID_EVENT;
	}

	OVERLAPPED_BUFFER_WRITE::~OVERLAPPED_BUFFER_WRITE()
	{
		ClearBuffer();

		Util::SafeRelease( pMsgs );
	}


	tag_OVERLAPPED_BUFFER_READ::tag_OVERLAPPED_BUFFER_READ()
	{
		memset( this, 0, sizeof(tag_OVERLAPPED_BUFFER_READ) );
		hEvent = WSA_INVALID_EVENT;

		iSockLen = sizeof(sockaddr_in6);
	}

	tag_OVERLAPPED_BUFFER_READ::~tag_OVERLAPPED_BUFFER_READ()
	{
		ClearBuffer();
	}

	tag_OVERLAPPED_BUFFER_ACCEPT::tag_OVERLAPPED_BUFFER_ACCEPT()
		//:pConnection(nullptr)
	{
		memset( this, 0, sizeof(tag_OVERLAPPED_BUFFER_ACCEPT) );
		hEvent = WSA_INVALID_EVENT;
	}


	tag_OVERLAPPED_BUFFER_ACCEPT::~tag_OVERLAPPED_BUFFER_ACCEPT()
	{
		ClearBuffer();
	}




	////////////////////////////////////////////////////////////////////////////////
	//
	//	Windows socket control
	//
	namespace WSASystem
	{
		// winsock system open count
		static std::atomic<LONG> g_lWSOpenCount = 0;

		// Buffer sharing for network
		//static BR::SpinSharedBuffer<OVERLAPPED_BUFFER_WRITE> *g_pOverlappedBuffer = nullptr;




		// Open network system
		HRESULT OpenSystem( UINT uiOverBufferCount )
		{
			if( g_lWSOpenCount == 0 )
			{
				WSADATA wsaData;
				int iErr = WSAStartup(MAKEWORD(2,2), &wsaData);
				if( iErr != 0 )
				{
					netTrace( Trace::TRC_ERROR, "Can't Initialize Winsock DLL err=%0%", iErr );
					return iErr;
				}

				BR::Net::OVERLAPPED_BUFFER_WRITE::MemoryPoolCache( uiOverBufferCount );
			}

			g_lWSOpenCount.fetch_add(1, std::memory_order_relaxed);

			return S_OK;
		}

		// Close network system
		void CloseSystem()
		{
			int iErr = 0;
			LONG lCount = g_lWSOpenCount;
			if( g_lWSOpenCount > 0 )
			{
				lCount = g_lWSOpenCount.fetch_sub(1, std::memory_order_relaxed) - 1;
			}

			if( lCount <= 0 )
			{
				int iErr = WSACleanup();
			}
		}



		HRESULT AllocBuffer( OVERLAPPED_BUFFER_WRITE* &pIOBuffer )
		{
			pIOBuffer = new OVERLAPPED_BUFFER_WRITE;

			return pIOBuffer == nullptr ? E_FAIL : S_OK;
		}

		HRESULT FreeBuffer( OVERLAPPED_BUFFER_WRITE *pIOBuffer )
		{
			Util::SafeDelete( pIOBuffer );

			return S_OK;
		}

	}; // namespace WSASystem





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
			OVERLAPPED_BUFFER *pOverlapped = NULL;
			OVERLAPPED *pOverlappedSys = NULL;
			ULONG_PTR ulKey = NULL;
			int iErr = 0;
			int iLastError;

			while( 1 )
			{
				dwTransferred = 0;
				pOverlapped = NULL;
				pOverlappedSys = NULL;
				ulKey = NULL;
				iErr = 0;
				iLastError = 0;
				hr = S_OK;


				// Getting IOCP status
				bResult = GetQueuedCompletionStatus( m_hIOCP, &dwTransferred, (PULONG_PTR)&ulKey, &pOverlappedSys, INFINITE );
				pOverlapped = (OVERLAPPED_BUFFER*)pOverlappedSys;

				// If End of IOCP signaled
				if( pOverlappedSys == nullptr || pOverlapped == nullptr )
				{
					// chain call to end all IOCP worker
					if( !PostQueuedCompletionStatus( m_hIOCP, 0, 0, NULL ) )
					{
						netTrace( Trace::TRC_ERROR, "PostQueuedCompletionStatus failed hr={0:X8}", GetLastHRESULT() );
					}
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
						if( pOverlapped->Operation != OVERLAPPED_BUFFER::OP_UDPREAD // UDP can't distingush which connection network err
							&& pOverlapped->Operation != OVERLAPPED_BUFFER::OP_PEERUDPREAD )
							netTrace( TRC_CONNECTION, "Closing Connection by iErr=%0%, hr={1:X8}", iLastError, hr );
						hr = E_NET_CONNECTION_CLOSED;
						break;
					case ERROR_OPERATION_ABORTED:
						netTrace( Trace::TRC_TRACE, "IOCP Operation aborted" );
						hr = E_NET_IO_ABORTED;
						break;
					default:
						netTrace( Trace::TRC_IERROR, "IOCP Operation failed iErr=%0%, hr={1:X8}", iLastError, hr );
						break;
					};
				}


				// Operation
				switch( pOverlapped->Operation )
				{
				case OVERLAPPED_BUFFER::OP_TCPACCEPT:
					{
						OVERLAPPED_BUFFER_ACCEPT *pAcceptOver = (OVERLAPPED_BUFFER_ACCEPT*)pOverlapped;
						IOCallBack *pCallback = (IOCallBack*)ulKey;
						hr = pCallback->OnIOAccept( hr, pAcceptOver );
					}
					break;
				case OVERLAPPED_BUFFER::OP_TCPWRITE:
				case OVERLAPPED_BUFFER::OP_UDPWRITE:
				case OVERLAPPED_BUFFER::OP_PEERUDPWRITE:
					{
						OVERLAPPED_BUFFER_WRITE *pIOBuffer = (OVERLAPPED_BUFFER_WRITE*)pOverlapped;
						IOCallBack *pCallback = (IOCallBack*)ulKey;
						hr = pCallback->OnIOSendCompleted( hr, pIOBuffer, dwTransferred );
					}
					break;
				case OVERLAPPED_BUFFER::OP_TCPREAD:
				case OVERLAPPED_BUFFER::OP_UDPREAD:
				case OVERLAPPED_BUFFER::OP_PEERUDPREAD:
					if( ulKey ) // TCP operation
					{
						OVERLAPPED_BUFFER_READ *pIOBuffer = (OVERLAPPED_BUFFER_READ*)pOverlapped;
						IOCallBack *pCallback = (IOCallBack*)ulKey;
						hr = pCallback->OnIORecvCompleted( hr, pIOBuffer, dwTransferred );
						pOverlapped = NULL;
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



} // namespace Net
} // namespace BR

