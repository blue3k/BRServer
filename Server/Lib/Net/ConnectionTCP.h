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
#include "Net/Connection.h"



namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network connection class
	//

	class ConnectionTCP : public Connection, public INetIOCallBack
	{
	public:

	private:
		// Guaranted send/ack count
		std::atomic<LONG> m_lGuarantedSent;
		std::atomic<LONG> m_lGuarantedAck;


		// Tempory incomming buffer for fragmented incomming packet buffer
		UINT	m_uiRecvTemUsed;
		std::vector<BYTE>	m_bufRecvTem;

		// Recv overlapped buffer. recv 
		//IOBUFFER_READ m_RecvBuffer;

		WriteBufferQueue m_WriteBuffer;

	protected:
		// Special net control packet count on the same connection state. ex)Connect packet
		UINT m_uiSendNetCtrlCount;

		// 
		bool m_isClientSide;
		bool m_isActuallyConnected;

	protected:

		virtual Result SendBuffer(IOBUFFER_WRITE *pSendBuffer) override;

		virtual Result SendRaw(Message::MessageData* &pMsg) override;

	public:
		// Constructor
		ConnectionTCP();
		virtual ~ConnectionTCP();


		//inline IOBUFFER_READ* GetRecvBuffer();

		// Process network control message
		Result ProcNetCtrl( const MsgNetCtrl* pNetCtrl );

		virtual INetIOCallBack* GetIOCallback() override { return this; }


		/////////////////////////////////////////////////////////////////////////////////////////////
		// INetIOCallBack

		//virtual SharedObject* AsSharedObject() override { return this; }

		virtual Result Recv(IOBUFFER_READ* pIOBuffer) override;

		// called when reciving TCP message
		virtual Result OnIORecvCompleted( Result hrRes, IOBUFFER_READ* &pIOBuffer ) override;

		virtual Result OnSendReady() override;

		// called when Send completed
		virtual Result OnIOSendCompleted( Result hrRes, IOBUFFER_WRITE *pIOBuffer ) override;


		// Pending recv New one
		Result PendingRecv();
		
		// Clear Queue
		virtual Result ClearQueues() override;


		// Called on connection result
		virtual void OnConnectionResult( Result hrConnect ) override;

		// Initialize connection
		virtual Result InitConnection( SOCKET socket, const ConnectionInformation &connectInfo ) override;

		Result Connect();

		virtual Result WaitConnect();

		// Close connection
		virtual Result CloseConnection() override;

		virtual void CloseSocket() override;

		// called when incomming message occure
		virtual Result OnRecv(UINT uiBuffSize, const BYTE* pBuff) override;
		virtual Result OnRecv(Message::MessageData *pMsg) override;

		// Send message to connected entity
		virtual Result Send(Message::MessageData* &pMsg) override;

		virtual Result SendNetCtrl(UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0) override;


		// Update send queue, Reliable UDP
		virtual Result UpdateSendQueue() override;
		// Update Send buffer Queue, TCP and UDP client connection
		virtual Result UpdateSendBufferQueue() override;
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network client connection class
	//

	class ConnectionTCPClient : public ConnectionTCP, public MemoryPoolObject<ConnectionTCPClient>
	{
	public:

	public:
		// Constructor
		ConnectionTCPClient();
		~ConnectionTCPClient();
		
		// Initialize connection
		virtual Result InitConnection( SOCKET socket, const ConnectionInformation &connectInfo ) override;

		// Wait connection event
		Result WaitConnect() override;

		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl() override;

	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network server connection class
	//

	class ConnectionTCPServer : public ConnectionTCP, public MemoryPoolObject<ConnectionTCPServer>
	{
	public:

	private:

	public:
		// Constructor
		ConnectionTCPServer();
		~ConnectionTCPServer();

		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl() override;

	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network server connection class
	//

	class ConnectionPeerTCP : public ConnectionTCP, public MemoryPoolObject<ConnectionPeerTCP>
	{
	public:

	private:

	public:
		// Constructor
		ConnectionPeerTCP();
		~ConnectionPeerTCP();

		virtual void OnConnectionResult(Result hrConnect) override;

		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl() override;

	};

#include "ConnectionTCP.inl"

}  // namespace Net
}; // namespace BR



