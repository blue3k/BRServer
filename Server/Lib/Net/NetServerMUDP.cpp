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

	
	// Make Ack packet and enqueue to SendNetCtrlqueue
	HRESULT ServerMUDP::SendNetCtrl( const sockaddr_in6& dstAddress, UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID )
	{
		HRESULT hr = S_OK, hrTem = S_OK;
		MsgMobileNetCtrl *pNetCtrl = NULL;
		Message::MessageData *pMsg = NULL;

		netMem( pMsg = Message::MessageData::NewMessage( uiCtrlCode, sizeof(MsgMobileNetCtrl) ) );

		pNetCtrl = (MsgMobileNetCtrl*)pMsg->GetMessageBuff();
		pNetCtrl->PeerID = UID;
		pNetCtrl->msgID.IDSeq.Sequence = uiSequence;
		pNetCtrl->rtnMsgID = msgID;

		pMsg->GetMessageHeader()->msgID.IDs.Mobile = true;
		pMsg->UpdateChecksum();

		hrTem = SendMsg( nullptr, dstAddress, pMsg );
		if( FAILED(hrTem) )
		{
			netTrace( TRC_GUARREANTEDCTRL, "NetCtrl Send failed in direct: DstAddr:{0}, msg:{1:X8}, seq:{2}, hr={3:X8}", 
							dstAddress, 
							msgID.ID, 
							uiSequence, 
							hrTem );

			// ignore io send fail except connection closed
			if( hrTem == E_NET_CONNECTION_CLOSED )
			{
				goto Proc_End;
			}
		}

	Proc_End:

		return hr;
	}

	HRESULT ServerMUDP::OnNoConnectionPacket(const struct sockaddr_in6& from, const BYTE* pData)
	{
		HRESULT hr = S_OK;

		if (pData == nullptr)
			return E_INVALIDARG;

		MsgMobileNetCtrl *pNetCtrl = (MsgMobileNetCtrl*)pData;
		if (pNetCtrl->Length != sizeof(MsgMobileNetCtrl) 
			&& pNetCtrl->Length != sizeof(MsgMobileNetCtrlSync) 
			&& pNetCtrl->Length != sizeof(Message::MobileMessageHeader))
		{
			// send disconnect
			netTrace(Trace::TRC_WARN, "Invalid incomming packet size. ignoring from:{0} msgID:{1}, len:{2}", from, pNetCtrl->msgID, pNetCtrl->Length);
			return E_NET_BADPACKET_SIZE;
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

			return S_FALSE;
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
	HRESULT ServerMUDP::OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ *pIOBuffer )
	{
		HRESULT hr = S_OK;
		SharedPointerT<Connection> pConnection;
		IConnection::ConnectionInformation connectionInfo;
		sockaddr_in6 from;
		if (pIOBuffer != nullptr) from = pIOBuffer->NetAddr.From;
		else memset(&from, 0, sizeof(from));

		if( FAILED( hrRes ) )
		{

			switch( hrRes )
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
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

		if (hrRes != E_NET_IO_ABORTED && pIOBuffer != nullptr)
			PendingRecv( (IOBUFFER_READ*)pIOBuffer );

		return hr;
	}
	

} // namespace Net
} // namespace BR

