////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net Server implementation 
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



namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server base class
	//

	Server::Server(ServerID InServerID, NetClass localClass)
		:ServerNet( InServerID, localClass )
	{
	}

	Server::~Server()
	{
	}

	
	// check about initialize
	bool Server::IsReady()
	{
		return GetSocket() != INVALID_SOCKET;
	}

	// Release instance
	void Server::Release()
	{
		delete this;
	}





	// Get connection from connection ID
	HRESULT Server::GetConnection(UINT_PTR uiCID, SharedPointerT<Connection> &pConn)
	{
		return GetConnectionManager().GetConnectionByCID( uiCID, pConn );
	}



	// Open host and start listen
	HRESULT Server::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		HRESULT hr = S_OK;
		NetAddress localAddr;

		netChk( WSASystem::OpenSystem( Const::SVR_OVERBUFFER_COUNT ) );

		m_NetClass = netCls;

		netChk( StrUtil::StringCpy( localAddr.strAddr, strLocalIP ) );
		localAddr.usPort = usLocalPort;

		SetLocalAddress( localAddr );

		netChk( IOCPSystem::GetSystem().InitIOCP( Const::SVR_NUM_RECV_THREAD ) );


	Proc_End:


		return hr;
	}

	// Close host and close all connections
	HRESULT Server::HostClose()
	{
		HRESULT hr = S_OK;

		GetConnectionManager().TerminateManager();

		netChk( IOCPSystem::GetSystem().CloseIOCP() );

		WSASystem::CloseSystem();

		if( GetSocket() != INVALID_SOCKET )
		{
			closesocket( GetSocket() );
			SetSocket( INVALID_SOCKET );
		}

	Proc_End:



		return hr;
	}



	// Release Connection, Make connection to send free state
	HRESULT Server::ReleaseConnection( IConnection* pIConnection )
	{
		HRESULT hr = S_OK;
		Connection* pConn = (Connection*)pIConnection;

		netChkPtr( pIConnection );

		netTrace(TRC_CONNECTION, "ReleaseConnection LPort:%0%, CID:%1%, Connection Count : %2%", GetLocalAddress().usPort, pIConnection->GetCID(), GetConnectionManager().GetNumActiveConnection());
		netChk( GetConnectionManager().PendingReleaseConnection( pConn ) );

	Proc_End:

		if( FAILED(hr) )
		{
			netTrace( Trace::TRC_ERROR, "ReleaseConnection Failed hr={0:X8}", hr );
		}

		return hr;
	}

	// take over connection management
	HRESULT Server::TakeOverConnection(IConnection* pIConnection)
	{
		HRESULT hr = S_OK;
		Connection* pConn = (Connection*)pIConnection;

		netChkPtr(pIConnection);

		if (GetIsEnableAccept())
		{
			netChk(GetConnectionManager().PendingManagedConnectionTakenOver(pConn));
		}

	Proc_End:

		if (FAILED(hr))
		{
			netTrace(Trace::TRC_ERROR, "ReleaseConnection Failed hr={0:X8}", hr);
		}

		return hr;
	}

	// Called when connection state changed
	HRESULT Server::OnConnectionStateChange( IConnection *pIConnection )
	{
		HRESULT hr = S_OK;

		netTrace(TRC_CONNECTION, "Net Con state Port:%0%, CID:%1%, state:%2%", GetLocalAddress().usPort, pIConnection->GetCID(), pIConnection->GetConnectionState());

		switch (pIConnection->GetConnectionState())
		{
		case IConnection::STATE_CONNECTING:
			break;
		case IConnection::STATE_CONNECTED:
			break;
		case IConnection::STATE_DISCONNECTED:
			EnqueueNetEvent( INet::Event( INet::Event::EVT_CONNECTION_DISCONNECTED ) );
			break;
		case IConnection::STATE_DISCONNECTING:
			break;
		};

	//Proc_End:

		return hr;
	}



} // namespace Net
} // namespace BR

