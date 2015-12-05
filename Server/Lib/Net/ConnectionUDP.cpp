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

BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionUDPServerPeer);
BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionUDPServer);
BR_MEMORYPOOL_IMPLEMENT(Net::ConnectionUDPClient);

namespace BR {
namespace Net {

	

	


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server UDP Network connection class
	//

	// Constructor
	ConnectionUDPServerPeer::ConnectionUDPServerPeer()
		: ConnectionUDP()
	{
		// limit server net retry maximum
		SetHeartbitTry( Const::SVR_HEARTBIT_TIME_PEER );
		SetMaxGuarantedRetry( Const::UDP_CLI_RETRY_ONETIME_MAX );
	}

	ConnectionUDPServerPeer::~ConnectionUDPServerPeer()
	{
		m_RecvReliableWindow.ClearWindow();
		m_SendReliableWindow.ClearWindow();
	}


	// Initialize packet synchronization
	HRESULT ConnectionUDPServerPeer::InitSynchronization()
	{
		HRESULT hr = S_OK;

		netChk(ConnectionUDP::InitSynchronization() );

		m_RecvReliableWindow.ClearWindow();
		m_SendReliableWindow.ClearWindow();

	Proc_End:


		return hr;
	}
	
	// Process network control message
	HRESULT ConnectionUDPServerPeer::ProcNetCtrl( const MsgNetCtrl* pNetCtrl )
	{
		HRESULT hr = S_OK;


		svrChk(ConnectionUDP::ProcNetCtrl( pNetCtrl ) );


		switch( pNetCtrl->msgID.IDs.MsgCode )
		{
		case NetCtrlCode_Connect:
			switch (GetConnectionState())
			{
			case IConnection::STATE_CONNECTED:
				// This case could be a reconnected case while  this connection didn't realized the disconnect
				//InitSynchronization();
				break;
			default:
				break;
			};
		default:
			break;
		};


	Proc_End:

		return hr;
	}


	// Update net control, process connection heartbit, ... etc
	HRESULT ConnectionUDPServerPeer::UpdateNetCtrl()
	{
		HRESULT hr = S_OK;

		netChk(ConnectionUDP::UpdateNetCtrl() );


	Proc_End:

		SendFlush();

		return hr;
	}





	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server UDP Network connection class
	//

	// Constructor
	ConnectionUDPServer::ConnectionUDPServer()
		: ConnectionUDP()
	{
		// limit server net retry maximum
		SetHeartbitTry( Const::SVR_HEARTBIT_TIME_UDP );
		SetMaxGuarantedRetry( Const::UDP_CLI_RETRY_ONETIME_MAX );
	}

	ConnectionUDPServer::~ConnectionUDPServer()
	{
		ClearQueues();
	}


	// Update net control, process connection heartbit, ... etc
	HRESULT ConnectionUDPServer::UpdateNetCtrl()
	{
		HRESULT hr = S_OK;

		netChk(ConnectionUDP::UpdateNetCtrl() );


	Proc_End:

		SendFlush();

		return hr;
	}

	




	////////////////////////////////////////////////////////////////////////////////
	//
	//	client UDP Network connection class
	//

	// Constructor
	ConnectionUDPClient::ConnectionUDPClient( )
		:ConnectionUDP()
	{
		// limit client net retry maximum
		SetMaxGuarantedRetry( Const::UDP_CLI_RETRY_ONETIME_MAX );

		SetLocalClass( NetClass::Client );

		// We use write queue in INetIOCallBack, however, assign same queue, just in case
		SetWriteQueue(new WriteBufferQueue);
	}

	ConnectionUDPClient::~ConnectionUDPClient()
	{
		ClearQueues();

		if (GetWriteQueue()) delete GetWriteQueue();
	}


	// called when New connection TCP accepted
	HRESULT ConnectionUDPClient::Recv(IOBUFFER_READ* pIOBuffer)
	{
		HRESULT hr = S_OK, hrErr = S_OK;

		netChkPtr(pIOBuffer);
		//pIOBuffer = GetRecvBuffer();
		pIOBuffer->SetupRecvUDP(GetCID());

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

	// called when reciving TCP message
	HRESULT ConnectionUDPClient::OnIORecvCompleted( HRESULT hrRes, IOBUFFER_READ *pIOBuffer )
	{
		HRESULT hr = S_OK;

		if( pIOBuffer->Operation != IOBUFFER_OPERATION::OP_UDPREAD )
		{
			netErr(E_UNEXPECTED);
		}

		DecPendingRecvCount();

		if( SUCCEEDED(hrRes) )
		{

			if( FAILED( hr = OnRecv(pIOBuffer->TransferredSize, (BYTE*)pIOBuffer->buffer ) ) )
				netTrace( TRC_RECVRAW, "Read IO failed with CID {0}, hr={1:X8}", GetCID(), hr );

			PendingRecv();

			SendFlush();
		}
		else
		{
			// TODO: need to mark close connection
			Disconnect();
		}


	Proc_End:

		return hr;
	}

	HRESULT ConnectionUDPClient::EnqueueBufferUDP(IOBUFFER_WRITE *pSendBuffer)
	{
		return EnqueueBuffer(pSendBuffer);
	}

	HRESULT ConnectionUDPClient::SendBuffer(IOBUFFER_WRITE *pSendBuffer)
	{
		return SendBufferUDP(pSendBuffer);
	}

	// called when Send completed
	HRESULT ConnectionUDPClient::OnIOSendCompleted( HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer )
	{
		NetSystem::FreeGatheringBuffer(pIOBuffer->pSendBuff);
		Util::SafeRelease( pIOBuffer->pMsgs );
		NetSystem::FreeBuffer( pIOBuffer );
		return S_OK;
	}
	
	// Pending recv New one
	HRESULT ConnectionUDPClient::PendingRecv()
	{
		if (!NetSystem::IsProactorSystem())
			return S_OK;

		IncPendingRecvCount();

		HRESULT hr = S_OK, hrErr = S_OK;

		while(1)
		{
			IOBUFFER_READ *pOver = GetRecvBuffer();
			hrErr = Recv(pOver);
			switch (hrErr)
			{
			case S_OK:
			case E_NET_IO_PENDING:
			case E_NET_TRY_AGAIN:
			case E_NET_WOULDBLOCK:
				goto Proc_End;// success
				break;
			default:
				Disconnect();
				goto Proc_End;
			}
		}


	Proc_End:

		return hr;
	}

	
	// Initialize connection
	HRESULT ConnectionUDPClient::InitConnection( SOCKET socket, const ConnectionInformation &connectInfo )
	{
		HRESULT hr = ConnectionUDP::InitConnection( socket, connectInfo );
		SetLocalClass( NetClass::Client );
		return hr;
	}


	// Reinitialize and set remote address
	HRESULT ConnectionUDPClient::ReInitialize( const sockaddr_in6& socAddr )
	{
		SetConnectionState(STATE_CONNECTING);

		UpdateConnectionTime();// m_tConnectionTime = Util::Time.GetTimeMs();

		ChangeRemoteAddress( socAddr );

		m_ulNetCtrlTryTime = Util::Time.GetTimeMs();
		m_ulNetCtrlTime = Util::Time.GetTimeMs();

		SetLocalClass( NetClass::Client );

		return S_OK;
	}


	// Update net control, process connection heartbit, ... etc
	HRESULT ConnectionUDPClient::UpdateNetCtrl()
	{
		HRESULT hr = S_OK;


		//netChk(IConnection::UpdateNetCtrl() );


	//Proc_End:


		SendFlush();

		return hr;
	}


} // namespace Net
} // namespace BR


