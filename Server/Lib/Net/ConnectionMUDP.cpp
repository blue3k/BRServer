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
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Net/NetTrace.h"
#include "Net/ConnectionMUDP.h"
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


BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionMUDPServer);
BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionMUDPClient);


namespace BR {
namespace Net {


	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server Mobile UDP Network connection class
	//

	// Constructor
	ConnectionMUDP::ConnectionMUDP()
		: ConnectionUDPBase( MUDP_BASE_WINDOW_SIZE )
		, m_bSendSyncThisTick(false)
	{
		// limit server net retry maximum
		SetMaxGuarantedRetry( Const::UDP_CLI_RETRY_ONETIME_MAX );
	}

	ConnectionMUDP::~ConnectionMUDP()
	{
	}

	
	// Make Ack packet and enqueue to SendNetCtrlqueue
	Result ConnectionMUDP::SendNetCtrl( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID )
	{
		Result hr = ResultCode::SUCCESS, hrTem = ResultCode::SUCCESS;

		MsgMobileNetCtrl *pNetCtrl = nullptr;
		Message::MessageData *pMsg = nullptr;

		netMem( pMsg = Message::MessageData::NewMessage( uiCtrlCode, sizeof(MsgMobileNetCtrl) ) );

		pNetCtrl = (MsgMobileNetCtrl*)pMsg->GetMessageBuff();
		pNetCtrl->PeerID = UID;
		pNetCtrl->msgID.SetSequence(uiSequence);
		pNetCtrl->rtnMsgID = msgID;

		pMsg->GetMessageHeader()->msgID.IDs.Mobile = true;
		pMsg->UpdateChecksum();


		hrTem = SendRaw(pMsg);
		//hrTem = GetNet()->SendMsg( this, pMsg );
		if( !(hrTem) )
		{
			netTrace( TRC_GUARREANTEDCTRL, "NetCtrl Send failed : CID:{0}, msg:{1:X8}, seq:{2}, hr={3:X8}", 
							GetCID(), 
							msgID.ID, 
							uiSequence, 
							hrTem );

			// ignore io send fail except connection closed
			if( hrTem == ((Result)ResultCode::E_NET_CONNECTION_CLOSED) )
			{
				goto Proc_End;
			}
		}

	Proc_End:

		if (!(hrTem))
		{
			netTrace(TRC_GUARREANTEDCTRL, "NetCtrl Send failed : CID:{0}, msg:{1:X8}, seq:{2}, hr={3:X8}",
				GetCID(),
				msgID.ID,
				uiSequence,
				hrTem);
		}

		return hr;
	}

	// Process network control message
	Result ConnectionMUDP::ProcNetCtrl(const MsgMobileNetCtrl* pNetCtrl)
	{
		Result hr = ResultCode::SUCCESS;
		Result hrTem = ResultCode::SUCCESS;

		if (pNetCtrl->msgID.IDs.Mobile == 0 || pNetCtrl->Length < sizeof(MsgMobileNetCtrl))
		{
			netTrace(Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote);
			netChk(Disconnect("Invalid packet"));
			netErr(ResultCode::E_NET_BADPACKET_NOTEXPECTED);
		}

		switch (pNetCtrl->msgID.IDs.MsgCode)
		{
		case NetCtrlCode_Ack:
			if (pNetCtrl->rtnMsgID.IDs.Type == Message::MSGTYPE_NETCONTROL)// connecting process
			{
				switch (pNetCtrl->rtnMsgID.IDs.MsgCode)
				{
				case NetCtrlCode_Disconnect:
					if (GetConnectionState() == IConnection::STATE_DISCONNECTING || GetConnectionState() == IConnection::STATE_CONNECTED)
					{
						netTrace(TRC_CONNECTION, "RECV Disconnected CID:{0}", GetCID());
						netChk(CloseConnection());
					}
					break;
				case NetCtrlCode_Connect:
					if (GetConnectionState() == IConnection::STATE_CONNECTING
						&& m_ConnectInfo.RemoteClass != NetClass::Unknown)
					{
						OnConnectionResult(ResultCode::SUCCESS);
					}
					break;
				default:
					netTrace(Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote);
					netChk(Disconnect("Invalid packet"));
					netErr(ResultCode::E_NET_BADPACKET_NOTEXPECTED);
					break;
				};
			}
			else // general message
			{
				netTrace(Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote);
				netChk(Disconnect("Invalid packet"));
				netErr(ResultCode::E_NET_BADPACKET_NOTEXPECTED);
			}
			break;
		case NetCtrlCode_Nack:
			if (pNetCtrl->rtnMsgID.IDs.Type == Message::MSGTYPE_NETCONTROL)// connecting process
			{
				switch (pNetCtrl->rtnMsgID.IDs.MsgCode)
				{
				case NetCtrlCode_Connect:
					if (GetConnectionState() == IConnection::STATE_CONNECTING || GetConnectionState() == IConnection::STATE_CONNECTED)
					{
						// Protocol version mismatch
						OnConnectionResult(ResultCode::E_NET_PROTOCOL_VERSION_MISMATCH);
					}
					netChk(Disconnect("Protocal mismatch"));
					break;
				default:
					break;
				};
			}
			break;
		case NetCtrlCode_HeartBit:
			netChk(SendNetCtrl(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
			break;
		case NetCtrlCode_TimeSync:
			netChk(SendNetCtrl(PACKET_NETCTRL_TIMESYNC, 0, pNetCtrl->msgID, Util::Time.GetRawUTCSec().time_since_epoch().count()));
			break;
		case NetCtrlCode_SyncReliable:
		{
			MutexScopeLock localLock(m_SendReliableWindow.GetLock());

			MsgMobileNetCtrlSync *pSyncCtrl = (MsgMobileNetCtrlSync*)pNetCtrl;
			if (pSyncCtrl->Length != sizeof(MsgMobileNetCtrlSync))
				netErr(ResultCode::E_NET_BADPACKET_SIZE);

			hrTem = m_SendReliableWindow.ReleaseMsg(pSyncCtrl->msgID.IDSeq.Sequence, pSyncCtrl->MessageMask);
			netTrace(TRC_GUARREANTEDCTRL, "NetCtrl Recv SendReliableMask : CID:{0}:{1}, seq:{2}, mask:{3:X8}, hr={4:X8}",
				GetCID(), m_SendReliableWindow.GetBaseSequence(), pSyncCtrl->msgID.IDSeq.Sequence, pSyncCtrl->MessageMask, hrTem);

			if (hrTem == Result(ResultCode::UNEXPECTED))
				CloseConnection();

			netChk(hrTem);

			if (GetEventHandler() != nullptr && (m_SendGuaQueue.GetEnqueCount() > 0 || m_SendReliableWindow.GetMsgCount() > 0))
				GetEventHandler()->OnNetSyncMessage(this);
			else
				SetSendSyncThisTick(true); // for regular ping. maybe I can get rid of this later
			break;
		}
		case NetCtrlCode_Connect:
		{
			UINT ProtocolVersion = pNetCtrl->rtnMsgID.ID;
			NetClass RemoteClass = (NetClass)pNetCtrl->msgID.IDSeq.Sequence;
			switch (GetConnectionState())
			{
			case  IConnection::STATE_CONNECTING:
				if (ProtocolVersion != BR_PROTOCOL_VERSION)
				{
					netChk(SendNetCtrl(PACKET_NETCTRL_NACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
					OnConnectionResult(ResultCode::E_NET_PROTOCOL_VERSION_MISMATCH);
					netChk(Disconnect("Protocol mismatch"));
					break;
				}
				else if (GetConnectionInfo().RemoteClass != NetClass::Unknown && RemoteClass != GetConnectionInfo().RemoteClass)
				{
					netChk(SendNetCtrl(PACKET_NETCTRL_NACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
					OnConnectionResult(ResultCode::E_NET_INVALID_NETCLASS);
					netChk(Disconnect("Invalid netclass"));
					break;
				}

				netChk(SendNetCtrl(PACKET_NETCTRL_ACK, (UINT)GetConnectionInfo().LocalClass, pNetCtrl->msgID, GetConnectionInfo().LocalID));

				netTrace(TRC_NETCTRL, "UDP Recv Connecting CID({0}) : C:{1}, Ver:{2})", GetCID(), RemoteClass, ProtocolVersion);
				m_ConnectInfo.SetRemoteInfo(RemoteClass, pNetCtrl->PeerID);

				// Set connection is succeeded and connected
				OnConnectionResult(ResultCode::SUCCESS);
				break;
			case IConnection::STATE_CONNECTED:
				netChk(SendNetCtrl(PACKET_NETCTRL_ACK, (UINT)GetConnectionInfo().LocalClass, pNetCtrl->msgID, GetConnectionInfo().LocalID));
				break;
			default:
				break;
			};

			break;
		}
		case NetCtrlCode_Disconnect:
			netChk(SendNetCtrl(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID));
			netTrace(TRC_DBGCON, "Disconnect from remote CID:{0}", GetCID());
			netChk(CloseConnection());
			break;
		default:
			netTrace(Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote);
			netChk(CloseConnection());
			netErr(ResultCode::UNEXPECTED);
			break;
		};


	Proc_End:


		return hr;
	}

	Result ConnectionMUDP::InitConnection(SOCKET socket, const ConnectionInformation &connectInfo)
	{
		return ConnectionUDPBase::InitConnection(socket, connectInfo);
	}

	// called when incomming message occure
	Result ConnectionMUDP::OnRecv( UINT uiBuffSize, const BYTE* pBuff )
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageData *pMsg = NULL;
		Message::MobileMessageHeader * pMsgHeader = (Message::MobileMessageHeader*)pBuff;

		netTrace(TRC_RECVRAW, "UDP Recv ip:{0}, msg:{1}, seq:{2}, len:{3}", GetConnectionInfo().Remote, pMsgHeader->msgID, pMsgHeader->msgID.IDSeq.Sequence, uiBuffSize);

		if( uiBuffSize == 0 )
		{
			IncZeroRecvCount();

			if( GetZeroRecvCount() > (ULONG)Const::CONNECTION_ZEROPACKET_MAX )
			{
				Disconnect("Too many zero packet");
			}
			goto Proc_End;
		}


		while( uiBuffSize && GetConnectionState() != ConnectionState::STATE_DISCONNECTED)
		{
			pMsgHeader = (Message::MobileMessageHeader*)pBuff;
			if( uiBuffSize < sizeof(Message::MobileMessageHeader) || uiBuffSize < pMsgHeader->Length )
			{
				netTrace( Trace::TRC_ERROR, "Unexpected packet buffer size:{0}, size in header:{1}", uiBuffSize, pMsgHeader->Length );
				netErr( ResultCode::E_NET_BADPACKET_SIZE );
			}

#ifdef UDP_PACKETLOS_EMULATE
			if( (Util::Random.Rand() % UDP_PACKETLOS_RATE) < UDP_PACKETLOS_BOUND )
				goto Proc_End;
#endif	// UDP_PACKETLOS_EMULATE

			ResetZeroRecvCount();

			if (pMsgHeader->msgID.IDs.Type == Message::MSGTYPE_NETCONTROL && pMsgHeader->msgID.IDs.Reliability == false) // if net control message then process immidiately
			{
				netChk(ProcNetCtrl((MsgMobileNetCtrl*)pMsgHeader));
			}
			else
			{
				if (GetConnectionState() == IConnection::STATE_CONNECTED)
				{
					netMem( pMsg = Message::MessageData::NewMessage( pMsgHeader->msgID.ID, pMsgHeader->Length, pBuff ) );

					hr = OnRecv( pMsg );
					pMsg = NULL;
					netChk( hr );
				}
				else
				{
					if (GetConnectionState() != IConnection::STATE_CONNECTING)
					{
						// Sending normal message packet without connection process.
						// Disconnect them
						netChk(Disconnect("Invalid packet type"));
						//netErr(ResultCode::E_NET_BADPACKET_NOTEXPECTED);
						hr = ResultCode::E_NET_BADPACKET_NOTEXPECTED;
						goto Proc_End;
					}
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


	Result ConnectionMUDP::OnRecv( Message::MessageData *pMsg )
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageHeader* pMsgHeader = pMsg->GetMessageHeader();

		if (GetConnectionState() != IConnection::STATE_CONNECTED)
		{
			goto Proc_End;
		}

		netChk( pMsg->ValidateChecksumNDecrypt() );

		if( pMsgHeader->msgID.IDs.Reliability )
		{
			Assert( !pMsgHeader->msgID.IDs.Encrypted );

			netTrace(TRC_GUARREANTEDCTRL, "RECVGua    : CID:{0} msg:{1}, seq:{2}, len:{3}",
				GetCID(), 
				pMsg->GetMessageHeader()->msgID,
				pMsg->GetMessageHeader()->msgID.IDSeq.Sequence,
				pMsg->GetMessageHeader()->Length);

			//hr = m_RecvGuaQueue.Enqueue( pMsg );
			//pMsg = nullptr;
			//netChk( hr );

			netChk(OnGuarrentedMessageRecv(pMsg));
			pMsg = nullptr;
		}
		else
		{
			hr = ConnectionUDPBase::OnRecv(pMsg);
			pMsg = nullptr;
			netChk( hr );
		}


	Proc_End:

		Util::SafeRelease( pMsg );

		if( !( hr ) )
		{
			CloseConnection();
		}

		return hr;
	}
	

	// gathering
	Result ConnectionMUDP::SendPending( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID )
	{
		Result hr = ResultCode::SUCCESS;

		MsgMobileNetCtrl *pNetCtrlMsg = nullptr;

		netChk( PrepareGatheringBuffer(sizeof(MsgNetCtrlBuffer)) );

		pNetCtrlMsg = (MsgMobileNetCtrl*)(m_pGatheringBuffer+m_uiGatheredSize);
		pNetCtrlMsg->msgID.ID = uiCtrlCode;
		pNetCtrlMsg->msgID.SetSequence( uiSequence);
		pNetCtrlMsg->rtnMsgID = msgID;
		pNetCtrlMsg->PeerID = UID;

		pNetCtrlMsg->msgID.IDs.Mobile = true;
		pNetCtrlMsg->Length = sizeof(MsgMobileNetCtrl);

		pNetCtrlMsg->SetCrc(Util::Crc32( sizeof(MsgMobileNetCtrl) - sizeof(Message::MobileMessageHeader), (BYTE*)pNetCtrlMsg + sizeof(Message::MobileMessageHeader) ));

		m_uiGatheredSize += pNetCtrlMsg->Length;


	Proc_End:

		return hr;
	}

	Result ConnectionMUDP::SendSync( UINT uiSequence, UINT64 uiSyncMask )
	{
		Result hr = ResultCode::SUCCESS;
		MsgMobileNetCtrlSync *pNetCtrlMsg = NULL;

		netChk( PrepareGatheringBuffer(sizeof(MsgMobileNetCtrlSync)) );

		pNetCtrlMsg = (MsgMobileNetCtrlSync*)(m_pGatheringBuffer+m_uiGatheredSize);
		pNetCtrlMsg->msgID = PACKET_NETCTRL_SYNCRELIABLE;
		pNetCtrlMsg->msgID.SetSequence(uiSequence);
		pNetCtrlMsg->MessageMask = uiSyncMask;
		pNetCtrlMsg->PeerID = GetPeerID();

		pNetCtrlMsg->msgID.IDs.Mobile = true;
		pNetCtrlMsg->Length = sizeof(MsgMobileNetCtrlSync);

		pNetCtrlMsg->SetCrc(Util::Crc32( sizeof(MsgMobileNetCtrlSync) - sizeof(Message::MobileMessageHeader), (BYTE*)pNetCtrlMsg + sizeof(Message::MobileMessageHeader) ));

		netTrace(TRC_GUARREANTEDCTRL, "NetCtrl Send RecvReliableMask : CID:{0} BaseSeq:{1}, seq:{2}, mask:{3:X8}",
			GetCID(), m_RecvReliableWindow.GetBaseSequence(), uiSequence, uiSyncMask);

		m_uiGatheredSize += pNetCtrlMsg->Length;


	Proc_End:

		return hr;
	}


	// Process NetCtrl queue
	Result ConnectionMUDP::ProcNetCtrlQueue()
	{
		Result hr = ResultCode::SUCCESS;

		assert(ThisThread::GetThreadID() == GetRunningThreadID());

		// Process received net ctrl messages
		MsgNetCtrlBuffer netCtrl;
		auto loopCount = m_RecvNetCtrlQueue.GetEnqueCount();
		for (decltype(loopCount) iLoop = 0; iLoop < loopCount; iLoop++)
		{
			if (!(m_RecvNetCtrlQueue.Dequeue(netCtrl)))
				break;

			if( netCtrl.msgID.ID != 0 )
			{
				netChk( ProcNetCtrl( (MsgMobileNetCtrl*)&netCtrl ) );

				if( GetConnectionState() == IConnection::STATE_DISCONNECTED )
					goto Proc_End;
			}
		}

	Proc_End:

		return hr;
	}


	Result ConnectionMUDP::OnGuarrentedMessageRecv(Message::MessageData *pIMsg)
	{
		Result hr = ResultCode::SUCCESS;

		auto msgID = pIMsg->GetMessageHeader()->msgID;
		auto seq = pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence;
		auto len = pIMsg->GetMessageHeader()->Length;

		Result hrTem = m_RecvReliableWindow.AddMsg(pIMsg);

		netTrace(TRC_GUARREANTEDCTRL, "RECVGuaAdd : CID:{0} BaseSeq:{1}, msg:{2}, seq:{3}, len:%4%, hr={5:X8}",
			GetCID(), m_RecvReliableWindow.GetBaseSequence(),
			msgID,
			seq,
			len,
			hrTem);

		if (hrTem == Result(ResultCode::S_NET_PROCESSED_SEQUENCE))
		{
			Util::SafeRelease(pIMsg);
			return hr;
		}
		else if (hrTem == Result(ResultCode::E_NET_INVALID_SEQUENCE) || hrTem == Result(ResultCode::E_NET_SEQUENCE_OVERFLOW))
		{
			// out of window, we are going to receive this message again
			Util::SafeRelease(pIMsg);
			return hr;
		}
		else
		{
			netChk(hrTem);
			// Added to msg window just send ACK
			pIMsg = nullptr;

			if (GetEventHandler())
			{
				GetEventHandler()->OnRecvMessage(this, nullptr);
			}
			else
			{
				netTrace(TRC_GUARREANTEDCTRL, "RECVGuaAdded : No Event Handler for CID:{0} BaseSeq:{1}",
					GetCID(), m_RecvReliableWindow.GetBaseSequence());
			}
		}

	Proc_End:

		Util::SafeRelease(pIMsg);

		return hr;
	}

	// Process recv reliable queue
	Result ConnectionMUDP::ProcRecvReliableQueue()
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageData *pIMsg = nullptr;

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			return ResultCode::SUCCESS;

		assert(ThisThread::GetThreadID() == GetRunningThreadID());

		// Recv guaranted queue process
		pIMsg = nullptr;
		auto loopCount = m_RecvGuaQueue.GetEnqueCount();
		for (decltype(loopCount) iLoop = 0; iLoop < loopCount; iLoop++)
		{
			if (!(m_RecvGuaQueue.Dequeue(pIMsg)))
				break;

			if (pIMsg == nullptr)
				break;

			Message::MessageHeader *pMsgHeader = pIMsg->GetMessageHeader();
			if( !pMsgHeader->msgID.IDs.Reliability )
			{
				netAssert(0);// this wil not be happened
				continue;
			}

			Assert(!pMsgHeader->msgID.IDs.Encrypted);

			netChk(OnGuarrentedMessageRecv(pIMsg));

			netChk(ProcGuarrentedMessageWindow([&](Message::MessageData* pRecvGuaMsg){ Connection::OnRecv(pRecvGuaMsg); }));
		}

		// pop if no event handler is assigned
		if (GetEventHandler() == nullptr)
		{
			netChk(ProcGuarrentedMessageWindow([&](Message::MessageData* pRecvGuaMsg){ Connection::OnRecv(pRecvGuaMsg); }));
		}

	Proc_End:

		//Util::SafeRelease( pIMsg );
		//if( pMessageElement && pMessageElement->pMsg ) pMessageElement->pMsg->Release();

		return hr;
	}

	// Process Send queue
	Result ConnectionMUDP::ProcSendReliableQueue()
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageData *pIMsg = NULL;
		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();

		assert(ThisThread::GetThreadID() == GetRunningThreadID());

		// Send guaranted message process
		CounterType NumProc = m_SendReliableWindow.GetAvailableSize();
		CounterType uiNumPacket = m_SendGuaQueue.GetEnqueCount();
		NumProc = Util::Min( NumProc, uiNumPacket );
		for( CounterType uiPacket = 0; uiPacket < NumProc ; uiPacket++ )
		{
			if( !(m_SendGuaQueue.Dequeue( pIMsg )) )
				break;

			AssertRel(pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence == 0);

			// check sending window size
			pIMsg->AddRef(); // inc before send
			hr = m_SendReliableWindow.EnqueueMessage(ulTimeCur, pIMsg);
			if (!(hr))
			{
				pIMsg->Release();
				netErr(hr);
			}

			netTrace( TRC_GUARREANTEDCTRL, "SENDENQReliable : CID:{0}, seq:{1}, msg:{2}, len:{3}", 
						GetCID(), 
						pIMsg->GetMessageHeader()->msgID.IDSeq.Sequence,
						pIMsg->GetMessageHeader()->msgID, 
						pIMsg->GetMessageHeader()->Length );
			netChk( SendPending( pIMsg ) );
			pIMsg = nullptr;
		}


	Proc_End:

		Util::SafeRelease( pIMsg );

		if( !(hr) )
			Disconnect("Failed to send reliable packets");

		return hr;
	}
		
	// Process message window queue
	Result ConnectionMUDP::ProcReliableSendRetry()
	{
		Result hr = ResultCode::SUCCESS;
		MsgWindow::MessageElement *pMessageElement = nullptr;
		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();

		assert(ThisThread::GetThreadID() == GetRunningThreadID());

		// Guaranted retry
		UINT uiMaxProcess = Util::Min( m_SendReliableWindow.GetMsgCount(), m_uiMaxGuarantedRetry );
		for( UINT uiIdx = 0, uiMsgProcessed = 0; uiIdx < (UINT)m_SendReliableWindow.GetWindowSize() && uiMsgProcessed < uiMaxProcess; uiIdx++ )
		{
			if( (m_SendReliableWindow.GetAt( uiIdx, pMessageElement ))
				&& pMessageElement && pMessageElement->pMsg != nullptr )
			{
				if (Util::TimeSince(pMessageElement->ulTimeStamp) <= DurationMS(Const::MUDP_SEND_RETRY_TIME))
					break;

				UINT totalGatheredSize = GetGatheredBufferSize() + pMessageElement->pMsg->GetMessageSize();
				if (GetGatheredBufferSize() > 0 && totalGatheredSize > Const::PACKET_GATHER_SIZE_MAX)
				{
					// too big to send
					// stop gathering here, and send
					pMessageElement = nullptr;
					break;
				}

				netTrace( TRC_GUARREANTEDCTRL, "SENDReliableRetry : CID:{0}, seq:{1}, msg:{2}, len:{3}", 
								GetCID(),
								pMessageElement->pMsg->GetMessageHeader()->msgID.IDSeq.Sequence,
								pMessageElement->pMsg->GetMessageHeader()->msgID, 
								pMessageElement->pMsg->GetMessageHeader()->Length );
				pMessageElement->ulTimeStamp = ulTimeCur;
				pMessageElement->pMsg->AddRef();
				SendPending( pMessageElement->pMsg );
			}
			pMessageElement = nullptr;
		}


	//Proc_End:

		return hr;
	}


	// Process connection state
	Result ConnectionMUDP::ProcConnectionState()
	{
		Result hr = ResultCode::SUCCESS;

		// Update connection status
		switch( GetConnectionState() )
		{
		case IConnection::STATE_DISCONNECTING:
			netChk( CloseConnection() );
			break;
		default:
			break;
		};

	Proc_End:

		return hr;
	}


	// Update net control, process connection heartbit, ... etc
	Result ConnectionMUDP::UpdateNetCtrl()
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageID msgIDTem;

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			goto Proc_End;


		// UDP connection thread owner ship is belong to the one calling update
		if (GetRunningThreadID() != ThisThread::GetThreadID())
		{
			if (GetRunningThreadID() == ThreadID())
			{
				// no one ownes this connection yet
				SetRunningThreadID(ThisThread::GetThreadID());
			}
			else
			{
				assert(false);
				goto Proc_End;
			}
		}

		hr = ProcNetCtrlQueue();
		if( !(hr) )
		{
			netTrace( TRC_CONNECTION, "Process NetCtrlQueue failed {0:X8}", hr );
		}

		hr = ProcConnectionState();
		if( !(hr) )
		{
			netTrace( TRC_CONNECTION, "Process Connection state failed {0:X8}", hr );
		}

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			goto Proc_End;

		if (GetEventHandler() == nullptr)
		{
			MutexScopeLock localLock(m_SendReliableWindow.GetLock());

			hr = ProcSendReliableQueue();
			if (!(hr))
			{
				netTrace(TRC_CONNECTION, "Process Recv Guaranted queue failed {0:X8}", hr);
			}


			if (m_bSendSyncThisTick)
			{
				hr = ProcReliableSendRetry();
				if (!(hr))
				{
					netTrace(TRC_CONNECTION, "Process message window failed {0:X8}", hr);
				}

				m_bSendSyncThisTick = false;
				netChk(SendSync(m_RecvReliableWindow.GetBaseSequence(), m_RecvReliableWindow.GetSyncMask()));
			}
		}

		if (GetConnectionState() == IConnection::STATE_DISCONNECTED)
			goto Proc_End;

		hr = ProcRecvReliableQueue();
		if( !(hr) )
		{
			netTrace( TRC_CONNECTION, "Process Recv Guaranted queue failed {0:X8}", hr );
		}


	Proc_End:

		SendFlush();

		return hr;
	}

	Result ConnectionMUDP::UpdateSendQueue()
	{
		Result hr = ResultCode::SUCCESS;

		MutexScopeLock localLock(m_SendReliableWindow.GetLock());

		assert(ThisThread::GetThreadID() == GetRunningThreadID());

		if (GetConnectionState() == Net::IConnection::STATE_DISCONNECTED)
			goto Proc_End;


		hr = ProcSendReliableQueue();
		if (!(hr))
		{
			netTrace(TRC_CONNECTION, "Process Send Guaranted queue failed {0:X8}", hr);
		}


		//if (m_bSendSyncThisTick)
		//{
			hr = ProcReliableSendRetry();
			if (!(hr))
			{
				netTrace(TRC_CONNECTION, "Process message window failed {0:X8}", hr);
			}

			m_bSendSyncThisTick = false;
			netChk(SendSync(m_RecvReliableWindow.GetBaseSequence(), m_RecvReliableWindow.GetSyncMask()));
		//}



	Proc_End:

		SendFlush();

		return hr;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server Mobile UDP Network connection class
	//

	// Constructor
	ConnectionMUDPServer::ConnectionMUDPServer()
	{

	}

	ConnectionMUDPServer::~ConnectionMUDPServer()
	{

	}


	// Process network control message
	Result ConnectionMUDPServer::ProcNetCtrl(const MsgMobileNetCtrl* pNetCtrl)
	{
		Result hr = ResultCode::SUCCESS;

		hr = ConnectionMUDP::ProcNetCtrl(pNetCtrl);
		if (!(hr)) return hr;

		// Server should reply this
		switch (pNetCtrl->msgID.IDs.MsgCode)
		{
		case NetCtrlCode_SyncReliable:
			// TODO: we can skip this when there is a send reliable message
			SendNetCtrl(PACKET_NETCTRL_ACK, pNetCtrl->msgID.IDSeq.Sequence, pNetCtrl->msgID);
			break;
		}

	//Proc_End:

		return hr;

	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client Mobile UDP Network connection class
	//

	// Constructor
	ConnectionMUDPClient::ConnectionMUDPClient()
		: INetIOCallBack(GetSocket())
	{

	}

	ConnectionMUDPClient::~ConnectionMUDPClient()
	{

	}

	// Process network control message
	Result ConnectionMUDPClient::ProcNetCtrl(const MsgMobileNetCtrl* pNetCtrl)
	{
		Result hr = ResultCode::SUCCESS;

		if (pNetCtrl->msgID.IDs.Mobile == 0 || pNetCtrl->Length < sizeof(MsgMobileNetCtrl))
		{
			netTrace(Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote);
			netChk(Disconnect("Invalid packet"));
			netErr(ResultCode::E_NET_BADPACKET_NOTEXPECTED);
		}

		switch (pNetCtrl->msgID.IDs.MsgCode)
		{
		case NetCtrlCode_Ack:
			if (pNetCtrl->rtnMsgID.IDs.Type == Message::MSGTYPE_NETCONTROL)// connecting process
			{
				switch (pNetCtrl->rtnMsgID.IDs.MsgCode)
				{
				case NetCtrlCode_Disconnect:
					if (GetConnectionState() == IConnection::STATE_DISCONNECTING || GetConnectionState() == IConnection::STATE_CONNECTED)
					{
						netTrace(TRC_CONNECTION, "RECV Disconnected CID:{0}", GetCID());
						netChk(CloseConnection());
					}
					break;
				case NetCtrlCode_Connect:
					if (GetConnectionState() == IConnection::STATE_CONNECTING) // we need to receive remote class
					{
						// put expected net class
						m_ConnectInfo.RemoteClass = NetClass::Game;
						OnConnectionResult(ResultCode::SUCCESS);
					}
					break;
				case NetCtrlCode_HeartBit:
					m_ulNetCtrlTime = Util::Time.GetTimeMs();
					m_ulNetCtrlTryTime = m_ulNetCtrlTime;
					break;
				default:
					netTrace(Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote);
					netChk(Disconnect("Invalid packet"));
					netErr(ResultCode::E_NET_BADPACKET_NOTEXPECTED);
					break;
				};
			}
			else // general message
			{
				netTrace(Trace::TRC_WARN, "HackWarn : Invalid packet CID:{0}, Addr {1}", GetCID(), GetConnectionInfo().Remote);
				netChk(Disconnect("Invalid packet"));
				netErr(ResultCode::E_NET_BADPACKET_NOTEXPECTED);
			}
			break;
		default:
			hr = ConnectionMUDP::ProcNetCtrl(pNetCtrl);
			break;
		}

	Proc_End:

		return hr;
	}


	Result ConnectionMUDPClient::InitConnection(SOCKET socket, const ConnectionInformation &connectInfo)
	{
		Result hr = ConnectionUDPBase::InitConnection(socket, connectInfo);
		if (!(hr)) return hr;

		// Clear local ID, MUDP server will expect peerID is zero on initial connection
		m_ConnectInfo.LocalID = 0;

		return hr;
	}

	// Process network control message
	Result ConnectionMUDPClient::ProcConnectionState()
	{
		Result hr = ResultCode::SUCCESS;
		Message::MessageID msgIDTem;
		TimeStampMS ulTimeCur = Util::Time.GetTimeMs();

		assert(ThisThread::GetThreadID() == GetRunningThreadID());

		hr = ConnectionMUDP::ProcConnectionState();
		if (!(hr)) return hr;


		// Update connection status
		msgIDTem.ID = PACKET_NETCTRL_NONE;
		switch (GetConnectionState())
		{
		case IConnection::STATE_CONNECTING:
			if ((INT)(ulTimeCur - m_ulNetCtrlTime).count() > (INT)GetConnectingTimeOut()) // connection time out
			{
				netTrace(TRC_CONNECTION, "UDP Connecting Timeout CID:{0}, ({1},{2},{3})", GetCID(), ulTimeCur, m_ulNetCtrlTime, GetConnectingTimeOut());
				netChk(CloseConnection());
			}
			else if ((INT)(ulTimeCur - m_ulNetCtrlTryTime).count() > Const::CONNECTION_RETRY_TIME) // retry
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netTrace(TRC_NETCTRL, "UDP Send Connecting CID({0}) : C:{1}, V:{2})", GetCID(), GetConnectionInfo().LocalClass, (UINT32)BR_PROTOCOL_VERSION);
				netChk(SendPending(PACKET_NETCTRL_CONNECT, (UINT)GetConnectionInfo().LocalClass, Message::MessageID(BR_PROTOCOL_VERSION), GetConnectionInfo().LocalID));
			}

			break;
		case IConnection::STATE_CONNECTED:
			if ((INT)(ulTimeCur - m_ulNetCtrlTime).count() > Const::HEARTBIT_TIMEOUT) // connection time out
			{
				netTrace(TRC_CONNECTION, "UDP Connection Timeout CID:{0}, ({1},{2})", GetCID(), ulTimeCur, m_ulNetCtrlTime);
				netChk(CloseConnection());
				goto Proc_End;
			}
			else if ((INT)(ulTimeCur - m_ulNetCtrlTryTime).count() > (INT)GetHeartbitTry()) // heartbit time
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk(SendPending(PACKET_NETCTRL_HEARTBIT, 0, msgIDTem));
			}
			break;
		case IConnection::STATE_DISCONNECTING:
			if ((INT)(ulTimeCur - m_ulNetCtrlTime).count() > Const::DISCONNECT_TIMEOUT) // connection time out
			{
				netTrace(TRC_CONNECTION, "UDP Disconnecting Timeout CID:{0}, ({1},{2})", GetCID(), ulTimeCur, m_ulNetCtrlTime);
				netChk(CloseConnection());
			}
			else if ((INT)(ulTimeCur - m_ulNetCtrlTryTime).count() > Const::DISCONNECT_RETRY_TIME) // retry
			{
				m_ulNetCtrlTryTime = ulTimeCur;
				netChk(SendPending(PACKET_NETCTRL_DISCONNECT, 0, msgIDTem));
			}

			break;
		default:
			break;
		};


	Proc_End:

		return hr;
	}


	void ConnectionMUDPClient::CloseSocket()
	{
		if (GetSocket() == INVALID_SOCKET) return;

		NetSystem::UnregisterSocket(this);

		Connection::CloseSocket();
	}


	// called when New connection TCP accepted
	Result ConnectionMUDPClient::Recv(IOBUFFER_READ* pIOBuffer)
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;

		netChkPtr(pIOBuffer);

		pIOBuffer->SetupRecvUDP(GetCID());

		hrErr = NetSystem::RecvFrom(GetSocket(), pIOBuffer);
		switch ((int64_t)hrErr)
		{
		case ResultCode::SUCCESS_FALSE:
			hr = ResultCode::E_NET_TRY_AGAIN;
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
			// some remove has problem with connection
			netTrace(TRC_NETCTRL, "UDP Remote has connection error err={0:X8}, {1}", hrErr, pIOBuffer->NetAddr.From);
		default:
			// Unknown error
			netTrace(Trace::TRC_ERROR, "UDP Read failed err={0:X8}", hrErr);
			netErr(hrErr);
			break;
		}

	Proc_End:

		return hr;
	}

	// called when reciving TCP message
	Result ConnectionMUDPClient::OnIORecvCompleted(Result hrRes, IOBUFFER_READ* &pIOBuffer)
	{
		Result hr = ResultCode::SUCCESS;

		if (pIOBuffer != nullptr && pIOBuffer->Operation != IOBUFFER_OPERATION::OP_UDPREAD)
		{
			netErr(ResultCode::UNEXPECTED);
		}

		DecPendingRecvCount();

		if ((hrRes))
		{
			netChkPtr(pIOBuffer);

			if (!(hr = OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer)))
				netTrace(TRC_RECVRAW, "Read IO failed with CID {0}, hr={1:X8}", GetCID(), hr);

			PendingRecv();

			//SendFlush();
		}
		else
		{
			// TODO: need to mark close connection
			Disconnect("Recv failed");
		}


	Proc_End:

		if (!NetSystem::IsProactorSystem())
		{
			Util::SafeDelete(pIOBuffer);
		}

		return hr;
	}

	Result ConnectionMUDPClient::EnqueueBufferUDP(IOBUFFER_WRITE *pSendBuffer)
	{
		return EnqueueBuffer(pSendBuffer);
	}

	Result ConnectionMUDPClient::SendBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		return SendBufferUDP(pSendBuffer);
	}


	Result ConnectionMUDPClient::OnSendReady()
	{
		if (GetEventHandler())
			return GetEventHandler()->OnNetSendReadyMessage(this);
		// process directly
		else
			return ProcessSendQueue();
	}


	// called when Send completed
	Result ConnectionMUDPClient::OnIOSendCompleted(Result hrRes, IOBUFFER_WRITE *pIOBuffer)
	{
		NetSystem::FreeGatheringBuffer(pIOBuffer->pSendBuff);
		Util::SafeRelease(pIOBuffer->pMsgs);
		NetSystem::FreeBuffer(pIOBuffer);
		return ResultCode::SUCCESS;
	}


	// Pending recv New one
	Result ConnectionMUDPClient::PendingRecv()
	{
		Result hr = ResultCode::SUCCESS, hrErr = ResultCode::SUCCESS;
		IOBUFFER_READ *pOver = nullptr;

		if (!NetSystem::IsProactorSystem())
			return ResultCode::SUCCESS;

		IncPendingRecvCount();


		while (1)
		{
			pOver = new IOBUFFER_READ;
			hrErr = Recv(pOver);
			switch ((uint32_t)hrErr)
			{
			case (uint32_t)ResultCode::SUCCESS:
			case (uint32_t)ResultCode::E_NET_IO_PENDING:
			case (uint32_t)ResultCode::E_NET_TRY_AGAIN:
			case (uint32_t)ResultCode::E_NET_WOULDBLOCK:
				pOver = nullptr;
				goto Proc_End;// success
				break;
			default:
				Disconnect("Failed to pending recv");
				goto Proc_End;
			}
		}


	Proc_End:

		Util::SafeDelete(pOver);

		return hr;
	}




} // namespace Net
} // namespace BR


