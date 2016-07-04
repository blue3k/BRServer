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
#include "Common/ResultCode/BRResultCodeNet.h"
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
#include "Net/NetCtrl.h"




namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Server class
	//

	ServerUDPBase::ServerUDPBase(ServerID InServerID, NetClass localClass)
		: Server(InServerID, localClass)
		, INetIOCallBack(GetSocket())
		, m_pRecvBuffers(nullptr)
	{
		// We will share the write queue in net system for UDP
		//SetWriteQueue(new WriteBufferQueue);
	}

	ServerUDPBase::~ServerUDPBase()
	{
		if( m_pRecvBuffers )
			delete[] m_pRecvBuffers;

		if (GetWriteQueue()) delete GetWriteQueue();
	}

	// Close all connection
	HRESULT ServerUDPBase::CloseAllConnection()
	{
		return GetConnectionManager().PendingCloseAllConnection();
	}

	HRESULT ServerUDPBase::Recv(IOBUFFER_READ* pIOBuffer)
	{
		HRESULT hr = S_SYSTEM_OK, hrErr = S_SYSTEM_OK;

		netChkPtr(pIOBuffer);

		pIOBuffer->SetupRecvUDP(0);

		hrErr = NetSystem::RecvFrom(GetSocket(), pIOBuffer);
		switch (hrErr)
		{
		case S_SYSTEM_FALSE:
			hr = E_NET_TRY_AGAIN;
			break;
		case S_SYSTEM_OK:
		case E_NET_IO_PENDING:
		case E_NET_TRY_AGAIN:
		case E_NET_WOULDBLOCK:
			hr = hrErr;
			goto Proc_End;// success
			break;
		case E_NET_NETUNREACH:
		case E_NET_CONNABORTED:
		case E_NET_CONNRESET:
		case E_NET_NETRESET:
			// some remove has problem with continue connection
			netTrace(TRC_NETCTRL, "UDP Remote has connection error err={0:X8}, {1}", hrErr, pIOBuffer->NetAddr.From);
			//break;
		default:
			// Unknown error
			netTrace(TRC_RECVRAW, "UDP Read Pending failed err={0:X8}", hrErr);
			//netErr( HRESULT_FROM_WIN32(iErr2) );
			break;
		};

	Proc_End:

		return hr;
	}


	HRESULT ServerUDPBase::OnSendReady()
	{
		return ProcessSendQueue();
	}

	HRESULT ServerUDPBase::SendBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		HRESULT hr = S_SYSTEM_OK, hrErr = S_SYSTEM_OK;
		auto addrTo = pSendBuffer->NetAddr.To;

		hrErr = NetSystem::SendTo(GetSocket(), pSendBuffer);
		switch (hrErr)
		{
		case E_NET_TRY_AGAIN:
			break;
		case S_SYSTEM_OK:
		case E_NET_IO_PENDING:
		case E_NET_WOULDBLOCK:
			break;
		case E_NET_CONNABORTED:
		case E_NET_CONNRESET:
		case E_NET_NETRESET:
		case E_NET_NOTCONN:
		case E_NET_NOTSOCK:
		case E_NET_SHUTDOWN:
			// Send fail by connection close
			// Need to disconnect
			hr = E_NET_CONNECTION_CLOSED;
			goto Proc_End;
			break;
		default:
			netErr(E_NET_IO_SEND_FAIL);
			break;
		};

	Proc_End:

		if (FAILED(hr))
		{
			//if (pSendBuffer)
			//{
			//	Util::SafeRelease(pSendBuffer->pMsgs);
			//	Net::NetSystem::FreeBuffer(pSendBuffer);
			//}

			if (hr != E_NET_IO_SEND_FAIL)
			{
				netTrace(Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", addrTo, hrErr, hr);
			}
			else
			{
				netTrace(Net::TRC_SENDRAW, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", addrTo, hrErr, hr);
				return S_SYSTEM_OK;
			}
		}
		else
		{
			//if (msgID.IDs.Type == Message::MSGTYPE_NETCONTROL)
			//{
			//	netTrace(TRC_NETCTRL, "UDP SendCtrl ip:{0}, msg:{1}, Len:{2}", addrTo);
			//}
			//else
			//{
			//	netTrace(TRC_SENDRAW, "UDP Send ip:{0}, msg:{1}, Len:{2}", addrTo);
			//}
		}

		return hr;
	}
	
	// called when send completed
	HRESULT ServerUDPBase::OnIOSendCompleted( HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		NetSystem::FreeGatheringBuffer(pIOBuffer->pSendBuff);
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		return S_SYSTEM_OK;
	}

	
	// Get Active(non-free) connection
	CounterType ServerUDPBase::GetActiveConnectionCount()
	{
		return GetConnectionManager().GetNumActiveConnection();
	}

	// Open host and start listen
	HRESULT ServerUDPBase::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		HRESULT hr = S_SYSTEM_OK;
		SOCKET socket = INVALID_SOCKET;
		INT iOptValue;
		sockaddr_storage bindAddr;
		INet::Event netEvent(INet::Event::EVT_NET_INITIALIZED);

		if( GetSocket() != INVALID_SOCKET )
			return S_SYSTEM_OK;


		netChk(Server::HostOpen( netCls, strLocalIP, usLocalPort ) );

		netTrace(Trace::TRC_TRACE, "Open Server UDP Host {0}:{1}", strLocalIP, usLocalPort );

		socket = NetSystem::Socket(GetLocalAddress().SocketFamily, SockType::DataGram);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Server Socket {0:X8}", GetLastWSAHRESULT());
			netErr( E_SYSTEM_UNEXPECTED );
		}

		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT() );
			netErr( E_SYSTEM_UNEXPECTED );
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT() );
			netErr( E_SYSTEM_UNEXPECTED );
		}

#if WINDOWS
		iOptValue = 0;
		{
			INT iOptLen = sizeof(iOptValue);
			if (getsockopt(socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *)&iOptValue, &iOptLen) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "Failed to get socket option SO_MAX_MSG_SIZE = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
				netErr(E_SYSTEM_UNEXPECTED);
			}
		}
		if (iOptValue < Const::PACKET_SIZE_MAX)
		{
			netTrace(Trace::TRC_WARN, "Socket max packet size too small, Change to socket maximum SocketMax={0}, SvrMax={1}, err = {2:X8}", iOptValue, (UINT)Const::PACKET_SIZE_MAX, GetLastWSAHRESULT());
			//Const::PACKET_SIZE_MAX = iOptValue;
		}
#endif

		//bOptValue = TRUE;
		//if( setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR )
		//{
		//	hr = GetLastWSAHRESULT();
		//	netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_REUSEADDR = {0}, err = {1:X8}", bOptValue, hr);
		//	netErr(hr);
		//}

		// enable dual stack all the time
		//if (GetLocalAddress().SocketFamily == SockFamily::IPV6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				hr = GetLastWSAHRESULT();
				netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, hr);
				netErr(hr);
			}
		}


		GetAnyBindAddr(GetSocketAddr(), bindAddr);
		if (bind(socket, (sockaddr*)&bindAddr, GetSocketAddrSize()) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, UDP err={0:X8}", GetLastWSAHRESULT() );
			netErr( E_SYSTEM_UNEXPECTED );
		}

		SetSocket( socket );
		socket = INVALID_SOCKET;

		netChk(NetSystem::RegisterSocket(SockType::DataGram, this));

		// Prepare connection
		netChk( GetConnectionManager().InitManager( Const::SVR_PUBLIC_CONNECTION_POOLCACHE ) );


		// Ready recv
		if (NetSystem::IsProactorSystem())
		{
			if (m_pRecvBuffers) delete[] m_pRecvBuffers;
			netMem(m_pRecvBuffers = new IOBUFFER_READ[Const::SVR_NUM_RECV_THREAD]);

			for (INT uiRecv = 0; uiRecv < Const::SVR_NUM_RECV_THREAD; uiRecv++)
			{
				netChk(PendingRecv(&m_pRecvBuffers[uiRecv]));
			}
		}

		EnqueueNetEvent( netEvent ); 


	Proc_End:

		if( FAILED(hr) )
			HostClose();

		if( socket != INVALID_SOCKET )
			NetSystem::CloseSocket( socket );

		netTrace( TRC_NET, "HostOpen {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}

	// Close host and close all connections
	HRESULT ServerUDPBase::HostClose()
	{
		HRESULT hr = S_SYSTEM_OK;
		INet::Event netEvent(INet::Event::EVT_NET_CLOSED);

		netChk(Server::HostClose() );

		if( GetSocket() != INVALID_SOCKET )
		{
			NetSystem::CloseSocket( GetSocket() );
			SetSocket( INVALID_SOCKET );
		}

		EnqueueNetEvent(netEvent);

	Proc_End:

		netTrace( TRC_NET, "HostClose {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}



	// Pending recv New one
	HRESULT ServerUDPBase::PendingRecv( IOBUFFER_READ *pOver )
	{
		HRESULT hr = S_SYSTEM_OK, hrErr;

		if (!NetSystem::IsProactorSystem())
			return S_SYSTEM_OK;

		netChk(pOver->SetPendingTrue());

		while (1)
		{
			hrErr = Recv(pOver);
			switch (hrErr)
			{
			case S_SYSTEM_OK:
			case E_NET_IO_PENDING:
			case E_NET_TRY_AGAIN:
			case E_NET_WOULDBLOCK:
				goto Proc_End;// success
				break;
			case E_NET_NETUNREACH:
			case E_NET_CONNABORTED:
			case E_NET_CONNRESET:
			case E_NET_NETRESET:
				// some remove has problem with continue connection
				netTrace(TRC_NETCTRL, "UDP Remote has connection error err={0:X8}, {1}", hrErr, pOver->NetAddr.From);
				//break;
			default:
				// Unknown error
				netTrace(TRC_RECVRAW, "UDP Read Pending failed err={0:X8}", hrErr);
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
	HRESULT ServerUDP::OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ* &pIOBuffer )
	{
		HRESULT hr = S_SYSTEM_OK;
		SharedPointerT<Connection> pConnection;
		IConnection::ConnectionInformation connectionInfo;
		bool bReleaseOnFail = false;
		sockaddr_storage from;
		if (pIOBuffer != nullptr) from = pIOBuffer->NetAddr.From;
		else memset(&from, 0, sizeof(from));


		if( FAILED( hrRes ) )
		{
			switch( hrRes )
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
				if (SUCCEEDED(GetConnectionManager().GetConnectionByAddr(from, pConnection)))
				{
					netTrace( TRC_RECV, "UDP bad connection state IP:{0}", from);
				}
				hr = hrRes;
				break;
			default:
				netTrace( Trace::TRC_ERROR, "UDP Recv Msg Failed, SvrUDP, IP:{0}, hr={1:X8}", from, hrRes );
				break;
			};
		}
		else if(pIOBuffer != nullptr)
		{
			if(pIOBuffer->TransferredSize < sizeof(Message::MessageHeader) )// invalid packet size
				goto Proc_End;

			if( GetIsEnableAccept()
				&& FAILED(GetConnectionManager().GetConnectionByAddr(pIOBuffer->NetAddr.From, pConnection)))
			{
				MsgNetCtrlConnect *pNetCtrl = (MsgNetCtrlConnect*)pIOBuffer->buffer;
				if (pNetCtrl->Length == sizeof(MsgNetCtrlConnect) && pNetCtrl->msgID.IDSeq.MsgID == PACKET_NETCTRL_CONNECT.IDSeq.MsgID && pNetCtrl->rtnMsgID.ID == BR_PROTOCOL_VERSION)
				{
					netChk( GetConnectionManager().PendingNewConnection(pIOBuffer->NetAddr.From, pNetCtrl) );
				}
				else if( pNetCtrl->msgID.ID == PACKET_NETCTRL_ACK )
				{
					// ignore this packet
				}
				else
				{
					// send disconnect
					netTrace( Trace::TRC_WARN, "HackWarn : Not allowered connection try from {0}", pIOBuffer->NetAddr.From );
				}

			}
			else
			{
				//pConnection = (*iterCon).pConnection;
			}

			if( pConnection != nullptr )
			{
				netChk( pConnection->OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer) );
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

		if (pIOBuffer != nullptr)
		{
			if (NetSystem::IsProactorSystem())
			{
				pIOBuffer->SetPendingFalse();
				if (hrRes != E_NET_IO_ABORTED)
				{
					PendingRecv(pIOBuffer);
				}
			}
			else
			{
				Util::SafeDelete(pIOBuffer);
			}
		}

		return hr;
	}
	
	
	


} // namespace Net
} // namespace BR

