////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net connection classes. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Net/NetDef.h"
#include "Net/NetCtrl.h"
#include "Net/NetSystem.h"
#include "Common/HRESNet.h"
#include "Common/PageQueue.h"
#include "Net/ConnectionUDP.h"


namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Mobile UDP Network connection class
	//

	class ConnectionMUDP : public ConnectionUDPBase
	{
	public:

		enum {
			MUDP_BASE_WINDOW_SIZE = 64,
		};

	private:
		bool m_bSendSyncThisTick;

	public:
		// Constructor
		ConnectionMUDP();
		virtual ~ConnectionMUDP();
	protected:

		// gathering
		virtual HRESULT SendPending( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 );
		virtual HRESULT SendPending( Message::MessageData* pMsg ) { return ConnectionUDPBase::SendPending(pMsg); }
		virtual HRESULT SendSync( UINT uiSequence, UINT64 uiSyncMask );

		virtual HRESULT SendNetCtrl( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 );


		// Process network control message
		virtual HRESULT ProcNetCtrl( const MsgMobileNetCtrl* pNetCtrl );
		HRESULT ProcNetCtrl( const MsgNetCtrl* pNetCtrl )			{ return E_NOTIMPL; }

		// Process NetCtrl queue
		virtual HRESULT ProcNetCtrlQueue();

		// Process Recv queue
		virtual HRESULT ProcRecvReliableQueue();

		// Process Send queue
		virtual HRESULT ProcSendReliableQueue();
		
		// Process message window queue
		virtual HRESULT ProcReliableSendRetry();

		// Process connection state
		virtual HRESULT ProcConnectionState();

		HRESULT OnGuarrentedMessageRecv(Message::MessageData *pMsg);

		void SetSendSyncThisTick(bool bEnable) { m_bSendSyncThisTick = bEnable; }

	public:
		
		virtual HRESULT InitConnection(SOCKET socket, const ConnectionInformation &connectInfo) override;

		// called when incomming message occure
		virtual HRESULT OnRecv(UINT uiBuffSize, const BYTE* pBuff) override;
		virtual HRESULT OnRecv( Message::MessageData *pMsg ) override;


		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl() override;

		virtual HRESULT UpdateSendQueue() override;
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server Mobile UDP Network connection class
	//

	class ConnectionMUDPServer : public ConnectionMUDP, public MemoryPoolObject<ConnectionMUDP>
	{
	public:

	public:
		// Constructor
		ConnectionMUDPServer();
		virtual ~ConnectionMUDPServer();
	protected:

		virtual HRESULT ProcNetCtrl(const MsgMobileNetCtrl* pNetCtrl) override;

	public:

		
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client Mobile UDP Network connection class
	//

	class ConnectionMUDPClient : public ConnectionMUDP, public MemoryPoolObject<ConnectionMUDP>
	{
	public:

	public:
		// Constructor
		ConnectionMUDPClient();
		virtual ~ConnectionMUDPClient();
	protected:

		virtual HRESULT ProcNetCtrl(const MsgMobileNetCtrl* pNetCtrl) override;

		// Process connection state
		virtual HRESULT ProcConnectionState() override;

	public:


	};


}  // namespace Net
} // namespace BR


