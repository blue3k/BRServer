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

#include "Common/PageQueue.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Net/NetDef.h"
#include "Net/NetCtrl.h"
#include "Net/NetSystem.h"
#include "Net/Connection.h"
#include "Net/MessageWindow.h"

namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	UDP Network connection class
	//

	class ConnectionUDPBase : public Connection
	{
	public:
		// These numbers must be same size with number of bits
		enum { 
			BASE_WINDOW_SIZE = 128,
		};

	protected:
		// Recive Sorted messages
		RecvMsgWindow		m_RecvReliableWindow;

		// Send Guaranted Messages
		SendMsgWindow		m_SendReliableWindow;

		// Maximum guaranted retry at once
		UINT				m_uiMaxGuarantedRetry;

		// packet gathering buffer
		UINT				m_uiGatheredSize;
		BYTE*				m_pGatheringBuffer;


		// Recv guaranted Message Queue, to enable MT enqueue
		MsgQueue			 m_RecvGuaQueue;

		// Net control recv queue
		PageQueue<MsgNetCtrlBuffer>	m_RecvNetCtrlQueue;

		// subframe message
		Message::MessageData*		m_SubFrameMessage;

		// UDP send queue
		WriteBufferQueue*			m_pWriteQueuesUDP;

	protected:
		
		// Process network control message
		virtual Result ProcNetCtrl( const MsgNetCtrl* pNetCtrl ) = 0;

		// Process NetCtrl queue
		virtual Result ProcNetCtrlQueue() = 0;

		// Process Recv queue
		virtual Result ProcRecvReliableQueue() = 0;

		// Process Send queue
		virtual Result ProcSendReliableQueue() = 0;
		
		// Process message window queue
		virtual Result ProcReliableSendRetry() = 0;

		// Process connection state
		virtual Result ProcConnectionState() = 0;


		WriteBufferQueue* GetWriteQueueUDP() { return m_pWriteQueuesUDP; }


		// Send packet buffer to connection with network device
		virtual Result SendBufferUDP(IOBUFFER_WRITE *pSendBuffer);
		virtual Result EnqueueBufferUDP(IOBUFFER_WRITE *pSendBuffer);

		virtual Result SendRaw(Message::MessageData* &pMsg) override;

	public:
		// Constructor
		ConnectionUDPBase( UINT reliableWindowSize );
		virtual ~ConnectionUDPBase();

		void SetWriteQueueUDP(WriteBufferQueue* writeQueue);


		// Set maximum guaranted retry count
		inline void SetMaxGuarantedRetry( UINT uiMaxGuarantedRetry );

		// Set message window size connection
		Result SetMessageWindowSize( UINT uiSend, UINT uiRecv );

		// Change remote Address
		void ChangeRemoteAddress( const sockaddr_storage& socAddr );

		// gathering
		virtual Result SendPending( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 );
		virtual Result SendPending( Message::MessageData* pMsg );
		Result SendFlush();

		// Prepare gathering buffer
		Result PrepareGatheringBuffer( UINT uiRequiredSize );
		UINT GetGatheredBufferSize() { return m_uiGatheredSize; }


		// frame sequence
		Result SendFrameSequenceMessage(Message::MessageData* pMsg);
		Result OnFrameSequenceMessage(Message::MessageData* pMsg, const std::function<void(Message::MessageData* pMsgData)>& action);

		// Initialize connection
		virtual Result InitConnection(SOCKET socket, const ConnectionInformation &connectInfo) override;

		// Close connection
		virtual Result CloseConnection() override;

		// Clear Queue
		virtual Result ClearQueues() override;

		// Disconnect connection
		virtual Result Disconnect(const char* reason) override;


		// Send message to connected entity
		virtual Result Send( Message::MessageData* &pMsg ) override;


		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl() = 0;

		// Update send queue and return remain item count in send queue
		virtual Result UpdateSendQueue() override;

		// Update Send buffer Queue, TCP and UDP client connection
		virtual Result UpdateSendBufferQueue() override;

		virtual Result ProcGuarrentedMessageWindow(const std::function<void(Message::MessageData* pMsgData)>& action);

	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server UDP Network connection class
	//

	class ConnectionUDP : public ConnectionUDPBase
	{
	protected:
		
		// Process network control message
		virtual Result ProcNetCtrl( const MsgNetCtrl* pNetCtrl );

		// Process NetCtrl queue
		virtual Result ProcNetCtrlQueue();

		// Process Recv queue
		virtual Result ProcRecvReliableQueue();

		// Process Send queue
		virtual Result ProcSendReliableQueue();
		
		// Process message window queue
		virtual Result ProcReliableSendRetry();

		// Process connection state
		virtual Result ProcConnectionState();


	public:
		
		ConnectionUDP() : ConnectionUDPBase( BASE_WINDOW_SIZE ) {}

		// called when incomming message occure
		virtual Result OnRecv( UINT uiBuffSize, const BYTE* pBuff );
		virtual Result OnRecv( Message::MessageData *pMsg );

		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl();
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server UDP Network connection class
	//

	class ConnectionUDPServerPeer : public ConnectionUDP, public MemoryPoolObject<ConnectionUDPServerPeer>
	{
	public:

	protected:


	public:
		// Constructor
		ConnectionUDPServerPeer();
		virtual ~ConnectionUDPServerPeer();


		// Initialize packet synchronization
		virtual Result InitSynchronization() override;

		Result ProcNetCtrl( const MsgNetCtrl* pNetCtrl ) override;

		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl() override;
	};

	


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server UDP Network connection class
	//

	class ConnectionUDPServer : public ConnectionUDP, public MemoryPoolObject<ConnectionUDPServer>
	{
	public:

	protected:


	public:
		// Constructor
		ConnectionUDPServer();
		virtual ~ConnectionUDPServer();

		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl() override;
	};
	



	////////////////////////////////////////////////////////////////////////////////
	//
	//	client UDP Network connection class
	//

	class ConnectionUDPClient : public ConnectionUDP, public INetIOCallBack, public MemoryPoolObject<ConnectionUDPClient>
	{
	public:

	private:

	protected:
		// Send packet buffer to connection with network device
		//virtual Result SendBufferUDP(IOBUFFER_WRITE *pSendBuffer) override;
		virtual Result EnqueueBufferUDP(IOBUFFER_WRITE *pSendBuffer) override;

		// Send message to connection with network device
		virtual Result SendBuffer(IOBUFFER_WRITE *pSendBuffer) override;

	public:
		// Constructor
		ConnectionUDPClient();
		~ConnectionUDPClient();

		virtual INetIOCallBack* GetIOCallback() override { return this; }


		// called when reciving TCP message
		virtual Result Recv(IOBUFFER_READ* pIOBuffer) override;
		virtual Result OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer ) override;

		virtual Result OnSendReady() override;

		// called when send completed
		virtual Result OnIOSendCompleted( Result hrRes, IOBUFFER_WRITE *pIOBuffer ) override;

		// Pending recv New one
		Result PendingRecv();


		
		// Initialize connection
		virtual Result InitConnection( SOCKET socket, const ConnectionInformation &connectInfo ) override;

		// Reinitialize and set remote address
		Result ReInitialize( const sockaddr_storage& socAddr );


		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl() override;

		// Update Send buffer Queue, TCP and UDP client connection
		virtual Result UpdateSendBufferQueue() override;
	};




	#include "ConnectionUDP.inl"

}  // namespace Net
}; // namespace BR



