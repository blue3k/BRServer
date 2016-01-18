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
#include "Common/ResultCode/BRResultCodeNet.h"
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
		HRESULT ProcNetCtrl( const MsgNetCtrl* pNetCtrl )			{ return E_SYSTEM_NOTIMPL; }

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

	class ConnectionMUDPServer : public ConnectionMUDP, public MemoryPoolObject<ConnectionMUDPServer>
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

	class ConnectionMUDPClient : public ConnectionMUDP, public INetIOCallBack, public MemoryPoolObject<ConnectionMUDPClient>
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


		// Send packet buffer to connection with network device
		//virtual HRESULT SendBufferUDP(IOBUFFER_WRITE *pSendBuffer) override;
		virtual HRESULT EnqueueBufferUDP(IOBUFFER_WRITE *pSendBuffer) override;

		// Send message to connection with network device
		virtual HRESULT SendBuffer(IOBUFFER_WRITE *pSendBuffer) override;


	public:

		virtual INetIOCallBack* GetIOCallback() override { return this; }


		HRESULT InitConnection(SOCKET socket, const ConnectionInformation &connectInfo) override;


		virtual SOCKET GetIOSocket() override { return GetSocket(); }

		// called when reciving TCP message
		virtual HRESULT Recv(IOBUFFER_READ* pIOBuffer) override;
		virtual HRESULT OnIORecvCompleted(HRESULT hrRes, IOBUFFER_READ* &pIOBuffer) override;

		virtual HRESULT OnSendReady() override;

		// called when send completed
		virtual HRESULT OnIOSendCompleted(HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer) override;


		// Pending recv New one
		HRESULT PendingRecv();

	};


}  // namespace Net
} // namespace BR


