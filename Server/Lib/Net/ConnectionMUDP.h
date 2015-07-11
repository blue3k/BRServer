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
	//	Server Mobile UDP Network connection class
	//

	class ConnectionMUDPServer : public ConnectionUDPBase, public MemoryPoolObject<ConnectionMUDPServer>
	{
	public:

		enum {
			MUDP_BASE_WINDOW_SIZE = 64,
		};

	private:
		bool m_bSendSyncThisTick;

	public:
		// Constructor
		ConnectionMUDPServer();
		virtual ~ConnectionMUDPServer();
	protected:

		// gathering
		virtual HRESULT SendPending( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 );
		virtual HRESULT SendPending( Message::MessageData* pMsg ) { return __super::SendPending(pMsg); }
		virtual HRESULT SendSync( UINT uiSequence, UINT64 uiSyncMask );

		virtual HRESULT SendNetCtrl( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 );


		// Process network control message
		HRESULT ProcNetCtrl( const MsgMobileNetCtrl* pNetCtrl );
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

	public:
		
		virtual HRESULT InitConnection(SOCKET socket, const ConnectionInformation &connectInfo) override;

		// called when incomming message occure
		virtual HRESULT OnRecv(UINT uiBuffSize, const BYTE* pBuff) override;
		virtual HRESULT OnRecv( Message::MessageData *pMsg ) override;


		// Process Send queue
		virtual HRESULT Send(Message::MessageData* &pMsg) override;


		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl() override;

		virtual ULONG UpdateSendQueue() override;
	};


}  // namespace Net
} // namespace BR


