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
#include "Common/ResultCode/BRResultCodeNet.h"
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
	Result LoopbackConnection::EnqueueConnectionEvent( const IConnection::Event &evt )
	{
		return m_EventQueue.Enqueue( evt );
	}


	// Message count currently in recv queue
	SysUInt LoopbackConnection::GetRecvMessageCount()
	{
		return 0;
	}

	// Initialize LoopbackConnection
	Result LoopbackConnection::InitConnectionNClass( const NetAddress& Addr, NetClass netClass )
	{
		Result hr = ResultCode::SUCCESS;

		netAssert(GetConnectionState() == STATE_DISCONNECTED);

		if (GetConnectionState() != STATE_DISCONNECTED)
			netChk( CloseConnection() );

		netChk(IConnection::InitConnectionNClass(Addr, netClass));

		SetConnectionState(STATE_CONNECTED);
		//m_RecvQueue.ClearQueue();


		EnqueueConnectionEvent( IConnection::Event( IConnection::Event::EVT_CONNECTION_RESULT, ResultCode::SUCCESS)  );
		EnqueueConnectionEvent( IConnection::Event( IConnection::Event::EVT_STATE_CHANGE, GetConnectionState())  );

		netTrace( TRC_CONNECTION, "Initialize LoopbackConnection CID:{0}, Addr:{1}:{2}", GetCID(), GetConnectionInfo().Remote.strAddr, GetConnectionInfo().Remote.usPort );

	Proc_End:

		return hr;
	}

	// Disconnect LoopbackConnection
	Result LoopbackConnection::Disconnect(const char* reason)
	{
		unused(reason);
		// cant disconnected
		return ResultCode::SUCCESS;
	}


	// Close LoopbackConnection
	Result LoopbackConnection::CloseConnection()
	{
		// cant closed
		return ResultCode::SUCCESS;
	}


	CounterType LoopbackConnection::GetConnectionEventCount()
	{
		return m_EventQueue.GetEnqueCount();
	}

	// Query LoopbackConnection event
	Result LoopbackConnection::DequeueConnectionEvent( Event& curEvent )
	{
		return m_EventQueue.Dequeue( curEvent );
	}


	// Get received Message
	Result LoopbackConnection::GetRecvMessage( Message::MessageData* &pIMsg )
	{
		return ResultCode::FAIL;
	}

	// Update net control, process connection heartbit, ... etc
	Result LoopbackConnection::UpdateNetCtrl( )
	{
		return ResultCode::FAIL;
	}



} // namespace Net
} // namespace BR


