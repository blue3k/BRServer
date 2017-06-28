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


#pragma once

#include "Net/NetDef.h"
#include "Net/NetCtrl.h"
#include "Common/PageQueue.h"
#include "Net/Connection.h"



namespace BR {
namespace Net {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network LoopbackConnection base class
	//

	class LoopbackConnection : public IConnection
	{
	public:

	protected:

	private:

		// Event queue
		PageQueue<IConnection::Event>	m_EventQueue;

		// Add network event to queue
		Result EnqueueConnectionEvent( const IConnection::Event& evt );


	public:
		LoopbackConnection();
		virtual ~LoopbackConnection();

		// Initialize LoopbackConnection
		virtual Result InitConnectionNClass( const NetAddress& Addr, NetClass netClass ) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual implementations
		//

		// from iSharedObj
		virtual void DeleteThis() const;

		// Disconnect connection
		virtual Result Disconnect(const char* reason) override;

		// Close connection immediately without notify
		virtual Result CloseConnection() override;

		// Send message to connected entity
		//virtual Result Send( Message::MessageData* &pMsg ) = 0;

		// Message count currently in recv queue
		virtual SysUInt GetRecvMessageCount() override;

		// Query connection event

		virtual CounterType GetConnectionEventCount() override;
		virtual Result DequeueConnectionEvent( Event& curEvent ) override;

		// Get received Message
		virtual Result GetRecvMessage( Message::MessageData* &pIMsg ) override;

		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl( ) override;


		// Update send queue, Reliable UDP
		virtual Result UpdateSendQueue() override { return ResultCode::FAIL; }
		// Update Send buffer Queue, TCP and UDP client connection
		virtual Result UpdateSendBufferQueue() override { return ResultCode::FAIL; }

	};


} // namespace Net
} // namespace BR



