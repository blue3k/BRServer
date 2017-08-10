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
		virtual Result SendPending( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 ) override;
		virtual Result SendPending( Message::MessageData* pMsg )  override { return ConnectionUDPBase::SendPending(pMsg); }
		virtual Result SendSync( UINT uiSequence, UINT64 uiSyncMask );

		virtual Result SendNetCtrl( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 ) override;


		// Process network control message
		virtual Result ProcNetCtrl( const MsgMobileNetCtrl* pNetCtrl );
		Result ProcNetCtrl( const MsgNetCtrl* pNetCtrl ) override { return ResultCode::NOT_IMPLEMENTED; }

		// Process NetCtrl queue
		virtual Result ProcNetCtrlQueue() override;

		// Process Recv queue
		virtual Result ProcRecvReliableQueue() override;

		// Process Send queue
		virtual Result ProcSendReliableQueue() override;
		
		// Process message window queue
		virtual Result ProcReliableSendRetry() override;

		// Process connection state
		virtual Result ProcConnectionState() override;

		Result OnGuarrentedMessageRecv(Message::MessageData *pMsg);

		void SetSendSyncThisTick(bool bEnable) { m_bSendSyncThisTick = bEnable; }

	public:
		
		virtual Result InitConnection(SOCKET socket, const ConnectionInformation &connectInfo) override;

		// called when incomming message occure
		virtual Result OnRecv(UINT uiBuffSize, const BYTE* pBuff) override;
		virtual Result OnRecv( Message::MessageData *pMsg ) override;


		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl() override;

		virtual Result UpdateSendQueue() override;
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

		virtual Result ProcNetCtrl(const MsgMobileNetCtrl* pNetCtrl) override;

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

		virtual Result ProcNetCtrl(const MsgMobileNetCtrl* pNetCtrl) override;

		// Process connection state
		virtual Result ProcConnectionState() override;


		// Send packet buffer to connection with network device
		//virtual Result SendBufferUDP(IOBUFFER_WRITE *pSendBuffer) override;
		virtual Result EnqueueBufferUDP(IOBUFFER_WRITE *pSendBuffer) override;

		// Send message to connection with network device
		virtual Result SendBuffer(IOBUFFER_WRITE *pSendBuffer) override;


	public:

		virtual INetIOCallBack* GetIOCallback() override { return this; }


		Result InitConnection(SOCKET socket, const ConnectionInformation &connectInfo) override;

		// If the connection have socket per connection, that need to be released by manually
		virtual void CloseSocket() override;


		// called when receiving TCP message
		virtual Result Recv(IOBUFFER_READ* pIOBuffer) override;
		virtual Result OnIORecvCompleted(Result hrRes, IOBUFFER_READ* &pIOBuffer) override;

		virtual Result OnSendReady() override;

		// called when send completed
		virtual Result OnIOSendCompleted(Result hrRes, IOBUFFER_WRITE *pIOBuffer) override;


		// Pending recv New one
		Result PendingRecv();

	};


}  // namespace Net
} // namespace BR


