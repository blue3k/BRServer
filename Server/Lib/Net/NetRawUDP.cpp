////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net Client
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Net/NetTrace.h"
#include "Net/NetRawUDP.h"
#include "Protocol/ProtocolVer.h"



namespace BR {
namespace Net {

	RawUDP::RawUDP()
		: m_Socket(INVALID_SOCKET)
		, m_pRecvBuffers(nullptr)
		, m_pMessageHandler(nullptr)
	{
		// we will share the write queue for UDP
		//SetWriteQueue(new WriteBufferQueue);
	}

	RawUDP::~RawUDP()
	{
		if (GetWriteQueue()) delete GetWriteQueue();
	}

	HRESULT RawUDP::InitializeNet(const NetAddress& localAddress, MessageHandler *pHandler)
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		INT iOptValue;
		int bOptValue = 0;
		sockaddr_in6 bindAddr;
		NetAddress myAddress;

		if (GetSocket() != INVALID_SOCKET)
			return S_OK;

		m_pMessageHandler = pHandler;

		netChk(NetSystem::OpenSystem(Const::SVR_OVERBUFFER_COUNT, Const::SVR_NUM_RECV_THREAD, Const::PACKET_GATHER_SIZE_MAX));

		if (localAddress.strAddr[0] == '\0')
		{
			if (!Net::GetLocalAddressIPv6(myAddress))
				return E_UNEXPECTED;
		}
		else
		{
			myAddress = localAddress;
		}

		m_LocalAddress = myAddress;
		Addr2SockAddr(m_LocalAddress, m_LocalSockAddress);


		netTrace(Trace::TRC_TRACE, "RawUDP: Opening UDP Net {0}:{1}", m_LocalAddress.strAddr, m_LocalAddress.usPort);

		//socket = WSASocket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		socket = NetSystem::Socket(SockFamily::IPV6, SockType::DataGram);
		if (socket == INVALID_SOCKET)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to Open RawUDP Socket {0:X8}", GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_RCVBUF = {0:X8}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		iOptValue = 0;
#if WINDOWS
		INT iOptLen = sizeof(iOptValue);
		if (getsockopt(socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *)&iOptValue, &iOptLen) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to get socket option SO_MAX_MSG_SIZE = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}
		if (iOptValue < Const::PACKET_SIZE_MAX)
		{
			netTrace(Trace::TRC_WARN, "RawUDP: Socket max packet size too small, Change to socket maximum SocketMax={0}, SvrMax={1}, err = {2:X8}", iOptValue, (UINT)Const::PACKET_SIZE_MAX, GetLastWSAHRESULT());
			//Const::PACKET_SIZE_MAX = iOptValue;
		}
#endif

		bOptValue = TRUE;
		if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_REUSEADDR = {0}, err = {1:X8}", bOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}

		iOptValue = FALSE;
		if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}


		bindAddr = m_LocalSockAddress;
		bindAddr.sin6_family = AF_INET6;
		bindAddr.sin6_addr = in6addr_any;
		if (bind(socket, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Socket bind failed, UDP err={0:X8}", GetLastWSAHRESULT());
			netErr(E_UNEXPECTED);
		}
		m_LocalSockAddress = bindAddr;


		netChk(NetSystem::RegisterSocket(socket, SockType::DataGram, this, false));

		m_Socket = socket;
		socket = INVALID_SOCKET;


		// Ready recv
		if (m_pRecvBuffers) delete[] m_pRecvBuffers;
		netMem(m_pRecvBuffers = new IOBUFFER_READ[Const::SVR_NUM_RECV_THREAD]);

		for (INT uiRecv = 0; uiRecv < Const::SVR_NUM_RECV_THREAD; uiRecv++)
		{
			netChk(PendingRecv(&m_pRecvBuffers[uiRecv]));
		}


	Proc_End:

		if (FAILED(hr))
			TerminateNet();

		if (socket != INVALID_SOCKET)
			NetSystem::CloseSocket(socket);

		netTrace(TRC_NET, "RawUDP: Opened {0}, hr={1:X8}", m_LocalAddress, hr);

		return hr;
	}

	HRESULT RawUDP::TerminateNet()
	{
		HRESULT hr = S_OK;

		if (GetSocket() != INVALID_SOCKET)
		{
			NetSystem::CloseSocket(GetSocket());
			m_Socket = INVALID_SOCKET;
		}

		NetSystem::CloseSystem();


	//Proc_End:

		netTrace(TRC_NET, "RawUDP Close {0}, hr={1}", m_LocalAddress, hr);

		return hr;
	}

	HRESULT RawUDP::PendingRecv(IOBUFFER_READ *pOver)
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		if (!NetSystem::IsProactorSystem())
			return S_OK;


		pOver->SetupRecvUDP(0);

		while (1)
		{
			hrErr = NetSystem::RecvFrom(GetSocket(), pOver);
			switch (hrErr)
			{
			case S_OK:
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


	HRESULT RawUDP::SendBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		HRESULT hr = S_OK, hrErr = S_OK;
		//UINT bufferLen = pSendBuffer->TransferredSize;

		hrErr = NetSystem::SendTo(GetSocket(), pSendBuffer);
		switch (hrErr)
		{
		case E_NET_TRY_AGAIN:
			break;
		case S_OK:
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
		}
		else
		{
			//netTrace(TRC_SENDRAW, "UDP Send ip:{0}, Len:{1}", GetConnectionInfo(), bufferLen);
		}

		return hr;
	}



	// Send message to connection with network device
	HRESULT RawUDP::SendMsg(const sockaddr_in6& dest, Message::MessageData *pMsg)
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		//UINT uiMsgLen = pMsg->GetMessageHeader()->Length;
		IOBUFFER_WRITE *pOverlapped = nullptr;


		netChk(Net::NetSystem::AllocBuffer(pOverlapped));
		pOverlapped->SetupSendUDP(m_Socket, dest, pMsg);
		hrErr = NetSystem::SendTo(m_Socket, pOverlapped);
		switch (hrErr)
		{
		case E_NET_TRY_AGAIN:
			hr = hrErr;
			break;
		case S_OK:
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
			netErrSilent(E_NET_CONNECTION_CLOSED);
			goto Proc_End;
			break;
		default:
			netErr(E_NET_IO_SEND_FAIL);
			break;
		};

	Proc_End:

		if (FAILED(hr))
		{
			if (pOverlapped)
			{
				Util::SafeRelease(pOverlapped->pMsgs);
				Net::NetSystem::FreeBuffer(pOverlapped);
			}
			else
			{
				Util::SafeRelease(pMsg);
			}

			if (hr != E_NET_IO_SEND_FAIL)
			{
				netTrace(Trace::TRC_ERROR, "RawUDP Send Failed, err:{1:X8}, hr:{2:X8}", hrErr, hr);
			}
			else
			{
				netTrace(Net::TRC_SENDRAW, "RawUDP Send Failed, err:{1:X8}, hr:{2:X8}", hrErr, hr);
				return S_OK;
			}
		}
		else
		{
			if (msgID.IDs.Type == Message::MSGTYPE_NETCONTROL)
			{
				//netTrace(TRC_RAW, "RawUDP SendCtrl dest:{0}, msg:{1}, Len:{2}", dest, msgID, uiMsgLen);
			}
			else
			{
				//netTrace(TRC_RAW, "RawUDP Send dest:{0}, msg:{1}, Len:{2}", dest, msgID, uiMsgLen);
			}
		}

		return hr;
	}


	// called when incomming message occure
	HRESULT RawUDP::OnRecv(const sockaddr_in6& remoteAddr, UINT uiBuffSize, const BYTE* pBuff)
	{
		HRESULT hr = S_OK;
		Message::MessageData *pMsg = nullptr;
		Message::MobileMessageHeader * pMsgHeader = (Message::MobileMessageHeader*)pBuff;

		//netTrace(TRC_RECVRAW, "UDP Recv ip:{0}, msg:{1}, seq:{2}, len:%3%", GetConnectionInfo().Remote, pMsgHeader->msgID, pMsgHeader->msgID.IDSeq.Sequence, uiBuffSize);

		if (uiBuffSize == 0)
			goto Proc_End;

		while (uiBuffSize)
		{
			pMsgHeader = (Message::MobileMessageHeader*)pBuff;
			if (uiBuffSize < sizeof(Message::MobileMessageHeader) || uiBuffSize < pMsgHeader->Length)
			{
				netTrace(Trace::TRC_ERROR, "Unexpected packet buffer size:{0}, size in header:{1}", uiBuffSize, pMsgHeader->Length);
				netErr(E_NET_BADPACKET_SIZE);
			}

			netMem(pMsg = Message::MessageData::NewMessage(pMsgHeader->msgID.ID, pMsgHeader->Length, pBuff));

			if (m_pMessageHandler != nullptr)
				m_pMessageHandler->OnRecv(remoteAddr, pMsg);

			pMsg = nullptr;
			netChk(hr);

			uiBuffSize -= pMsgHeader->Length;
			pBuff += pMsgHeader->Length;

			Util::SafeRelease(pMsg);
		}

	Proc_End:

		Util::SafeRelease(pMsg);

		return hr;
	}

	HRESULT RawUDP::Recv(IOBUFFER_READ* pIOBuffer)
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		netChkPtr(pIOBuffer);
		//pIOBuffer = m_pRecvBuffers;
		pIOBuffer->SetupRecvUDP(0);

		hrErr = NetSystem::RecvFrom(GetSocket(), pIOBuffer);
		switch (hrErr)
		{
		case S_OK:
		case E_NET_IO_PENDING:
		case E_NET_TRY_AGAIN:
		case E_NET_WOULDBLOCK:
			goto Proc_End;// success
			break;
		case E_NET_NETUNREACH:
		case E_NET_CONNABORTED:
		case E_NET_CONNRESET:
		case E_NET_NETRESET:
			// some remove has problem with connection
			netTrace(TRC_NETCTRL, "UDP Remote has connection error err={0:X8}, {1}", hrErr, pIOBuffer->NetAddr.From);
		default:
			// Unknown error
			netTrace(Trace::TRC_ERROR, "UDP Read Pending failed err={0:X8}", hrErr);
			netErr(hrErr);
			break;
		}

	Proc_End:

		return hr;
	}

	// called when reciving messag is completed
	HRESULT RawUDP::OnIORecvCompleted(HRESULT hrRes, IOBUFFER_READ *pIOBuffer)
	{
		HRESULT hr = S_OK;

		if (FAILED(hrRes))
		{
			switch (hrRes)
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
				hr = hrRes;
				break;
			default:
				netTrace(Trace::TRC_ERROR, "UDP Recv Msg Failed, RawUDP, IP:{0}, hr={1:X8}", pIOBuffer->NetAddr.From, hrRes);
				break;
			};
		}
		else
		{
			if (pIOBuffer->TransferredSize < sizeof(Message::MessageHeader))// invalid packet size
				goto Proc_End;

			OnRecv(pIOBuffer->NetAddr.From, pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer);
		}

	Proc_End:


		if (hrRes != E_NET_IO_ABORTED)
			PendingRecv((IOBUFFER_READ*)pIOBuffer);

		return hr;
	}

	// called when send completed
	HRESULT RawUDP::OnIOSendCompleted(HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer)
	{
		Util::SafeDeleteArray(pIOBuffer->pSendBuff);
		Util::SafeRelease(pIOBuffer->pMsgs);
		NetSystem::FreeBuffer(pIOBuffer);
		return S_OK;
	}


} // namespace Net
} // namespace BR

