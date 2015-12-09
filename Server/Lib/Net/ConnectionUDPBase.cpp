////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Connection implementations
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common/Thread.h"
#include "Common/BrAssert.h"
#include "Common/TimeUtil.h"
#include "Common/HRESNet.h"
#include "Net/NetTrace.h"
#include "Net/ConnectionUDP.h"
#include "Net/NetDef.h"
#include "Net/NetServer.h"
#include "Net/NetCtrl.h"
#include "Net/NetConst.h"
#include "Net/NetCtrl.h"

#include "Protocol/ProtocolVer.h"


// define if emulate packet loss
//#ifdef _DEBUG
//#define UDP_PACKETLOS_EMULATE
//#endif


#ifdef UDP_PACKETLOS_EMULATE
	#define UDP_PACKETLOS_RATE	5
	#define UDP_PACKETLOS_BOUND	1
#endif // #ifdef UDP_PACKETLOS_EMULATE


namespace BR {
namespace Net {

	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Network connection class
	//


	// Constructor
	ConnectionUDPBase::ConnectionUDPBase( UINT reliableWindowSize )
		: m_RecvReliableWindow()
		, m_SendReliableWindow(reliableWindowSize)
		, m_uiMaxGuarantedRetry(Const::UDP_SVR_RETRY_ONETIME_MAX)
		, m_uiGatheredSize(0)
		, m_pGatheringBuffer(nullptr)
		, m_RecvGuaQueue( reliableWindowSize / 2 )
		, m_SubFrameMessage(nullptr)
		, m_pWriteQueuesUDP(nullptr)
	{
		SetHeartbitTry( Const::UDP_HEARTBIT_START_TIME );
	}

	ConnectionUDPBase::~ConnectionUDPBase()
	{
		m_RecvReliableWindow.ClearWindow();
		m_SendReliableWindow.ClearWindow();

		m_RecvNetCtrlQueue.ClearQueue();

		if(m_pGatheringBuffer != nullptr)
			NetSystem::FreeGatheringBuffer(m_pGatheringBuffer);
		m_pGatheringBuffer = nullptr;

		Util::SafeRelease(m_SubFrameMessage);
	}

	// Set message window size connection
	HRESULT ConnectionUDPBase::SetMessageWindowSize( UINT uiSend, UINT uiRecv )
	{
		if( m_RecvReliableWindow.GetMsgCount() || m_SendReliableWindow.GetMsgCount() )
			return E_FAIL;

		// TODO: Not impl
#ifdef _DEBUG
#else
		//m_SendReliableWindow.SetWndSize( uiSend );
		//m_RecvReliableWindow.SetWndSize( uiRecv );
#endif

		return S_OK;
	}

	// Change remote Address
	void ConnectionUDPBase::ChangeRemoteAddress( const sockaddr_in6& socAddr )
	{
		SockAddr2Addr( socAddr, m_ConnectInfo.Remote );

		m_sockAddrRemote = socAddr;
	}

	ULONG ConnectionUDPBase::UpdateSendQueue()
	{
		HRESULT hr = S_OK;

		if (GetConnectionState() == Net::IConnection::STATE_DISCONNECTED)
			goto Proc_End;

		hr = ProcSendReliableQueue();
		if (FAILED(hr))
		{
			netTrace(TRC_CONNECTION, "Process Send Guaranted queue failed {0:X8}", hr);
		}


	Proc_End:

		SendFlush();

		return hr;
	}

	HRESULT ConnectionUDPBase::ProcGuarrentedMessageWindow(const std::function<void(Message::MessageData* pMsgData)>& action)
	{
		HRESULT hr = S_OK;
		Message::MessageData *pIMsg = nullptr;

		// slide recv window
		while (SUCCEEDED(m_RecvReliableWindow.PopMsg(pIMsg)))
		{
			Assert(pIMsg);
			Message::MessageHeader *pQMsgHeader = pIMsg->GetMessageHeader();
			netTrace(TRC_GUARREANTEDCTRL, "RECVGuaDEQ : CID:{0}:{1}, seq:{2}, msg:{3}, len:%4%",
				GetCID(), m_RecvReliableWindow.GetBaseSequence(),
				pQMsgHeader->msgID.IDSeq.Sequence,
				pQMsgHeader->msgID,
				pQMsgHeader->Length);

			Connection::PrintDebugMessage("RecvMsg", pIMsg);

			if (pIMsg->GetMessageHeader()->msgID.GetMsgID() == PACKET_NETCTRL_SEQUENCE_FRAME.GetMsgID())
			{
				OnFrameSequenceMessage(pIMsg, action);
			}
			else
			{
				action(pIMsg);
			}
			//netChk(Connection::OnRecv(pIMsg));
			pIMsg = nullptr;
		}

	//Proc_End:

		Util::SafeRelease(pIMsg);

		return hr;
	}

	// gathering
	HRESULT ConnectionUDPBase::SendPending( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID )
	{
		HRESULT hr = S_OK;

		MsgNetCtrl *pNetCtrlMsg = NULL;

		netChk( PrepareGatheringBuffer(sizeof(MsgNetCtrlBuffer)) );

		pNetCtrlMsg = (MsgNetCtrl*)(m_pGatheringBuffer+m_uiGatheredSize);
		pNetCtrlMsg->msgID.ID = uiCtrlCode;
		pNetCtrlMsg->msgID.IDSeq.Sequence = uiSequence;
		pNetCtrlMsg->rtnMsgID = msgID;

		if( UID != 0 )
		{
			MsgNetCtrlConnect *pNetCtrlCon = (MsgNetCtrlConnect*)pNetCtrlMsg;
			pNetCtrlCon->PeerUID = UID;
			pNetCtrlCon->Address = GetConnectionInfo().Local;
			pNetCtrlMsg->Length = sizeof(MsgNetCtrlConnect);
		}
		else
		{
			pNetCtrlMsg->Length = sizeof(MsgNetCtrl);
		}
		m_uiGatheredSize += pNetCtrlMsg->Length;


	Proc_End:

		return hr;
	}

	HRESULT ConnectionUDPBase::SendPending( Message::MessageData* pMsg )
	{
		HRESULT hr = S_OK;

		netChkPtr( pMsg );

		if( pMsg->GetMessageSize() > (UINT)Const::INTER_PACKET_SIZE_MAX )
		{
			netErr( E_NET_BADPACKET_TOOBIG );
		}

		if( FAILED(PrepareGatheringBuffer(pMsg->GetMessageSize()) ) )
		{
			return Send(pMsg);
		}

		memcpy( m_pGatheringBuffer+m_uiGatheredSize, pMsg->GetMessageBuff(), pMsg->GetMessageSize() );

		m_uiGatheredSize += pMsg->GetMessageSize();


	Proc_End:

		Util::SafeRelease( pMsg );

		return hr;
	}

	HRESULT ConnectionUDPBase::SendFlush()
	{
		HRESULT hr = S_OK;

		if( m_uiGatheredSize && m_pGatheringBuffer )
		{
			UINT GatherSize = m_uiGatheredSize;
			BYTE* pGatherBuff = m_pGatheringBuffer;

			m_uiGatheredSize = 0;
			m_pGatheringBuffer = nullptr;

			HRESULT hrTem = GetNet()->SendMsg( this, GatherSize, pGatherBuff );
			if( FAILED(hrTem) )
			{
				netTrace( TRC_SENDRAW, "Gathered Send failed : CID:{0}, Len={1}, hr={2:X8}", 
					GetCID(), GatherSize, hrTem );

				// ignore io send fail except connection closed
				if( hrTem == E_NET_CONNECTION_CLOSED )
					netErr( hrTem );
			}
		}


	Proc_End:

		m_uiGatheredSize = 0;
		if( m_pGatheringBuffer != nullptr )
			NetSystem::FreeGatheringBuffer(m_pGatheringBuffer);
		m_pGatheringBuffer = nullptr;


		return hr;
	}

	// Prepare gathering buffer
	HRESULT ConnectionUDPBase::PrepareGatheringBuffer( UINT uiRequiredSize )
	{
		HRESULT hr = S_OK;

		// You tryed to use this method in a wrong way
		if( uiRequiredSize > Const::PACKET_GATHER_SIZE_MAX )
		{
			return E_FAIL;
		}

		if( (m_uiGatheredSize + uiRequiredSize) > (UINT)Const::PACKET_GATHER_SIZE_MAX )
		{
			netChk( SendFlush() );
			Assert(m_uiGatheredSize == 0);
		}

		if( m_pGatheringBuffer == nullptr )
		{
			UINT bufferSize = 0;
			netChk(NetSystem::AllocGatheringBuffer(m_pGatheringBuffer, bufferSize));
			Assert(Const::PACKET_GATHER_SIZE_MAX == bufferSize);
		}

	Proc_End:

		return hr;
	}


	// frame sequence
	HRESULT ConnectionUDPBase::SendFrameSequenceMessage(Message::MessageData* pMsg)
	{
		HRESULT hr = S_OK;
		Message::MessageData* pNewMessageData = nullptr;
		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();
		auto pMsgHeader = pMsg->GetMessageHeader();
		auto remainSize = pMsgHeader->Length;
		auto offset = 0;

		netTrace(TRC_NETCTRL, "SEND Spliting : CID:{0}, seq:{1}, msg:{2}, len:{3}",
			GetCID(),
			pMsg->GetMessageHeader()->msgID.IDSeq.Sequence,
			pMsg->GetMessageHeader()->msgID,
			pMsg->GetMessageHeader()->Length);

		for (UINT iSequence = 0; remainSize > 0; iSequence++)
		{
			auto frameSize = Util::Min(pMsgHeader->Length, (UINT)Message::MAX_SUBFRAME_SIZE);

			MsgMobileNetCtrlSequenceFrame* pCurrentFrame = nullptr;

			svrChkPtr(pNewMessageData = Message::MessageData::NewMessage(PACKET_NETCTRL_SEQUENCE_FRAME, frameSize + sizeof(MsgMobileNetCtrlSequenceFrame), pMsg->GetMessageBuff() + offset));

			pCurrentFrame = (MsgMobileNetCtrlSequenceFrame*)pNewMessageData->GetMessageBuff();
			pCurrentFrame->TotalSize = pMsgHeader->Length;
			pCurrentFrame->SubSequence = iSequence;

			svrChk(m_SendReliableWindow.EnqueueMessage(ulTimeCur, pNewMessageData));
			pNewMessageData->AddRef();// Inc Ref for send
			SendPending(pNewMessageData);
			netTrace(TRC_GUARREANTEDCTRL, "SENDENQReliable : CID:{0}, seq:{1}, msg:{2}, len:{3}",
				GetCID(),
				pNewMessageData->GetMessageHeader()->msgID.IDSeq.Sequence,
				pNewMessageData->GetMessageHeader()->msgID,
				pNewMessageData->GetMessageHeader()->Length);

			pNewMessageData = nullptr;

			remainSize -= frameSize;
			offset += frameSize;
		}

		Util::SafeRelease(pMsg);

	Proc_End:

		Util::SafeRelease(pNewMessageData);

		return hr;
	}

	HRESULT ConnectionUDPBase::OnFrameSequenceMessage(Message::MessageData* pMsg, const std::function<void(Message::MessageData* pMsgData)>& action)
	{
		HRESULT hr = S_OK;
		Message::MessageData* pFrameMessage = nullptr;
		if (pMsg == nullptr)
			return E_POINTER;

		Message::MessageHeader* pMsgHeader = pMsg->GetMessageHeader();

		auto* pFrame = (MsgMobileNetCtrlSequenceFrame*)pMsgHeader;
		UINT subFrameSequence = pFrame->SubSequence;
		UINT frameSize = pFrame->Length - sizeof(MsgMobileNetCtrlSequenceFrame);
		UINT totalSize = pFrame->TotalSize;
		auto* dataPtr = (const BYTE*)(pFrame + 1);
		UINT offset = subFrameSequence * Message::MAX_SUBFRAME_SIZE;

		Assert(frameSize <= Message::MAX_SUBFRAME_SIZE);
		if (frameSize > Message::MAX_SUBFRAME_SIZE)
		{
			netErr(E_NET_BADPACKET_NOTEXPECTED);
		}

		if (subFrameSequence == 0) // first frame
		{
			//Assert(m_SubFrameMessage == nullptr);
			Util::SafeRelease(m_SubFrameMessage);

			UINT dummyID = PACKET_NETCTRL_SEQUENCE_FRAME;
			svrChkPtr(pFrameMessage = Message::MessageData::NewMessage(dummyID, totalSize));
			memcpy(pFrameMessage->GetMessageHeader(), dataPtr, frameSize);
			if (pFrameMessage->GetMessageHeader()->Length != totalSize)
			{
				Assert(false);
				netErr(E_NET_BADPACKET_NOTEXPECTED);
			}

			m_SubFrameMessage = pFrameMessage;
			pFrameMessage = nullptr;
		}
		else
		{
			if (m_SubFrameMessage == nullptr)
			{
				Assert(m_SubFrameMessage != nullptr);
				netErr(E_NET_BADPACKET_NOTEXPECTED);
			}

			BYTE* pDest = m_SubFrameMessage->GetMessageBuff() + offset;

			if (m_SubFrameMessage->GetMessageHeader()->Length != totalSize)
			{
				Assert(false);
				netErr(E_NET_BADPACKET_NOTEXPECTED);
			}

			if ((offset + frameSize) > totalSize)
			{
				Assert(false);
				netErr(E_NET_BADPACKET_NOTEXPECTED);
			}

			memcpy(pDest, dataPtr, frameSize);
		}

		if (m_SubFrameMessage != nullptr && (offset + frameSize) == totalSize)
		{
			svrChk(m_SubFrameMessage->ValidateChecksumNDecrypt());
			action(m_SubFrameMessage);
			m_SubFrameMessage = nullptr;
			svrChk(hr);
		}

	Proc_End:


		Util::SafeRelease(pFrameMessage);

		return hr;
	}

	// Initialize connection
	HRESULT ConnectionUDPBase::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
	{
		HRESULT hr = S_OK;

		m_uiGatheredSize = 0;

		m_RecvNetCtrlQueue.ClearQueue();
		m_RecvGuaQueue.ClearQueue();

		netChk(Connection::InitConnection( socket, connectInfo ) );

		netChk( ClearQueues() );

		Util::SafeRelease(m_SubFrameMessage);

	Proc_End:

		return hr;
	}

	// Close connection
	HRESULT ConnectionUDPBase::CloseConnection()
	{
		HRESULT hr = S_OK;

		if (GetConnectionState() != STATE_DISCONNECTED)
			Disconnect();

		hr = Connection::CloseConnection();

		Util::SafeRelease(m_SubFrameMessage);

		return hr;
	}

	// Clear Queue
	HRESULT ConnectionUDPBase::ClearQueues()
	{
		Connection::ClearQueues();

		m_RecvReliableWindow.ClearWindow();
		m_SendReliableWindow.ClearWindow();
		m_RecvNetCtrlQueue.ClearQueue();

		Util::SafeRelease(m_SubFrameMessage);

		return S_OK;
	}


	// Disconnect connection
	HRESULT ConnectionUDPBase::Disconnect()
	{
		if (GetConnectionState() != STATE_DISCONNECTING
			&& GetConnectionState() != STATE_DISCONNECTED)
		{
			// Send disconnect signal
			Message::MessageID msgIDTem;

			msgIDTem.ID = PACKET_NETCTRL_NONE;
			SendNetCtrl(PACKET_NETCTRL_DISCONNECT, 0, msgIDTem);
		}

		return Connection::Disconnect();
	}

	// Send packet buffer to connection with network device
	HRESULT ConnectionUDPBase::SendBufferUDP(IOBUFFER_WRITE *pSendBuffer)
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
			Disconnect();
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
				netTrace(Trace::TRC_ERROR, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", GetConnectionInfo(), hrErr, hr);
			}
			else
			{
				netTrace(Net::TRC_SENDRAW, "UDP Send Failed, ip:{0}, err:{1:X8}, hr:{2:X8}", GetConnectionInfo(), hrErr, hr);
				return S_OK;
			}
		}
		else
		{
			//netTrace(TRC_SENDRAW, "UDP Send ip:{0}, Len:{1}", GetConnectionInfo(), bufferLen);
		}

		return hr;
	}

	HRESULT ConnectionUDPBase::EnqueueBufferUDP(IOBUFFER_WRITE *pSendBuffer)
	{
		if (GetWriteQueueUDP() == nullptr) return E_UNEXPECTED;

		return GetWriteQueueUDP()->Enqueue(pSendBuffer);
	}


	// Send message to connected entity
	HRESULT ConnectionUDPBase::Send( Message::MessageData* &pMsg )
	{
		HRESULT hr = S_OK;
		Message::MessageID msgID;
		UINT uiMsgLen;
		IOBUFFER_WRITE *pSendBuffer = nullptr;

		Assert(pMsg->GetDataLength() == 0 || pMsg->GetMessageHeader()->Crc32 != 0 || pMsg->GetMessageHeader()->msgID.IDs.Policy == POLICY_NONE);

		if (GetConnectionState() == STATE_DISCONNECTED)
			return S_FALSE;

		Message::MessageHeader* pMsgHeader = pMsg->GetMessageHeader();
		msgID = pMsgHeader->msgID;
		uiMsgLen = pMsg->GetMessageHeader()->Length;

		if( (msgID.IDs.Type != Message::MSGTYPE_NETCONTROL && GetConnectionState() == STATE_DISCONNECTING)
			|| GetConnectionState() == STATE_DISCONNECTED)
		{
			// Send fail by connection closed
			Util::SafeRelease( pMsg );
			goto Proc_End;
		}

		if( pMsg->GetMessageSize() > (UINT)Const::INTER_PACKET_SIZE_MAX )
		{
			Util::SafeRelease( pMsg );
			netErr( E_NET_BADPACKET_TOOBIG );
		}

		PrintDebugMessage( "SendMsg ", pMsg );

		if (!msgID.IDs.Reliability
			&& m_SendGuaQueue.GetEnqueCount() > Const::TCP_GUARANT_PENDING_MAX)
		{
			// Drop if there is too many reliable packets are pending
			netErr(E_NET_SEND_FAIL);
		}

		pMsg->UpdateChecksumNEncrypt();


		if( !msgID.IDs.Reliability )
		{
			if( !pMsg->GetIsSequenceAssigned() )
				pMsg->AssignSequence( NewSeqNone() );

			netTrace( TRC_NETCTRL, "SEND : msg:{0}, seq:{1}, len:{2}", 
				msgID,
				msgID.IDSeq.Sequence,
				uiMsgLen);

			netChk(Net::NetSystem::AllocBuffer(pSendBuffer));
			pSendBuffer->SetupSendUDP(GetSocket(), GetRemoteSockAddr(), pMsg);

			if (NetSystem::IsProactorSystem())
			{
				netChk(SendBufferUDP(pSendBuffer));
			}
			else
			{
				netChk(EnqueueBufferUDP(pSendBuffer));
			}
			//netChk( GetNet()->SendMsg( this, pMsg ) ); // ignore error to process all message
		}
		else
		{
			if( pMsg->GetIsSequenceAssigned() )
			{
				netChk(Net::NetSystem::AllocBuffer(pSendBuffer));
				pSendBuffer->SetupSendUDP(GetSocket(), GetRemoteSockAddr(), pMsg);

				if (NetSystem::IsProactorSystem())
				{
					netChk(SendBufferUDP(pSendBuffer));
				}
				else
				{
					netChk(EnqueueBufferUDP(pSendBuffer));
				}
				//netChk( GetNet()->SendMsg( this, pMsg ) );
			}
			else
			{
				netTrace( TRC_GUARREANTEDCTRL, "SENDGuaQueued : CID:{0}, msg:{1}, seq:{2}, len:{3}", 
					GetCID(),
					msgID,
					msgID.IDSeq.Sequence,
					uiMsgLen);

				AssertRel(pMsg->GetMessageHeader()->msgID.IDSeq.Sequence == 0);

				netChk( m_SendGuaQueue.Enqueue( pMsg ) );

				// poke send
				if (GetEventHandler() != nullptr && (m_SendGuaQueue.GetEnqueCount() > 0 && m_SendReliableWindow.GetMsgCount() <= 2))
					GetEventHandler()->OnNetSyncMessage(this);
			}
		}
		pMsg = nullptr;

	Proc_End:

		if (FAILED(hr))
		{
			Util::SafeRelease(pMsg);
		}
		else
		{
			if (msgID.IDs.Type == Message::MSGTYPE_NETCONTROL)
			{
				netTrace(TRC_TCPNETCTRL, "TCP Ctrl CID:{2}, ip:{0}, msg:{1}", GetConnectionInfo().Remote, msgID, GetCID());
			}
			else
			{
				netTrace(TRC_TCPSENDRAW, "TCP Send CID:{2}, ip:{0}, msg:{1}", GetConnectionInfo().Remote, msgID, GetCID());
			}
		}

		return hr;
	}




	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP connection class
	//

	// called when incomming message occure
	HRESULT ConnectionUDP::OnRecv( UINT uiBuffSize, const BYTE* pBuff )
	{
		HRESULT hr = S_OK;
		Message::MessageData *pMsg = NULL;

		Message::MessageHeader * pMsgHeader = (Message::MessageHeader*)pBuff;

		netTrace(TRC_RECVRAW, "UDP Recv ip:{0}, msg:{1}, seq:{2}, len:{3}", GetConnectionInfo().Remote, pMsgHeader->msgID, pMsgHeader->msgID.IDSeq.Sequence, uiBuffSize);

		if( uiBuffSize == 0 )
		{
			IncZeroRecvCount();

			if( GetZeroRecvCount() > (ULONG)Const::CONNECTION_ZEROPACKET_MAX )
			{
				Disconnect();
			}
			goto Proc_End;
		}


		while( uiBuffSize )
		{
			pMsgHeader = (Message::MessageHeader*)pBuff;

			if( uiBuffSize < pMsgHeader->Length )
			{
				//Assert(0); // Will not occure with UDP packet
				netErr( E_NET_BADPACKET_SIZE );
			}


#ifdef UDP_PACKETLOS_EMULATE
			if( (Util::Random.Rand() % UDP_PACKETLOS_RATE) < UDP_PACKETLOS_BOUND )
				goto Proc_End;
#endif	// UDP_PACKETLOS_EMULATE

			ResetZeroRecvCount();

			// if net control message then process immidiately except reliable messages
			if (pMsgHeader->msgID.IDs.Type == Message::MSGTYPE_NETCONTROL && pMsgHeader->msgID.IDs.Reliability == false) 
			{
				MsgNetCtrlBuffer netCtrl;
				memcpy( &netCtrl, pMsgHeader, Util::Min( (size_t)pMsgHeader->Length, sizeof(netCtrl) ) );
				netChk( m_RecvNetCtrlQueue.Enqueue( netCtrl ) );
			}
			else
			{
				if (GetConnectionState() == Net::IConnection::STATE_CONNECTED)
				{
					netMem( pMsg = Message::MessageData::NewMessage( pMsgHeader->msgID.ID, pMsgHeader->Length, pBuff ) );

					hr = OnRecv( pMsg );
					pMsg = nullptr;
					netChk( hr );
				}
			}


			uiBuffSize -= pMsgHeader->Length;
			pBuff += pMsgHeader->Length;

			Util::SafeRelease( pMsg );
		}

	Proc_End:

		Util::SafeRelease( pMsg );

		return hr;
	}


	HRESULT ConnectionUDP::OnRecv( Message::MessageData *pMsg )
	{
		HRESULT hr = S_OK;
		UINT length = 0;
		BYTE* pDataPtr = nullptr;

		Message::MessageHeader* pMsgHeader = pMsg->GetMessageHeader();

		if (GetConnectionState() != Net::IConnection::STATE_CONNECTED)
		{
			goto Proc_End;
		}


		Assert( MemoryPool::CheckMemoryHeader( pMsg ) );
		pMsg->GetLengthNDataPtr( length, pDataPtr );
		Assert(length == 0 || pMsgHeader->Crc32 != 0 );

		netChk( pMsg->ValidateChecksumNDecrypt() );

		if( pMsgHeader->msgID.IDs.Reliability )
		{
			Assert( !pMsgHeader->msgID.IDs.Encrypted );

			// This message need to be merged before adding recv queue
			if (pMsgHeader->msgID.GetMsgID() == PACKET_NETCTRL_SEQUENCE_FRAME.GetMsgID())
			{
				hr = OnFrameSequenceMessage(pMsg, [&](Message::MessageData* pMsgData) { hr = m_RecvGuaQueue.Enqueue(pMsgData); });
			}
			else
			{
				hr = m_RecvGuaQueue.Enqueue(pMsg);
			}

			pMsg = nullptr;
			netChk(hr);
		}
		else
		{
			hr = ConnectionUDPBase::OnRecv( pMsg );
			pMsg = nullptr;
			netChk( hr );
		}


	Proc_End:

		Util::SafeRelease( pMsg );

		if( FAILED( hr ) )
		{
			CloseConnection();
		}

		return hr;
	}


	// Process network control message
	HRESULT ConnectionUDP::ProcNetCtrl( const MsgNetCtrl* pNetCtrl )
	{
		HRESULT hr = S_OK;
		HRESULT hrTem = S_OK;

		switch( pNetCtrl->msgID.IDs.MsgCode )
		{
		case NetCtrlCode_Ack:
			if( pNetCtrl->rtnMsgID.IDs.Type == Message::MSGTYPE_NETCONTROL )// connecting process
			{
				switch( pNetCtrl->rtnMsgID.IDs.MsgCode )
				{
				case NetCtrlCode_Disconnect:
					if (GetConnectionState() == IConnection::STATE_DISCONNECTING || GetConnectionState() == IConnection::STATE_CONNECTED)
					{
						netTrace( TRC_CONNECTION, "RECV Disconnected CID:{0}", GetCID() );
						netChk( CloseConnection() );
					}
					break;
				case NetCtrlCode_Connect:
					if (GetConnectionState() == IConnection::STATE_CONNECTING
						&& GetConnectionInfo().RemoteClass != NetClass::Unknown )
					{
						OnConnectionResult( S_OK );
					}
					break;
				case NetCtrlCode_HeartBit:
					m_ulNetCtrlTime = Util::Time.GetTimeMs();
					break;
				default:
					m_ulNetCtrlTime = Util::Time.GetTimeMs();
					break;
				};
			}
			else // general message
			{
				// Remove from Guranted queue
				if( pNetCtrl->rtnMsgID.IDs.Reliability )
				{
					hrTem = m_SendReliableWindow.ReleaseMsg(pNetCtrl->msgID.IDSeq.Sequence);
					netTrace( TRC_GUARREANTEDCTRL, "NetCtrl Recv GuaAck : CID:{0}:{1}, seq:{2}, rtnmsg:{3}, hr={4:X8}", 
						GetCID(), m_SendReliableWindow.GetBaseSequence(), pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->rtnMsgID, hrTem);
					netChk( hrTem );
				}
			}
			break;
		case NetCtrlCode_Nack:
			if( pNetCtrl->rtnMsgID.IDs.Type == Message::MSGTYPE_NETCONTROL )// connecting process
			{
				switch( pNetCtrl->rtnMsgID.IDs.MsgCode )
				{
				case NetCtrlCode_Disconnect:
					break;
				case NetCtrlCode_Connect:
					if (GetConnectionState() == IConnection::STATE_CONNECTING || GetConnectionState() == IConnection::STATE_CONNECTED)
					{
						// Protocol version mismatch
						OnConnectionResult( E_NET_PROTOCOL_VERSION_MISMATCH );
					}
					netChk( Disconnect() );
					break;
				case NetCtrlCode_HeartBit:
					break;
				default:
					break;
				};
			}
			else // general message
			{
			}
			break;
		case NetCtrlCode_HeartBit:
			m_ulNetCtrlTime = Util::Time.GetTimeMs();
			netChk(SendPending(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
			break;
		case NetCtrlCode_Connect:
		{
			if( pNetCtrl->Length < sizeof(MsgNetCtrlConnect) )
			{
				netTrace( Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote );
				netChk( Disconnect() );
				netErr( E_UNEXPECTED );
			}
			const MsgNetCtrlConnect *pNetCtrlCon = (const MsgNetCtrlConnect*)pNetCtrl;
			UINT ProtocolVersion = pNetCtrl->rtnMsgID.ID;
			NetClass RemoteClass = (NetClass)pNetCtrl->msgID.IDSeq.Sequence;
			switch (GetConnectionState())
			{
			case  IConnection::STATE_CONNECTING:
				if( pNetCtrl->rtnMsgID.ID != BR_PROTOCOL_VERSION )
				{
					netChk(SendNetCtrl(PACKET_NETCTRL_NACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
					OnConnectionResult( E_NET_PROTOCOL_VERSION_MISMATCH );
					netChk( Disconnect() );
					break;
				}
				else if( GetConnectionInfo().RemoteClass != NetClass::Unknown && RemoteClass != GetConnectionInfo().RemoteClass )
				{
					netChk(SendNetCtrl(PACKET_NETCTRL_NACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
					OnConnectionResult( E_NET_INVALID_NETCLASS );
					netChk( Disconnect() );
					break;
				}

				netTrace( TRC_NETCTRL, "UDP Recv Connecting CID({0}) : C:{1}, Ver:{2})", GetCID(), RemoteClass, ProtocolVersion );
				m_ConnectInfo.SetRemoteInfo( RemoteClass, pNetCtrlCon->PeerUID );

			case IConnection::STATE_CONNECTED:
				m_ulNetCtrlTime = Util::Time.GetTimeMs();
				netChk(SendNetCtrl(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
				break;
			default:
				break;
			};

			break;
		}
		case NetCtrlCode_ConnectPeer:
		{
			break;
		}
		case NetCtrlCode_Disconnect:
			SendFlush();
			netChk(SendNetCtrl(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
			netChk(SendNetCtrl(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
			SendFlush();
			netTrace( TRC_CONNECTION, "Disconnect from remote CID:{0}", GetCID() );
			netChk( CloseConnection() );
			break;
		default:
			//netAssert( 0 );
			netTrace( Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote );
			netChk( CloseConnection() );
			netErr( E_UNEXPECTED );
			break;
		};


	Proc_End:

		return hr;
	}


	// Process NetCtrl queue
	HRESULT ConnectionUDP::ProcNetCtrlQueue()
	{
		HRESULT hr = S_OK;

		// Process received net ctrl messages
		MsgNetCtrlBuffer netCtrl;
		auto loopCount = m_RecvNetCtrlQueue.GetEnqueCount();
		for (unsigned iCount = 0; iCount < loopCount; iCount++)
		{
			if (FAILED(m_RecvNetCtrlQueue.Dequeue(netCtrl)))
				break;

			if( netCtrl.msgID.ID != 0 )
			{
				netChk( ProcNetCtrl( &netCtrl ) );

				if (GetConnectionState() == Net::IConnection::STATE_DISCONNECTED)
					goto Proc_End;
			}
		}

	Proc_End:

		return hr;
	}

	// Process Recv queue
	HRESULT ConnectionUDP::ProcRecvReliableQueue()
	{
		HRESULT hr = S_OK;
		Message::MessageData *pIMsg = nullptr;
		Message::MessageID msgIDTem;
		Message::MessageHeader *pMsgHeader = nullptr;
		MsgWindow::MessageElement *pMessageElement = nullptr;


		if (GetConnectionState() == Net::IConnection::STATE_DISCONNECTED)
			return S_OK;


		// Recv guaranted queue process
		auto loopCount = m_RecvGuaQueue.GetEnqueCount();
		for (unsigned iCount = 0; iCount < loopCount; iCount++)
		{
			if (FAILED(m_RecvGuaQueue.Dequeue(pIMsg)))
				break;

			if( pIMsg == nullptr )
				break;

			pMsgHeader = pIMsg->GetMessageHeader();

			Assert( MemoryPool::CheckMemoryHeader( pIMsg ) );
			Assert(pMsgHeader->Crc32 != 0);

			if( !pMsgHeader->msgID.IDs.Reliability )
			{
				Assert(0);// this wil not be happened
				continue;
			}

			Assert( !pMsgHeader->msgID.IDs.Encrypted );

			HRESULT hrTem = m_RecvReliableWindow.AddMsg( pIMsg );

			netTrace( TRC_GUARREANTEDCTRL, "RECVGuaAdd : CID:{0}:{1}, msg:{2}, seq:{3}, len:%4%, hr={5:X8}", 
							GetCID(), m_RecvReliableWindow.GetBaseSequence(), 
							pIMsg->GetMessageHeader()->msgID, 
							pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence,
							pIMsg->GetMessageHeader()->Length,
							hrTem );

			if( hrTem == S_NET_PROCESSED_SEQUENCE )
			{
				SendPending(PACKET_NETCTRL_ACK, pMsgHeader->msgID.IDSeq.Sequence, pMsgHeader->msgID);
				Util::SafeRelease( pIMsg );
				continue;
			}
			else if (hrTem == E_NET_INVALID_SEQUENCE || hrTem == E_NET_SEQUENCE_OVERFLOW)
			{
				Util::SafeRelease( pIMsg );
				continue;
			}
			else
			{
				// Added to msg window just send ACK
				pIMsg = NULL;
				SendPending(PACKET_NETCTRL_ACK, pMsgHeader->msgID.IDSeq.Sequence, pMsgHeader->msgID);
			}

			netChk(ProcGuarrentedMessageWindow([&](Message::MessageData* pMsg){ Connection::OnRecv(pIMsg); }));
		}


Proc_End:

		if( pIMsg ) pIMsg->Release();
		if( pMessageElement && pMessageElement->pMsg ) pMessageElement->pMsg->Release();


		return hr;
	}


	// Process Send queue
	HRESULT ConnectionUDP::ProcSendReliableQueue()
	{
		HRESULT hr = S_OK;
		Message::MessageData *pIMsg = nullptr;
		Message::MessageHeader *pMsgHeader = nullptr;
		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();
		UINT halfWindowSize = m_SendReliableWindow.GetWindowSize() >> 1;
		Assert(halfWindowSize >= 16);

		// Send guaranted message process
		auto availableWindow = m_SendReliableWindow.GetAvailableSize();
		auto uiNumPacket = m_SendGuaQueue.GetEnqueCount();
		CounterType availablePush = Util::Min(availableWindow, halfWindowSize);
		auto NumProc = Util::Min(availablePush, uiNumPacket);
		for( CounterType uiPacket = 0; uiPacket < NumProc; uiPacket++ )
		{
			if( FAILED(m_SendGuaQueue.Dequeue( pIMsg )) )
				break;

			if (pMsgHeader->msgID.IDs.Reliability)
			{
				pMsgHeader = pIMsg->GetMessageHeader();
				if (pMsgHeader->Length > Message::MAX_SUBFRAME_SIZE)
				{
					svrChk(SendFrameSequenceMessage(pIMsg));
				}
				else
				{
					if (SUCCEEDED(m_SendReliableWindow.EnqueueMessage(ulTimeCur, pIMsg)))
					{
						pIMsg->AddRef();// Inc Ref for send
						Assert(pIMsg->GetDataLength() == 0 || pIMsg->GetMessageHeader()->Crc32 != 0);
						SendPending(pIMsg);
						Assert(pIMsg->GetDataLength() == 0 || pIMsg->GetMessageHeader()->Crc32 != 0);
						netTrace(TRC_GUARREANTEDCTRL, "SENDENQReliable : CID:{0}, seq:{1}, msg:{2}, len:{3}",
							GetCID(),
							pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence,
							pIMsg->GetMessageHeader()->msgID,
							pIMsg->GetMessageHeader()->Length);
					}
				}
				pIMsg = nullptr;
			}
			else
			{
				Assert(false);
			}
		}


	Proc_End:

		Util::SafeRelease(pIMsg);

		return hr;
	}
		
	// Process message window queue
	HRESULT ConnectionUDP::ProcReliableSendRetry()
	{
		HRESULT hr = S_OK;
		MsgWindow::MessageElement *pMessageElement = nullptr;
		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();

		// Guaranted retry
		UINT uiMaxProcess = Util::Min( m_SendReliableWindow.GetMsgCount(), m_uiMaxGuarantedRetry );
		for( UINT uiIdx = 0, uiMsgProcessed = 0; uiIdx < (UINT)m_SendReliableWindow.GetWindowSize() && uiMsgProcessed < uiMaxProcess; uiIdx++ )
		{
			if( SUCCEEDED(m_SendReliableWindow.GetAt( uiIdx, pMessageElement ))
				&& pMessageElement && pMessageElement->pMsg != NULL )
			{
				if( (ulTimeCur-pMessageElement->ulTimeStamp) > DurationMS(Const::SEND_RETRY_TIME) )
				{
					netTrace( TRC_GUARREANTEDCTRL, "SENDGuaRetry : CID:{0}, seq:{1}, msg:{2}, len:{3}", 
									GetCID(),
									pMessageElement->pMsg->GetMessageHeader()->msgID.IDSeq.Sequence,
									pMessageElement->pMsg->GetMessageHeader()->msgID, 
									pMessageElement->pMsg->GetMessageHeader()->Length );
					pMessageElement->ulTimeStamp = ulTimeCur;
					pMessageElement->pMsg->AddRef();
					SendPending( pMessageElement->pMsg );
				}
			}
			pMessageElement = NULL;
		}


	//Proc_End:

		if( pMessageElement && pMessageElement->pMsg ) pMessageElement->pMsg->Release();

		return hr;
	}

	
	// Process connection state
	HRESULT ConnectionUDP::ProcConnectionState()
	{
		HRESULT hr = S_OK;
		Message::MessageID msgIDTem;

		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();


		// Update connection status
		msgIDTem.ID = PACKET_NETCTRL_NONE;
		switch (GetConnectionState())
		{
		case IConnection::STATE_CONNECTING:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime).count() > (INT)GetConnectingTimeOut() ) // connection time out
			{
				netTrace( TRC_CONNECTION, "UDP Connecting Timeout CID:{0}, ({1},{2},{3})", GetCID(), ulTimeCur, m_ulNetCtrlTime, GetConnectingTimeOut() );
				netChk( CloseConnection() );
			}
			else if( (INT)(ulTimeCur-m_ulNetCtrlTryTime).count() > Const::CONNECTION_RETRY_TIME ) // retry
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netTrace( TRC_NETCTRL, "UDP Send Connecting CID({0}) : C:{1}, V:{2})", GetCID(), GetConnectionInfo().LocalClass, (UINT32)BR_PROTOCOL_VERSION );
				netChk( SendPending( PACKET_NETCTRL_CONNECT, (UINT)GetConnectionInfo().LocalClass, Message::MessageID( BR_PROTOCOL_VERSION ), GetConnectionInfo().LocalID ) );
			}

			break;
		case IConnection::STATE_CONNECTED:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime).count() > Const::HEARTBIT_TIMEOUT ) // connection time out
			{
				netTrace( TRC_CONNECTION, "UDP Connection Timeout CID:{0}, ({1},{2})", GetCID(), ulTimeCur, m_ulNetCtrlTime );
				netChk( CloseConnection() );
				goto Proc_End;
			}
			else if( (INT)(ulTimeCur-m_ulNetCtrlTryTime).count() > (INT)GetHeartbitTry() ) // heartbit time
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk( SendPending( PACKET_NETCTRL_HEARTBIT, 0, msgIDTem ) );
			}
			break;
		case IConnection::STATE_DISCONNECTING:
			if( (INT)(ulTimeCur-m_ulNetCtrlTime).count() > Const::DISCONNECT_TIMEOUT ) // connection time out
			{
				netTrace( TRC_CONNECTION, "UDP Disconnecting Timeout CID:{0}, ({1},{2})", GetCID(), ulTimeCur, m_ulNetCtrlTime );
				netChk( CloseConnection() );
			}
			else if( (INT)(ulTimeCur-m_ulNetCtrlTryTime).count() > Const::DISCONNECT_RETRY_TIME ) // retry
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk( SendPending( PACKET_NETCTRL_DISCONNECT, 0, msgIDTem ) );
			}

			break;
		default:
			break;
		};

	Proc_End:

		return hr;
	}


	// Update net control, process connection heartbit, ... etc
	HRESULT ConnectionUDP::UpdateNetCtrl()
	{
		HRESULT hr = S_OK;
		Message::MessageID msgIDTem;

		if (GetConnectionState() == Net::IConnection::STATE_DISCONNECTED)
			goto Proc_End;

		hr = ProcNetCtrlQueue();
		if( FAILED(hr) )
		{
			netTrace( TRC_CONNECTION, "Process NetCtrlQueue failed {0:X8}", hr );
		}

		hr = ProcConnectionState();
		if( FAILED(hr) )
		{
			netTrace( TRC_CONNECTION, "Process Connection state failed {0:X8}", hr );
		}


		hr = ProcRecvReliableQueue();
		if( FAILED(hr) )
		{
			netTrace( TRC_CONNECTION, "Process Recv Guaranted queue failed {0:X8}", hr );
		}

		hr = ProcSendReliableQueue();
		if( FAILED(hr) )
		{
			netTrace( TRC_CONNECTION, "Process Send Guaranted queue failed {0:X8}", hr );
		}

		hr = ProcReliableSendRetry();
		if( FAILED(hr) )
		{
			netTrace( TRC_CONNECTION, "Process message window failed {0:X8}", hr );
		}


	Proc_End:

		//SendFlush();

		return hr;
	}



	

} // namespace Net
} // namespace BR


