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
		HRESULT hr = S_OK;
		MsgMobileNetCtrl *pNetCtrl = NULL;
		Message::MessageData *pMsg = NULL;

		netMem( pMsg = Message::MessageData::NewMessage( uiCtrlCode, sizeof(MsgMobileNetCtrl) ) );

		pNetCtrl = (MsgMobileNetCtrl*)pMsg->GetMessageBuff();
		pNetCtrl->PeerID = UID;
		pNetCtrl->msgID.IDSeq.Sequence = uiSequence;
		pNetCtrl->rtnMsgID = msgID;

		pMsg->GetMessageHeader()->msgID.IDs.Mobile = true;
		pMsg->UpdateChecksum();

		HRESULT hrTem = SendMsg( nullptr, dstAddress, pMsg );
		if( FAILED(hrTem) )
		{
			netTrace( TRC_GUARREANTEDCTRL, "NetCtrl Send failed in direct: DstAddr:%0%, msg:%1%, seq:%2%, hr=%3%", 
							dstAddress, 
							BR::Arg<UINT32>(msgID.ID,-1,16), 
							uiSequence, 
							BR::Arg<UINT32>(hrTem,-1,16) );

			// ignore io send fail except connection closed
			if( hrTem == E_NET_CONNECTION_CLOSED )
			{
				goto Proc_End;
			}
		}

	Proc_End:

		return hr;
	}


	// called when reciving message
	HRESULT ServerMUDP::OnIORecvCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred )
	{
		HRESULT hr = S_OK;
		SharedPointerT<Connection> pConnection;
		IConnection::ConnectionInformation connectionInfo;
		Message::MessageData *pIMsg = nullptr;
		//bool bReleaseOnFail = false;


		if( FAILED( hrRes ) )
		{
			switch( hrRes )
			{
			case E_NET_CONNECTION_CLOSED:
			case E_NET_IO_ABORTED:
				//if( SUCCEEDED( m_ConnectionManager.GetConnectionByAddr( pIOBuffer->From, iterCon ) ) )
				//{
					// This error is no more the reason to disconnect a user, just report it
					netTrace( TRC_RECV, "UDP bad connection state IP:%0%", pIOBuffer->From );
				//}
				hr = hrRes;
				break;
			default:
				netTrace( Trace::TRC_ERROR, "UDP Recv Msg Failed, SvrMUDP, IP:%0%, hr=%1%", pIOBuffer->From, ArgHex32(hrRes) );
				break;
			};
		}
		else
		{
			if( dwTransferred < sizeof(Message::MobileMessageHeader) )// invalid packet size
				goto Proc_End;

			Message::MobileMessageHeader *pHeader = (Message::MobileMessageHeader*)pIOBuffer->wsaBuff.buf;
			if( !pHeader->msgID.IDs.Mobile )
			{
				netTrace( Trace::TRC_WARN, "HackWarn : Not allowered connection try from %0%", pIOBuffer->From );
				goto Proc_End;
			}

			if (pHeader->PeerID != 0)
			{
				m_ConnectionManager.GetConnectionByPeerID(pHeader->PeerID, pConnection);
			}

			if (pConnection == nullptr)
			{
				GetConnectionManager().GetConnectionByAddr(pIOBuffer->From, pConnection);
			}

			if (pConnection == nullptr)
			{
				if (GetIsEnableAccept())
				{
					MsgMobileNetCtrl *pNetCtrl = (MsgMobileNetCtrl*)pIOBuffer->wsaBuff.buf;
					if (pHeader->Length == sizeof(MsgMobileNetCtrl)
						&& pHeader->msgID.IDSeq.MsgID == PACKET_NETCTRL_CONNECT.IDSeq.MsgID
						&& pNetCtrl->rtnMsgID.ID == BR::PROTOCOL_VERSION)
					{
						netChk(m_ConnectionManager.PendingNewConnection(pIOBuffer->From, pNetCtrl));
					}
					else if (pNetCtrl->msgID.ID == PACKET_NETCTRL_ACK)
					{
						// ignore this packet
					}
					else
					{
						// send disconnect
						netTrace(TRC_SENDRAW, "Invalid incomming address. Try to disconnect %0%", pIOBuffer->From);
						netChk(SendNetCtrl(pIOBuffer->From, PACKET_NETCTRL_DISCONNECT, 0, PACKET_NETCTRL_NONE, 0));
					}
					goto Proc_End;
				}
				else
				{
					// Ignore this case
					// TODO: We need to record of these case because it could be a hacking try.
					goto Proc_End;
				}
			}

			netAssert( pHeader->PeerID == 0 || pConnection->GetConnectionInfo().RemoteID == 0 || pConnection->GetConnectionInfo().RemoteID == pHeader->PeerID );
			if( pConnection->GetConnectionInfo().RemoteID == 0 && pHeader->PeerID != 0 )
			{
				netTrace( TRC_CONNECTION, "Mapping CID: %0%, PeerID %1%", pConnection->GetCID(), pHeader->PeerID );
				netChk( m_ConnectionManager.PendingRemapPeerID((Connection*)pConnection, pHeader->PeerID) );
			}

			// Update sock address table if need, This is important because all the returning ticket will be use it
			if( pConnection->GetRemoteSockAddr() != pIOBuffer->From )
			{
				netTrace(TRC_CONNECTION, "Remapping Address CID: %0%, Address from:%1%, to:%2%", pConnection->GetCID(), pConnection->GetRemoteSockAddr(), pIOBuffer->From);
				if (GetIsEnableAccept())
				{
					// address map update. We can skip this when we don't allow unregistered connection because we are going to update the remote address in place
					// FYI, I think this update only required when login + game server, otherwise This will only need for connection manager for releasing connections
					netChk(m_ConnectionManager.PendingAddressRemap((Connection*)pConnection, pConnection->GetRemoteSockAddr(), pIOBuffer->From));
				}
				// in-place address change
				((ConnectionUDPBase*)(Connection*)pConnection)->ChangeRemoteAddress(pIOBuffer->From);
			}

			netChk( pConnection->OnRecv( dwTransferred, (BYTE*)pIOBuffer->wsaBuff.buf ) );
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

		if( hrRes != E_NET_IO_ABORTED )
			PendingRecv( (OVERLAPPED_BUFFER_READ*)pIOBuffer );

		return hr;
	}
	

} // namespace Net
} // namespace BR

