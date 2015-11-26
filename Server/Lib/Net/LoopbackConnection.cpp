////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Loopback fake connection
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/Thread.h"
#include "Common/BrAssert.h"
#include "Common/Utility.h"
#include "Common/TimeUtil.h"
#include "Common/HRESNet.h"
#include "Net/NetConst.h"
#include "Net/LoopbackConnection.h"
#include "Net/NetTrace.h"
#include "Net/NetCtrl.h"

#include "Protocol/ProtocolVer.h"




namespace BR {
namespace Net {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network LoopbackConnection base class
	//


	LoopbackConnection::LoopbackConnection()
	{
	}

	LoopbackConnection::~LoopbackConnection()
	{
//		m_RecvQueue.ClearQueue();

		//for( INT iPolicy = 0; iPolicy < BR::POLICY_NETMAX; iPolicy++ )
		//{
		//	if( m_pPolicy[iPolicy] )
		//		delete m_pPolicy[iPolicy];
		//}
	}

	

	// from iSharedObj
	void LoopbackConnection::DeleteThis() const
	{
		//AssertRel(!"Do not use this" );
		delete this;
	}

	
	// Add network event to queue
	HRESULT LoopbackConnection::EnqueueConnectionEvent( const IConnection::Event &evt )
	{
		return m_EventQueue.Enqueue( evt );
	}


	// Message count currently in recv queue
	SysUInt LoopbackConnection::GetRecvMessageCount()
	{
		return 0;
	}

	// Initialize LoopbackConnection
	HRESULT LoopbackConnection::InitConnection( const NetAddress& Addr, NetClass netClass )
	{
		HRESULT hr = S_OK;

		netAssert(GetConnectionState() == STATE_DISCONNECTED);

		if (GetConnectionState() != STATE_DISCONNECTED)
			netChk( CloseConnection() );

		netChk(__super::InitConnection(Addr, netClass));

		SetConnectionState(STATE_CONNECTED);
		//m_RecvQueue.ClearQueue();


		EnqueueConnectionEvent( IConnection::Event( IConnection::Event::EVT_CONNECTION_RESULT, S_OK)  );
		EnqueueConnectionEvent( IConnection::Event( IConnection::Event::EVT_STATE_CHANGE, GetConnectionState())  );

		netTrace( TRC_CONNECTION, "Initialize LoopbackConnection CID:%0%, Addr:%1%:%2%", GetCID(), GetConnectionInfo().Remote.strAddr, GetConnectionInfo().Remote.usPort );

	Proc_End:

		return hr;
	}

	// Disconnect LoopbackConnection
	HRESULT LoopbackConnection::Disconnect()
	{
		// cant disconnected
		return S_OK;
	}


	// Close LoopbackConnection
	HRESULT LoopbackConnection::CloseConnection()
	{
		// cant closed
		return S_OK;
	}


	CounterType LoopbackConnection::GetConnectionEventCount()
	{
		return m_EventQueue.GetEnqueCount();
	}

	// Query LoopbackConnection event
	HRESULT LoopbackConnection::DequeueConnectionEvent( Event& curEvent )
	{
		return m_EventQueue.Dequeue( curEvent );
	}


	// Get received Message
	HRESULT LoopbackConnection::GetRecvMessage( Message::MessageData* &pIMsg )
	{
		return E_FAIL;
	}

	// Update net control, process connection heartbit, ... etc
	HRESULT LoopbackConnection::UpdateNetCtrl( )
	{
		return E_FAIL;
	}



} // namespace Net
} // namespace BR


