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

	}

	RawUDP::~RawUDP()
	{

	}

	HRESULT RawUDP::InitializeNet(const NetAddress& localAddress, MessageHandler *pHandler)
	{
		HRESULT hr = S_OK;
		SOCKET socket = INVALID_SOCKET;
		INT iOptValue, iOptLen;
		BOOL bOptValue = 0;
		sockaddr_in6 bindAddr;
		NetAddress myAddress;

		if (GetSocket() != INVALID_SOCKET)
			return S_OK;

		m_pMessageHandler = pHandler;

		netChk(WSASystem::OpenSystem(Const::SVR_OVERBUFFER_COUNT));

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

		netChk(IOCPSystem::GetSystem().InitIOCP(Const::SVR_NUM_RECV_THREAD));


		netTrace(Trace::TRC_TRACE, "RawUDP: Opening UDP Net %0%:%1%", m_LocalAddress.strAddr, m_LocalAddress.usPort);

		socket = WSASocket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		if (socket == INVALID_SOCKET)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to Open RawUDP Socket %0%", WSAGetLastError());
			netErr(E_UNEXPECTED);
		}

		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_RCVBUF = %0%, err = %1%", iOptValue, WSAGetLastError());
			netErr(E_UNEXPECTED);
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_SNDBUF = %0%, err = %1%", iOptValue, WSAGetLastError());
			netErr(E_UNEXPECTED);
		}

		iOptValue = 0;
		iOptLen = sizeof(iOptValue);
		if (getsockopt(socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *)&iOptValue, &iOptLen) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to get socket option SO_MAX_MSG_SIZE = %0%, err = %1%", iOptValue, WSAGetLastError());
			netErr(E_UNEXPECTED);
		}
		if (iOptValue < Const::PACKET_SIZE_MAX)
		{
			netTrace(Trace::TRC_WARN, "RawUDP: Socket max packet size too small, Change to socket maximum SocketMax=%0%, SvrMax=%1%, err = %2%", iOptValue, (UINT)Const::PACKET_SIZE_MAX, WSAGetLastError());
			//Const::PACKET_SIZE_MAX = iOptValue;
		}

		bOptValue = TRUE;
		if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_REUSEADDR = %0%, err = %1%", bOptValue, WSAGetLastError());
			netErr(E_UNEXPECTED);
		}

		iOptValue = FALSE;
		if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option IPV6_V6ONLY = %0%, err = %1%", iOptValue, WSAGetLastError());
			netErr(E_UNEXPECTED);
		}


		bindAddr = m_LocalSockAddress;
		bindAddr.sin6_family = AF_INET6;
		bindAddr.sin6_addr = in6addr_any;
		if (bind(socket, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Socket bind failed, UDP err=%0%", WSAGetLastError());
			netErr(E_UNEXPECTED);
		}
		m_LocalSockAddress = bindAddr;


		if (!CreateIoCompletionPort((HANDLE)socket, IOCPSystem::GetSystem().GetIOCP(), (ULONG_PTR)(IOCPSystem::IOCallBack*)this, 0))
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: CreateIoCompletionPort Failed Udp, hr = %0%", GetLastHRESULT());
			netErr(E_UNEXPECTED);
		}


		m_Socket = socket;
		socket = INVALID_SOCKET;


		// Ready recv
		if (m_pRecvBuffers) delete[] m_pRecvBuffers;
		netMem(m_pRecvBuffers = new OVERLAPPED_BUFFER_READ[Const::SVR_NUM_RECV_THREAD]);

		for (INT uiRecv = 0; uiRecv < Const::SVR_NUM_RECV_THREAD; uiRecv++)
		{
			netChk(PendingRecv(&m_pRecvBuffers[uiRecv]));
		}


	Proc_End:

		if (FAILED(hr))
			TerminateNet();

		if (socket != INVALID_SOCKET)
			closesocket(socket);

		netTrace(TRC_NET, "RawUDP: Opened %0%, hr=%1%", m_LocalAddress, ArgHex32(hr));

		return hr;
	}

	HRESULT RawUDP::TerminateNet()
	{
		HRESULT hr = S_OK;

		if (GetSocket() != INVALID_SOCKET)
		{
			closesocket(GetSocket());
			m_Socket = INVALID_SOCKET;
		}

		netChk(IOCPSystem::GetSystem().CloseIOCP());

		WSASystem::CloseSystem();


	Proc_End:

		netTrace(TRC_NET, "RawUDP Close %0%, hr=%1%", m_LocalAddress, ArgHex32(hr));

		return hr;
	}

	HRESULT RawUDP::PendingRecv(OVERLAPPED_BUFFER_READ *pOver)
	{
		HRESULT hr = S_OK;
		INT iErr = 0, iErr2 = 0;


		pOver->SetupRecvUDP(0);

		while ((iErr = WSARecvFrom(GetSocket(), &pOver->wsaBuff, 1, NULL, &pOver->dwFlags, (sockaddr*)&pOver->From, &pOver->iSockLen, pOver, nullptr)) == SOCKET_ERROR)
		{
			iErr2 = WSAGetLastError();
			switch (iErr2)
			{
			case WSA_IO_PENDING:
				goto Proc_End;// success
				break;
			case WSAENETUNREACH:
			case WSAECONNABORTED:
			case WSAECONNRESET:
			case WSAENETRESET:
				// some remove has problem with continue connection
				netTrace(TRC_NETCTRL, "UDP Remote has connection error err=%0%, %1%", iErr2, pOver->From);
				//break;
			default:
				// Unknown error
				netTrace(TRC_RECVRAW, "UDP Read Pending failed err=%0%, hr=%1%", iErr2, ArgHex32(HRESULT_FROM_WIN32(iErr2)));
				//netErr( HRESULT_FROM_WIN32(iErr2) );
				break;
			};
		}

	Proc_End:

		return hr;
	}

	// Send message to connection with network device
	HRESULT RawUDP::SendMsg(const sockaddr_in6& dest, Message::MessageData *pMsg)
	{
		HRESULT hr = S_OK;
		int iWSAErr = 0;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		UINT uiMsgLen = pMsg->GetMessageHeader()->Length;
		OVERLAPPED_BUFFER_WRITE *pOverlapped = nullptr;

		UINT length = 0;
		BYTE* pDataPtr = nullptr;
		pMsg->GetLengthNDataPtr(length, pDataPtr);
		//Assert(length == 0 || pMsg->GetMessageHeader()->Crc32 != 0 || pMsg->GetMessageHeader()->msgID.IDs.Policy == POLICY_NONE);


		netChk(Net::WSASystem::AllocBuffer(pOverlapped));

		pOverlapped->SetupSendUDP(pMsg);

		if (WSASendTo(m_Socket, &pOverlapped->wsaBuff, 1, nullptr, 0,
			(sockaddr*)&dest, sizeof(sockaddr_in6),
			pOverlapped, nullptr) == SOCKET_ERROR)
		{
			iWSAErr = WSAGetLastError();
			if (iWSAErr != WSA_IO_PENDING)
			{
				switch (iWSAErr)
				{
				case WSAECONNABORTED:
				case WSAECONNRESET:
				case WSAENETRESET:
				case WSAENOTCONN:
				case WSAENOTSOCK:
				case WSAESHUTDOWN:
					// Send fail by connection close
					// Need to disconnect
					netErrSilent(E_NET_CONNECTION_CLOSED);
					break;
				default:
					netErr(E_NET_IO_SEND_FAIL);
					break;
				};
			}
		}
		else
		{
			// Send done will be handled by IOCP
		}

	Proc_End:

		if (FAILED(hr))
		{
			if (pOverlapped)
			{
				Util::SafeRelease(pOverlapped->pMsgs);
				Net::WSASystem::FreeBuffer(pOverlapped);
			}
			else
			{
				Util::SafeRelease(pMsg);
			}

			if (hr != E_NET_IO_SEND_FAIL)
			{
				netTrace(Trace::TRC_ERROR, "RawUDP Send Failed, err:%1%, hr:%2%", iWSAErr, ArgHex32(hr));
			}
			else
			{
				netTrace(Net::TRC_SENDRAW, "RawUDP Send Failed, err:%1%, hr:%2%", iWSAErr, ArgHex32(hr));
				return S_OK;
			}
		}
		else
		{
			if (msgID.IDs.Type == Message::MSGTYPE_NETCONTROL)
			{
				netTrace(TRC_NETCTRL, "RawUDP SendCtrl ip:%0%, msg:%1%, Len:%2%", msgID, uiMsgLen);
			}
			else
			{
				netTrace(TRC_SENDRAW, "RawUDP Send ip:%0%, msg:%1%, Len:%2%", msgID, uiMsgLen);
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

		//netTrace(TRC_RECVRAW, "UDP Recv ip:%0%, msg:%1%, seq:%2%, len:%3%", GetConnectionInfo().Remote, pMsgHeader->msgID, pMsgHeader->msgID.IDSeq.Sequence, uiBuffSize);

		if (uiBuffSize == 0)
			goto Proc_End;

		while (uiBuffSize)
		{
			pMsgHeader = (Message::MobileMessageHeader*)pBuff;
			if (uiBuffSize < sizeof(Message::MobileMessageHeader) || uiBuffSize < pMsgHeader->Length)
			{
				netTrace(Trace::TRC_ERROR, "Unexpected packet buffer size:%0%, size in header:%1%", uiBuffSize, pMsgHeader->Length);
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

	// called when New connection TCP accepted
	HRESULT RawUDP::OnIOAccept(HRESULT hrRes, OVERLAPPED_BUFFER_ACCEPT *pAcceptInfo)
	{
		return E_NOTIMPL;
	}

	// called when reciving messag is completed
	HRESULT RawUDP::OnIORecvCompleted(HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred)
	{
		HRESULT hr = S_OK;
		bool bReleaseOnFail = false;

		if (FAILED(hrRes))
		{
			switch (hrRes)
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
				hr = hrRes;
				break;
			default:
				netTrace(Trace::TRC_ERROR, "UDP Recv Msg Failed, RawUDP, IP:%0%, hr=%1%", pIOBuffer->From, ArgHex32(hrRes));
				break;
			};
		}
		else
		{
			if (dwTransferred < sizeof(Message::MessageHeader))// invalid packet size
				goto Proc_End;

			OnRecv(pIOBuffer->From, dwTransferred, (BYTE*)pIOBuffer->buffer);
		}

	Proc_End:


		if (hrRes != E_NET_IO_ABORTED)
			PendingRecv((OVERLAPPED_BUFFER_READ*)pIOBuffer);

		return hr;
	}

	// called when send completed
	HRESULT RawUDP::OnIOSendCompleted(HRESULT hrRes, OVERLAPPED_BUFFER_WRITE *pIOBuffer, DWORD dwTransferred)
	{
		Util::SafeDeleteArray(pIOBuffer->pSendBuff);
		Util::SafeRelease(pIOBuffer->pMsgs);
		WSASystem::FreeBuffer(pIOBuffer);
		return S_OK;
	}


} // namespace Net
} // namespace BR

