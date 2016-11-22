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
#include "Common/ResultCode/BRResultCodeNet.h"
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
		: INetIOCallBack(m_Socket)
		, m_Socket(INVALID_SOCKET)
		, m_pRecvBuffers(nullptr)
		, m_pMessageHandler(nullptr)
	{
		// we will share the write queue for UDP
		//SetWriteQueue(new WriteBufferQueue);
	}

	RawUDP::~RawUDP()
	{
		//if (GetWriteQueue()) delete GetWriteQueue();
	}

	Result RawUDP::InitializeNet(const NetAddress& localAddress, MessageHandler *pHandler)
	{
		Result hr = ResultCode::SUCCESS;
		SOCKET socket = INVALID_SOCKET;
		INT iOptValue;
		int bOptValue = 0;
		sockaddr_storage bindAddr;
		NetAddress myAddress;

		if (GetSocket() != INVALID_SOCKET)
			return ResultCode::SUCCESS;

		m_pMessageHandler = pHandler;

		netChk(NetSystem::OpenSystem(Const::SVR_OVERBUFFER_COUNT, Const::SVR_NUM_RECV_THREAD, Const::PACKET_GATHER_SIZE_MAX));

		if (localAddress.strAddr[0] == '\0')
		{
			netChk(Net::GetLocalAddressIPv6(myAddress));
		}
		else
		{
			myAddress = localAddress;
		}

		m_LocalAddress = myAddress;
		Addr2SockAddr(m_LocalAddress, m_LocalSockAddress);


		netTrace(Trace::TRC_TRACE, "RawUDP: Opening UDP Net {0}:{1}", m_LocalAddress.strAddr, m_LocalAddress.usPort);

		socket = NetSystem::Socket(m_LocalAddress.SocketFamily, SockType::DataGram);
		if (socket == INVALID_SOCKET)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to Open RawUDP Socket {0:X8}", GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}

		iOptValue = Const::SVR_RECV_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_RCVBUF = {0:X8}, err = {1:X8}", iOptValue, GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}

		iOptValue = Const::SVR_SEND_BUFFER_SIZE;
		if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_SNDBUF = {0}, err = {1:X8}", iOptValue, GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}

		iOptValue = 0;
#if WINDOWS
		INT iOptLen = sizeof(iOptValue);
		if (getsockopt(socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *)&iOptValue, &iOptLen) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to get socket option SO_MAX_MSG_SIZE = {0}, err = {1:X8}", iOptValue, GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}
		if (iOptValue < Const::PACKET_SIZE_MAX)
		{
			netTrace(Trace::TRC_WARN, "RawUDP: Socket max packet size too small, Change to socket maximum SocketMax={0}, SvrMax={1}, err = {2:X8}", iOptValue, (UINT)Const::PACKET_SIZE_MAX, GetLastWSAResult());
			//Const::PACKET_SIZE_MAX = iOptValue;
		}
#endif

		bOptValue = TRUE;
		if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptValue, sizeof(bOptValue)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option SO_REUSEADDR = {0}, err = {1:X8}", bOptValue, GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}

		if (m_LocalAddress.SocketFamily == SockFamily::IPV6)
		{
			iOptValue = FALSE;
			if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&iOptValue, sizeof(iOptValue)) == SOCKET_ERROR)
			{
				netTrace(Trace::TRC_ERROR, "RawUDP: Failed to change socket option IPV6_V6ONLY = {0}, err = {1:X8}", iOptValue, GetLastWSAResult());
				netErr(ResultCode::UNEXPECTED);
			}
		}


		GetAnyBindAddr(m_LocalSockAddress, bindAddr);
		if (bind(socket, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			netTrace(Trace::TRC_ERROR, "RawUDP: Socket bind failed, UDP err={0:X8}", GetLastWSAResult());
			netErr(ResultCode::UNEXPECTED);
		}
		m_LocalSockAddress = bindAddr;

		m_Socket = socket;
		socket = INVALID_SOCKET;


		netChk(NetSystem::RegisterSocket(SockType::DataGram, this));


		// Ready recv
		if (NetSystem::IsProactorSystem())
		{
			if (m_pRecvBuffers) delete[] m_pRecvBuffers;
			netMem(m_pRecvBuffers = new IOBUFFER_READ[Const::SVR_NUM_RECV_THREAD]);

			for (INT uiRecv = 0; uiRecv < Const::SVR_NUM_RECV_THREAD; uiRecv++)
			{
				PendingRecv(&m_pRecvBuffers[uiRecv]);
			}
		}


	Proc_End:

		if (!(hr))
			TerminateNet();

		if (socket != INVALID_SOCKET)
			NetSystem::CloseSocket(socket);

		netTrace(TRC_NET, "RawUDP: Opened {0}, hr={1:X8}", m_LocalAddress, hr);

		return hr;
	}

	Result RawUDP::TerminateNet()
	{
		Result hr = ResultCode::SUCCESS;

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

	Result RawUDP::PendingRecv(IOBUFFER_READ *pOver)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

		if (!NetSystem::IsProactorSystem())
			return ResultCode::SUCCESS;

		netChk(pOver->SetPendingTrue());
		pOver->SetupRecvUDP(0);

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


	Result RawUDP::SendBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

		hrErr = NetSystem::SendTo(GetSocket(), pSendBuffer);
		switch ((int32_t)hrErr)
		{
		case ResultCode::E_NET_TRY_AGAIN:
			break;
		case ResultCode::SUCCESS:
		case ResultCode::E_NET_IO_PENDING:
		case ResultCode::E_NET_WOULDBLOCK:
			break;
		case ResultCode::E_NET_CONNABORTED:
		case ResultCode::E_NET_CONNRESET:
		case ResultCode::E_NET_NETRESET:
		case ResultCode::E_NET_NOTCONN:
		case ResultCode::E_NET_NOTSOCK:
		case ResultCode::E_NET_SHUTDOWN:
			// Send fail by connection close
			// Need to disconnect
			hr = ResultCode::E_NET_CONNECTION_CLOSED;
			goto Proc_End;
			break;
		default:
			netErr(ResultCode::E_NET_IO_SEND_FAIL);
			break;
		};

	Proc_End:

		if (!(hr))
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
	Result RawUDP::SendMsg(const sockaddr_storage& dest, Message::MessageData *pMsg)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

		Message::MessageID msgID = pMsg->GetMessageHeader()->msgID;
		IOBUFFER_WRITE *pOverlapped = nullptr;


		netChk(Net::NetSystem::AllocBuffer(pOverlapped));
		pOverlapped->SetupSendUDP(m_Socket, dest, pMsg);
		pMsg = nullptr;

		if (NetSystem::IsProactorSystem())
		{
			netChk(SendBuffer(pOverlapped));
		}
		else
		{
			netChk(EnqueueBuffer(pOverlapped));
		}

	Proc_End:

		if (!(hr))
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

			if (hr != Result(ResultCode::E_NET_IO_SEND_FAIL))
			{
				netTrace(Trace::TRC_ERROR, "RawUDP Send Failed, err:{1:X8}, hr:{2:X8}", hrErr, hr);
			}
			else
			{
				netTrace(Net::TRC_SENDRAW, "RawUDP Send Failed, err:{1:X8}, hr:{2:X8}", hrErr, hr);
				return ResultCode::SUCCESS;
			}
		}
		else
		{
			if (msgID.IDs.Type == Message::MSGTYPE_NETCONTROL)
			{
				netTrace(TRC_RAW, "RawUDP SendCtrl dest:{0}, msg:{1}", dest, msgID);
			}
			else
			{
				netTrace(TRC_RAW, "RawUDP Send dest:{0}, msg:{1}", dest, msgID);
			}
		}

		return hr;
	}


	// called when incomming message occure
	Result RawUDP::OnRecv(const sockaddr_storage& remoteAddr, UINT uiBuffSize, const BYTE* pBuff)
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageData *pMsg = nullptr;
		Message::MobileMessageHeader * pMsgHeader = (Message::MobileMessageHeader*)pBuff;

		//netTrace(TRC_RECVRAW, "UDP Recv ip:{0}, msg:{1}, seq:{2}, len:{3}", GetConnectionInfo().Remote, pMsgHeader->msgID, pMsgHeader->msgID.IDSeq.Sequence, uiBuffSize);

		if (uiBuffSize == 0)
			goto Proc_End;

		while (uiBuffSize)
		{
			pMsgHeader = (Message::MobileMessageHeader*)pBuff;
			if (uiBuffSize < sizeof(Message::MobileMessageHeader) || uiBuffSize < pMsgHeader->Length)
			{
				netTrace(Trace::TRC_ERROR, "Unexpected packet buffer size:{0}, size in header:{1}", uiBuffSize, pMsgHeader->Length);
				netErr(ResultCode::E_NET_BADPACKET_SIZE);
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

	Result RawUDP::OnSendReady()
	{
		return ProcessSendQueue();
	}

	Result RawUDP::Recv(IOBUFFER_READ* pIOBuffer)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

		netChkPtr(pIOBuffer);

		pIOBuffer->SetupRecvUDP(0);

		hrErr = NetSystem::RecvFrom(GetSocket(), pIOBuffer);
		switch ((int32_t)hrErr)
		{
		case ResultCode::SUCCESS_FALSE:
			hr = ResultCode::E_NET_TRY_AGAIN;
			break;
		case ResultCode::SUCCESS:
		case ResultCode::E_NET_IO_PENDING:
		case ResultCode::E_NET_TRY_AGAIN:
		case ResultCode::E_NET_WOULDBLOCK:
			hr = hrErr;
			goto Proc_End;// success, but nothing to read
			break;
		case ResultCode::E_NET_NETUNREACH:
		case ResultCode::E_NET_CONNABORTED:
		case ResultCode::E_NET_CONNRESET:
		case ResultCode::E_NET_NETRESET:
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
	Result RawUDP::OnIORecvCompleted(Result hrRes, IOBUFFER_READ* &pIOBuffer)
	{
		Result hr = ResultCode::SUCCESS;
		sockaddr_storage from;
		if (pIOBuffer != nullptr) from = pIOBuffer->NetAddr.From;
		else memset(&from, 0, sizeof(from));

		if (!(hrRes))
		{
			switch ((int32_t)hrRes)
			{
			case ResultCode::E_NET_CONNECTION_CLOSED:
			case ResultCode::E_NET_IO_ABORTED:
				hr = hrRes;
				break;
			default:
				netTrace(Trace::TRC_ERROR, "UDP Recv Msg Failed, RawUDP, IP:{0}, hr={1:X8}", from, hrRes);
				break;
			};
		}
		else if(pIOBuffer != nullptr)
		{
			if (pIOBuffer->TransferredSize < sizeof(Message::MessageHeader))// invalid packet size
				goto Proc_End;

			OnRecv(pIOBuffer->NetAddr.From, pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer);
		}

	Proc_End:

		if (NetSystem::IsProactorSystem())
		{
			Assert(pIOBuffer->bIsPending);
			pIOBuffer->bIsPending.store(false, std::memory_order_relaxed);
			if (hrRes != Result(ResultCode::E_NET_IO_ABORTED) && pIOBuffer != nullptr)
				PendingRecv(pIOBuffer);
		}
		else
		{
			Util::SafeDelete(pIOBuffer);
		}

		return hr;
	}

	// called when send completed
	Result RawUDP::OnIOSendCompleted(Result hrRes, IOBUFFER_WRITE *pIOBuffer)
	{
		Util::SafeDeleteArray(pIOBuffer->pSendBuff);
		Util::SafeRelease(pIOBuffer->pMsgs);
		NetSystem::FreeBuffer(pIOBuffer);
		return ResultCode::SUCCESS;
	}


} // namespace Net
} // namespace BR

