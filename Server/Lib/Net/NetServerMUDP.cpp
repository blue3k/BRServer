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
	//	Mobile UDP Server class
	//

	
	ServerMUDP::ServerMUDP(ServerID InServerID, NetClass localClass)
		:ServerUDPBase(InServerID, localClass)
		,m_ConnectionManager(Const::SVR_PUBLIC_CONNECTION_BUCKET_SIZE)
	{
		m_ConnectionManager.SetNetOwner( this );
	}

	ServerMUDP::~ServerMUDP()
	{
	}

	Result ServerMUDP::SendRaw(const sockaddr_storage& dstAddress, Message::MessageData* &pMsg)
	{
		Result hr = ResultCode::SUCCESS;
		IOBUFFER_WRITE *pSendBuffer = nullptr;

		netChkPtr(pMsg);

		netChk(Net::NetSystem::AllocBuffer(pSendBuffer));
		pSendBuffer->SetupSendUDP(GetSocket(), dstAddress, pMsg);

		if (NetSystem::IsProactorSystem())
		{
			netChk(SendBuffer(pSendBuffer));
		}
		else
		{
			netChk(EnqueueBuffer(pSendBuffer));
		}
		pMsg = nullptr;
		pSendBuffer = nullptr;

	Proc_End:

		if (pSendBuffer != nullptr)
		{
			NetSystem::FreeBuffer(pSendBuffer);
		}

		return hr;
	}

	// Make Ack packet and enqueue to SendNetCtrlqueue
	Result ServerMUDP::SendNetCtrl( const sockaddr_storage& dstAddress, UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID )
	{
		Result hr = ResultCode::SUCCESS, hrTem = ResultCode::SUCCESS;
		MsgMobileNetCtrl *pNetCtrl = NULL;
		Message::MessageData *pMsg = NULL;

		netMem( pMsg = Message::MessageData::NewMessage( uiCtrlCode, sizeof(MsgMobileNetCtrl) ) );

		pNetCtrl = (MsgMobileNetCtrl*)pMsg->GetMessageBuff();
		pNetCtrl->PeerID = UID;
		pNetCtrl->msgID.IDSeq.Sequence = uiSequence;
		pNetCtrl->rtnMsgID = msgID;

		pMsg->GetMessageHeader()->msgID.IDs.Mobile = true;
		pMsg->UpdateChecksum();

		hrTem = SendRaw(dstAddress, pMsg);
		//hrTem = SendMsg( nullptr, dstAddress, pMsg );
		if( !(hrTem) )
		{
			netTrace( TRC_GUARREANTEDCTRL, "NetCtrl Send failed in direct: DstAddr:{0}, msg:{1:X8}, seq:{2}, hr={3:X8}", 
							dstAddress, 
							msgID.ID, 
							uiSequence, 
							hrTem );

			// ignore io send fail except connection closed
			if( hrTem == Result(ResultCode::E_NET_CONNECTION_CLOSED) )
			{
				goto Proc_End;
			}
		}

	Proc_End:

		return hr;
	}

	Result ServerMUDP::OnNoConnectionPacket(const struct sockaddr_storage& from, const BYTE* pData)
	{
		Result hr = ResultCode::SUCCESS;

		if (pData == nullptr)
			return ResultCode::INVALID_ARG;

		MsgMobileNetCtrl *pNetCtrl = (MsgMobileNetCtrl*)pData;
		if (pNetCtrl->Length != sizeof(MsgMobileNetCtrl) 
			&& pNetCtrl->Length != sizeof(MsgMobileNetCtrlSync) 
			&& pNetCtrl->Length != sizeof(Message::MobileMessageHeader))
		{
			// send disconnect
			netTrace(Trace::TRC_WARN, "Invalid incomming packet size. ignoring from:{0} msgID:{1}, len:{2}", from, pNetCtrl->msgID, pNetCtrl->Length);
			return ResultCode::E_NET_BADPACKET_SIZE;
		}

		if (!GetIsEnableAccept())
		{
			if (pNetCtrl->msgID.GetMsgID() == PACKET_NETCTRL_CONNECT.GetMsgID()
				|| pNetCtrl->msgID.GetMsgID() == PACKET_NETCTRL_SYNCRELIABLE.GetMsgID()
				|| pNetCtrl->msgID.GetMsgID() == PACKET_NETCTRL_HEARTBIT.GetMsgID()
				|| pNetCtrl->msgID.GetMsgID() == PACKET_NETCTRL_TIMESYNC.GetMsgID())
			{
				// send disconnect
				//netTrace(Trace::TRC_WARN, "Invalid packet size. Try to disconnect from:{0} msg:{1}", from, pNetCtrl->msgID);
				netChk(SendNetCtrl(from, PACKET_NETCTRL_DISCONNECT, 0, PACKET_NETCTRL_NONE, 0));
			}
			else
			{
				netTrace(Trace::TRC_WARN, "Invalid packet size. Try to disconnect from:{0} msg:{1}", from, pNetCtrl->msgID);
			}

			return ResultCode::SUCCESS_FALSE;
		}


		// Handle new connection
		if (pNetCtrl->msgID.GetMsgID() != PACKET_NETCTRL_CONNECT.GetMsgID())
		{
			// send disconnect
			netTrace(TRC_SENDRAW, "Invalid incomming packet. Try to disconnect {0}", from);
			netChk(SendNetCtrl(from, PACKET_NETCTRL_DISCONNECT, 0, PACKET_NETCTRL_NONE, 0));
		}
		else if (pNetCtrl->rtnMsgID.ID != BR_PROTOCOL_VERSION)
		{
			// send disconnect
			netTrace(TRC_SENDRAW, "Invalid incomming packet version, received:{0}, expected:{1}. Try to disconnect {2}", pNetCtrl->rtnMsgID.ID, (UINT)BR_PROTOCOL_VERSION, from);
			netChk(SendNetCtrl(from, PACKET_NETCTRL_NACK, 0, pNetCtrl->msgID, 0));
			//netChk(SendNetCtrl(pIOBuffer->From, PACKET_NETCTRL_DISCONNECT, 0, PACKET_NETCTRL_NONE, 0));
		}
		else if (pNetCtrl->msgID.ID == PACKET_NETCTRL_ACK)
		{
			// ignore this packet

		}
		else
		{
			netChk(m_ConnectionManager.PendingNewConnection(from, pNetCtrl));
		}

	Proc_End:

		return hr;
	}

	// called when reciving message
	Result ServerMUDP::OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer )
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<Connection> pConnection;
		IConnection::ConnectionInformation connectionInfo;
		sockaddr_storage from;
		if (pIOBuffer != nullptr) from = pIOBuffer->NetAddr.From;
		else memset(&from, 0, sizeof(from));

		if( !( hrRes ) )
		{

			switch((uint32_t)hrRes )
			{
			case ResultCode::E_NET_CONNECTION_CLOSED:
			case ResultCode::E_NET_IO_ABORTED:
				// This error is no more the reason to disconnect a user, just report it
				netTrace( TRC_RECV, "UDP bad RECV IP:{0}", from);
				hr = hrRes;
				break;
			default:
				netTrace( Trace::TRC_ERROR, "UDP Recv Msg Failed, SvrMUDP, IP:{0}, hr={1:X8}", from, hrRes );
				break;
			};
		}
		else if(pIOBuffer != nullptr)
		{
			if(pIOBuffer->TransferredSize < sizeof(Message::MobileMessageHeader) )// invalid packet size
				goto Proc_End;

			Message::MobileMessageHeader *pHeader = (Message::MobileMessageHeader*)pIOBuffer->buffer;
			if( !pHeader->msgID.IDs.Mobile )
			{
				netTrace( Trace::TRC_WARN, "HackWarn : Not allowered connection try from {0}", from);
				goto Proc_End;
			}

			if (pHeader->PeerID != 0)
			{
				m_ConnectionManager.GetConnectionByPeerID(pHeader->PeerID, pConnection);
			}

			if (pConnection == nullptr)
			{
				GetConnectionManager().GetConnectionByAddr(from, pConnection);
			}

			if (pConnection == nullptr)
			{
				OnNoConnectionPacket(pIOBuffer->NetAddr.From, (const BYTE*)pIOBuffer->buffer);
				goto Proc_End;
			}

			netAssert( pHeader->PeerID == 0 || pConnection->GetConnectionInfo().RemoteID == 0 || pConnection->GetConnectionInfo().RemoteID == pHeader->PeerID );
			if( pConnection->GetConnectionInfo().RemoteID == 0 && pHeader->PeerID != 0 )
			{
				netTrace( TRC_CONNECTION, "Mapping CID: {0}, PeerID {1}", pConnection->GetCID(), pHeader->PeerID );
				netChk( m_ConnectionManager.PendingRemapPeerID((Connection*)pConnection, pHeader->PeerID) );
			}

			// Update sock address table if need, This is important because all the returning ticket will be use it
			if( pConnection->GetRemoteSockAddr() != pIOBuffer->NetAddr.From )
			{
				netTrace(TRC_CONNECTION, "Remapping Address CID: {0}, Address from:{1}, to:{2}", pConnection->GetCID(), pConnection->GetRemoteSockAddr(), pIOBuffer->NetAddr.From);
				if (GetIsEnableAccept())
				{
					// address map update. We can skip this when we don't allow unregistered connection because we are going to update the remote address in place
					// FYI, I think this update only required when login + game server, otherwise This will only need for connection manager for releasing connections
					netChk(m_ConnectionManager.PendingAddressRemap((Connection*)pConnection, pConnection->GetRemoteSockAddr(), pIOBuffer->NetAddr.From));
				}
				// in-place address change
				((ConnectionUDPBase*)(Connection*)pConnection)->ChangeRemoteAddress(pIOBuffer->NetAddr.From);
			}

			netChk( pConnection->OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer) );
			// Keep these connection can be readable until Onrecv is done
			//iterCon = nullptr;
			//iterPIDCon = nullptr;
		}

	Proc_End:

		//if( pConnection != nullptr )
		//{
		//	if( bReleaseOnFail )
		//	{
		//		if( pConnection->GetState() != IConnection::STATE_DISCONNECTED )
		//			pConnection->CloseConnection();
		//		ReleaseConnection( (Connection*)pConnection );
		//	}
		//}

		if (NetSystem::IsProactorSystem())
		{
			if (hrRes != Result(ResultCode::E_NET_IO_ABORTED) && pIOBuffer != nullptr)
			{
				pIOBuffer->SetPendingFalse();
				PendingRecv(pIOBuffer);
			}
		}
		else
		{
			Util::SafeDelete(pIOBuffer);
		}

		return hr;
	}
	

} // namespace Net
} // namespace BR

