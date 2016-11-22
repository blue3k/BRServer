////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network Server Peer implementation 
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Net/Connection.h"
#include "Net/NetTrace.h"
#include "Net/NetDef.h"
#include "Net/NetSystem.h"
#include "Net/NetServer.h"
#include "Net/NetServerPeer.h"
#include "Net/NetConst.h"
#include "Protocol/ProtocolVer.h"
#include "Net/NetCtrl.h"


namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Peer network class
	//

	ServerPeer::ServerPeer( ServerID InServerID, NetClass localClass )
		: ServerNet( InServerID, localClass )
		, INetIOCallBack(GetSocket())
		, m_ConnectionManager(Net::Const::SVR_PRIVATE_CONNECTION_BUCKET_SIZE)
		, m_CIDGen(0)
		, m_pRecvBuffers(nullptr)
		, m_PendingRecvCnt(0)
	{
		m_ConnectionManager.SetNetOwner( this );

		if (!NetSystem::IsProactorSystem())
			SetWriteQueue(new WriteBufferQueue);
	}

	ServerPeer::~ServerPeer()
	{
		HostClose();

		Util::SafeDeleteArray( m_pRecvBuffers );
		if (GetWriteQueue()) delete GetWriteQueue();
	}


	
	// Close all connection
	Result ServerPeer::CloseAllConnection()
	{
		return m_ConnectionManager.PendingCloseAllConnection();
	}


	// Get Active(non-free) connection
	CounterType ServerPeer::GetActiveConnectionCount()
	{
		return m_ConnectionManager.GetNumActiveConnection();
	}


	// called when reciving message
	Result ServerPeer::OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer )
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<Connection> pConnection;

		sockaddr_storage from;
		if (pIOBuffer != nullptr) from = pIOBuffer->NetAddr.From;
		else memset(&from, 0, sizeof(from));



		if( !( hrRes ) )
		{
			switch((int32_t)hrRes )
			{
			case ResultCode::E_NET_CONNECTION_CLOSED:
			case ResultCode::E_NET_IO_ABORTED:
				if ((m_ConnectionManager.GetConnectionByAddr(from, pConnection)))
				{
					// Release connection table
					if (pConnection->GetConnectionState() != IConnection::STATE_DISCONNECTED)
						pConnection->CloseConnection();
				}
				hr = hrRes;
				break;
			default:
				//netTrace( TRC_RECV, "UDP Recv Msg Failed, SvrUDP, IP:{0}, hr={1:X8}", addr, hrRes );
				break;
			};
		}


		if(pIOBuffer == nullptr || pIOBuffer->TransferredSize == 0)
			goto Proc_End;

		//Assert(pIOBuffer->bIsPending == true);
		//pIOBuffer->bIsPending = false;
		m_PendingRecvCnt.fetch_sub(1, std::memory_order_relaxed);


		if (!(m_ConnectionManager.GetConnectionByAddr(from, pConnection))) // not mapped yet. We need to make a new connection
		{
			// check control packet
			MsgNetCtrlConnect *pNetCtrl = (MsgNetCtrlConnect*)pIOBuffer->buffer;
			if (pNetCtrl->Length == sizeof(MsgNetCtrlConnect) && pNetCtrl->msgID.IDSeq.MsgID == PACKET_NETCTRL_CONNECT.IDSeq.MsgID && pNetCtrl->rtnMsgID.ID == BR_PROTOCOL_VERSION)
			{
				if( GetIsEnableAccept() )
				{
					// Peer network only allow registered connection
					netChk( m_ConnectionManager.PendingNewConnection(from, pNetCtrl) );
				}
			}
			else
			{
				netTrace( Trace::TRC_WARN, "HackWarn : Invalid packet From {0}", from);
				netErr( ResultCode::UNEXPECTED );
			}

		}
		else
		{
			if(pIOBuffer->TransferredSize >= sizeof(Message::MessageHeader) )// invalid packet size
			{
				if (pConnection->GetConnectionState() == IConnection::STATE_DISCONNECTED)
				{
					MsgNetCtrlConnect *pNetCtrl = (MsgNetCtrlConnect*)pIOBuffer->buffer;
					if( pNetCtrl->Length == sizeof(MsgNetCtrlConnect) 
						&& pNetCtrl->msgID.IDSeq.MsgID == PACKET_NETCTRL_CONNECT.IDSeq.MsgID
						&& pNetCtrl->rtnMsgID.ID == BR_PROTOCOL_VERSION )
					{
						// Peer network only allow registered connection
						netChk( m_ConnectionManager.PendingInitConnection((Connection*)pConnection) );
					}
					else
					{
						netTrace( Trace::TRC_WARN, "Unexpected packet From {0}", from);
						netErr( ResultCode::UNEXPECTED );
					}
				}
				else
				{
					netChk( pConnection->OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer) );
				}
			}
		}


	Proc_End:

		// Release connection table
		//iterCon = nullptr;

		if (NetSystem::IsProactorSystem())
		{
			pIOBuffer->SetPendingFalse();

			if (hrRes != Result(ResultCode::E_NET_IO_ABORTED))
			{
				PendingRecv(pIOBuffer);
			}
			else
			{
				netTrace(Trace::TRC_ERROR, "NoPending {0}", from);
			}
		}
		else
		{
			Util::SafeDelete(pIOBuffer);
		}


		return hr;
	}


	Result ServerPeer::OnSendReady()
	{
		return ProcessSendQueue();
	}

	// called when send completed
	Result ServerPeer::OnIOSendCompleted( Result hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		NetSystem::FreeGatheringBuffer( pIOBuffer->pSendBuff );
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		return ResultCode::SUCCESS;
	}


	// Release instance
	void ServerPeer::Release()
	{
		delete this;
	}

	// check about initialize
	bool ServerPeer::IsReady()
	{
		return GetSocket() != INVALID_SOCKET;
	}

	// Get connection from connection ID
	Result ServerPeer::GetConnection(UINT uiCID, SharedPointerT<Connection> &pIConnection)
	{
		return m_ConnectionManager.GetConnectionByCID(uiCID, pIConnection);
	}



	//// Send message to connection with network device
	//Result ServerPeer::SendMsg( IConnection *pConnection, Message::MessageData *pMsg )
	//{
	//	Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

	//	Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
	//	UINT uiMsgLen = pMsg->GetMessageHeader()->Length;
	//	ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;
	//	IOBUFFER_WRITE *pOverlapped = nullptr;

	//	Assert(uiMsgLen == 0 || pMsg->GetMessageHeader()->Crc32 != 0);


	//	netChk( Net::NetSystem::AllocBuffer(pOverlapped) );
	//	pOverlapped->SetupSendUDP(pUDPCon->GetSocket(), pUDPCon->GetRemoteSockAddr(), pMsg );
	//	hrErr = NetSystem::SendTo(pUDPCon->GetSocket(), pOverlapped);
	//	switch (hrErr)
	//	{
	//	case ResultCode::SUCCESS:
	//	case ResultCode::E_NET_IO_PENDING:
	//	case ResultCode::E_NET_TRY_AGAIN:
	//	case ResultCode::E_NET_WOULDBLOCK:
	//		break;
	//	case ResultCode::E_NET_CONNABORTED:
	//	case ResultCode::E_NET_CONNRESET:
	//	case ResultCode::E_NET_NETRESET:
	//	case ResultCode::E_NET_NOTCONN:
	//	case ResultCode::E_NET_NOTSOCK:
	//	case ResultCode::E_NET_SHUTDOWN:
	//		// Send fail by connection close
	//		// Need to disconnect
	//		pUDPCon->Disconnect();
	//		hr = ResultCode::E_NET_CONNECTION_CLOSED;
	//		goto Proc_End;
	//		break;
	//	default:
	//		netErr(ResultCode::E_NET_IO_SEND_FAIL);
	//		break;
	//	};

	//Proc_End:

	//	if( !(hr) )
	//	{
	//		if( pOverlapped )
	//		{
	//			Util::SafeRelease( pOverlapped->pMsgs );
	//			Net::NetSystem::FreeBuffer(pOverlapped);
	//		}
	//		else
	//		{
	//			Util::SafeRelease( pMsg );
	//		}

	//		if( hr != ResultCode::E_NET_IO_SEND_FAIL )
	//		{
	//			netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pUDPCon->GetConnectionInfo().Remote, hrErr, hr );
	//		}
	//		else
	//			return ResultCode::SUCCESS;
	//	}
	//	else
	//	{
	//		if( msgID.IDs.Type == Message::MSGTYPE_NETCONTROL )
	//		{
	//			netTrace( TRC_NETCTRL, "UDP SendCtrl ip:{0}, msg:{1}, Len:{2}", pUDPCon->GetConnectionInfo().Remote, msgID, uiMsgLen );
	//		}
	//		else
	//		{
	//			netTrace( TRC_SENDRAW, "UDP Send ip:{0}, msg:{1}, Len:{2}", pUDPCon->GetConnectionInfo().Remote, msgID, uiMsgLen );
	//		}
	//	}

	//	return hr;
	//}

	//// Send message to connection with network device to dst addr
	//Result ServerPeer::SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg )
	//{
	//	return ResultCode::NOT_IMPLEMENTED;
	//}

	//Result ServerPeer::SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff )
	//{
	//	Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

	//	ConnectionUDP *pUDPCon = (ConnectionUDP*)pConnection;

	//	IOBUFFER_WRITE *pOverlapped = nullptr;
	//	netChk( Net::NetSystem::AllocBuffer(pOverlapped) );

	//	pOverlapped->SetupSendUDP(pUDPCon->GetSocket(), pUDPCon->GetRemoteSockAddr(), uiBuffSize, pBuff );


	//	hrErr = NetSystem::SendTo(pUDPCon->GetSocket(), pOverlapped);
	//	switch (hrErr)
	//	{
	//	case ResultCode::SUCCESS:
	//	case ResultCode::E_NET_IO_PENDING:
	//	case ResultCode::E_NET_TRY_AGAIN:
	//	case ResultCode::E_NET_WOULDBLOCK:
	//		break;
	//	case ResultCode::E_NET_CONNABORTED:
	//	case ResultCode::E_NET_CONNRESET:
	//	case ResultCode::E_NET_NETRESET:
	//	case ResultCode::E_NET_NOTCONN:
	//	case ResultCode::E_NET_NOTSOCK:
	//	case ResultCode::E_NET_SHUTDOWN:
	//		// Send fail by connection close
	//		// Need to disconnect
	//		pUDPCon->Disconnect();
	//		hr = ResultCode::E_NET_CONNECTION_CLOSED;
	//		goto Proc_End;
	//		break;
	//	default:
	//		netErr(ResultCode::E_NET_IO_SEND_FAIL);
	//		break;
	//	};

	//Proc_End:

	//	if( !(hr) )
	//	{
	//		Util::SafeDelete( pBuff );
	//		if( pOverlapped )
	//		{
	//			pOverlapped->pSendBuff = nullptr;
	//			Net::NetSystem::FreeBuffer(pOverlapped);
	//		}

	//		if( hr != ResultCode::E_NET_IO_SEND_FAIL )
	//		{
	//			netTrace( Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", pUDPCon->GetConnectionInfo().Remote, hrErr, hr );
	//		}
	//		else
	//			return ResultCode::SUCCESS;
	//	}
	//	else
	//	{
	//		netTrace( TRC_SENDRAW, "UDP Send ip:{0}, Len:{1}", pUDPCon->GetConnectionInfo().Remote, uiBuffSize );
	//	}

	//	return hr;
	//}

	// Pending recv New one
	Result ServerPeer::PendingRecv( IOBUFFER_READ *pOver )
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;
		//int iErr = 0;//, iSockLen = sizeof(sockaddr_in6);

		if (!NetSystem::IsProactorSystem())
			return ResultCode::SUCCESS;

		netChk(pOver->SetPendingTrue());
		pOver->SetupRecvUDP(0);

		//Assert(pOver->bIsPending == false);
		//pOver->bIsPending = true;
		m_PendingRecvCnt.fetch_add(1, std::memory_order_relaxed);


		while (1)
		{
			hrErr = NetSystem::RecvFrom(GetSocket(), pOver);
			switch ((int32_t)hrErr)
			{
			case ResultCode::SUCCESS_FALSE:
				hr = ResultCode::E_NET_TRY_AGAIN;
				goto Proc_End;// success
				break;
			case ResultCode::SUCCESS:
			case ResultCode::E_NET_IO_PENDING:
			case ResultCode::E_NET_TRY_AGAIN:
			case ResultCode::E_NET_WOULDBLOCK:
				hr = hrErr;
				goto Proc_End;// success
				break;
			case ResultCode::E_NET_NETUNREACH:
			case ResultCode::E_NET_CONNABORTED:
			case ResultCode::E_NET_CONNRESET:
			case ResultCode::E_NET_NETRESET:
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


	// Open host and start listen
	Result ServerPeer::ServerHostOpen(NetClass netCls, const char *strLocalIP, USHORT usLocalPort)
	{
		Result hr = ResultCode::SUCCESS;
		NetAddress localAddr;

		netChk(NetSystem::OpenSystem(Const::SVR_OVERBUFFER_COUNT, Const::SVR_NUM_RECV_THREAD, Const::PACKET_GATHER_SIZE_MAX));

		m_NetClass = netCls;

		netChk(SetLocalNetAddress(localAddr, strLocalIP, usLocalPort));

		SetLocalAddress(localAddr);

	Proc_End:


		return hr;
	}


	// Open host and start listen
	Result ServerPeer::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		Result hr = ResultCode::SUCCESS;
		SOCKET socket = INVALID_SOCKET;
		INT iOptValue;
		sockaddr_storage bindAddr;
		INet::Event netEvent(INet::Event::EVT_NET_INITIALIZED);


		if( GetSocket() != INVALID_SOCKET )
			return ResultCode::SUCCESS;

		netTrace(Trace::TRC_TRACE, "Opening Server Peer, {0}:{1}", strLocalIP, usLocalPort);

		netChk(ServerHostOpen(netCls, strLocalIP, usLocalPort));

		netTrace(Trace::TRC_TRACE, "Open Server Peer Host {0}:{1}", strLocalIP, usLocalPort );

		socket = NetSystem::Socket(GetLocalAddress().SocketFamily, SockType::DataGram);
		if( socket == INVALID_SOCKET )
		{
			netTrace(Trace::TRC_ERROR, "Failed to Open Server Socket {0:X8}", GetLastWSAResult());
			netErr( ResultCode::UNEXPECTED );
		}

		iOptValue = Const::CLI_RECV_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_RCVBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAResult() );
			netErr( ResultCode::UNEXPECTED );
		}

		iOptValue = Const::CLI_SEND_BUFFER_SIZE;
		if( setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR )
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAResult() );
			netErr( ResultCode::UNEXPECTED );
		}

		iOptValue = FALSE;
		if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}


		GetAnyBindAddr(GetSocketAddr(), bindAddr);
		if (bind(socket, (sockaddr*)&bindAddr, GetSocketAddrSize()) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "Socket bind failed, UDP err={0:X8}", GetLastWSAResult() );
			netErr( ResultCode::UNEXPECTED );
		}

		SetSocket( socket );

		netChk(NetSystem::RegisterSocket(SockType::DataGram, this));

		m_NetClass = netCls;

		netChk( m_ConnectionManager.InitManager( Const::SVR_PRIVATE_CONNECTION_POOLCACHE ) );


		if (NetSystem::IsProactorSystem())
		{
			// Pending recv
			Util::SafeDeleteArray(m_pRecvBuffers);
			netMem(m_pRecvBuffers = new IOBUFFER_READ[Const::SVR_NUM_RECV_THREAD]);
			memset(m_pRecvBuffers, 0, sizeof(IOBUFFER_READ)*Const::SVR_NUM_RECV_THREAD);

			m_PendingRecvCnt = 0;

			for (INT iRecv = 0; iRecv < Const::SVR_NUM_RECV_THREAD; iRecv++)
				netChk(PendingRecv(&m_pRecvBuffers[iRecv]));
		}


		EnqueueNetEvent(netEvent);


	Proc_End:

		if( !(hr) )
			HostClose();

		netTrace( TRC_NET, "HostOpen {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}

	// Close host and close all connections
	Result ServerPeer::HostClose()
	{
		Result hr = ResultCode::SUCCESS;
		INet::Event myEvent(INet::Event::EVT_NET_CLOSED);

		m_ConnectionManager.Stop( true );


		if( GetSocket() != INVALID_SOCKET )
		{
			NetSystem::CloseSocket( GetSocket() );
			SetSocket( INVALID_SOCKET );

			NetSystem::CloseSystem();
		}

		EnqueueNetEvent(myEvent);


	//Proc_End:

		netTrace( TRC_NET, "HostClose {0}, hr={1:X8}", GetLocalAddress(), hr );

		return hr;
	}



	// Connect to other peer
	Result ServerPeer::RegisterServerConnection( ServerID serverID, NetClass netClass, const NetAddress& destAddress, Net::IConnection* &pConnection )
	{
		Result hr = ResultCode::SUCCESS;
		Net::IConnection::ConnectionInformation connectionInfo;
		ConnectionUDPServerPeer *pConn = nullptr;
		uintptr_t CID = 0;


		netMem( pConn = dynamic_cast<ConnectionUDPServerPeer*>(m_ConnectionManager.NewConnection()) );

		memset( &connectionInfo, 0, sizeof(connectionInfo) );

		connectionInfo.SetLocalInfo( GetNetClass(), GetLocalAddress(), GetServerID() );

		connectionInfo.Remote = destAddress;
		connectionInfo.RemoteClass = netClass;
		connectionInfo.RemoteID = serverID;

		pConn->SetHeartbitTry( Const::SVR_HEARTBIT_TIME_PEER );
		pConn->SetConnectingTimeOut( LONG_MAX ); // infinite retry

		netChk( pConn->SetMessageWindowSize( 256, 256 ) );
		netChk( pConn->InitConnection( GetSocket(), connectionInfo ) );
		netTrace(TRC_CONNECTION, "Initialize connection CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);

		pConn->SetWriteQueueUDP(GetWriteQueue());

		// Server entity will update this connection, thus just adding address map is enough
		netChk( m_ConnectionManager.PendingConnection( pConn ) );

		CID = pConn->GetCID();

		pConnection = pConn;
		pConn = nullptr;

	Proc_End:

		if( pConn ) delete pConn;

		if( (hr) )
		{
			netTrace(TRC_NET, "ServerPeer Allowing Server:{2}:{3}, {0}, CID:{1}", destAddress, CID, netClass, serverID);
		}

		return hr;
	}



} // namespace Net	
} // namespace BR

