////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network Server implementation 
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Net/Connection.h"
#include "Net/NetTrace.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "Protocol/ProtocolVer.h"
#include "Net/NetServerUDP.h"




namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Server class
	//

	ServerUDPBase::ServerUDPBase(ServerID InServerID, NetClass localClass)
		:Server(InServerID, localClass),
		m_pRecvBuffers(nullptr)
	{
	}

	ServerUDPBase::~ServerUDPBase()
	{
		if( m_pRecvBuffers )
			delete[] m_pRecvBuffers;
	}

	// Close all connection
	HRESULT ServerUDPBase::CloseAllConnection()
	{
		return GetConnectionManager().PendingCloseAllConnection();
	}

	HRESULT ServerUDPBase::OnIOAccept( HRESULT hrRes, OVERLAPPED_BUFFER_ACCEPT *pAcceptInfo )
	{
		Assert(!"Invalid operation");
		return E_NOTIMPL;
	}
	
	
	// called when send completed
	HRESULT ServerUDPBase::OnIOSendCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_WRITE *pIOBuffer, DWORD dwTransferred )
	{
		ConnectionUDPBase::ReleaseGatheringBuffer(pIOBuffer->pSendBuff);
		Util::SafeRelease( pIOBuffer->pMsgs );
		WSASystem::FreeBuffer( pIOBuffer );
		return S_OK;
	}

	
	// Get Active(non-free) connection
	CounterType ServerUDPBase::GetActiveConnectionCount()
	{
		return GetConnectionManager().GetNumActiveConnection();
	}

	// Open host and start listen
	HRESULT ServerUDPBase::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		INT iOptValue, iOptLen;
		BOOL bOptValue = 0;
		sockaddr_in6 bindAddr;

		if( GetSocket() != INVALID_SOCKET )
			return S_OK;


		netChk( __super::HostOpen( netCls, strLocalIP, usLocalPort ) );

		netTrace(Trace::TRC_TRACE, "Open Server UDP Host %0%:%1%", strLocalIP, usLocalPort );

		socket = WSASocket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Server Socket %0%", WSAGetLastError());
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = 0;
		iOptLen = sizeof(iOptValue);
		if( getsockopt( socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *)&iOptValue, &iOptLen ) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to get socket option SO_MAX_MSG_SIZE = %0%, err = %1%", iOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}
		if( iOptValue < Const::PACKET_SIZE_MAX )
		{
			netTrace(Trace::TRC_WARN, "Socket max packet size too small, Change to socket maximum SocketMax=%0%, SvrMax=%1%, err = %2%", iOptValue, (UINT)Const::PACKET_SIZE_MAX, WSAGetLastError() );
			//Const::PACKET_SIZE_MAX = iOptValue;
		}

		bOptValue = TRUE;
		if( setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_REUSEADDR = %0%, err = %1%", bOptValue, WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		iOptValue = FALSE;
		if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = %0%, err = %1%", iOptValue, WSAGetLastError());
			netErr(E_UNEXPECTED);
		}


		bindAddr = GetSocketAddr();
		bindAddr.sin6_family = AF_INET6;
		bindAddr.sin6_addr = in6addr_any;
		if (bind(socket, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, UDP err=%0%", WSAGetLastError() );
			netErr( E_UNEXPECTED );
		}

		if( !CreateIoCompletionPort((HANDLE)socket, IOCPSystem::GetSystem().GetIOCP(), (ULONG_PTR)(IOCPSystem::IOCallBack*)this, 0) )
		{
			netTrace(Trace::TRC_ERROR, "CreateIoCompletionPort Failed Udp, hr = %0%", GetLastHRESULT() );
			netErr( E_UNEXPECTED );
		}


		SetSocket( socket );
		socket = INVALID_SOCKET;

		// Prepare connection
		netChk( GetConnectionManager().InitManager( Const::SVR_PUBLIC_CONNECTION_POOLCACHE ) );


		// Ready recv
		if( m_pRecvBuffers ) delete[] m_pRecvBuffers;
		netMem( m_pRecvBuffers = new OVERLAPPED_BUFFER_READ[Const::SVR_NUM_RECV_THREAD] );

		for( INT uiRecv = 0; uiRecv < Const::SVR_NUM_RECV_THREAD; uiRecv++ )
		{
			netChk( PendingRecv( &m_pRecvBuffers[uiRecv] ) );
		}

		EnqueueNetEvent( INet::Event( INet::Event::EVT_NET_INITIALIZED ) ); 


	Proc_End:

		if( FAILED(hr) )
			HostClose();

		if( socket != INVALID_SOCKET )
			closesocket( socket );

		netTrace( TRC_NET, "HostOpen %0%, hr=%1%", GetLocalAddress(), ArgHex32(hr) );

		return hr;
	}

	// Close host and close all connections
	HRESULT ServerUDPBase::HostClose()
	{
		HRESULT hr = S_OK;

		netChk( __super::HostClose() );

		if( GetSocket() != INVALID_SOCKET )
		{
			closesocket( GetSocket() );
			SetSocket( INVALID_SOCKET );
		}

		EnqueueNetEvent( INet::Event( INet::Event::EVT_NET_CLOSED ) ); 

	Proc_End:

		netTrace( TRC_NET, "HostClose %0%, hr=%1%", GetLocalAddress(), ArgHex32(hr) );

		return hr;
	}


	// Send message to connection with network device
	HRESULT ServerUDPBase::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK;
		int iWSAErr = 0;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		UINT uiMsgLen = pMsg->GetMessageHeader()->Length;
		ConnectionUDPBase *pUDPCon = (ConnectionUDPBase*)pConnection;
		OVERLAPPED_BUFFER_WRITE *pOverlapped = nullptr;

		UINT length = 0;
		BYTE* pDataPtr = nullptr;
		pMsg->GetLengthNDataPtr( length, pDataPtr );
		Assert(length == 0 || pMsg->GetMessageHeader()->Crc32 != 0 || pMsg->GetMessageHeader()->msgID.IDs.Policy == POLICY_NONE);


		netChk( Net::WSASystem::AllocBuffer(pOverlapped) );

		pOverlapped->SetupSendUDP( pMsg );

		if( WSASendTo( pUDPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0,
			(sockaddr*)&pUDPCon->GetRemoteSockAddr(), sizeof(sockaddr_in6), 
			pOverlapped, nullptr ) == SOCKET_ERROR )
		{
			iWSAErr = WSAGetLastError();
			if( iWSAErr != WSA_IO_PENDING )
			{
				switch( iWSAErr )
				{
				case WSAECONNABORTED:
				case WSAECONNRESET:
				case WSAENETRESET:
				case WSAENOTCONN:
				case WSAENOTSOCK:
				case WSAESHUTDOWN:
					// Send fail by connection close
					// Need to disconnect
					pUDPCon->Disconnect();
					netErrSilent( E_NET_CONNECTION_CLOSED );
					break;
				default:
					netErr( E_NET_IO_SEND_FAIL );
					break;
				};
			}
		}
		else
		{
			// Send done will be handled by IOCP
		}

	Proc_End:

		if( FAILED(hr) )
		{
			if( pOverlapped )
			{
				Util::SafeRelease( pOverlapped->pMsgs );
				Net::WSASystem::FreeBuffer(pOverlapped);
			}
			else
			{
				Util::SafeRelease( pMsg );
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:%0%, err:%1%, hr:%2%", pUDPCon->GetConnectionInfo(), iWSAErr, ArgHex32(hr) );
			}
			else
			{
				netTrace(Net::TRC_SENDRAW, "UDP Send Failed, ip:%0%, err:%1%, hr:%2%", pUDPCon->GetConnectionInfo(), iWSAErr, ArgHex32(hr));
				return S_OK;
			}
		}
		else
		{
			if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
			{
				netTrace( TRC_NETCTRL, "UDP SendCtrl ip:%0%, msg:%1%, Len:%2%", pUDPCon->GetConnectionInfo(), msgID, uiMsgLen );
			}
			else
			{
				netTrace( TRC_SENDRAW, "UDP Send ip:%0%, msg:%1%, Len:%2%", pUDPCon->GetConnectionInfo(), msgID, uiMsgLen );
			}
		}

		return hr;
	}

	// Send message to connection with network device to dst addr
	HRESULT ServerUDPBase::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK;
		int iWSAErr = 0;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		UINT uiMsgLen = pMsg->GetMessageHeader()->Length;

		ConnectionUDPBase *pUDPCon = static_cast<ConnectionUDPBase*>(pConnection);
		NetAddress dstAddress;

		SOCKET socketToSend = INVALID_SOCKET;
		if( pConnection != nullptr && pUDPCon != nullptr )
		{
			socketToSend = pUDPCon->GetSocket();
			dstAddress = pUDPCon->GetConnectionInfo().Remote;
		}
		else
		{
			socketToSend = GetSocket();
			SockAddr2Addr( dstAddr, dstAddress);
		}

		
		// If the address isn't mapped yet. ignore it
		if( pUDPCon != nullptr && pUDPCon->GetRemoteSockAddr().sin6_port == 0
			|| dstAddr.sin6_port == 0)
		{
			netTrace( Net::TRC_NET, "Remote address isn't mapped yet, ignoring send CID:%0%", pConnection->GetCID() );
			goto Proc_End;
		}


		OVERLAPPED_BUFFER_WRITE *pOverlapped = NULL;
		netChk( WSASystem::AllocBuffer(pOverlapped) );

		pOverlapped->SetupSendUDP( pMsg );
		pMsg = nullptr;

		if( WSASendTo( socketToSend, &pOverlapped->wsaBuff, 1, nullptr, 0,
			(sockaddr*)&dstAddr, sizeof(sockaddr_in6), 
			pOverlapped, NULL ) == SOCKET_ERROR )
		{
			iWSAErr = WSAGetLastError();
			if( iWSAErr != WSA_IO_PENDING )
			{
				switch( iWSAErr )
				{
				case WSAECONNABORTED:
				case WSAECONNRESET:
				case WSAENETRESET:
				case WSAENOTCONN:
				case WSAENOTSOCK:
				case WSAESHUTDOWN:
					// Send fail by connection close
					// Need to disconnect
					pUDPCon->Disconnect();
					netErrSilent( E_NET_CONNECTION_CLOSED );
					break;
				default:
					netErr( E_NET_IO_SEND_FAIL );
					break;
				};
			}
		}

	Proc_End:

		Util::SafeRelease( pMsg );

		if( FAILED(hr) )
		{
			if( pOverlapped )
			{
				Util::SafeRelease( pOverlapped->pMsgs );
				WSASystem::FreeBuffer(pOverlapped);
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:%0%, err:%1%, hr:%2%", dstAddress, iWSAErr, ArgHex32(hr) );
			}
			else
				return S_OK;
		}
		else
		{
			if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
			{
				netTrace( TRC_NETCTRL, "UDP SendCtrl ip:%0%, msg:%1%, Len:%2%", dstAddress, msgID, uiMsgLen );
			}
			else
			{
				netTrace( TRC_SENDRAW, "UDP Send ip:%0%, msg:%1%, Len:%2%", dstAddress, msgID, uiMsgLen );
			}
		}

		return hr;
	}


	HRESULT ServerUDPBase::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	{
		HRESULT hr = S_OK;
		//DWORD dwSend = 0;
		int iWSAErr = 0;

		ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;

		// If the address isn't mapped yet. ignore it
		if (pUDPCon->GetRemoteSockAddr().sin6_port == 0)
		{
			netTrace( Net::TRC_NET, "Remote address isn't mapped yet, ignoring send CID:%0%", pConnection->GetCID() );
			goto Proc_End;
		}

		OVERLAPPED_BUFFER_WRITE *pOverlapped = nullptr;
		netChk( BR::Net::WSASystem::AllocBuffer(pOverlapped) );

		pOverlapped->SetupSendUDP( uiBuffSize, pBuff );
		pBuff = nullptr;


		if( WSASendTo( pUDPCon->GetSocket(), &pOverlapped->wsaBuff, 1, nullptr, 0,
			(sockaddr*)&pUDPCon->GetRemoteSockAddr(), sizeof(sockaddr_in6), 
			pOverlapped, nullptr ) == SOCKET_ERROR )
		{
			iWSAErr = WSAGetLastError();
			if( iWSAErr != WSA_IO_PENDING )
			{
				switch( iWSAErr )
				{
				case WSAECONNABORTED:
				case WSAECONNRESET:
				case WSAENETRESET:
				case WSAENOTCONN:
				case WSAENOTSOCK:
				case WSAESHUTDOWN:
					// Send fail by connection close
					// Need to disconnect
					pUDPCon->Disconnect();
					netErrSilent( E_NET_CONNECTION_CLOSED );
					break;
				default:
					netErr( E_NET_IO_SEND_FAIL );
					break;
				};
			}
		}
		else
		{
		}

	Proc_End:

		if( pBuff )
		{
			ConnectionUDPBase::ReleaseGatheringBuffer(pBuff);
		}

		if( FAILED(hr) )
		{
			if( pOverlapped )
			{
				if( pOverlapped->pSendBuff )
					ConnectionUDPBase::ReleaseGatheringBuffer(pOverlapped->pSendBuff);
				pOverlapped->pSendBuff = nullptr;
				BR::Net::WSASystem::FreeBuffer(pOverlapped);
			}

			if( hr != E_NET_IO_SEND_FAIL )
			{
				netTrace( TRC_NETCTRL, "UDP Send Failed, ip:%0%, err:%1%, hr:%2%", pUDPCon->GetConnectionInfo(), iWSAErr, ArgHex32(hr) );
			}
			else
				return S_OK;
		}
		else
		{
			netTrace( TRC_SENDRAW, "UDP Send ip:%0%, Len:%1%", pUDPCon->GetConnectionInfo(), uiBuffSize );
		}

		return hr;
	}


	// Pending recv New one
	HRESULT ServerUDPBase::PendingRecv( OVERLAPPED_BUFFER_READ *pOver )
	{
		HRESULT hr = S_OK;
		INT iErr = 0, iErr2 = 0;


		pOver->SetupRecvUDP(0);

		while( (iErr = WSARecvFrom( GetSocket(), &pOver->wsaBuff, 1, NULL, &pOver->dwFlags, (sockaddr*)&pOver->From, &pOver->iSockLen, pOver, nullptr )) == SOCKET_ERROR )
		{
			iErr2 = WSAGetLastError();
			switch( iErr2 )
			{
			case WSA_IO_PENDING:
				goto Proc_End;// success
				break;
			case WSAENETUNREACH:
			case WSAECONNABORTED:
			case WSAECONNRESET:
			case WSAENETRESET:
				// some remove has problem with continue connection
				netTrace( TRC_NETCTRL, "UDP Remote has connection error err=%0%, %1%", iErr2, pOver->From );
				//break;
			default:
				// Unknown error
				netTrace( TRC_RECVRAW, "UDP Read Pending failed err=%0%, hr=%1%", iErr2, ArgHex32(HRESULT_FROM_WIN32(iErr2)) );
				//netErr( HRESULT_FROM_WIN32(iErr2) );
				break;
			};
		}

	Proc_End:



		return hr;
	}

	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Server class
	//

	ServerUDP::ServerUDP(ServerID InServerID, NetClass localClass)
		:ServerUDPBase(InServerID, localClass)
		,m_ConnectionManager(Const::SVR_PUBLIC_CONNECTION_BUCKET_SIZE)
	{
		m_ConnectionManager.SetNetOwner( this );
	}

	ServerUDP::~ServerUDP()
	{
	}

	// called when reciving message
	HRESULT ServerUDP::OnIORecvCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred )
	{
		HRESULT hr = S_OK;

		SharedPointerT<Connection> pConnection;
		IConnection::ConnectionInformation connectionInfo;
		bool bReleaseOnFail = false;

		if( FAILED( hrRes ) )
		{
			switch( hrRes )
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
				if (SUCCEEDED(GetConnectionManager().GetConnectionByAddr(pIOBuffer->From, pConnection)))
				{
					netTrace( TRC_RECV, "UDP bad connection state IP:%0%", pIOBuffer->From );
				}
				hr = hrRes;
				break;
			default:
				netTrace( Trace::TRC_ERROR, "UDP Recv Msg Failed, SvrUDP, IP:%0%, hr=%1%", pIOBuffer->From, ArgHex32(hrRes) );
				break;
			};
		}
		else
		{
			if( dwTransferred < sizeof(Message::MessageHeader) )// invalid packet size
				goto Proc_End;

			if( GetIsEnableAccept()
				&& FAILED(GetConnectionManager().GetConnectionByAddr(pIOBuffer->From, pConnection)))
			{
				MsgNetCtrlConnect *pNetCtrl = (MsgNetCtrlConnect*)pIOBuffer->wsaBuff.buf;
				if (pNetCtrl->Length == sizeof(MsgNetCtrlConnect) && pNetCtrl->msgID.IDSeq.MsgID == PACKET_NETCTRL_CONNECT.IDSeq.MsgID && pNetCtrl->rtnMsgID.ID == BR::PROTOCOL_VERSION)
				{
					netChk( GetConnectionManager().PendingNewConnection(pIOBuffer->From, pNetCtrl) );
				}
				else if( pNetCtrl->msgID.ID == PACKET_NETCTRL_ACK )
				{
					// ignore this packet
				}
				else
				{
					// send disconnect
					netTrace( Trace::TRC_WARN, "HackWarn : Not allowered connection try from %0%", pIOBuffer->From );
				}

			}
			else
			{
				//pConnection = (*iterCon).pConnection;
			}

			if( pConnection != nullptr )
			{
				netChk( pConnection->OnRecv( dwTransferred, (BYTE*)pIOBuffer->wsaBuff.buf ) );
			}
		}

	Proc_End:

		if( pConnection != nullptr )
		{
			if( bReleaseOnFail )
			{
				if (pConnection->GetConnectionState() != IConnection::STATE_DISCONNECTED)
					pConnection->CloseConnection();
				ReleaseConnection( (Connection*)pConnection );
			}
		}

		if( hrRes != E_NET_IO_ABORTED )
			PendingRecv( (OVERLAPPED_BUFFER_READ*)pIOBuffer );

		return hr;
	}
	
	
	


} // namespace Net
} // namespace BR

