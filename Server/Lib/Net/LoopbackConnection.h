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
#include "Net/NetMessage.h"
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
		BR::PageQueue<IConnection::Event>	m_EventQueue;

		// Add network event to queue
		HRESULT EnqueueConnectionEvent( const IConnection::Event& evt );


	public:
		LoopbackConnection();
		virtual ~LoopbackConnection();

		// Initialize LoopbackConnection
		virtual HRESULT InitConnection( const NetAddress& Addr, NetClass netClass );


		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual implementations
		//

		// from iSharedObj
		virtual void DeleteThis() const;

		// Disconnect connection
		virtual HRESULT Disconnect();

		// Close connection immediately without notify
		virtual HRESULT CloseConnection();


		// Send message to connected entity
		virtual HRESULT Send( Message::MessageData* &pMsg ) = 0;

		// Message count currently in recv queue
		virtual SysUInt GetRecvMessageCount();

		// Query connection event

		virtual CounterType GetConnectionEventCount() override;
		virtual HRESULT DequeueConnectionEvent( Event& curEvent ) override;

		// Get received Message
		virtual HRESULT GetRecvMessage( Message::MessageData* &pIMsg );

		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl( );



	};


} // namespace Net
} // namespace BR



