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
	Result Server::GetConnection(uintptr_t uiCID, SharedPointerT<Connection> &pConn)
	{
		return GetConnectionManager().GetConnectionByCID( uiCID, pConn );
	}



	// Open host and start listen
	Result Server::HostOpen( NetClass netCls, const char *strLocalIP, USHORT usLocalPort )
	{
		Result hr = ResultCode::SUCCESS;
		NetAddress localAddr;

		netChk( NetSystem::OpenSystem( Const::SVR_OVERBUFFER_COUNT, Const::SVR_NUM_NET_THREAD, Const::PACKET_GATHER_SIZE_MAX) );

		m_NetClass = netCls;

		netChk(SetLocalNetAddress(localAddr, strLocalIP, usLocalPort));

		SetLocalAddress( localAddr );

	Proc_End:


		return hr;
	}

	// Close host and close all connections
	Result Server::HostClose()
	{
		Result hr = ResultCode::SUCCESS;

		GetConnectionManager().TerminateManager();

		NetSystem::CloseSystem();

		if( GetSocket() != INVALID_SOCKET )
		{
			NetSystem::CloseSocket( GetSocket() );
			SetSocket( INVALID_SOCKET );
		}

	//Proc_End:



		return hr;
	}



	// Release Connection, Make connection to send free state
	Result Server::ReleaseConnection( IConnection* pIConnection )
	{
		Result hr = ResultCode::SUCCESS;
		ConnectionPtr pConn = (Connection*)pIConnection;

		netChkPtr( pIConnection );

		netTrace(TRC_DBGCON, "ReleaseConnection Port:{0}, CID:{1}, Connection Count : {2}", GetLocalAddress().usPort, pIConnection->GetCID(), GetConnectionManager().GetNumActiveConnection());
		netChk( GetConnectionManager().PendingReleaseConnection( pConn ) );

	Proc_End:

		if( !(hr) )
		{
			netTrace( Trace::TRC_ERROR, "ReleaseConnection Failed hr={0:X8}", hr );
		}

		return hr;
	}

	// take over connection management
	Result Server::TakeOverConnection(IConnection* pIConnection)
	{
		Result hr = ResultCode::SUCCESS;
		ConnectionPtr pConn = (Connection*)pIConnection;

		netChkPtr(pIConnection);

		netChk(GetConnectionManager().PendingManagedConnectionTakenOver(pConn));

	Proc_End:

		if (!(hr))
		{
			netTrace(Trace::TRC_ERROR, "ReleaseConnection Failed hr={0:X8}", hr);
		}

		return hr;
	}

	// Called when connection state changed
	Result Server::OnConnectionStateChange( IConnection *pIConnection )
	{
		Result hr = ResultCode::SUCCESS;
		INet::Event netDisEvent(INet::Event::EVT_CONNECTION_DISCONNECTED);

		netTrace(TRC_CONNECTION, "Net Con state Port:{0}, CID:{1}, state:{2}", GetLocalAddress().usPort, pIConnection->GetCID(), pIConnection->GetConnectionState());

		switch (pIConnection->GetConnectionState())
		{
		case IConnection::STATE_CONNECTING:
			break;
		case IConnection::STATE_CONNECTED:
			break;
		case IConnection::STATE_DISCONNECTED:
			EnqueueNetEvent( netDisEvent );
			break;
		case IConnection::STATE_DISCONNECTING:
			break;
		default:
			break;
		};

	//Proc_End:

		return hr;
	}



} // namespace Net
} // namespace BR

