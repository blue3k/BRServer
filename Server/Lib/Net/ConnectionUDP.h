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

		// Sending wait queue
		MsgQueue			m_SendGuaQueue;

		// Send Guaranted Messages
		SendMsgWindow		m_SendReliableWindow;

		// Maximum guaranted retry at once
		UINT				m_uiMaxGuarantedRetry;

		// packet gathering buffer
		UINT				m_uiGatheredSize;
		BYTE*				m_pGatheringBuffer;
		// Memory pool for gathering buffer
		static MemoryPool*	m_pGatheringBufferPool;


		// Recv guaranted Message Queue, to enable MT enqueue
		MsgQueue			 m_RecvGuaQueue;

		// Net control Sending wait queue
		PageQueue<MsgNetCtrlBuffer>	m_RecvNetCtrlQueue;

		Message::MessageData*		m_SubFrameMessage;

	protected:
		
		// Process network control message
		virtual HRESULT ProcNetCtrl( const MsgNetCtrl* pNetCtrl ) = 0;

		// Process NetCtrl queue
		virtual HRESULT ProcNetCtrlQueue() = 0;

		// Process Recv queue
		virtual HRESULT ProcRecvReliableQueue() = 0;

		// Process Send queue
		virtual HRESULT ProcSendReliableQueue() = 0;
		
		// Process message window queue
		virtual HRESULT ProcReliableSendRetry() = 0;

		// Process connection state
		virtual HRESULT ProcConnectionState() = 0;

	public:
		// Constructor
		ConnectionUDPBase( UINT reliableWindowSize );
		virtual ~ConnectionUDPBase();

		// Set maximum guaranted retry count
		inline void SetMaxGuarantedRetry( UINT uiMaxGuarantedRetry );

		// Set message window size connection
		HRESULT SetMessageWindowSize( UINT uiSend, UINT uiRecv );

		// Change remote Address
		void ChangeRemoteAddress( const sockaddr_in6& socAddr );

		// gathering
		virtual HRESULT SendPending( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 );
		virtual HRESULT SendPending( Message::MessageData* pMsg );
		HRESULT SendFlush();

		// Prepare gathering buffer
		HRESULT PrepareGatheringBuffer( UINT uiRequiredSize );
		static HRESULT ReleaseGatheringBuffer( BYTE *pBuffer );
		UINT GetGatheredBufferSize()                                  { return m_uiGatheredSize; }

		// Called on connection result
		virtual void OnConnectionResult( HRESULT hrConnect );

		// frame sequence
		HRESULT SendFrameSequenceMessage(Message::MessageData* pMsg);
		HRESULT OnFrameSequenceMessage(Message::MessageData* pMsg, const std::function<void(Message::MessageData* pMsgData)>& action);

		// Initialize connection
		virtual HRESULT InitConnection(SOCKET socket, const ConnectionInformation &connectInfo) override;

		// Close connection
		virtual HRESULT CloseConnection() override;

		// Clear Queue
		virtual HRESULT ClearQueues() override;

		// Disconnect connection
		virtual HRESULT Disconnect() override;


		// Send message to connected entity
		virtual HRESULT Send( Message::MessageData* &pMsg ) override;


		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl() = 0;

		// Update send queue and return remain item count in send queue
		virtual ULONG UpdateSendQueue();


		virtual HRESULT ProcGuarrentedMessageWindow(const std::function<void(Message::MessageData* pMsgData)>& action);

	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server UDP Network connection class
	//

	class ConnectionUDP : public ConnectionUDPBase
	{
	protected:
		
		// Process network control message
		virtual HRESULT ProcNetCtrl( const MsgNetCtrl* pNetCtrl );

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

	public:
		
		ConnectionUDP() : ConnectionUDPBase( BASE_WINDOW_SIZE ) {}

		// called when incomming message occure
		virtual HRESULT OnRecv( UINT uiBuffSize, const BYTE* pBuff );
		virtual HRESULT OnRecv( Message::MessageData *pMsg );

		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl();
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
		virtual HRESULT InitSynchronization() override;

		HRESULT ProcNetCtrl( const MsgNetCtrl* pNetCtrl ) override;

		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl() override;
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

		// Clear Queue
		virtual HRESULT ClearQueues() override;

		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl() override;
	};
	



	////////////////////////////////////////////////////////////////////////////////
	//
	//	client UDP Network connection class
	//

	class ConnectionUDPClient : public ConnectionUDP, public IOCPSystem::IOCallBack, public MemoryPoolObject<ConnectionUDPClient>
	{
	public:

	private:
		// recv io buffer
		OVERLAPPED_BUFFER_READ m_RecvBuffer;


	public:
		// Constructor
		ConnectionUDPClient();
		~ConnectionUDPClient();

		// Clear Queue
		virtual HRESULT ClearQueues();


		// called when New connection TCP accepted
		virtual HRESULT OnIOAccept( HRESULT hrRes, OVERLAPPED_BUFFER_ACCEPT *pAcceptInfo ) override;

		// called when reciving TCP message
		virtual HRESULT OnIORecvCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred ) override;

		// called when send completed
		virtual HRESULT OnIOSendCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_WRITE *pIOBuffer, DWORD dwTransferred ) override;

		// Pending recv New one
		HRESULT PendingRecv();


		// Get recv buffer pointer
		inline OVERLAPPED_BUFFER_READ* GetRecvBuffer();

		
		// Initialize connection
		virtual HRESULT InitConnection( SOCKET socket, const ConnectionInformation &connectInfo ) override;

		// Reinitialize and set remote address
		HRESULT ReInitialize( const sockaddr_in6& socAddr );


		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl() override;
	};


	#include "ConnectionUDP.inl"

}  // namespace Net
}; // namespace BR



